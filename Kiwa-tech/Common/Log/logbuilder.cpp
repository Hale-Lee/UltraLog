///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogBuilder
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	Log�ļ��Ĵ��������̳���CLogRecord��ͬʱ�ְ������CLogRecord����
//              ����ļ����Ų�ͬ��ʹ�û��������ʵ����޸ġ�
//	
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogBuilder.h"
#include "LogFile.h"
#include "LogConfig.h"
#include "LogScreen.h"
#include <stdarg.h>		//���̶�����������ͷ�ļ���

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
//  ��ʼ����������LogConfig�ж�ȡ������CLogRecord������Ϣ��ͬʱ�������ǡ�
//  �����ﲻ��ʹ��RTTI�����������Զ���������Log���࣬��Ϊ��������Ĳ�����һ����
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	�����ɹ�����ʧ��
//
//	---------------------------------------------------------------------------
BOOL CLogBuilder::Initialize(LPCSTR szConfigFile /*= NULL */)
{

	if( __super::IsInitialzed())
	{
		return TRUE;
	}

	BOOL bReturn = TRUE;

	//����Log�����ļ���
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
//  ���˻����������б��е�CLogRecord����رյ�������ɾ������ա�
//
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	�����ɹ�����ʧ��
//
//	---------------------------------------------------------------------------
BOOL CLogBuilder::Finalize()
{
	if( !__super::IsInitialzed())
	{
		return TRUE;
	}

	//�ر����е�CLogRecord����ͬʱɾ������
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

	// ɾ��������Ϣ��
	CLogConfig::ClearInstance();

	__super::Finalize();

	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  дһ��Log�����е�CLogRecord��Ҫд��
//
//
// Input:
//	LPCTSTR szMessage			--	Ҫд���Log��Ϣ��
//	LogType type				--	Log���͡�
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
//  дһ��Log�����е�CLogRecord��Ҫд��
//
//
// Input:
//	LPCTSTR lpszFormat			--	Ҫд���Log��Ϣ��
//	LogType type				--	Log���͡�
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
//  ������չ�ã����������µ�Log�����ʽ����Log�ĳ�ʼ������������û����ɡ�
//
//
// Input:
//	CLogRecord *pNewLog			--	��������Log��
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CLogBuilder::AddNewOutput( ILog *pNewLog)
{
	m_LogArray.Add(pNewLog);
}