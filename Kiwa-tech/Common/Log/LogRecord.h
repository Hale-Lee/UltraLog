///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogRecord
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	Log对象的基类，定义了一串Log操作的接口。
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGRECORD_H__4689EE5D_04A9_49FE_A0AB_55A662BF92E8__INCLUDED_)
#define AFX_LOGRECORD_H__4689EE5D_04A9_49FE_A0AB_55A662BF92E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ILog.h"

class  CLogRecord : public ILog
{

protected:
	CLogRecord() {m_bInitialized = FALSE;};

private:

	BOOL m_bInitialized;
public:
	virtual ~CLogRecord(){};

	virtual VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) = NULL;
	virtual VOID AddRecord(LogType type , LPCTSTR lpszFormat, ...) = NULL;
/*
	virtual VOID Info( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Warning( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Error( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Fatal( LPCTSTR lpszFormat, ...) = NULL;
*/
	virtual BOOL Initialize(LPCSTR szConfigFile = NULL) {m_bInitialized = TRUE; return TRUE;};
	virtual BOOL Finalize() {m_bInitialized = FALSE; return TRUE;};

	virtual BOOL IsInitialzed() {return m_bInitialized;}



};

extern CLogRecord *theLog ;


#endif // !defined(AFX_LOGRECORD_H__4689EE5D_04A9_49FE_A0AB_55A662BF92E8__INCLUDED_)
