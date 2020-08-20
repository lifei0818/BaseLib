#include "common/sys_incl.h"
#include "common/macro.h"
#include "common/tolists.h"
#include "common/stdfile.h"
#include "common/debug.h"
#include "common/os.h"
//#include <boost/tokenizer.hpp>
//#include <boost/lexical_cast.hpp>
//#include <boost/algorithm/string.hpp>
//#include <xhash>

namespace BaseLib
{
template<class T, class S>
T
SplitString (const std::string &szString,
             LPCSTR lpChar,
             S s)
{
    T t;
    std::string::size_type start = 0;
    std::string::size_type end = 0;
    while ((end = szString.find(lpChar, start)) != std::string::npos)
    {
        if (end != start)
        {
            std::string szItem = szString.substr(start, end - start);
            s (t, szItem);
        }
        start = end + strlen(lpChar);
    }
    //最后一项可能没有分隔符
    if (start < szString.length())
    {
        std::string szItem = szString.substr(start);
        s (t, szItem);
    }
    return t;
}

template<class T>
struct _tag_split_string
{
    void operator()(T &t, std::string &szItem)
    {
        t.push_back (szItem);
    }
};

//  把字符串按分割符转换为vector
VECTOR_STRING
ToVectors (const std::string &szString,
           LPCSTR lpChar)
{
    return SplitString <VECTOR_STRING>(szString, lpChar, _tag_split_string<VECTOR_STRING>());
}

//  把字符串按分割符转换为list
LIST_STRING
ToLists(const std::string &szString,
        LPCSTR lpChar)
{
    return SplitString <LIST_STRING>(szString, lpChar, _tag_split_string<LIST_STRING>());
}

template<class T>
struct _tag_split_string_set
{
	void operator()(T &t, std::string &szItem)
	{
		t.insert (szItem);
	}
};
//  把字符串按分割符转换为set
SET_STRING
ToSets(const std::string &szString,
		LPCSTR lpChar)
{
	return SplitString <SET_STRING>(szString, lpChar, _tag_split_string_set<SET_STRING>());
}

//  把字符串按分割符转换为DWIRD list
struct _tag_split_string_to_number
{
    void operator()(LIST_DWORD &t, std::string &szItem)
    {
        DWORD dwNum = 0;
//        sscanf (szItem.c_str(), "%lu", &dwNum);
        dwNum = StringToUINT32(szItem);
        t.push_back (dwNum);
    }
};

LIST_DWORD
ToNumLists(const std::string &szString,
           LPCSTR lpChar)
{
    return SplitString <LIST_DWORD>(szString, lpChar, _tag_split_string_to_number());
}

//  把字符串按转换为DWIRD list (16进制)
struct _tag_split_string_to_hexnumber
{
    void operator()(LIST_DWORD &t, std::string &szItem)
    {
        DWORD dwNum = 0;
        sscanf (szItem.c_str(), "%x", &dwNum);
        t.push_back (dwNum);
    }
};

//  把字符串按转换为DWIRD list (可以识别 16进制)
struct _tag_split_string_to_numexnumber
{
    void operator()(LIST_DWORD &t, std::string &szItem)
    {
        DWORD dwNum = 0;
        if (strncmp (szItem.c_str(), "0x", 2) == 0)
        {
            sscanf (szItem.c_str(), "%x", &dwNum);
        }
        else
        {
//            sscanf (szItem.c_str(), "%lu", &dwNum);
            dwNum = BaseLib::StringToUINT32(szItem);
        }
        t.push_back (dwNum);
    }
};

LIST_DWORD
ToNumExLists(const std::string &szString,
           LPCSTR lpChar)
{
    return SplitString <LIST_DWORD>(szString, lpChar, _tag_split_string_to_numexnumber());
}

LIST_DWORD
ToHexNumLists(const std::string &szString,
             LPCSTR lpChar)
{
    return SplitString <LIST_DWORD>(szString, lpChar, _tag_split_string_to_hexnumber());
}

struct _tag_split_string_to_ipaddrsets
{
    void operator()(SET_DWORD &t, std::string &szItem)
    {
        try
        {
            DWORD dwIPAddr = inet_addr (szItem.c_str());
            t.insert (dwIPAddr);
        }
        catch (...)
        {
        }
    }
};

SET_DWORD
ToIPSets(IN const std::string &szString,
          IN LPCSTR lpChar)
{
    return SplitString <SET_DWORD>(szString, lpChar, _tag_split_string_to_ipaddrsets());
}

struct _tag_split_string_to_ipaddr
{
    void operator()(LIST_DWORD &t, std::string &szItem)
    {
        try
        {
            DWORD dwIPAddr = inet_addr (szItem.c_str());
            t.push_back (dwIPAddr);
        }
        catch (...)
        {
        }
    }
};

LIST_DWORD
ToIPLists(IN const std::string &szString,
          IN LPCSTR lpChar)
{
    return SplitString <LIST_DWORD>(szString, lpChar, _tag_split_string_to_ipaddr());
}

//  把字符串按分割符转换为map表
template<class T>
struct _tag_split_string_to_map
{
    void operator()(T &t, std::string &szText)
    {
        for (size_t i=0; i<szText.length(); i++)
        {
            if (_T('=') == szText[i])
            {
                std::string szItem = szText.substr (0, i);
                //boost::trim_left (szItem);
                BaseLib::StringTrimLeft(szItem);
                std::string szValue = szText.substr (i+1, szText.length());
                t.insert (typename T::value_type (szItem, szValue)) ;
                break;
            }
        }
    }
};

//  把字符串按分割符转换为map表
struct _tag_split_string_to_numbermap
{
    void operator()(MAP_DWORD &t, std::string &szText)
    {
        for (size_t i=0; i<szText.length(); i++)
        {
            if (_T('=') == szText[i])
            {
                std::string szItem = szText.substr (0, i);
                //boost::trim_left (szItem);
                BaseLib::StringTrimLeft(szItem);
                std::string szValue = szText.substr (i+1, szText.length());
                DWORD dwValue = 0;
//                sscanf (szValue.c_str(), "%u", &dwValue);
                dwValue = StringToUINT32(szValue);
                t.insert (MAP_DWORD::value_type (szItem, dwValue));
                break;
            }
        }
    }
};

/************************************************************************/
/*
/************************************************************************/

MULTIMAP_STRING
ToMultiMaps(const std::string &szString,
						LPCSTR lpChar)
{
	return SplitString <MULTIMAP_STRING>(szString, lpChar, _tag_split_string_to_map<MULTIMAP_STRING>());
}

INT
CMaps::ToMaps (IN const std::string &szString,
    IN LPCSTR lpChar)
{
    m_maps = SplitString <MAP_STRING>(szString, lpChar, _tag_split_string_to_map<MAP_STRING>());
    return m_maps.size();
}

BOOL
CMaps::GetValue (IN OUT std::string &szString)
{
    MAP_STRING::iterator iter = m_maps.find (szString);
    if (iter == m_maps.end())
    {
        return FALSE;
    }
    szString = (*iter).second;
    return TRUE;
}

BOOL 
CMaps::GetValue (IN LPCSTR lpKey, OUT std::string& strValue)
{
	if (INVALID_PARAM_LPSTR (lpKey))
	{
		return FALSE;
	}

	strValue = lpKey;
	BOOL bRetern = GetValue (strValue);
	
	if (!bRetern)
	{
		strValue = _T ("");
	}

	return bRetern;
}

BOOL
CMaps::GetValue (IN OUT std::string &szString, int iIndex)
{
    char szBuf[64] = {0};
    sprintf (szBuf, "%d", iIndex);
    szString += szBuf;
    return GetValue (szString);
}

MAP_STRING&
CMaps::GetMaps ()
{
    return m_maps;
}

std::string
CMaps::FormatMaps ()
{
    std::ostringstream o;
    for (MAP_STRING::iterator iter = m_maps.begin();
        iter != m_maps.end();
        iter++)
    {
        o << NEWLINE_FORMAT_OUT ((*iter).first, (*iter).second);
    }

    return o.str().c_str();
}

INT
CNumberMaps::ToMaps (IN const std::string &szString,
               IN LPCSTR lpChar)
{
    m_maps = SplitString <MAP_DWORD>(szString, lpChar, _tag_split_string_to_numbermap());
    return m_maps.size();
}

BOOL
CNumberMaps::GetValue (std::string strKey, OUT DWORD &dwValue)
{
    MAP_DWORD::iterator iter = m_maps.find (strKey);
    if (iter == m_maps.end())
    {
        return FALSE;
    }
    dwValue = (*iter).second;
    return TRUE;
}

BOOL
CNumberMaps::GetValue (std::string strKey, int iIndex, OUT DWORD &dwValue)
{
    char szBuf[64] = {0};
    sprintf (szBuf, "%d", iIndex);
    strKey += szBuf;
    return GetValue (strKey, dwValue);
}

MAP_DWORD&
CNumberMaps::GetMaps ()
{
    return m_maps;
}

std::string
CNumberMaps::FormatMaps ()
{
    std::ostringstream o;
    for (MAP_DWORD::iterator iter = m_maps.begin();
        iter != m_maps.end();
        iter++)
    {
        o << NEWLINE_FORMAT_OUT ((*iter).first, (*iter).second);
    }

    return o.str().c_str();
}

/************************************************************************/
/* 
/************************************************************************/

DWORD MyHash (LPCSTR lpString)
{
    DWORD h;
    UCHAR *p;
    for (h = 0, p = (UCHAR*)lpString; *p; p++)
    {
        h = 31 * h + *p;
    }
    return h;
}

DWORD MyHash2 (LPCSTR lpString)
{
    size_t h = 2166136261U;
    UCHAR *p;

    for (p = (UCHAR*)lpString; *p; p++)
    {
        h = 16777619U * h ^ (size_t)*p;
    }
    return h;
}

DWORD
GetUniqueId (LPCSTR lpData)
{
#ifdef _MACRO_OS_WIN32
	DWORD dwValue1 = MyHash2 (lpData);
	DWORD dwValue2 = MyHash (lpData);
#else
	u_int32_t dwValue1 = MyHash2 (lpData);
	u_int32_t dwValue2 = MyHash (lpData);
#endif
    return ((dwValue2 & 0x0000ffff) | (dwValue1 & 0xffff0000));
}

void StringReplaceAll(std::string &strIn, const std::string &strSrc, const std::string &strDest)
{
    std::string::size_type pos = 0;
    std::string::size_type srcLen = strSrc.size();
    std::string::size_type destLen = strDest.size();
	DWORD t = 0;
    while ((pos = strIn.find(strSrc, pos)) != std::string::npos)
    {
        strIn.replace(pos, srcLen, strDest);
        pos += destLen;
		if (++t % 8 == 0)
		{
			BaseLib::OS::msleep(1);
		}
			
    }
}

void StringReplaceAll( std::wstring &strIn, const std::wstring &strSrc, const std::wstring &strDest )
{
	std::wstring::size_type pos = 0;
	std::wstring::size_type srcLen = strSrc.size();
	std::wstring::size_type destLen = strDest.size();
	DWORD t = 0;
	while ((pos = strIn.find(strSrc, pos)) != std::wstring::npos)
	{
		strIn.replace(pos, srcLen, strDest);
		pos += destLen;
		if (++t % 8 == 0)
		{
			BaseLib::OS::msleep(1);
		}

	}
}

void
StringToLower(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
}

void
StringToUpper(std::string &str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
}

UINT32
StringToUINT32(const std::string &str)
{
    UINT32 dwNum = 0;
    sscanf (str.c_str(), "%u", &dwNum);
    return dwNum;
}

}
