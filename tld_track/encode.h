#pragma once

#ifdef DLL_PROCESS_ATTACH
#define MATHFUNCSDLL_API __declspec(dllimport)
#else
#define MATHFUNCSDLL_API __declspec(dllexport)
#endif
#include <windows.h>

// �������ݽṹ
typedef struct _MD5CTX {
	DWORD	aState[4];		// ��¼���ݵı仯״̬
	DWORD	aCount[2];		// ��¼���ݵ�ԭʼ����(��bitΪ��λ)
	BYTE	aBuffer[64];	// ԭʼ����
} MD5CTX;

MATHFUNCSDLL_API INT EN_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer );
MATHFUNCSDLL_API INT EN_HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer );

