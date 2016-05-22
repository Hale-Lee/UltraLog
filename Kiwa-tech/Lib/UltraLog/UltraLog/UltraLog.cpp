// UltraLog.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "UltraLog.h"
//#include "..\\..\\..\\Common\\Log\\LogRecord.h"
#include "..\\..\\..\\Common\\Log\\LogBuilder.h"
#include "..\\..\\..\\Common\\Log\\ILog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//#pragma comment(linker, "/EXPORT:GetLogInstance=?GetLogInstance@@YGPAVILog@@XZ,PRIVATE")
//#pragma comment(linker, "/EXPORT:AddNewLog=?AddNewLog@@YGXPAVILog@@@Z,PRIVATE")
//#pragma comment(linker, "/EXPORT:GetLogInstance=?GetLogInstance@@YGPAUILog@@XZ,PRIVATE")
//#pragma comment(linker, "/EXPORT:AddNewLog=?AddNewLog@@YGXPAUILog@@@Z,PRIVATE")

//
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//



// CUltraLogApp

BEGIN_MESSAGE_MAP(CUltraLogApp, CWinApp)
END_MESSAGE_MAP()


// CUltraLogApp ����

CUltraLogApp::CUltraLogApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CUltraLogApp ����

CUltraLogApp theApp;


// CUltraLogApp ��ʼ��

BOOL CUltraLogApp::InitInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CWinApp::InitInstance();

	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    GetLogInstance\RemoveLogInstance()
//
// Description:
//  DLL�ĵ���������һ����ȡ��Log�Ķ���һ����ɾ��Log�Ķ���
//
//
// Input:
//	Nothing
//
// Returns:
//  ILog */Nothing					--	ȡ�õĶ���
//
//	---------------------------------------------------------------------------
ILog * GetLogInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	return CLogBuilder::GetInstance();
}

void RemoveLogInstance()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLogBuilder * theLog = (CLogBuilder * )CLogBuilder::GetInstance();
	if( theLog->IsInitialzed())
	{
		theLog->Finalize();
	}
	CLogBuilder::ClearInstance();
}


void AddNewLog(ILog * pNewLog)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	CLogBuilder * Builder = (CLogBuilder *)CLogBuilder::GetInstance() ;
	Builder->AddNewOutput(pNewLog);
}