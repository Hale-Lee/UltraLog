/////////////////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CMsgListBox
//	Version: 	1.0
//	Date: 		2012-03-01
//	
//	Purpose:	ListBox����չ�࣬�������Զ�����ˮƽ�������Ĺ��ܡ�
//              ͬʱ�Զ�������MsgListBox��ClassName��
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
//   ˢ��ˮƽ��������              
//
// Input:
//  LPCTSTR lpszText	--�����ӵ��е����ݣ������NULL�Ǿ���ζ����Ҫ���¼���ˮƽ��������
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CMsgListBox::RefushHorizontalScrollBar(LPCTSTR lpszText /* = NULL */)
{

	// �����û�����ݣ���ô��ˮƽ��������Ϊû�С�
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

	static int nMaxExtent = 0;		//�����������ֵ��static������
	if( lpszText != NULL)
	{
		CSize &cs = pDC->GetTextExtent( lpszText );		//ȡ�ַ��������س��ȡ�
		if ( cs.cx > nMaxExtent )
		{
			nMaxExtent = cs.cx;
		}
	}else
	{
		// �����Ҫ����ȡ�����ֵ����ô�Ƚ����ֵ��Ϊ0.
		nMaxExtent = 0;

		// ��ʱ������Ϊ�˷�ֹ��ͣ�ķ����ڴ棬����Ϊstatic��ʽ��
		const int MAX_LENGTH = 1024;
		static CHAR szText[MAX_LENGTH] = {0};	
		
		for ( int iLoop = 0; iLoop < nCount; iLoop++ )
		{
			
			//  ���������MAX_LENGTH����ô��ȡMAX_LENGTH �� M�ַ��Ŀ�ȡ�
			if( this->GetTextLen(iLoop) > MAX_LENGTH )
			{
				CSize &cs = pDC->GetTextExtent("M", 1);
				if ( nMaxExtent < cs.cx * MAX_LENGTH )
				{
					nMaxExtent = cs.cx * MAX_LENGTH ; 
				}

			}else	// Ĭ��ΪListBox�е���󳤶Ȳ�����MAX_LENGTH���ַ���
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
