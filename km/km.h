
// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the KM_EXPORTS
// symbol defined on the command line. this symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// KM_API functions as being imported from a DLL, wheras this DLL sees symbols
// defined with this macro as being exported.
#ifdef KM_EXPORTS
#define KM_API __declspec(dllexport)
#else
#define KM_API __declspec(dllimport)
#endif


// ������ ��� � ������
#define EDS_SIGN_SIZE			128

// ������ ����� �������� �������
#define EDS_CHECK_KEY_SIZE		128

// A.D. 23.03.10 ������� ��� �������� ����������� ������
// ������ ���� - 32 �����.
#define HASH_QP_SIZE			32
// ������ ������������ - 4 �����.
#define MAC_QP_SIZE				4


extern "C"
{
KM_API BOOL VerifyDigitSign(PUCHAR pPublicKey, PUCHAR pInput, ULONG dwInputSize, 
								PUCHAR pSign, PBOOL pIsEqual, LPOVERLAPPED pOverlapped);

KM_API BOOL GenerateDigitSign(BYTE *pInput, BYTE *pSign, DWORD dwInputSize, LPOVERLAPPED pOverlapped);

KM_API BOOL CreateHash(BYTE *pInput, DWORD dwInputSize, BYTE *pHashBuf, long lHashBufSize, 
						   long *plHashRealSize);
KM_API BOOL KmGetUserCheckKey_A(LPCTSTR cUserId, PBYTE CheckKey, 
								PDWORD pdwResultLen, PDWORD dwErrorCode);

	// A.D. 04.05.07 ������� ��� ���������� ��������.
KM_API BOOL GetCurrentUserMndtData(char **ppTextMndtData);
KM_API void FreeUserMndtData(void *p);
KM_API long MndtLevelsCmp(const char *chLevel1, const char *chLevel2);	// chLevel1 - chLevel2, ���� ������������ ��� �������� �� ������, INT_MAX

// A.D. 15.12.08 ����� ������� ��� ���������� �������� - ��������� ������������� ���������.
	// ����� �� ��������� �������� �� ����������� ��������� ������������� ���������
	// ��� ������� ������� �� �������� �������� � ������� ���������� ��������.
KM_API BOOL MndtHierarchicalCanSave(const char *chLevel1, const char *chLevel2);
	// ��������� ��������� ������������� ���������  (chLevel1 - chLevel2) � 
	// ���������� ���������� ��������� � ������� *ppCmpResult, ������� ���� ���������� FreeUserMndtData
	// ��� ���������� ������ (��������, ���������� ������� ������ �����������) ���������� -1.
KM_API long MndtHierarchicalCmp(const char *chLevel1, const char *chLevel2, long **ppCmpResult);
// A.D. 23.03.10, 06.04.10 ������� ��� �������� ����������� ������
// ������� ���������� ���� ������ ��������� QP ��� ���������
// ���� ���������� ���������� DWORD g_dwQPKeyId �� ����� 0, ����������� ������������ �� ����� � ���� ��.
KM_API BOOL CreateHash_QP(BYTE *pInput, DWORD dwInputSize, BYTE *pHashBuf, long lHashBufSize, 
						   long *plHashRealSize);
// ������� ���������� ���� ������ ������, ����� ������ � ������ ��������� \0, � ����� \0\0
KM_API BOOL CreateHashOfFilesSet(BYTE *pFilesNames, BYTE *pHashBuf, long lHashBufSize, 
						  long *plHashRealSize);
// A.D. 06.04.10 ������� ��� �������� ���������� ��������� ��������� QP.
KM_API BOOL IsMndtControlActive();
KM_API BOOL IsEdsActive();
KM_API BOOL IsCryptoActive();

// A.D. 28.06.10 ������� ��� ��������� ������������ �� � ������� (���). 
// ������ ������������ ��������� ����������, ������� ������ ������ ���������.
KM_API BOOL GetMinimumSystemMndtData(char **ppTextMndtData);

// A.D. 07.09.10 ������� ��� �������� ���������� �� ��� ��������
KM_API BOOL AlarmToAudit(WORD wType, DWORD dwEventID, 
						 PSID pUserSid, LPCTSTR* lpszStrings,
                         LPCTSTR pszDllName, LPCTSTR pszAppName);

};

