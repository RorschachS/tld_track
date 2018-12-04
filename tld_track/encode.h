#pragma once

#ifdef DLL_PROCESS_ATTACH
#define MATHFUNCSDLL_API __declspec(dllimport)
#else
#define MATHFUNCSDLL_API __declspec(dllexport)
#endif
#include <windows.h>

// 定义数据结构
typedef struct _MD5CTX {
	DWORD	aState[4];		// 记录数据的变化状态
	DWORD	aCount[2];		// 记录数据的原始长度(以bit为单位)
	BYTE	aBuffer[64];	// 原始数据
} MD5CTX;

MATHFUNCSDLL_API INT EN_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer );
MATHFUNCSDLL_API INT EN_HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer );

