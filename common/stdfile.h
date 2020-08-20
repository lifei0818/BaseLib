#ifndef BASELIB_STDFILE_H
#define BASELIB_STDFILE_H

#include "common/sys_incl.h"
#include "common/template.h"


namespace BaseLib
{

/************************************************************************/
/*	���ļ���ȡ���ݵ� vector,һ��Ϊһ����λ
/*	@Param	Vectors �ɹ����������
/*	@Param	bDelete ��ȡ��Ϻ��Ƿ�ɾ��
/************************************************************************/

/// ����㷨
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

/// д�ļ�ʱ�����ݼ��� (ʹ�� ����㷨)
BOOL AppendFileDataNeedEncrypTion (LPCSTR lpFile, LPCSTR lpData);

#if 0
/// md5�ļ�
#define MD5_INI_FILE "md5.ini"
/// md5�ļ���md5ֵ
#define MD5_CRC_FILE "md5.dat"

/// ��ȡ�ļ���md5,����ļ������ڷ��ؿ�
std::string Md5File (LPCSTR lpFile, unsigned int interval = 600);

/// ʹ��md5�ļ�У���ļ�,���md5�ļ��ڲ����ڸ��ļ���md5����ҪУ����ļ������ڶ�������
BOOL Md5VerityFile (LPCSTR lpIniFile, LPCSTR lpFile);

/// У���ļ���md5,ʹ�ø��ļ�Ŀ¼�µ�md5�ļ�
BOOL Md5VerityFile (LPCSTR lpFile);

/// У��md5.ini�ļ�
BOOL Md5VerityIni (LPCSTR lpDirectory);

/// У��Ŀ¼�������ļ���md5��ʹ�ø�Ŀ¼�µ�md5�ļ�
BOOL Md5VerityDirectory (LPCSTR lpDirectory);

/// У��Ŀ¼�������ļ���md5��ʹ�ø�Ŀ¼�µ�md5�ļ���������У��δͨ�����ļ��б�(��;�ָ�)
BOOL Md5VerityDirectory (LPCSTR lpDirectory, std::string &strBadFile);

#endif

/// �ж��ļ��Ƿ���ڣ���дȨ��
//  �ļ��Ƿ����
#define FILE_EXIST_ACCESS_EX   0
//  дȨ��
#define FILE_WRITE_ACCESS_EX   2
//  ��Ȩ��
#define FILE_READ_ACCESS_EX    4
//  ��дȨ��
#define FILE_ALL_ACCESS_EX     6

// ����ֵ: 
//		0 Ϊӵ��Ȩ�޻��ļ�����
//		-1 Ϊ��ӵ�л��ļ�������
INT FileAccess ( LPCSTR lpFilePath, INT nAccess =  FILE_EXIST_ACCESS_EX );

/// �鿴�ƶ�·���ļ��Ƿ����
BOOL IsFileExist (LPCSTR lpDirectory);

/// �����ļ���, ���� ���� c:\aa\bb.txt, ��� bb �� txt
//BOOL AnalyFileSuffix (
//					  IN LPCSTR lpFileName, 
//					  OUT std::string& strFilePrifix,
//					  OUT std::string& strFileSuffix
//					  );

}
#endif
