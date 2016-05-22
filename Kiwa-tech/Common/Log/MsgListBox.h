/////////////////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CMsgListBox
//	Version: 	1.0
//	Date: 		2012-03-01
//	
//	Purpose:	ListBox的扩展类，增加了自动设置水平滚动条的功能。
//	
/////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MSGLISTBOXEX_H__BD1A8EB5_FCB0_4474_964C_E1A8906129FD__INCLUDED_)
#define AFX_MSGLISTBOXEX_H__BD1A8EB5_FCB0_4474_964C_E1A8906129FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMsgListBox window
extern LPCTSTR  ListBoxClassName  ;

class CMsgListBox : public CListBox
{
// Construction
public:
	CMsgListBox();
	virtual ~CMsgListBox();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMsgListBox)
	//}}AFX_VIRTUAL

// Implementation
public:
	void ResetContent();
	int DeleteString(UINT nIndex);
	void RefushHorizontalScrollBar(LPCTSTR lpszText = NULL );
	int InsertString(int nIndex, LPCTSTR lpszItem);
	int AddString(LPCTSTR lpszItem);


	// Generated message map functions
protected:
	//{{AFX_MSG(CMsgListBox)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs );
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MSGLISTBOXEX_H__BD1A8EB5_FCB0_4474_964C_E1A8906129FD__INCLUDED_)
