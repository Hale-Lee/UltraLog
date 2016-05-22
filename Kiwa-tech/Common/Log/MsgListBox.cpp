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
//              同时自动设置了MsgListBox的ClassName。
//	
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "MsgListBox.h"
#include "LogConfig.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMsgListBox

CMsgListBox::CMsgListBox()
{
}

CMsgListBox::~CMsgListBox()
{
}


BEGIN_MESSAGE_MAP(CMsgListBox, CListBox)
	//{{AFX_MSG_MAP(CMsgListBox)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMsgListBox message handlers

int CMsgListBox::AddString(LPCTSTR lpszItem)
{
	int nResult = CListBox::AddString( lpszItem );
	RefushHorizontalScrollBar(lpszItem);
	return nResult;
}

int CMsgListBox::InsertString(int nIndex, LPCTSTR lpszItem)
{
	int nResult = CListBox::InsertString( nIndex, lpszItem );
	RefushHorizontalScrollBar(lpszItem);
	return nResult;
}




//	---------------------------------------------------------------------------
//
// Function:    RefushHorizontalScrollBar()
//
// Description:
//   刷新水平滚动条。              
//
// Input:
//  LPCTSTR lpszText	--新增加的行的内容，如果是NULL那就意味着需要重新计算水平滚动条。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CMsgListBox::RefushHorizontalScrollBar(LPCTSTR lpszText /* = NULL */)
{

	// 如果是没有数据，那么把水平滚动条设为没有。
	int nCount = this->GetCount();
	if ( nCount < 1 )
	{
	   this->SetHorizontalExtent( 0 );
	   return;
	}

	CDC *pDC = this->GetDC();
	if ( NULL == pDC )
	{
	   return;
	}

	static int nMaxExtent = 0;		//滚动条的最大值。static变量。
	if( lpszText != NULL)
	{
		CSize &cs = pDC->GetTextExtent( lpszText );		//取字符串的像素长度。
		if ( cs.cx > nMaxExtent )
		{
			nMaxExtent = cs.cx;
		}
	}else
	{
		// 如果需要重新取得最大值，那么先将最大值赋为0.
		nMaxExtent = 0;

		// 临时变量，为了防止不停的分配内存，设置为static形式。
		const int MAX_LENGTH = 1024;
		static CHAR szText[MAX_LENGTH] = {0};	
		
		for ( int iLoop = 0; iLoop < nCount; iLoop++ )
		{
			
			//  如果超过了MAX_LENGTH，那么就取MAX_LENGTH × M字符的宽度。
			if( this->GetTextLen(iLoop) > MAX_LENGTH )
			{
				CSize &cs = pDC->GetTextExtent("M", 1);
				if ( nMaxExtent < cs.cx * MAX_LENGTH )
				{
					nMaxExtent = cs.cx * MAX_LENGTH ; 
				}

			}else	// 默认为ListBox行的最大长度不超过MAX_LENGTH个字符。
			{
				memset(szText,0, sizeof(CHAR) * MAX_LENGTH);
				this->GetText( iLoop, szText ) ;

				CSize &cs = pDC->GetTextExtent( szText ,(int) strlen(szText));
				if ( cs.cx > nMaxExtent )
				{
					nMaxExtent = cs.cx;
				}
			}
		}
	}

	this->SetHorizontalExtent( nMaxExtent );
	this->ReleaseDC(pDC);
}

int CMsgListBox::DeleteString(UINT nIndex)
{
	int nResult = CListBox::DeleteString( nIndex );
	RefushHorizontalScrollBar();
	return nResult;
}

void CMsgListBox::ResetContent()
{
	CListBox::ResetContent();
	RefushHorizontalScrollBar();
}



BOOL CMsgListBox::PreCreateWindow(CREATESTRUCT& cs )
{
	cs.lpszClass = "MessageListBox";
	
	CLogConfig *theConfig = CLogConfig::GetInstance();
	cs.lpszName = theConfig->GetListBoxName();

	__super::PreCreateWindow(cs);
	return 0;
}
