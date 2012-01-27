// hashtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
#include "sha1.h"
#include "..\km\km.h"

void ShowHelp()
{
	puts("Usage:\nhashtest <filename>\n or\nhashtest <dirname>\\\n");
	exit(0);
}

void ShowError(const char *errstr)
{
	puts(errstr);
	putchar('\n');
	getchar();
	exit(0);
}
void ShowHexString(unsigned char* buf, int len)
{
	do
	{
		unsigned val = *buf++;
		printf("%02X", val);
	}
	while(--len > 0);
}

void hashfile(int h)
{
	unsigned char sha1[20];
	size_t in_sz = 64 * 1024;
	unsigned char *in_buf = (unsigned char*) malloc(in_sz);
	git_SHA_CTX c;
	git_SHA1_Init(&c);

	//для совпадения с git
	_lseek(h, 0, SEEK_END);
	__int64 len = _telli64(h);
	int n = sprintf_s((char *)in_buf, in_sz, "blob %llu", len) + 1;
	git_SHA1_Update(&c, in_buf, n);

	_lseek(h, 0, SEEK_SET);
	while(!_eof(h))
	{
		int n = _read(h, in_buf, in_sz);
		if(!n)
			ShowError("Error read file");
		git_SHA1_Update(&c, in_buf, n);
	}
	git_SHA1_Final(sha1, &c);
	free(in_buf);

	ShowHexString(sha1, sizeof(sha1));
}

BYTE *CreateFileMap(const char *chFileName, DWORD *pdwFileSize, HANDLE &hFileForMap, HANDLE &hFileMap);
void FreeFileMap(char *chFileMap, HANDLE &hFileForMap, HANDLE &hFileMap);

void hashfile2(const char *fname)
{
    unsigned char hash[32];
    long actual_hash_size = sizeof(hash);
    size_t in_sz = 64 * 1024;
    unsigned char *in_buf = (unsigned char*) malloc(in_sz);
    memset(hash, 0, sizeof(hash));

    BYTE *pbtFileData=NULL;
    DWORD dwFileSize=0;
    HANDLE hFileForMap, hFileMap;
    BOOL bRc=FALSE;

    pbtFileData=CreateFileMap(fname, &dwFileSize, hFileForMap, hFileMap);
    if(pbtFileData)
    {
        int ret=CreateHash_QP(pbtFileData, dwFileSize, hash, sizeof(hash), &actual_hash_size);
        if(ret == 0)
            ShowError("Error hash file");
        FreeFileMap((char*)pbtFileData, hFileForMap, hFileMap);
    }

    ShowHexString(hash, actual_hash_size);
    putchar(' ');
    puts(fname);
}

bool dohashfile(const char *fname)
{
    hashfile2(fname);
	return true;
}

bool scandir(const char *dir)
{
	char namebuf[MAX_PATH];
	strcpy_s(namebuf, MAX_PATH, dir);
	int dirlen = strlen(namebuf);
	int lastlen = MAX_PATH - dirlen;
	char *s = namebuf + dirlen;
	strcpy_s(s, lastlen, "*.*");

	_finddata_t ffd;
	intptr_t h = _findfirst(namebuf, &ffd);
	if(h == -1)
		return false;
	do
	{
		if(ffd.name[0] == '.' && (ffd.name[1] == 0 || (ffd.name[1] == '.' && ffd.name[2] == 0)))
			continue;
		strcpy_s(s, lastlen, ffd.name);
		if(ffd.attrib & _A_SUBDIR)
		{
			strcat_s(s, lastlen, "\\");
			scandir(namebuf);
		}
		else
			dohashfile(namebuf);
	} while (_findnext(h, &ffd) == 0);
	_findclose(h);
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2 || strlen(argv[1]) == 0)
	{
		ShowHelp();
	}
	char *fname = argv[1];
	if(fname[strlen(fname)-1] == '\\')
	{
		scandir(fname);
	}
	else
	{
		if(!dohashfile(fname))
			ShowError("Error open file");
	}

	return 0;
}

BYTE *CreateFileMap(const char *chFileName, DWORD *pdwFileSize, HANDLE &hFileForMap, HANDLE &hFileMap)
{
    hFileForMap=CreateFile(chFileName, GENERIC_READ, 0, NULL,
        OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if(hFileForMap==INVALID_HANDLE_VALUE || hFileForMap==NULL)
        return NULL;
    hFileMap=CreateFileMapping(hFileForMap, NULL, PAGE_READONLY,
        0, 0, NULL);
    if(hFileMap==NULL)
    {
        CloseHandle(hFileForMap);
        return NULL;
    }
    BYTE *pFile=(BYTE*)MapViewOfFile(hFileMap, FILE_MAP_READ, 0, 0, 0);
    if(pFile==NULL)
    {
        CloseHandle(hFileMap);
        CloseHandle(hFileForMap);
        return NULL;
    }
    *pdwFileSize=GetFileSize(hFileForMap, NULL);
    return (BYTE*)pFile;
}

void FreeFileMap(char *chFileMap, HANDLE &hFileForMap, HANDLE &hFileMap)
{
    char str[500];
    BOOL bRc=UnmapViewOfFile((void*)chFileMap);
    if(!bRc)
    {
        DWORD dwErr=GetLastError();
    }

    bRc=CloseHandle(hFileMap);
    if(!bRc)
    {
        DWORD dwErr=GetLastError();
    }
    bRc=CloseHandle(hFileForMap);
    if(!bRc)
    {
        DWORD dwErr=GetLastError();
    }
}
