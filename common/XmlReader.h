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
/* 函数功能： 获取XML格式文件中的节点信息 
   
   参数： std::string strFileContent，XML格式文件内容; 
		  std::string strElementName，节点名;（如下例中：a, b）	
   返回值：节点内容 （下例中：text1,text2）
--------------------------------------------------------------
   例如: <A>   
            <a>text1</a>
            <b>text2</b>
		 </A>
*/
std::string GetElementText(std::string strFileContent,std::string strElementName);

}

#endif
