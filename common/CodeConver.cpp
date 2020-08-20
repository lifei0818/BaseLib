#include "common/sys_incl.h"
#include "common/CodeConver.h"

CCodeConver::CCodeConver()
	: _lpChar (NULL)
	, _lpUnicodeChar (NULL)
	, _lpUtf8Char (NULL)
	, _lpUnicodeBigEndian(NULL)
{
}

CCodeConver::~CCodeConver ()
{
	if (_lpChar) {
		delete []_lpChar;
		_lpChar = NULL;
	}
	if (_lpUnicodeChar) {
		delete []_lpUnicodeChar;
		_lpUnicodeChar = NULL;
	}
	if (_lpUtf8Char) {
		delete []_lpUtf8Char;
		_lpUtf8Char = NULL;
	}
	if (_lpUnicodeBigEndian) {
		delete []_lpUnicodeBigEndian;
		_lpUnicodeBigEndian = NULL;
	}
}

#ifndef _MACRO_OS_WIN32
static int CodeConvert(const char *fromcode, char *inbuf,  size_t inlen, 
                       const char *tocode,   char *outbuf, size_t outlen)
{
    size_t restlen = outlen;
    //也可以用mbstowcs/wcstombs来实现，但需要先setlocale比较麻烦
    iconv_t cd = ::iconv_open(tocode, fromcode);
    int iRet = ::iconv(cd, &inbuf, &inlen, &outbuf, &restlen);
    int err = errno;
    ::iconv_close(cd);

    if (iRet < 0)
    {
        errno = err;
        return iRet;
    }

    return outlen - restlen;
}
#endif

LPWSTR
CCodeConver::UTF8ToUnicode (LPCWSTR lpUtf8Char, int *lpSize)
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;

#ifdef _MACRO_OS_WIN32
	int i = MultiByteToWideChar (CP_UTF8, 0, (LPCSTR)lpUtf8Char, iSize, NULL, NULL);
	int iCount = i + 1;
#else
    if (iSize < 0)
    {
        iSize = wcslen(lpUtf8Char);
    }
    int iCount = iSize * 2 + 2;     // FF FE
#endif

	if (_lpUnicodeChar) delete []_lpUnicodeChar;
	_lpUnicodeChar = new WCHAR[iCount];
	memset (_lpUnicodeChar, 0, sizeof (WCHAR) * iCount);

#ifdef _MACRO_OS_WIN32
	MultiByteToWideChar (CP_UTF8, 0, (LPCSTR)lpUtf8Char, iSize, _lpUnicodeChar, i);
	iSize = i;
#else
    iSize = CodeConvert("utf-8",   (LPSTR)lpUtf8Char, iSize, 
                        "unicode", (LPSTR)_lpUnicodeChar, iCount);
#endif

	return _lpUnicodeChar;
}

LPWSTR
CCodeConver::UnicodeToUTF8 (LPCWSTR lpUnicodeChar, int *lpSize)
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;

#ifdef _MACRO_OS_WIN32
	int i = WideCharToMultiByte (CP_UTF8, 0, lpUnicodeChar, iSize, NULL, NULL, NULL, NULL); 
	int iCount = i + 1;
#else
    if (iSize < 0)
    {
        iSize = wcslen(lpUnicodeChar);
    }
    int iCount = iSize;
#endif

	if (_lpUtf8Char) delete []_lpUtf8Char;
	_lpUtf8Char = new WCHAR[iCount];
	memset (_lpUtf8Char, 0, sizeof (WCHAR) * iCount);

#ifdef _MACRO_OS_WIN32
	WideCharToMultiByte (CP_UTF8, 0, lpUnicodeChar, iSize, (LPSTR)_lpUtf8Char, i, NULL, NULL );
	iSize = i;
#else
    iSize = CodeConvert("unicode", (LPSTR)lpUnicodeChar, iSize, 
                        "utf-8",   (LPSTR)_lpUtf8Char,   iCount);
#endif

	return _lpUtf8Char;
}

LPSTR
CCodeConver::UnicodeToAnsi (LPCWSTR lpUnicodeChar, int *lpSize)
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;

#ifdef _MACRO_OS_WIN32
	int i = WideCharToMultiByte (CP_ACP, 0, lpUnicodeChar, iSize, NULL, NULL, NULL, NULL ); 
	int iCount = i + 1;
#else
    if (iSize < 0)
    {
        iSize = wcslen(lpUnicodeChar);
    }
    int iCount = iSize;
#endif

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

