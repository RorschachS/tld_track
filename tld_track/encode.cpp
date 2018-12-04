//#include "stdafx.h"
#include "encode.h"
extern "C"
{
	extern INT MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer );
	extern INT HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer );
}

int  MATHFUNCSDLL_API EN_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, TCHAR* outputBuffer )
{
	return MD5_BASE64( inputBuffer,inputCount, outputBuffer );
}

int  MATHFUNCSDLL_API EN_HMAC_MD5_BASE64( const BYTE* inputBuffer, UINT inputCount, const BYTE* userKey, UINT UserKeyLen, TCHAR* outputBuffer )
{
	return HMAC_MD5_BASE64( inputBuffer, inputCount, userKey, UserKeyLen, outputBuffer );
}