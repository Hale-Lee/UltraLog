// UltraLog.cpp : 定义 DLL 的初始化例程。
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
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//



// CUltraLogApp

BEGIN_MESSAGE_MAP(CUltraLogApp, CWinApp)
END_MESSAGE_MAP()


// CUltraLogApp 构造

CUltraLogApp::CUltraLogApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CUltraLogApp 对象

CUltraLogApp theApp;


// CUltraLogApp 初始化

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
//  DLL的导出函数，一个是取得Log的对象，一个是删除Log的对象。
//
//
// Input:
//	Nothing
//
// Returns:
//  ILog */Nothing					--	取得的对象。
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