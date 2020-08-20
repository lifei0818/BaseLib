#ifndef BASELIB_STDFILE_H
#define BASELIB_STDFILE_H

#include "common/sys_incl.h"
#include "common/template.h"


namespace BaseLib
{

/************************************************************************/
/*	从文件读取数据到 vector,一行为一个单位
/*	@Param	Vectors 成功读入的数据
/*	@Param	bDelete 读取完毕后是否删除
/************************************************************************/

/// 异或算法
void XorData (IN OUT UCHAR* lpBuffer, int iLen);

BOOL
ReadFileToList (IN LPCSTR lpFileName, 
				OUT LIST_STRING &List,
				IN BOOL bDelete = FALSE);

std::string
ReadFileToString (LPCSTR lpFileName);

BOOL ReadFileData (LPCSTR lpFile, TArrayString &arrayString);

BOOL WriteFileData (LPCSTR lpFile, LPCSTR lpData);

BOOL AppendFileBinData (LPCSTR lpFile, LPVOID lpData, int iLen);

BOOL AppendFileData (LPCSTR lpFile, LPCSTR lpData);

/// 写文件时对数据加密 (使用 异或算法)
BOOL AppendFileDataNeedEncrypTion (LPCSTR lpFile, LPCSTR lpData);

#if 0
/// md5文件
#define MD5_INI_FILE "md5.ini"
/// md5文件的md5值
#define MD5_CRC_FILE "md5.dat"

/// 获取文件的md5,如果文件不存在返回空
std::string Md5File (LPCSTR lpFile, unsigned int interval = 600);

/// 使用md5文件校验文件,如果md5文件内不存在该文件的md5或者要校验的文件不存在都返回真
BOOL Md5VerityFile (LPCSTR lpIniFile, LPCSTR lpFile);

/// 校验文件的md5,使用该文件目录下的md5文件
BOOL Md5VerityFile (LPCSTR lpFile);

/// 校验md5.ini文件
BOOL Md5VerityIni (LPCSTR lpDirectory);

/// 校验目录内所有文件的md5，使用该目录下的md5文件
BOOL Md5VerityDirectory (LPCSTR lpDirectory);

/// 校验目录内所有文件的md5，使用该目录下的md5文件，并返回校验未通过的文件列表(以;分隔)
BOOL Md5VerityDirectory (LPCSTR lpDirectory, std::string &strBadFile);

#endif

/// 判断文件是否存在，读写权限
//  文件是否存在
#define FILE_EXIST_ACCESS_EX   0
//  写权限
#define FILE_WRITE_ACCESS_EX   2
//  读权限
#define FILE_READ_ACCESS_EX    4
//  读写权限
#define FILE_ALL_ACCESS_EX     6

// 返回值: 
//		0 为拥有权限或文件存在
//		-1 为不拥有或文件不存在
INT FileAccess ( LPCSTR lpFilePath, INT nAccess =  FILE_EXIST_ACCESS_EX );

/// 查看制定路径文件是否存在
BOOL IsFileExist (LPCSTR lpDirectory);

/// 解析文件名, 例如 输入 c:\aa\bb.txt, 输出 bb 和 txt
//BOOL AnalyFileSuffix (
//					  IN LPCSTR lpFileName, 
//					  OUT std::string& strFilePrifix,
//					  OUT std::string& strFileSuffix
//					  );

}
#endif
