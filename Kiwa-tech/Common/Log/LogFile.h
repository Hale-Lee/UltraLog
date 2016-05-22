///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogFile
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	文件类型的Log，Log记录到文件中。
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGFILE_H__98C55922_C02C_4804_8D8D_8C48FC227A38__INCLUDED_)
#define AFX_LOGFILE_H__98C55922_C02C_4804_8D8D_8C48FC227A38__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecord.h"
class CCriticalSection;

#include <afxtempl.h>

class CLogFile : public CLogRecord  
{
public:

	VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo);
	VOID AddRecord(LogType type, LPCTSTR lpszFormat, ...) ;
/*
	VOID Info( LPCTSTR lpszFormat, ...)  ;
	VOID Warning( LPCTSTR lpszFormat, ...) ;
	VOID Error( LPCTSTR lpszFormat, ...) ;
	VOID Fatal( LPCTSTR lpszFormat, ...) ;
*/
	BOOL Finalize();
	BOOL Initialize(LPCSTR szConfigFile = NULL);
	CLogFile(LPCTSTR szFileName);
	virtual ~CLogFile();


private:
	CString m_szFileName;
	
	CFile *m_pFileHandle;

	CStringArray  m_AryLog;

	// 临界取区，用于数据同步。
	CCriticalSection *m_pSection;

	BOOL m_bOpened;

};

#endif // !defined(AFX_LOGFILE_H__98C55922_C02C_4804_8D8D_8C48FC227A38__INCLUDED_)
