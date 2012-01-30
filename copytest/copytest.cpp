// copytest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <KtmW32.h>

int stopproc = 20;
int oldproc;

DWORD CALLBACK CopyProgressRoutine(
								   __in      LARGE_INTEGER TotalFileSize,
								   __in      LARGE_INTEGER TotalBytesTransferred,
								   __in      LARGE_INTEGER StreamSize,
								   __in      LARGE_INTEGER StreamBytesTransferred,
								   __in      DWORD dwStreamNumber,
								   __in      DWORD dwCallbackReason,
								   __in      HANDLE hSourceFile,
								   __in      HANDLE hDestinationFile,
								   __in_opt  LPVOID lpData
								   )
{
	int proc = (int) ((TotalBytesTransferred.QuadPart)*100/(TotalFileSize.QuadPart));
	if(oldproc == -100)
	{
		stopproc = proc + 10;
	}
	if(proc < oldproc)
	{
		oldproc = proc;
	}
	else
	{
		if(proc - oldproc >= 5)
		{
			printf("%d\n", proc);
			oldproc = proc;
		}
	}
	if(proc >= stopproc)
	{
		return PROGRESS_STOP;
	}
	return PROGRESS_CONTINUE;
}

int _tmain(int argc, _TCHAR* argv[])
{
	std::string fname;
//	fname = local file name
	std::string infname;
//	infname = Local Path
	infname += fname;
	std::string outfname;
//	outfname = network path
	outfname += fname;
	outfname += ".ttt";
	int attempts = 0;

test:
	oldproc = -100;
	BOOL ret = CopyFileEx(infname.c_str(), outfname.c_str(), &CopyProgressRoutine, NULL, NULL, COPY_FILE_RESTARTABLE);
	if(ret)
		puts("File copied");
	else
	{
		DWORD err = GetLastError();
		if(err == ERROR_REQUEST_ABORTED)
		{
			if(++attempts < 2)
			{
				puts("aborted. try continue");
				goto test;
			}
		}

		printf("Copy error %d\n", err);
		LPVOID lpMsgBuf;
		FormatMessage( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_ENGLISH, SUBLANG_DEFAULT), // Default language
			(LPTSTR) &lpMsgBuf,
			0,
			NULL 
			);
		puts((LPCTSTR)lpMsgBuf);
		LocalFree( lpMsgBuf );
	}

//	getchar();
	return 0;
}
