///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogConfig
//	Version: 	1.0
//	Date: 		2012-01-09
//	
//	Purpose:	Log配置类,从Ini文件中读取Log配置的各个属性.
//	
///////////////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "LogConfig.h"
#include "..\\..\\Common\\Utility\\IniFile.h"
#include "..\\..\\Common\\Utility\\Utility.h"

static CLogConfig * thisInstance =NULL;

CLogConfig::CLogConfig(LPCTSTR lpszFileName)
{
	m_szIniFile = lpszFileName;
	m_bLoaded = FALSE;
	thisInstance = this;
	m_pIniFile = NULL;
}

CLogConfig::~CLogConfig(void)
{
	if( m_pIniFile != NULL)
	{
		m_pIniFile->Close();
		delete m_pIniFile;
		m_pIniFile = NULL;
	}
}



BOOL CLogConfig::Load()
{
	BOOL bReturn = FALSE;

	if( m_bLoaded)
	{
		return TRUE;
	}

	m_pIniFile = new CIniFile(m_szIniFile);
	bReturn  = m_pIniFile->Open(TRUE);
	if( bReturn)
	{

		// Load Log  Section，查找Log的输出类型。
		CString szOutput = m_pIniFile->Find("Log","Output");
		szOutput.MakeLower();
		m_OutputType.toFile = (szOutput.Find("file") >= 0 );
		m_OutputType.toScreen = (szOutput.Find("screen") >= 0 );
		m_OutputType.toMail = (szOutput.Find("mail") >= 0 );
		m_OutputType.toSMS = (szOutput.Find("sms") >= 0 );

		if( m_OutputType.toFile)
		{
			m_FileDef.FileName = m_pIniFile->Find("FileLog","FileName");

			CString szLevel = m_pIniFile->Find("FileLog","LogLevel");
			szLevel.MakeLower();
			m_FileDef.LogLevel = ILog::GetLogLevel(szLevel);

			m_FileDef.MaxSize = ReadFileLogSize();

			m_FileDef.Remark  = m_pIniFile->Find("FileLog","Remark");
		}
		
		if( m_OutputType.toScreen)
		{
			m_ScreenDef.ListBoxName = m_pIniFile->Find("ScreenLog","ListBoxName");

			CString szLevel = m_pIniFile->Find("ScreenLog","LogLevel");
			szLevel.MakeLower();
			m_ScreenDef.LogLevel = CLogRecord::GetLogLevel(szLevel);

			CString szSize = m_pIniFile->Find("ScreenLog","MaxSize");
			m_ScreenDef.MaxSize = atol(szSize);

			m_ScreenDef.Remark  = m_pIniFile->Find("ScreenLog","Remark");

		}
		
		if( m_OutputType.toMail)
		{
			m_MailDef.SMTPServer = m_pIniFile->Find("MailLog","SMTPServer");

			m_MailDef.User  = m_pIniFile->Find("MailLog","User");

			m_MailDef.Password  = m_pIniFile->Find("MailLog","password");
			
			CString szGroupSize  = m_pIniFile->Find("MailLog","GroupSize");
			m_MailDef.GroupSize = atol(szGroupSize);

			CString szLevel = m_pIniFile->Find("MailLog","LogLevel");
			szLevel.MakeLower();
			m_MailDef.LogLevel = CLogRecord::GetLogLevel(szLevel);

			m_MailDef.Remark  = m_pIniFile->Find("MailLog","Remark");
		}

		if( m_OutputType.toSMS)
		{
			m_SMSDef.MoblieNo  = m_pIniFile->Find("SMSLog","MoblieNo");

			CString szGroupSize  = m_pIniFile->Find("SMSLog","GroupSize");
			m_SMSDef.GroupSize = atol(szGroupSize);

			CString szLevel = m_pIniFile->Find("SMSLog","LogLevel");
			szLevel.MakeLower();
			m_SMSDef.LogLevel = CLogRecord::GetLogLevel(szLevel);

			m_SMSDef.Remark  = m_pIniFile->Find("SMSLog","Remark");

		}

		m_pIniFile->Close();
	}	// end if( bReturn)

	delete m_pIniFile;
	m_pIniFile  = NULL;

	m_bLoaded = bReturn;
	return bReturn;
}

CLogConfig* CLogConfig::GetInstance()
{
	// 因为new 函数是带文件名作为参数的， 这里只是调用一下。
	return thisInstance;
}

void CLogConfig::ClearInstance()
{
	if( thisInstance != NULL)
	{
		delete thisInstance;
	}

	thisInstance = NULL;
}

LONG CLogConfig::ReadFileLogSize()
{
	
	CString szSize = m_pIniFile->Find("FileLog","MaxSize");
	szSize.MakeLower();
	
	CString szNumber = szSize.Left(szSize.GetLength()-1);

	LONG lSize = atol((LPCTSTR)szNumber);

	if(szSize.Right(1) == "m")
	{
		lSize = lSize * 1024 *1024;
	}else if(szSize.Right(1) == "k")
	{
		lSize = lSize * 1024;
	}
	m_FileDef.MaxSize = lSize;

	return m_FileDef.MaxSize ;
}

