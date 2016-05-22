/////////////////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CIniFile
//	Version: 	1.0
//	Date: 		2012-01-09
//	
//	Purpose:	Ini文件的标准输入输出类，用来读入Ini文件和写入Ini文件。
//	
/////////////////////////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "IniFile.h"
#include "Utility.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static const LPTSTR CommentFlag = "#";
static const int MAX_PATH_SIZE = 256;

//	---------------------------------------------------------------------------
//
// Function:    CIniFile()
//
// Description:
//  构造函数，根据指定的Ini文件来生成本对象。
//	（如果Ini文件中不带\\，那么自动添加当前目录。）
//
//
// Input:
//  LPCTSTR szFileName			--	Ini文件名
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
CIniFile::CIniFile(LPCTSTR szFileName)
{
	CString szFilePath (szFileName);

	if( szFilePath.ReverseFind(TEXT('\\')) < 0)
	{
		CHAR szPath[MAX_PATH] = {0};
		
		//::GetCurrentDirectory(MAX_PATH,szPath); 
		// the current directory would be changed frequently, so destory this call.
		::GetModuleFileName(AfxGetApp()->m_hInstance, szPath, MAX_PATH);
		CString szModuleFileName(szPath);
		int iPos = szModuleFileName.ReverseFind('\\');

		m_szFileName = szModuleFileName.Left(iPos);
		m_szFileName += "\\";
		m_szFileName += szFileName;		
	}else
	{
		m_szFileName = szFileName;
	}

	m_pTxtFile = new CFile();
	m_bOpened = FALSE;
	m_bIsReadOnly = FALSE;
}

//	---------------------------------------------------------------------------
//
// Function:    CIniFile()
//
// Description:
//  析构函数，删除成员变量，释放成员变量的指针。
//
//
// Input:
//  Nothing
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
CIniFile::~CIniFile()
{

	Section *pSection = NULL;
	for( int iLoop =0 ; iLoop < m_AryLine.GetSize(); iLoop++)
	{
		pSection = m_AryLine.GetAt(iLoop);
		delete pSection;
		m_AryLine.SetAt(iLoop, NULL);
	}
	m_AryLine.RemoveAll();

	if(m_pTxtFile != NULL)
	{
		if( m_bOpened)
		{
			m_pTxtFile->Close();
		}
		delete m_pTxtFile;
		m_pTxtFile = NULL;
	}
}

//	---------------------------------------------------------------------------
//
// Function:    Open()
//
// Description:
//  打开Ini文件,同时读入到成员变量中。
//
//
// Input:
//  BOOL isReadOnly           -- 是否以只读方式打开，缺省为非只读。
//
// Returns:
//  BOOL		TRUE/FALSE	--文件打开成功或者失败。
//
//	---------------------------------------------------------------------------
BOOL CIniFile::Open(BOOL isReadOnly /* = FALSE */)
{
	BOOL bReturn = FALSE;
	m_bIsReadOnly = isReadOnly;

	if( isReadOnly)
	{
		bReturn =m_pTxtFile->Open(m_szFileName,CFile::modeRead);
	}
	else
	{
		bReturn =m_pTxtFile->Open(m_szFileName,CFile::modeReadWrite);
	}

	if( !bReturn)
	{
//		bReturn = m_pTxtFile->Open(m_szFileName,CFile::modeCreate | CFile::modeReadWrite );
		if( !bReturn)
		{
			return bReturn;
		}
	}

	int iLength= (int)m_pTxtFile->GetLength();
	CHAR *szContent = (CHAR *) malloc((iLength +1)* sizeof (CHAR));
	memset(szContent, 0, (iLength +1) * sizeof(CHAR));
	m_pTxtFile->Read(szContent,iLength);
	m_szContent = szContent;
	free(szContent);
	szContent = NULL;

	CStringArray StringLine ;
	CUtility::SplitString(m_szContent,"\r\n",&StringLine);
	
	CString szTemp = "";
	for(int iLoop =0 ; iLoop < StringLine.GetSize(); iLoop++)
	{
		szTemp = StringLine.GetAt(iLoop);
		szTemp.TrimLeft(" ");
		szTemp.TrimRight(" ");
		
		// re-write the trimed string to string array;
		StringLine.SetAt(iLoop,szTemp);
		
		// if the line is started with # -- CommentFlag.
		if( szTemp.Left((int)strlen(CommentFlag)) == CommentFlag)
		{
			Section *pSec = new Section();
			// Get the left string started with 1, except #.
			pSec->Comment = szTemp.Right(szTemp.GetLength() -1 ); 
			pSec->Type = IsComment;

			m_AryLine.Add(pSec);

		// if the line is started with [ and the have the ].
		}else if(szTemp.Left(1) == "[" && szTemp.Find("]") > 0)		
		{
			int iPos = szTemp.Find("]");

			CString szSectionName = szTemp.Mid(1,iPos-1);
			szSectionName.TrimLeft(" ");
			szSectionName.TrimRight(" ");
			Section *pSec = new Section();

			pSec->SectionName = szSectionName;
			
			// split the comment from #. 
			iPos = szTemp.Find(CommentFlag, iPos);
			if(iPos > 0)
			{
				CString szComment = szTemp.Mid(iPos+1);
				szComment.TrimLeft(" ");
				pSec->Comment = szComment;
			}

			pSec->Type = IsSection;

			m_AryLine.Add(pSec);

		// if include =, that means the type is Item.
		}else if( szTemp.Find("=",0) > 0 )
		{
			Section *pSec = new Section();

			int iPos = szTemp.Find("=",0);
			CString szItem = szTemp.Left(iPos);
			szItem.TrimLeft(" ");
			szItem.TrimRight(" ");
			pSec->Item = szItem;
			pSec->Type = IsItem;

			iPos ++; // escape the = charactor
			int iPos2 = szTemp.Find(CommentFlag, iPos );

			if(iPos2 > 0 ) // if found the comment flag
			{
				// get right value of the item.
				CString szValue = szTemp.Mid(iPos , iPos2- iPos);
				szValue.TrimLeft(" ");
				szValue.TrimRight(" ");

				pSec->Value = szValue;
				
				// get the comment content.
				CString szComment = szTemp.Right(szTemp.GetLength() - iPos2);
				szComment.TrimLeft(" ");
				pSec->Comment  = szComment;

				m_AryLine.Add(pSec);
			}else
			{
				// read all content as the right value.
				CString szValue = szTemp.Right(szTemp.GetLength() - iPos);
				szValue.TrimLeft(" ");

				pSec->Value = szValue;
				m_AryLine.Add(pSec);
			}
		
		}

	}

	m_bOpened = bReturn;
	return bReturn;
	
}

