// UltraLog.h : UltraLog DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CUltraLogApp
// �йش���ʵ�ֵ���Ϣ������� UltraLog.cpp
//

class CUltraLogApp : public CWinApp
{
public:
	CUltraLogApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
