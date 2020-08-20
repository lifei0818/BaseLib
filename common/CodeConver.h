#ifndef CODECONVER_H
#define CODECONVER_H

#include "common/sys_incl.h"

class CCodeConver
{
public:
	CCodeConver ();
	~CCodeConver ();

	LPSTR UTF8ToAnsi (LPCWSTR lpUtf8Char, int *lpSize = NULL);
	LPWSTR AnsiToUTF8 (LPCSTR lpChar, int *lpSize = NULL);

	LPWSTR UTF8ToUnicode (LPCWSTR lpUtf8Char, int *lpSize = NULL);
	LPWSTR UnicodeToUTF8 (LPCWSTR lpwChar, int *lpSize = NULL);

	LPWSTR UnicodeBigEndianToUnicode (LPCWSTR lpwChar, int lpSize);

	LPSTR UnicodeToAnsi (LPCWSTR lpUnicodeChar, int *lpSize = NULL);
	LPWSTR AnsiToUnicode (LPCSTR lpChar, int *lpSize = NULL);

	LPCSTR Char() { return _lpChar; }
	LPWSTR UnicodeChar() { return _lpUnicodeChar; }
	LPWSTR Utf8Char() { return _lpUtf8Char; }

	void Char(LPSTR lpChar){ _lpChar = lpChar;}
	void UnicodeChar(LPWSTR lpUnicodeChar) { _lpUnicodeChar = lpUnicodeChar;}
	void Utf8Char(LPWSTR lpUtf8Char) { _lpUtf8Char = lpUtf8Char;}

#ifndef _MACRO_OS_WIN32
	LPSTR LinuxUTF8ToAnsi (LPCSTR lpUtf8Char, int *lpSize = NULL);

	LPSTR LinuxUTF8ToGB18030 (LPCSTR lpUtf8Char, int *lpSize = NULL);
	LPSTR LinuxGB18030ToUTF8 (LPCSTR lpGb18030Char, int *lpSize = NULL);

	LPSTR LinuxUnicodeToGB18030 (LPCSTR lpUnicodeChar, int *lpSize = NULL);
	LPSTR LinuxGB18030ToUnicode (LPCSTR lpGb18030Char, int *lpSize = NULL);
#endif

private:
	LPSTR _lpChar;
	LPWSTR _lpUnicodeChar;
	LPWSTR _lpUnicodeBigEndian;
	LPWSTR _lpUtf8Char;
};

std::string AnsiOrUTF8ToAnsi (LPCSTR lpUTF8);
std::string UrlGB2312Decode(std::string &str);


#endif
