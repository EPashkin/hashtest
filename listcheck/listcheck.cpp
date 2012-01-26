// listcheck.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
/*
#include <io.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <share.h>
*/

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

int compstrings(const char *str1, const char *str2)
{
	int len =0;
	while(*str1 && *str2)
	{
		if(*str1++ != *str2++)
			return len;
		len++;
	}
	return len;
}

bool checklist(const char *fname)
{
	FILE* f;
	if(fopen_s(&f, fname, "r") != 0)
		return false;
	if(f == NULL)
		return false;
	char buf[1024];
	char buf2[1024];
	int lineind = 0;
	int maxlen = 0;

	while(!feof(f))
	{
		if(!fgets(buf, sizeof(buf), f))
			break;
		lineind++;
		if(lineind > 1)
		{
			int len = compstrings(buf, buf2);
			if(len > 41)
			{
				printf("%d %s", lineind - 1, buf2);
				printf("%d %s\n", lineind, buf);
			}
			else
			{
				if(len > maxlen)
					maxlen = len;
			}
		}
		strcpy_s(buf2, sizeof(buf), buf);
	}

	fclose(f);
	printf("maxlen %d\n", maxlen);
	return true;
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 2 || strlen(argv[1]) == 0)
	{
		ShowHelp();
	}
	char *fname = argv[1];

	if(!checklist(fname))
		ShowError("Error open file");

	return 0;
}