#ifdef _MACRO_OS_WIN32
	WideCharToMultiByte (CP_ACP, 0, lpUnicodeChar, iSize, _lpChar, i, NULL, NULL);
	iSize = i;
#else
    iSize = CodeConvert("unicode", (LPSTR)lpUnicodeChar, iSize, 
                        "gb18030", (LPSTR)_lpChar,       iCount); //"gb18030"
#endif

	return _lpChar;
}

LPWSTR
CCodeConver::AnsiToUnicode (LPCSTR lpChar, int *lpSize)
{
	int iTemp = -1;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;

#ifdef _MACRO_OS_WIN32
	int i = MultiByteToWideChar (CP_ACP, 0, lpChar, iSize, NULL, NULL); 
	int iCount = i + 1;
#else
    if (iSize < 0)
    {
        iSize = strlen(lpChar) + 1;
    }
    int iCount = iSize * 2 + 2;     // FF FE
#endif

	if (_lpUnicodeChar) delete []_lpUnicodeChar;
	_lpUnicodeChar = new WCHAR[iCount];
	memset (_lpUnicodeChar, 0, sizeof (WCHAR) * iCount);

#ifdef _MACRO_OS_WIN32
	MultiByteToWideChar (CP_ACP, 0, lpChar, iSize, _lpUnicodeChar, i);
	iSize = i;
#else
    iSize = CodeConvert("gb18030", (LPSTR)lpChar,         iSize, 
                        "unicode", (LPSTR)_lpUnicodeChar, iCount);
#endif

	return _lpUnicodeChar;
}

LPSTR
CCodeConver::UTF8ToAnsi (LPCWSTR lpUtf8Char, int *lpSize)
{
	LPWSTR lpUnicodeChar = UTF8ToUnicode (lpUtf8Char, lpSize);
	LPSTR lpChar = UnicodeToAnsi (lpUnicodeChar, lpSize);
	return lpChar;
}

#ifndef _MACRO_OS_WIN32
LPSTR
CCodeConver::LinuxUTF8ToAnsi (LPCSTR lpUtf8Char, int *lpSize)
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;
    if (iSize < 0)
    {
        iSize = strlen(lpUtf8Char);
    }
    int iCount = iSize*2 + 2;

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

    iSize = CodeConvert("utf-8", (LPSTR)lpUtf8Char, iSize, 
                        "gb2312", (LPSTR)_lpChar,    iCount); //"gb18030"
	return _lpChar;
}

LPSTR 
CCodeConver::LinuxUTF8ToGB18030( LPCSTR lpUtf8Char, int *lpSize /*= NULL*/ )
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;
	if (iSize < 0)
	{
		iSize = strlen(lpUtf8Char);
	}
	int iCount = iSize * 2 + 2;

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

	iSize = CodeConvert("UTF-8", (LPSTR)lpUtf8Char, iSize, 
		"GB18030", (LPSTR)_lpChar,    iCount); //"gb18030"

	return _lpChar;
}

LPSTR 
CCodeConver::LinuxGB18030ToUTF8( LPCSTR lpGb18030Char, int *lpSize /*= NULL*/ )
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;
	if (iSize < 0)
	{
		iSize = strlen(lpGb18030Char);
	}
	int iCount = iSize * 2 + 2;

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

	iSize = CodeConvert("GB18030", (LPSTR)lpGb18030Char, iSize, 
		"UTF-8", (LPSTR)_lpChar,    iCount); //"gb18030"

	return _lpChar;
}

LPSTR 
CCodeConver::LinuxUnicodeToGB18030( LPCSTR lpUnicodeChar, int *lpSize /*= NULL*/ )
{
	int iTemp = -1;;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;
	if (iSize < 0)
	{
		iSize = strlen(lpUnicodeChar);
	}
	int iCount = iSize * 2 + 2;

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

	iSize = CodeConvert("UNICODE", (LPSTR)lpUnicodeChar, iSize, 
		"GB18030", (LPSTR)_lpChar,    iCount); //"gb18030"

	return _lpChar;
}

LPSTR 
CCodeConver::LinuxGB18030ToUnicode( LPCSTR lpGb18030Char, int *lpSize /*= NULL*/ )
{
	int iTemp = -1;
	int &iSize = lpSize == 0 ? iTemp : *lpSize;
	if (iSize < 0)
	{
		iSize = strlen(lpGb18030Char) + 1;
	}
	int iCount = iSize * 2 + 2;     // FF FE

	if (_lpChar) delete []_lpChar;
	_lpChar = new char[iCount];
	memset (_lpChar, 0, sizeof (char) * iCount);

	iSize = CodeConvert("GB18030", (LPSTR)lpGb18030Char, iSize, 
		"UNICODE", (LPSTR)_lpChar, iCount);

	return _lpChar;
}

