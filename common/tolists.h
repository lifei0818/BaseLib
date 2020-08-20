#ifndef TOLIST_H
#define TOLIST_H

#include "common/sys_incl.h"
#include "common/template.h"

namespace BaseLib
{

//	把字符串按分割符转换为vector
BaseLib::VECTOR_STRING
ToVectors (IN const std::string &szString,
		   IN LPCSTR lpChar);

//	把字符串按分割符转换为list
BaseLib::LIST_STRING
ToLists(IN const std::string &szString,
		IN LPCSTR lpChar);

BaseLib::SET_STRING
ToSets(IN const std::string &szString,
		IN LPCSTR lpChar);

MULTIMAP_STRING
ToMultiMaps(const std::string &szString,
						LPCSTR lpChar);

//	把字符串按分割符转换为DWIRD list
BaseLib::LIST_DWORD
ToNumLists(IN const std::string &szString,
			IN LPCSTR lpChar);

//	把字符串按分割符转换为DWIRD list
BaseLib::LIST_DWORD
ToIPLists(IN const std::string &szString,
		   IN LPCSTR lpChar);

SET_DWORD
ToIPSets(IN const std::string &szString,
          IN LPCSTR lpChar);

//	把字符串按分割符转换为DWIRD list (可以识别 16进制)
BaseLib::LIST_DWORD
ToNumLists(IN const std::string &szString,
		  IN LPCSTR lpChar);

//	把字符串按分割符转换为DWIRD list (认为是16进制)
BaseLib::LIST_DWORD
ToHexNumLists(IN const std::string &szString,
		  IN LPCSTR lpChar);

class CMaps
{
public:
	INT ToMaps (IN const std::string &szString,
		IN LPCSTR lpChar);

	BOOL GetValue (IN OUT std::string &szString);
	BOOL GetValue (IN OUT std::string &szString, int iIndex);

	// BOOL GetValue (IN OUT std::string &szString) 参数分离 
	BOOL GetValue (IN LPCSTR lpKey, OUT std::string& strValue);

	BaseLib::MAP_STRING& GetMaps ();

	virtual std::string FormatMaps ();

protected:
	BaseLib::MAP_STRING m_maps;
};

class CNumberMaps
{
public:
	INT ToMaps (IN const std::string &szString,
		IN LPCSTR lpChar);

	BOOL GetValue (std::string strKey, OUT DWORD &dwValue);
	BOOL GetValue (std::string strKey, int iIndex, OUT DWORD &dwValue);

	BaseLib::MAP_DWORD& GetMaps ();

	virtual std::string FormatMaps ();

protected:
	BaseLib::MAP_DWORD m_maps;
};

DWORD
GetUniqueId (LPCSTR lpData);

template  <typename STRT, typename CHART>
inline
void StringTrimLeftIf(STRT &str, const CHART char_set)
{
    str.erase(0, str.find_first_not_of(char_set));
}

inline
void StringTrimLeft(std::wstring &str)
{
    StringTrimLeftIf(str, L" \t\r\n");
}

inline
void StringTrimLeft(std::string &str)
{
    StringTrimLeftIf(str, " \t\r\n");
}

template  <typename STRT, typename CHART>
inline
void StringTrimRightIf(STRT &str, CHART char_set)
{
    str.erase(str.find_last_not_of(char_set) + 1);
}

inline
void StringTrimRight(std::wstring &str)
{
    StringTrimRightIf(str, L" \t\r\n");
}

inline
void StringTrimRight(std::string &str)
{
    StringTrimRightIf(str, " \t\r\n");
}

template  <typename STRT, typename CHART>
inline
void StringTrimIf(STRT &str, const CHART char_set)   
{
    StringTrimLeftIf(str, char_set);
    StringTrimRightIf(str, char_set);
}

template  <typename STRT>
inline
void StringTrim(STRT &str)               
{
    StringTrimLeft(str);
    StringTrimRight(str);
}

void
StringReplaceAll(std::string &strIn, const std::string &strSrc, const std::string &strDest);
void
StringReplaceAll(std::wstring &strIn, const std::wstring &strSrc, const std::wstring &strDest);

void
StringToLower(std::string &str);

void
StringToUpper(std::string &str);

UINT32
StringToUINT32(const std::string &str);

}
#endif