//	---------------------------------------------------------------------------
//
// Function:    Close()
//
// Description:
//  关闭Ini文件,同时把成员变量写到文件中。
//
//
// Input:
//  Nothing
//
// Returns:
//  BOOL		TRUE/FALSE	--文件关闭成功或者失败。
//
//	---------------------------------------------------------------------------
BOOL CIniFile::Close()
{
	if(m_bOpened)
	{
		if( !m_bIsReadOnly)
		{
			this->Flush();
		}

		m_pTxtFile->Close();
		delete m_pTxtFile;
		m_pTxtFile = NULL;
		m_bOpened = FALSE;
	}

	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    Find()
//
// Description:
//  根据SectionName和Item名称来查找对应的值。
//
//
// Input:
//  LPCTSTR szSection		--	Section Name，不能为空。
//	LPCTSTR szItem			--	Item Name，不能为空。
//
// Returns:
//  CString					--	返回的Value，如果没有找到，那么返回空串
//
//	---------------------------------------------------------------------------
CString CIniFile::Find(LPCTSTR szSection, LPCTSTR szItem)
{
	ASSERT(szSection.length() > 0);

	CString strSection(szSection);
	CString strItem(szItem);

	strSection.TrimLeft();
	strSection.TrimRight();

	strItem.TrimLeft();
	strItem.TrimRight();

	CString szValue = "";
	BOOL bFoundSection = FALSE;

	CString szTemp = "";
	for(int iLoop =0 ; iLoop < m_AryLine.GetSize(); iLoop++)
	{
		Section *pSec = m_AryLine.GetAt(iLoop);
		
		szTemp = pSec->SectionName; 
		szTemp.MakeLower();
		strSection.MakeLower();

		if(szTemp== strSection)
		{
			bFoundSection = TRUE;
		}
		
		szTemp = pSec->Item ;
		szTemp.MakeLower();
		strItem.MakeLower();

		if( bFoundSection && szTemp == strItem)
		{
			szValue = pSec->Value ;
			break;
		}
	}

	return szValue;
}

//	---------------------------------------------------------------------------
//
// Function:    WriteValue()
//
// Description:
//  把参数中的内容写到类的成员变量中。
//
//
// Input:
//  LPCTSTR szSection		--	Section Name，不能为空。
//	LPCTSTR szItem			--	Item Name，可以为空。
//	LPCTSTR szValue			--	要写入的值。
//	LPCTSTR szComment		--	要写入的Comment，可以为空。
//
// Returns:
//  BOOL		TRUE/FALSE	--	写操作成功或者失败。
//
//	---------------------------------------------------------------------------
BOOL CIniFile::WriteValue(LPCTSTR szSection, LPCTSTR szItem, LPCTSTR szValue, LPCTSTR szComment /* = NULL*/)
{
	
	CString strSection(szSection);
	strSection.MakeLower();

	CString strItem(szItem);
	if(szItem == NULL)
	{
		strItem.Empty();
	}	
	strItem.MakeLower();

	
	BOOL bFoundSection = FALSE;
	BOOL bFoundItem = FALSE;

	int iPosSection = 0;
	CString szTemp;

	
	for(int iLoop =0 ; iLoop < m_AryLine.GetSize(); iLoop++)
	{
		Section *pSec = m_AryLine.GetAt(iLoop);
		
		// re-locate the position of section.
		if( pSec->Type == IsSection)
		{
			szTemp = pSec->SectionName ;
			szTemp.MakeLower();
			szTemp.TrimLeft(" ");

			if(szTemp == strSection )
			{
				iPosSection = iLoop;
				bFoundSection = TRUE;
			}
		}
		
		// relocated the position of item.
		if( pSec->Type == IsItem)
		{
			szTemp = pSec->Item;
			szTemp.MakeLower();

			if( bFoundSection && szTemp == strItem)	// Found Item & Found Section
			{
				// change the value of located item.
				bFoundItem = TRUE;
				pSec->Value = szValue;
				if(szComment != NULL)
				{
					pSec->Comment = szComment;
				}
				break;
			}
		}
	}

	CArray< Section *, Section *> newArray;

	if( !bFoundSection )
	{
		// even if the section is not found, write a new section to ini file.
		Section *pSec = new Section();
		pSec->SectionName = szSection;
		if( szComment != NULL )
		{
			pSec->Comment = szComment;
		}
		pSec->Type = IsSection;

		m_AryLine.Add(pSec);

		// if szItem is aslo defined, then write the item to ini file too.
		if( szItem != NULL)
		{
			Section *pItem = new Section();
			pItem->Item = szItem;
			pItem->Value = szValue;
			if(szComment != NULL)
			{
				pItem->Comment = szComment;
			}
			pItem->Type = IsItem;
			m_AryLine.Add(pItem);
		}
	}else // if found section.
	{
		 if (!bFoundItem)	// if not found the item, then add a new item to the located section.
		 {
			 Section *pSec = new Section();
			 pSec->Item = szItem;
			 pSec->Value = szValue;
			 if(szComment != NULL)
			 {
				pSec->Comment = szComment;
			 }
			 pSec->Type = IsItem;
			
			 // insert the pSec to current postion of a new array.
			 for( int jLoop =0 ; jLoop < iPosSection ; jLoop ++)
			 {
				 newArray.Add(m_AryLine.GetAt(jLoop));
			 }

			 newArray.Add(pSec);
			 for(int jLoop = iPosSection; jLoop < m_AryLine.GetSize(); jLoop++)
			 {
				 newArray.Add(m_AryLine.GetAt(jLoop));
			 }

			// copy the value to m_AryLine;
			 for( int lLoop = 0; lLoop < m_AryLine.GetSize(); lLoop ++)
			 {
				 delete m_AryLine.GetAt(lLoop);
			 }
			 m_AryLine.RemoveAll();
			 for( int kLoop = 0; kLoop < newArray.GetSize(); kLoop++)
			 {
			     m_AryLine.Add(newArray.GetAt(kLoop));
			 }

		 }// else do nothing because change the item value has already be processed during for loop.
	}
	
	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    Flush()
//
// Description:
//   write the m_AryLine value to file.
//
// Input:
//  Nothing.
//
// Returns:
//  BOOL : TRUE - success.
//
//	---------------------------------------------------------------------------

BOOL CIniFile::Flush()
{
	if(m_bOpened)
	{
		m_szContent.Empty();

		for(int iLoop =0; iLoop <m_AryLine.GetSize(); iLoop++)
		{
			Section *pSec = m_AryLine.GetAt(iLoop);
			switch (pSec->Type )
			{
			case IsSection:

				m_szContent += "\r\n";			// add new blank line.
				m_szContent += "[";
				m_szContent += pSec->SectionName ;
				m_szContent += "]";

				if( !pSec->Comment.IsEmpty())
				{
					m_szContent += "     ";		// five spaces
					m_szContent += CommentFlag;
					m_szContent += pSec->Comment ;
				}

				break;
			case IsComment:

				m_szContent += CommentFlag;
				m_szContent += pSec->Comment ;

				break;
			case IsItem:

				m_szContent += pSec->Item;
				m_szContent += " = ";
				m_szContent += pSec->Value ;
				if( !pSec->Comment.IsEmpty())
				{
					m_szContent += "     ";		// five spaces
					m_szContent += CommentFlag;
					m_szContent += pSec->Comment ;
				}

				break;
			default:
				break;
			}

			m_szContent += "\r\n";
		}

		int iLength = m_szContent.GetLength();
		m_pTxtFile->SeekToBegin();		// rewrite the ini file from the first postion.
		m_pTxtFile->SetLength(0);
		m_pTxtFile->Write((LPCTSTR)m_szContent, iLength);

		m_pTxtFile->Flush();
	
	} //if(m_bOpened)

	return TRUE;
}

//	---------------------------------------------------------------------------
//
// Function:    IsFileExist()
//
// Description:
//  文件操作类函数，这个函数用来判断文件是否存在。
//
//
// Input:
//  CString szFileName		--	文件名（全路径），不能为空。
//
// Returns:
//  BOOL		TRUE/FALSE	--	存在或则不存在。
//
//	---------------------------------------------------------------------------
bool CIniFile::IsFileExist(CString szFileName)
{
	WIN32_FIND_DATA  data;
	HANDLE  hd= FindFirstFile(szFileName,&data);
	if(hd != INVALID_HANDLE_VALUE)
		return true;

	return false;
}

//	---------------------------------------------------------------------------
//
// Function:    CopyFileTo()
//
// Description:
//  文件操作类函数，拷贝一个文件到另外一个文件名。
//
//
// Input:
//  CString szDestFileName		--	目的文件名（全路径），不能为空。
//  CString szSrcFileName		--	源文件名（全路径），不能为空。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CIniFile::CopyFileTo(CString szDestFileName, CString szSrcFileName)
{
	bool bb=IsFileExist(szSrcFileName);
	if(!bb)return;
	CopyFile( szSrcFileName, szDestFileName, FALSE);
}

//	---------------------------------------------------------------------------
//
// Function:    GetWinSysPath()
//
// Description:
//  文件操作类函数，读取系统文件夹的路径。
//
//
// Input:
//  Nothing
//
// Returns:
//  CString               -- 读出来的路径。
//
//	---------------------------------------------------------------------------
CString CIniFile::GetWinSysPath(void)
{
	char lpBuffer[MAX_PATH_SIZE];
	GetSystemDirectory( lpBuffer, MAX_PATH_SIZE);
	CString ss=lpBuffer;
	ss += "\\";
	return ss;
}

//	---------------------------------------------------------------------------
//
// Function:    GetAppPath()
//
// Description:
//  文件操作类函数，读取应用程序文件夹的路径。
//
//
// Input:
//  Nothing
//
// Returns:
//  CString               -- 读出来的路径。
//
//	---------------------------------------------------------------------------
CString CIniFile::GetAppPath(void)
{
	char lpBuffer[MAX_PATH_SIZE];
	GetCurrentDirectory(MAX_PATH_SIZE,lpBuffer);
	CString ss=lpBuffer;
	ss += "\\";
	return ss;
}

//	---------------------------------------------------------------------------
//
// Function:    GetKeyVal()
//
// Description:
//  文件操作类函数，根据Section和Key来读取一个整形值。
//
//
// Input:
//  CString szSectionName		--	目的文件名（全路径），不能为空。
//  CString szKeyName		    --	源文件名（全路径），不能为空。
//  int ipDefault		        --	缺省值
//
// Returns:
//  int                         --  读出来的值，或者是缺省值。
//
//	---------------------------------------------------------------------------
int CIniFile::GetKeyVal(CString szSectionName, CString szKeyName, int ipDefault)
{
	int iValue = ipDefault;
	CString szValue = Find(szSectionName, szKeyName);
	if (szValue != "")
	{
		iValue = atoi(szValue);
	}
	return iValue;
}

//	---------------------------------------------------------------------------
//
// Function:    GetKeyVal()
//
// Description:
//  文件操作类函数，根据Section和Key来读取一个字符串值。
//
//
// Input:
//  CString szSectionName		--	目的文件名（全路径），不能为空。
//  CString szKeyName		    --	源文件名（全路径），不能为空。
//  CString szDefault		    --	缺省值
//
// Returns:
//  CString                     --  读出来的值，或者是缺省值。
//
//	---------------------------------------------------------------------------
CString CIniFile::GetKeyVal(CString szSectionName, CString szKeyName, LPCTSTR szDefault)
{
	CString szValue = Find(szSectionName, szKeyName);
	if (szValue == "")
	{
		szValue = szDefault;
	}
	return szValue;
}
