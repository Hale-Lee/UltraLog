///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CLogFile
//	Version: 	1.0
//	Date: 		2012-02-09
//	
//	Purpose:	�ļ����͵�Log��Log��¼���ļ��У����Log���̰߳�ȫ�ġ�
//	
///////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "LogFile.h"
#include "..\\..\\Common\\Utility\\IniFile.h"
#include "LogConfig.h"
#include "..\\..\\Common\\Utility\\Utility.h"

#include <afxmt.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

const int MAX_LOG_LINE = 8;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CLogFile::CLogFile(LPCTSTR szFileName)
{
	m_szFileName = szFileName;
	m_pFileHandle = NULL;
	m_bOpened  = FALSE;
	m_pSection = new CCriticalSection();
}

CLogFile::~CLogFile()
{
	Finalize();
	delete m_pSection;
//	AfxMessageBox("CLogFile::~CLogFile()");
	m_pSection = NULL;

}
//	---------------------------------------------------------------------------
//
// Function:    Initialize()
//
// Description:
//  ���ļ���ͬʱ���ļ�ָ�붨λ���ļ�β���Է�����Append�ķ�ʽд�ļ���
//
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	�ļ��򿪳ɹ�����ʧ��
//
//	---------------------------------------------------------------------------
BOOL CLogFile::Initialize(LPCSTR szConfigFile /*= NULL*/)
{

	if(__super::IsInitialzed())
	{
		return TRUE;
	}

	BOOL bReturn = FALSE;

	m_pFileHandle = new CFile();
	
	bReturn = m_pFileHandle->Open(m_szFileName, CFile::modeReadWrite | CFile::shareDenyWrite );

	if( !bReturn)
	{
		bReturn = m_pFileHandle->Open(m_szFileName,CFile::modeCreate | CFile::modeReadWrite | CFile::shareDenyWrite);
		if( !bReturn)
		{
			return bReturn;
		}
	}
/*	
	m_pFileHandle->SeekToEnd();

	int iLength= m_pFileHandle->GetLength();
	CHAR * szContent = (CHAR *) malloc( (iLength +1) * sizeof(CHAR));
	memset (szContent,0, (iLength +1)* sizeof(CHAR));
	m_pFileHandle->Read(szContent,iLength);
	m_szContent = szContent;
	free(szContent);
	szContent  = NULL;
*/	
	m_bOpened = TRUE;

	__super::Initialize(szConfigFile);

	return bReturn;

}

//	---------------------------------------------------------------------------
//
// Function:    Finalize()
//
// Description:
//  �ر��ļ������ļ��е����ݱ��浽Ӳ�̡�
//
//
// Input:
//	Nothing
//
// Returns:
//  BOOL						--	�ļ��رճɹ�����ʧ��
//
//	---------------------------------------------------------------------------
BOOL CLogFile::Finalize()
{
	if(m_bOpened)
	{
		// ��������л������ݵĻ�����ôҪ���������д���ļ�����ȥ��
		if(m_AryLog.GetSize() > 0 )
		{

			// �����е����ݶ����뵽һ���ַ���szBigLine�С�
			CString szBigLine , szLog;
			for ( int iLoop =0; iLoop < m_AryLog.GetSize(); iLoop++)
			{
				szLog = m_AryLog.GetAt(iLoop);
				szBigLine += szLog;
			}
			m_AryLog.RemoveAll();

			int iLength = (int)m_pFileHandle->GetLength();

			m_pFileHandle->SeekToEnd();
			iLength = szBigLine.GetLength();
			m_pFileHandle->Write((LPCTSTR)szBigLine,iLength);
			m_pFileHandle->Flush();

		}
		
		//�ر��ļ�����������
		m_pFileHandle->Close();
		delete m_pFileHandle;
		m_pFileHandle = NULL;
		m_bOpened = FALSE;
	}

	__super::Finalize();

	return TRUE;
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
//	LPCTSTR szMessage			--	Ҫд���Log��Ϣ��
//	LogType type				--	Log���͡�
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
VOID CLogFile::AddRecord(LPCTSTR szMessage, LogType type)
{

	//��ȡ������Ϣ��
	CLogConfig *pConfig = CLogConfig::GetInstance();
	
	// dertmine that the information level.
	if(pConfig->GetFileLogLevel() > type)
	{
		return;
	}

	if( !pConfig->IsOutput2File())
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

#ifdef _DEBUG
//	szLog += THIS_FILE;
//	szLog += "    ";		//4���ո�
#endif
	szLog += szMessage;
	szLog += "\r\n";

	m_pSection->Lock();
	m_AryLog.Add (szLog);

	if ( m_AryLog.GetSize() > MAX_LOG_LINE)
	{
	
		//�ѻ�������ݸ��Ƶ�һ������ַ���������
		CString szBigLine ;
		for ( int iLoop =0; iLoop < m_AryLog.GetSize(); iLoop++)
		{
			szLog = m_AryLog.GetAt(iLoop);
			szBigLine += szLog;
		}
		m_AryLog.RemoveAll();

		int iLength = (int)m_pFileHandle->GetLength();

		// ���Log�ļ�����ָ���Ĵ�С����ô�������ݣ����¼�¼Log.
		if( iLength > pConfig->GetFileLogSize())
		{
			Finalize();
			szTime = time.Format(".%Y%m%D%H%M%S");
			CString szNewFileName = m_szFileName + ".bak" + szTime;
			CFile::Rename(m_szFileName,szNewFileName);
			Initialize();
		}
		
		m_pFileHandle->SeekToEnd();
		iLength = szBigLine.GetLength();
		m_pFileHandle->Write((LPCTSTR)szBigLine,iLength);
		m_pFileHandle->Flush();
	}

	m_pSection->Unlock();
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
VOID CLogFile::AddRecord(LogType type, LPCTSTR lpszFormat, ...)
{
	va_list argList;
	va_start(argList, lpszFormat);
	
	CString szMessage;
	szMessage.FormatV(lpszFormat, argList);
	va_end(argList);

	AddRecord(szMessage, type);

}

