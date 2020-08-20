#include <stdio.h>
#include <iostream>
#include <sstream>
#include "baselib.h"

using namespace std;

void testJson();

int main()
{
	BaseLib::InitBaseLib();
	INIT_DBG_CODE;
	
	char buf[256];
	sprintf_s(buf, "%x,%x", 0x3001, 0x3000);
	//BaseLib::TSingleton<BaseLib::CDbgPrint>::Instance()->SetDebug(buf);
	DBGPRINT(DBG_ERROR,"qweasd");
	DBGPRINT(0x3001,"3001");
	DBGPRINT(0x6666,"6666");
	return 0;
}
void testJson()
{
	/*
	Json::CharReaderBuilder builder;
    std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
	string strInfo("[false,0.3,0.3,0.3,0.03,0.3,0.03]");
	Json::Value m_jsPredefineValue;
	std::string err;
	
	if(reader->parse(strInfo.c_str(),strInfo.c_str()+strInfo.length(), &m_jsPredefineValue, &err))
    {
        return -1;
    }else {
        cout << "Predefined variable resolution failed, error:"<<err<<endl;
    }
	*/
	
	Json::StreamWriterBuilder builder;
    //builder.settings_["indentation"] = "";
    std::unique_ptr<Json::StreamWriter> writeInfo(builder.newStreamWriter());
    Json::Value root, lang;
    root["0"] = false;
    root["id"] = 123;
    lang["first"] = "one";
    lang["last"] = "last";
    root["value"] = lang;
    ostringstream oStr;
    writeInfo->write(root,&oStr);
    string str = oStr.str();
    cout<<str<<endl;

    bool bOK=false;
    Json::Value rootV;
    {
        Json::CharReaderBuilder jsreader;
        std::unique_ptr<Json::CharReader> const readerinfo(jsreader.newCharReader());
        string err;
        int nLen = str.length();
        if (readerinfo->parse(str.c_str(), str.c_str() + nLen, &rootV, &err))
        {
            bOK=true;
        }
    }

    bool bVa = root["0"].asBool();
    if(root["id"].isInt())
    {
        int id = root["id"].asInt();
        cout <<"id="<<id<<endl;
    }
}