#endif

LPWSTR
CCodeConver::AnsiToUTF8 (LPCSTR lpChar, int *lpSize)
{
	LPWSTR lpUnicodeChar = AnsiToUnicode (lpChar, lpSize);
	LPWSTR lpUtf8Char = UnicodeToUTF8 (lpUnicodeChar, lpSize);

	return lpUtf8Char;
}

LPWSTR CCodeConver::UnicodeBigEndianToUnicode( LPCWSTR lpwChar, int iSize )
{
	if (iSize == 0 && lpwChar == NULL)
	{
		return NULL;
	}
	int nCount = iSize + 1;
	if (_lpUnicodeBigEndian)
	{
		delete []_lpUnicodeBigEndian;
	}
	_lpUnicodeBigEndian = new WCHAR[nCount];
	memset (_lpUnicodeBigEndian, 0, sizeof (WCHAR) * nCount);
	_swab((LPSTR)lpwChar, (LPSTR)_lpUnicodeBigEndian, iSize);
	return _lpUnicodeBigEndian;
}


BOOL IsUTF8 (const char * pzInfo)
{
#ifdef _MACRO_OS_WIN32
	int nWSize = MultiByteToWideChar (CP_UTF8, MB_ERR_INVALID_CHARS, pzInfo, -1, NULL, 0);
	if (GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
	{
		return FALSE;
	}
#else
    int nWSize = CodeConvert("utf-8", (LPSTR)pzInfo, -1, "unicode", NULL, 0);
    if (nWSize < 0 && errno == EILSEQ)
    {
        return FALSE;
    }
#endif	
	
	return TRUE;
}

bool IsGBK (const char *pzInfo)
{
#ifdef _MACRO_OS_WIN32
	// 936
	int nWSize = MultiByteToWideChar (CP_ACP, MB_ERR_INVALID_CHARS, pzInfo, -1, NULL, 0);
	if (GetLastError() == ERROR_NO_UNICODE_TRANSLATION)
	{
		return FALSE;
	}
	//判断是否是CP_UTF8,只要把936用CP_UTF8代替即可.
#else
    int nWSize = CodeConvert("gb18030", (LPSTR)pzInfo, -1, "unicode", NULL, 0);
    if (nWSize < 0 && errno == EILSEQ)
    {
        return FALSE;
    }
#endif	

	return TRUE;
}

std::string AnsiOrUTF8ToAnsi (LPCSTR lpUTF8)
{
	std::string strResult;
	LPSTR lpValue = NULL;

#ifdef _MACRO_OS_WIN32

	CCodeConver codeVer;
	if (IsUTF8 (lpUTF8))
	{
		lpValue = codeVer.UTF8ToAnsi((WCHAR*)lpUTF8);
	}

#else

	CCodeConver codeVer;
	lpValue = codeVer.LinuxUTF8ToAnsi(lpUTF8);

#endif

	if (NULL != lpValue)
	{
		strResult = lpValue;
		return strResult;
	}

	return lpUTF8;
}

char CharToInt(char ch)
{
	if (ch>='0' && ch<='9') return (char)(ch-'0');
	if (ch>='a' && ch<='f') return (char)(ch-'a'+10);
	if (ch>='A' && ch<='F') return (char)(ch-'A'+10);

	return -1;
}

char StrToBin (char *str)
{
	char tempWord[2];
	char chn;

	tempWord[0] = CharToInt(str[0]);                         //make the B to 11 -- 00001011
	tempWord[1] = CharToInt(str[1]);                         //make the 0 to 0 -- 00000000

	chn = (tempWord[0] << 4) | tempWord[1];                //to change the BO to 10110000

	return chn;
}

std::string UrlGB2312Decode(std::string &str)
{
	std::string strOutput="";
	char tmp[2];
	int i=0,idx=0,ndx,len=str.length();

	while(i<len)
	{
		if(str[i]=='%')
		{
			if (i < len-2)
			{
				tmp[0]=str[i+1];
				tmp[1]=str[i+2];
				strOutput += StrToBin(tmp);
				i=i+3;
			}
			else
			{
				break;
			}
		}
		else if(str[i]=='+')
		{
			strOutput+=' ';
			i++;
		}
		else
		{
			strOutput+=str[i];
			i++;
		}
	}

	return strOutput;
}
