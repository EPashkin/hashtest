// splitlines.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

//макс размер пропускаемой строки
const int MAXLEN = 88;

void ShowHelp()
{
	puts("Usage:\nsplilines <input-filename> <output-filename>\n");
	exit(0);
}

int _tmain(int argc, _TCHAR* argv[])
{
	if(argc < 3 || strlen(argv[1]) == 0 || strlen(argv[2]) == 0)
	{
		ShowHelp();
	}
	char *fnameFrom = argv[1];
	char *fnameTo = argv[2];

	FILE* fIn;
	FILE* fOut;
	if(fopen_s(&fIn, fnameFrom, "rt") != 0)
		return false;
	if(fIn == NULL)
		return false;
	if(fopen_s(&fOut, fnameTo, "wt") != 0)
		return false;
	if(fOut == NULL)
		return false;

	char buf[4*1024];

	while(!feof(fIn))
	{
		if(!fgets(buf, sizeof(buf), fIn))
			break;
		char *s = buf;
		int len;

		while((len = strlen(s)) > MAXLEN)
		{
			char *ss = s + MAXLEN;
			//найдем пробел перед максимальной длиной
			while(ss > s)
			{
				if(strchr(" \t", *ss) != NULL)
					break;
				ss--;
			}
			//начало следующей части
			char *sNext = ss + 1;
			//найдем первый не конец слова (не пробел) перед пробелом
			while(ss > s)
			{
				if(strchr(" \t", *ss) == NULL)
					break;
				ss--;
			}

			//если не нашли пробел выходим из цикла
			if(ss <= s)
			{
				break;
			}
			//сохраняем первую часть
			ss++;
			*ss = 0;
			fputs(s, fOut);
			//для отметки того, что у этой строки есть продолжение, добавим пробел в конце
			fputs(" \n", fOut);
			s = sNext;
		}
		//сохраним остаток строки
		fputs(s, fOut);
	}

	fclose(fIn);
	fclose(fOut);

	return 0;
}
