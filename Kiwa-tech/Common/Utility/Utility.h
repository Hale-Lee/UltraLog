///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CUtility
//	Version: 	1.0
//	Date: 		2012-01-09
//	
//	Purpose:	工具类,封装了一些常用的工具,比如字符串的操作, 格式化操作.
//				注意: 本类是静态类,所有的成员函数均是静态的.
//	
///////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_UTILITY_H__974E9CB0_7823_43B1_93C7_6A3CD2307088__INCLUDED_)
#define AFX_UTILITY_H__974E9CB0_7823_43B1_93C7_6A3CD2307088__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <afxtempl.h>
#include <nb30.h> 

//typedef CArray<CString, CString> StringArray;
typedef	CArray<LONG,LONG> LongArray;

class CCommand;
//interface IFormattedMessage;

extern const LPSTR TimeFormat;

class CUtility
{
	typedef struct _ASTAT_ 
	{ 
		ADAPTER_STATUS adapt; 
		NAME_BUFFER 
		NameBuff [30]; 
	} ASTAT, * PASTAT; 


public:
	static void SplitString(LPCTSTR szSource, LPCTSTR szToken, CStringArray *pArray);
	static BOOL IncludeSymbol(LPCTSTR lpszText);
	static BOOL IsInteger(LPCTSTR lpszText);
	static BOOL IsNumeric(LPCTSTR lpszText);
	static CString GetHostIPAddress();
	static void FormatIPAddress(CString &szIPAddress);
	static void Decode(LPCTSTR szValue, BYTE *pReturn, LONG *plSize);
	static CString Encode(BYTE *pBuf, LONG lSize);
	static LONG CalculateMaxiumDivisor(LongArray *pArray);
	static int FindBytes(TCHAR *szSrc, BYTE *bGoal, int nGoalLength);
	static void GetMacAddress (int NetCardNo, CString &szMacAddr) ;
	static void GetFirstEffectiveAddr(CString &szpAddr);

private:
	CUtility();
	virtual ~CUtility();

};



#endif // !defined(AFX_UTILITY_H__974E9CB0_7823_43B1_93C7_6A3CD2307088__INCLUDED_)
