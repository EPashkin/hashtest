
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


// Размер ЭЦП в байтах
#define EDS_SIGN_SIZE			128

// Размер ключа проверки подписи
#define EDS_CHECK_KEY_SIZE		128

// A.D. 23.03.10 Функции для контроля целостности данных
// Размер хэша - 32 байта.
#define HASH_QP_SIZE			32
// Размер имитовставки - 4 байта.
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

	// A.D. 04.05.07 Функции для мандатного контроля.
KM_API BOOL GetCurrentUserMndtData(char **ppTextMndtData);
KM_API void FreeUserMndtData(void *p);
KM_API long MndtLevelsCmp(const char *chLevel1, const char *chLevel2);	// chLevel1 - chLevel2, если несовместимы или проверка не прошла, INT_MAX

// A.D. 15.12.08 Новые функции для мандатного контроля - сравнение иерархических категорий.
	// Можно ли сохранять документ по результатам сравнения иерархических категорий
	// Эта функция зависит от принятой политики в области мандатного контроля.
KM_API BOOL MndtHierarchicalCanSave(const char *chLevel1, const char *chLevel2);
	// Результат сравнения иерархических категорий  (chLevel1 - chLevel2) в 
	// Возвращает количество элементов в массиве *ppCmpResult, который надо освободить FreeUserMndtData
	// При внутренней ошибке (например, переданные входные данные некорректны) возвращает -1.
KM_API long MndtHierarchicalCmp(const char *chLevel1, const char *chLevel2, long **ppCmpResult);
// A.D. 23.03.10, 06.04.10 Функции для контроля целостности данных
// Функция вычисления хэша буфера функциями QP или имитатора
// Если глобальная переменная DWORD g_dwQPKeyId не равна 0, вычисляется имитовставка на ключе с этим ИД.
KM_API BOOL CreateHash_QP(BYTE *pInput, DWORD dwInputSize, BYTE *pHashBuf, long lHashBufSize, 
						   long *plHashRealSize);
// Функция вычисления хэша набора файлов, имена файлов в буфере разделены \0, в конце \0\0
KM_API BOOL CreateHashOfFilesSet(BYTE *pFilesNames, BYTE *pHashBuf, long lHashBufSize, 
						  long *plHashRealSize);
// A.D. 06.04.10 Функции для проверки активности различных подсистем QP.
KM_API BOOL IsMndtControlActive();
KM_API BOOL IsEdsActive();
KM_API BOOL IsCryptoActive();

// A.D. 28.06.10 Функция для получения минимального КУ в системе (ДСП). 
// Способ программного получения неизвестен, поэтому просто выдает константу.
KM_API BOOL GetMinimumSystemMndtData(char **ppTextMndtData);

// A.D. 07.09.10 Функция для отправки оповещения на АРМ аудитора
KM_API BOOL AlarmToAudit(WORD wType, DWORD dwEventID, 
						 PSID pUserSid, LPCTSTR* lpszStrings,
                         LPCTSTR pszDllName, LPCTSTR pszAppName);

};

