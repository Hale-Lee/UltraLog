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
//              这个文件随着不同的使用环境可以适当的修改。
//	
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogBuilder.h"
#include "LogFile.h"
#include "LogConfig.h"
#include "LogScreen.h"
#include <stdarg.h>		//不固定个数参数的头文件。

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
static CLogRecord *theInstance = NULL;

CLogBuilder::CLogBuilder()
{
	
}

CLogBuilder::~CLogBuilder()
{
}

CLogRecord* CLogBuilder::GetInstance()
{
	if (theInstance == NULL)
	{
		theInstance = new CLogBuilder();
	}

	return theInstance;
}

void CLogBuilder::ClearInstance()
{
	if (theInstance != NULL)
	{
		delete theInstance;
	}

	theInstance = NULL;
}


//	---------------------------------------------------------------------------
//
// Function:    Initialize()
//
// Description:
//  初始化函数，从LogConfig中读取创建的CLogRecord对象信息，同时创建它们。
//  在这里不能使用RTTI技术来做到自动创建各个Log子类，因为各个子类的参数不一样。
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	创建成功或者失败
//
//	---------------------------------------------------------------------------
BOOL CLogBuilder::Initialize(LPCSTR szConfigFile /*= NULL */)
{

	if( __super::IsInitialzed())
	{
		return TRUE;
	}

	BOOL bReturn = TRUE;

	//加载Log配置文件。
	CLogConfig *pConfig = new CLogConfig(szConfigFile); 
	pConfig->Load();

	if( pConfig->IsOutput2File())
	{
		CString szLogFileName = pConfig->GetLogFileName();
		if(szLogFileName.ReverseFind('\\') <= 0)
		{
			TCHAR szPath[MAX_PATH] = {0};
			::GetCurrentDirectory(MAX_PATH,szPath);
			
			CString szFullPath = szPath;
			szFullPath += "\\";
			szFullPath += szLogFileName;
			szLogFileName = szFullPath;
		}
		CLogRecord *pLog = new CLogFile((LPCTSTR)szLogFileName);

		bReturn = bReturn & pLog->Initialize();

		m_LogArray.Add(pLog);
	}

	if( pConfig->IsOutput2Screen() )
	{

	//	CLogRecord *pLog = new CLogScreen( pConfig->GetListBoxName() );
	//	bReturn = bReturn & pLog->Initialize();
	//	m_LogArray.Add(pLog);
	}

	if( pConfig->IsOutput2Mail())
	{

	}

	if( pConfig->IsOutput2SMS())
	{

	}

	__super::Initialize(szConfigFile);

	return bReturn;
}

//	---------------------------------------------------------------------------
//
// Function:    Finalize()
//
// Description:
//  终了化函数，把列表中的CLogRecord对象关闭掉，或者删除，清空。
//
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	创建成功或者失败
//
//	---------------------------------------------------------------------------
BOOL CLogBuilder::Finalize()
{
	if( !__super::IsInitialzed())
	{
		return TRUE;
	}

	//关闭所有的CLogRecord对象，同时删除它。
	ILog *pLog = NULL;
	for( int iLoop =0 ; iLoop < m_LogArray.GetSize(); iLoop++)
	{
		pLog = m_LogArray.GetAt(iLoop);
		if( pLog != NULL)
		{
			pLog->Finalize();
		}
//		AfxMessageBox("CLogBuilder::Finalize()");
		delete pLog;
	}
	m_LogArray.RemoveAll();

	// 删除配置信息。
	CLogConfig::ClearInstance();

	__super::Finalize();

	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  写一条Log，所有的CLogRecord都要写。
//
//
// Input:
//	LPCTSTR szMessage			--	要写入的Log消息。
//	LogType type				--	Log类型。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
VOID CLogBuilder::AddRecord(LPCTSTR szMessage, LogType type /*= logInfo */)
{
	ILog *pLog = NULL;
	for( int iLoop = 0 ; iLoop < m_LogArray.GetSize() ; iLoop++)
	{
		pLog = m_LogArray.GetAt(iLoop);
		pLog->AddRecord(szMessage, type);
	}
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  写一条Log，所有的CLogRecord都要写。
//
//
// Input:
//	LPCTSTR lpszFormat			--	要写入的Log消息。
//	LogType type				--	Log类型。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
VOID CLogBuilder::AddRecord(LogType type, LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	
	CString szMessage;
	szMessage.FormatV(lpszFormat, argList);
	va_end(argList);

	ILog *pLog = NULL;
	for( int iLoop = 0 ; iLoop < m_LogArray.GetSize() ; iLoop++)
	{
		pLog = m_LogArray.GetAt(iLoop);
		pLog->AddRecord(szMessage, type);
	}	

}

//	---------------------------------------------------------------------------
//
// Function:    AddNewOutput()
//
// Description:
//  功能扩展用，用来增加新的Log输出形式。新Log的初始化工作在这里没有完成。
//
//
// Input:
//	CLogRecord *pNewLog			--	输出对象的Log。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CLogBuilder::AddNewOutput( ILog *pNewLog)
{
	m_LogArray.Add(pNewLog);
}