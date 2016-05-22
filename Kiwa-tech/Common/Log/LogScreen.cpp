///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogScreen
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	����Ļ��дLog��Ҳ���ǰ�Logֱ�������DlgMessage�ϡ�
//	
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogScreen.h"
#include "..\\..\\Common\\Utility\\Utility.h"
#include "LogConfig.h"
#include "MsgListBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LPCTSTR  ListBoxClassName  = "MessageListBox";
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogScreen::CLogScreen(LPCTSTR ListBoxTitle)
{
	m_szListBoxTitle = ListBoxTitle;
	m_pListBox = NULL;
}

CLogScreen::~CLogScreen()
{

}

BOOL CLogScreen::Initialize()
{

	HWND  hListBox = ::FindWindow(ListBoxClassName,m_szListBoxTitle);
	if( hListBox != NULL)
	{
		m_pListBox = new CMsgListBox();
		m_pListBox->Attach(hListBox);
	}

	return TRUE;
}

BOOL CLogScreen::Finalize()
{
	if(m_pListBox !=NULL)
	{
		m_pListBox->Detach();
	}

	delete m_pListBox;
	m_pListBox = NULL;
	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  дһ��Log����Ļ����ȥ��
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
VOID CLogScreen::AddRecord(LPCTSTR szMessage, LogType type)
{
	if(!Initialize())
	{
		return ;
	}

	CLogConfig *pConfig = CLogConfig::GetInstance();

	if(pConfig->GetScreenLogLevel() > type)
	{
		return;
	}
	
	if( !pConfig->IsOutput2Screen ())
	{
		return;
	}

	CString szLog = "";
	szLog.Empty();

	switch (type)
	{
	case CLogRecord::logInfo:
		szLog += "Info   ";
		break;
	case CLogRecord::logWarning:
		szLog += "Warning";
		break;
	case CLogRecord::logError:
		szLog += "Error  ";
		break;
	case CLogRecord::logFatal:
		szLog += "Fatal  ";
		break;
	case CLogRecord::logDebug:
		szLog += "Debug  ";
		break;
	default:
		szLog += "Info   ";
		break;
	}

	szLog += "    ";		//4���ո�

	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(TimeFormat);
	szLog += szTime;
	szLog += "    ";		//4���ո�

	szLog += szMessage;
	
	if( m_pListBox != NULL)
	{
		m_pListBox->AddString(szLog);
		
		//����������õ���������������ô���ListBox�����ݡ�
		if( m_pListBox->GetCount() >= pConfig->GetScreenLogSize())
		{
			m_pListBox->ResetContent();
		}
		//����õ����һ�У�����������ɹ���Ч����
		m_pListBox->SetCurSel( m_pListBox->GetCount() -1 );
	}
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  дһ��Log���ļ��С�
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
VOID CLogScreen::AddRecord(LogType type, LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	
	CString szMessage;
	szMessage.FormatV(lpszFormat, argList);
	va_end(argList);

	AddRecord(szMessage, type);
}
