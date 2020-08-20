#include "common/sys_incl.h"
#include "common/XmlReader.h"
#include "common/macro.h"

CXmlReader::CXmlReader (TiXmlNode *lpNode)
	: m_lpNode (lpNode)
{
}

LPCTSTR
CXmlReader::ReadElementString (LPCTSTR lpItem)
{
	if (m_lpNode)
	{
		for (TiXmlNode *lpNode = m_lpNode;
			lpNode;
			lpNode = lpNode->NextSibling ())
		{
			std::string strItem = lpNode->Value();
			TiXmlNode* lpSubNode = lpNode->FirstChild();

			if (stricmp (strItem.c_str(), lpItem) == 0)
			{
				if (lpSubNode)
				{
					return lpSubNode->Value ();
				}
			}
		}
	}

	return NULL_STRING;
}

LPCTSTR
CXmlReader::ReadElementString (std::string &strKey, int iIndex)
{
	char szBuf[64] = {0};
	sprintf (szBuf, "%d", iIndex);
	strKey += szBuf;
	return ReadElementString (strKey.c_str());
}

LPCTSTR
CXmlReader::ReadAttribString (LPCTSTR lpItem)
{
	if (m_lpNode)
	{
		TiXmlElement* lpElement = m_lpNode->ToElement();
		if (lpElement)
		{
			for (TiXmlAttribute* lpAttr = lpElement->FirstAttribute();
				lpAttr;
				lpAttr = lpAttr->Next())
			{
				if (stricmp (lpAttr->Name(), lpItem) == 0)
				{
					return lpAttr->Value();
				}
			}
		}
	}

	return NULL_STRING;
}

LPCTSTR
CXmlReader::ReadAttribString (std::string &strKey, int iIndex)
{
	char szBuf[64] = {0};
	sprintf (szBuf, "%d", iIndex);
	strKey += szBuf;
	return ReadAttribString (strKey.c_str());
}

 namespace BaseLib
 {
std::string GetElementText(std::string strFileContent,std::string strElementName)
{
	if( strElementName.empty())
	{
		return "";
	}
	std::string strSubContent("");	
	std::string strBeginNode(strElementName); 
	std::string strEndNode(strElementName); 
	//set begin node identity
	strBeginNode.insert(0,"<");
	//set end node identity
	strEndNode.insert(0,"</");
	strEndNode.push_back('>');
	UINT nBegin = strFileContent.find(strBeginNode);
	//	Sleep(1);
	UINT nEnd = strFileContent.find(strEndNode);
	if (nBegin != -1 && nEnd != -1)
	{
		strSubContent = strFileContent.substr(nBegin + strElementName.length() + 2, nEnd - nBegin - strElementName.length() - 2);
	}
	return strSubContent;
}

}