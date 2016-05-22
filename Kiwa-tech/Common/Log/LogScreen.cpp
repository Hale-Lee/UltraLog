///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogScreen
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	在屏幕上写Log，也就是把Log直接输出到DlgMessage上。
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
//  写一条Log到屏幕里面去。
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

	szLog += "    ";		//4个空格。

	CTime time = CTime::GetCurrentTime();
	CString szTime = time.Format(TimeFormat);
	szLog += szTime;
	szLog += "    ";		//4个空格。

	szLog += szMessage;
	
	if( m_pListBox != NULL)
	{
		m_pListBox->AddString(szLog);
		
		//如果超过设置的最大输出个数，那么清除ListBox的内容。
		if( m_pListBox->GetCount() >= pConfig->GetScreenLogSize())
		{
			m_pListBox->ResetContent();
		}
		//光标置到最后一行，这样可以造成滚动效果。
		m_pListBox->SetCurSel( m_pListBox->GetCount() -1 );
	}
}

//	---------------------------------------------------------------------------
//
// Function:    AddRecord()
//
// Description:
//  写一条Log到文件中。
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
VOID CLogScreen::AddRecord(LogType type, LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	
	CString szMessage;
	szMessage.FormatV(lpszFormat, argList);
	va_end(argList);

	AddRecord(szMessage, type);
}
