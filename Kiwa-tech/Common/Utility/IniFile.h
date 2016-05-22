/////////////////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CIniFile
//	Version: 	1.0
//	Date: 		2012-01-09
//	
//	Purpose:	Ini文件的标准输入输出类，用来读入Ini文件和写入Ini文件。
//	
/////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_INIFILE_H__99B07947_FEDA_4970_ACBA_B2B6578CD9C8__INCLUDED_)
#define AFX_INIFILE_H__99B07947_FEDA_4970_ACBA_B2B6578CD9C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>



class CIniFile : public CObject  
{
	enum SectionType
	{
		IsSection =0,
		IsItem = 1,
		IsComment =2,
	};

	struct Section
	{
		CString SectionName;
		CString Item;
		CString Value;
		CString Comment;
		SectionType Type;
	};

public:

	BOOL Flush();
	BOOL WriteValue(LPCTSTR szSection, LPCTSTR szItem, LPCTSTR szValue, LPCTSTR szComment = NULL);
	CString Find(LPCTSTR szSection, LPCTSTR szItem);
	BOOL Close();
	BOOL Open(BOOL isReadOnly = FALSE);
	CIniFile(LPCTSTR szFileName);
	virtual ~CIniFile();

private:

	CString m_szFileName;
	CString m_szContent;

	CArray< Section *, Section *> m_AryLine;

	CFile *m_pTxtFile;
	BOOL m_bOpened;

	BOOL m_bIsReadOnly;

public:
	static bool IsFileExist(CString szFileName);
	static void CopyFileTo(CString szDestFileName, CString szSrcFileName);
	static CString GetWinSysPath(void);
	static CString GetAppPath(void);
	int GetKeyVal(CString szSectionName, CString szKeyName, int ipDefault);
	CString GetKeyVal(CString szSectionName, CString szKeyName, LPCTSTR szDefault);
};

#endif // !defined(AFX_INIFILE_H__99B07947_FEDA_4970_ACBA_B2B6578CD9C8__INCLUDED_)
