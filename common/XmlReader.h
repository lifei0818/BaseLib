#ifndef BASELIB_XMLREADER_H
#define BASELIB_XMLREADER_H

#include "common/sys_incl.h"
#include "tinyxml/tinyxml.h"

class CXmlReader
{
public:
	CXmlReader (TiXmlNode *lpNode);

	LPCTSTR ReadElementString (LPCTSTR lpItem);
	LPCTSTR ReadElementString (std::string &strKey, int iIndex);

	LPCTSTR ReadAttribString (LPCTSTR lpItem);
	LPCTSTR ReadAttribString (std::string &strKey, int iIndex);

private:
	TiXmlNode *m_lpNode;
};

 namespace BaseLib
 {
/* �������ܣ� ��ȡXML��ʽ�ļ��еĽڵ���Ϣ 
   
   ������ std::string strFileContent��XML��ʽ�ļ�����; 
		  std::string strElementName���ڵ���;���������У�a, b��	
   ����ֵ���ڵ����� �������У�text1,text2��
--------------------------------------------------------------
   ����: <A>   
            <a>text1</a>
            <b>text2</b>
		 </A>
*/
std::string GetElementText(std::string strFileContent,std::string strElementName);

}

#endif
