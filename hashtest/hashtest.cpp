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


void ShowHelp()
{
	puts("Usage:\nhashtest <filename>\n");
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
	_lseek(h, 0, SEEK_END);
	__int64 len = _telli64(h);

	int n = sprintf_s((char *)in_buf, in_sz, "blob %llu", len) + 1;
	git_SHA1_Init(&c);
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

bool dohashfile(const char *fname)
{
	int h;
	_sopen_s(&h,fname, _O_RDONLY|_O_BINARY, _SH_DENYNO, _S_IREAD | _S_IWRITE);
	if(h < 0)
		return false;
	hashfile(h);
	_close(h);
	putchar(' ');
	puts(fname);
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2 || strlen(argv[1]) == 0)
	{
		ShowHelp();
	}
	char *fname = argv[1];
	if(!dohashfile(fname))
		ShowError("Error open file");

	return 0;
}
