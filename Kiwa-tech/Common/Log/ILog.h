///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		ILog
//	Version: 	1.0
//	Date: 		2015-02-09
//	
//	Purpose:	DLL导出类的接口，用来声明这些对象是怎么实现的。
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_ILOG_H__7AA2B229_5D1B_45BB_933D_C4A5A2525F09__INCLUDED_)
#define AFX_ILOG_H__7AA2B229_5D1B_45BB_933D_C4A5A2525F09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef AFX_LOGDLL_EXPORT 
#define LOGDLL_DECLARE  __declspec(dllimport) 
#else
#define LOGDLL_DECLARE  __declspec(dllexport) 
#endif

interface  ILog 
{
public :
	enum LogType
	{
		logInfo = 1, 
		logWarning = 2,
		logError = 3,
		logFatal = 4,
		logDebug = 0,
	};

public:
	virtual VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) = NULL;
	virtual VOID AddRecord(LogType type , LPCTSTR lpszFormat, ...) = NULL;
	virtual ~ILog(){};
/*
	virtual VOID Info( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Warning( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Error( LPCTSTR lpszFormat, ...) = NULL;
	virtual VOID Fatal( LPCTSTR lpszFormat, ...) = NULL;
*/
	virtual BOOL Initialize(LPCSTR szConfigFile = NULL) = NULL;
	virtual BOOL Finalize() = NULL;
	virtual BOOL IsInitialzed() = NULL;

	static LogType GetLogLevel(LPCSTR lpszLevel)
	{
		//static char *szAllLevel[] = {"debug", "info". "warning", "error", "fatal"};

		CString szLevel(lpszLevel);
		szLevel.MakeLower();

		LogType mtReturn;
		if(szLevel == "info")
		{
			mtReturn = logInfo;
		}else if( szLevel =="warning" )
		{
			mtReturn = logWarning;

		}else if( szLevel == "error")
		{
			mtReturn = logError;
		}else if (szLevel == "fatal")
		{
			mtReturn = logFatal;
		}else if( szLevel == "debug")
		{
			mtReturn = logDebug;
		}else
		{
			mtReturn = logInfo;
		}

		return mtReturn;
	}

};

extern "C" LOGDLL_DECLARE ILog *  GetLogInstance();
extern "C" LOGDLL_DECLARE void AddNewLog(ILog * pNewLog);
extern "C" LOGDLL_DECLARE void RemoveLogInstance();


#endif // !defined(AFX_ILOG_H__7AA2B229_5D1B_45BB_933D_C4A5A2525F09__INCLUDED_)
