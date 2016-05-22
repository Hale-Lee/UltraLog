///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogConfig
//	Version: 	1.0
//	Date: 		2015-01-09
//	
//	Purpose:	Log配置类,从Ini文件中读取Log配置的各个属性.
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGCONFIG_H__4F3A8BEF_F6ED_47DE_BC93_AF5719E6FA95__INCLUDED_)
#define AFX_LOGCONFIG_H__4F3A8BEF_F6ED_47DE_BC93_AF5719E6FA95__INCLUDED_

#pragma once

#include "LogRecord.h" 	// Added by ClassView
class CIniFile;

class CLogConfig
{
	// 定义输出Log的类型，指Log出力到什么地方去。
	struct LogOutput
	{
		BOOL  toFile;
		BOOL  toScreen;
		BOOL  toMail;
		BOOL  toSMS;
	};

	// 文件类型的Log配置
	struct FileLogDef
	{
		CString             FileName;
		LONG                MaxSize;
		CLogRecord::LogType LogLevel;
		CString             Remark;
	};

	// 屏幕类型的Log配置
	struct ScreenLogDef
	{
		CString 		    ListBoxName;
		LONG                MaxSize;
		CLogRecord::LogType LogLevel;
		CString             Remark;
	};

	// 邮件类型的Log配置
	struct MailLogDef
	{
		CString             SMTPServer;
		CString             User;
		CString             Password;
		LONG                GroupSize;
		CLogRecord::LogType LogLevel;   
		CString             Remark;
	};

	// 短信类型的Log配置
	struct SMSLogDef
	{
		CString             MoblieNo;
		LONG                GroupSize;
		CLogRecord::LogType LogLevel; 
		CString             Remark;
	};

private:

	LONG ReadFileLogSize();
	CIniFile * m_pIniFile;

	BOOL m_bLoaded;

	FileLogDef      m_FileDef;
	ScreenLogDef    m_ScreenDef;
	MailLogDef      m_MailDef;
	SMSLogDef       m_SMSDef;

	LogOutput       m_OutputType;

	CString m_szIniFile;

public:
	CLogConfig(LPCTSTR lpszFileName);
	~CLogConfig(void);

public:

	inline BOOL IsOutput2File() {return m_OutputType.toFile;}
	inline BOOL IsOutput2Screen() {return m_OutputType.toScreen;}
	inline BOOL IsOutput2Mail() {return m_OutputType.toMail;}
	inline BOOL IsOutput2SMS() {return m_OutputType.toSMS;}
	
	inline CLogRecord::LogType GetFileLogLevel() { return m_FileDef.LogLevel;}
	inline LONG GetFileLogSize() { return m_FileDef.MaxSize ; }
	inline CString GetLogFileName() {return m_FileDef.FileName; }
	inline CString GetFileLogRemart() {return m_FileDef.Remark ; }

	inline CLogRecord::LogType GetScreenLogLevel() { return m_ScreenDef.LogLevel;}
	inline LONG GetScreenLogSize() { return m_ScreenDef.MaxSize ; }
	inline CString GetListBoxName() {return m_ScreenDef.ListBoxName; }
	inline CString GetScreenLogRemart() {return m_ScreenDef.Remark ; }

	inline CLogRecord::LogType GetMailLogLevel() { return m_MailDef.LogLevel;}
	inline CString GetMailServer() { return m_MailDef.SMTPServer ;}
	inline CString GetMailLogUser() { return m_MailDef.User ;}
	inline CString GetMailLogPassword() { return m_MailDef.Password ;}
	inline CString GetMailLogRemark() { return m_MailDef.Remark ;}
	inline LONG GetMaiiLogSize() { return m_MailDef.GroupSize ;}

	inline CLogRecord::LogType GetSMSLogLevel() { return m_SMSDef.LogLevel;}
	inline CString GetMoblieNo() { return m_SMSDef.MoblieNo;}
	inline LONG GetSMSLogSize() { return m_SMSDef.GroupSize ;}
	inline CString GetSMSLogRemark() { return m_SMSDef.Remark ;}

	BOOL Load();
	static CLogConfig* GetInstance();

	static void ClearInstance();

};

#endif // !defined(AFX_LOGCONFIG_H__4F3A8BEF_F6ED_47DE_BC93_AF5719E6FA95__INCLUDED_)

