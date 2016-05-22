///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogBuilder
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	Log文件的创建器，继承了CLogRecord，同时又包含多个CLogRecord对象。
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGBUILDER_H__CFC411D1_C4A0_4760_92F8_1A72CE1BEAAC__INCLUDED_)
#define AFX_LOGBUILDER_H__CFC411D1_C4A0_4760_92F8_1A72CE1BEAAC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecord.h"
#include <afxtempl.h>



class CLogBuilder : public CLogRecord  
{
protected:

	CLogBuilder();
public:

	virtual ~CLogBuilder();

	VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) ;
	VOID AddRecord(LogType type, LPCTSTR lpszFormat, ...) ;
/*
	VOID Info( LPCTSTR lpszFormat, ...)  ;
	VOID Warning( LPCTSTR lpszFormat, ...) ;
	VOID Error( LPCTSTR lpszFormat, ...) ;
	VOID Fatal( LPCTSTR lpszFormat, ...) ;
*/

	static CLogRecord * GetInstance();
	static void ClearInstance();

	BOOL Initialize(LPCSTR szConfigFile = NULL);
	BOOL Finalize() ;

	void AddNewOutput( ILog * pNewLog);

private:
	
	CArray< ILog *, ILog * > m_LogArray;

};

#endif // !defined(AFX_LOGBUILDER_H__CFC411D1_C4A0_4760_92F8_1A72CE1BEAAC__INCLUDED_)
