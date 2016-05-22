///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogScreen
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	在屏幕上写Log，也就是把Log直接输出到ListBox上。
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_LOGSCREEN_H__07AF768B_58FD_42D4_BF71_DEF7B6459D6D__INCLUDED_)
#define AFX_LOGSCREEN_H__07AF768B_58FD_42D4_BF71_DEF7B6459D6D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "LogRecord.h"
class CMsgListBox;

class CLogScreen : public CLogRecord  
{
public:
	CLogScreen(LPCTSTR ListBoxTitle);
	virtual ~CLogScreen();

	VOID AddRecord(LPCTSTR szMessage, LogType type= logInfo) ;
	VOID AddRecord(LogType type, LPCTSTR lpszFormat, ...) ;
/*
	VOID Info( LPCTSTR lpszFormat, ...)  ;
	VOID Warning( LPCTSTR lpszFormat, ...) ;
	VOID Error( LPCTSTR lpszFormat, ...) ;
	VOID Fatal( LPCTSTR lpszFormat, ...) ;
*/
	BOOL Initialize();
	BOOL Finalize() ;

private :
	CMsgListBox   *m_pListBox;
	LPCTSTR       m_szListBoxTitle;
};

#endif // !defined(AFX_LOGSCREEN_H__07AF768B_58FD_42D4_BF71_DEF7B6459D6D__INCLUDED_)
