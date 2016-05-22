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
#include "stdafx.h"
#include "Utility.h"
#include <Winsock2.h>


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment( lib, "ws2_32.lib" ) 
#pragma comment(lib,"netapi32.lib")

static const LPSTR TimeFormat = "%Y/%m/%d %H:%M:%S";
CString m_szHostIPAddress;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
/*
CUtility::CUtility()
{

}

CUtility::~CUtility()
{

}
*/
//	---------------------------------------------------------------------------
//
// Function:    SplitString()
//
// Description:
//  把一个字符串分割成一个字符串数组，按照指定的分割符号。
//
//
// Input:
//	LPCTSTR szSource			--	要分割的对象。
//	LPCTSTR szToken				--	分割符号
//  CStringArray *pArray        --  分割后的字符串数组。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::SplitString(LPCTSTR szSource, LPCTSTR szToken, CStringArray *pArray)
{
	// 定义一块临时内存	
	int iLength = (int)strlen(szSource) + 1;
	CHAR *szValue = (CHAR *)malloc(iLength *sizeof(CHAR));
	memset(szValue,0,iLength * sizeof(CHAR));

	CString strSource(szSource);

	int iFirst = 0 , iSecond = 0;
	do
	{
		iSecond = strSource.Find(szToken,iFirst);
		if(iSecond != -1)
		{
			// 如果找到了， 那么把找到的那一段字符串拷贝到临时内存中去。
			memset(szValue,0,iLength * sizeof(CHAR));
			memcpy(szValue , szSource + iFirst, iSecond-iFirst);
			
			// 拷贝这个临时内存值到输出中去，同时开始下一次寻找。
			pArray->Add(szValue);
			iFirst = iSecond + (int)strlen(szToken);

		}else
		{
			// 如果已经到了末尾了， 那么拷贝所有的内容到临时内存中去。
			memset(szValue,0,iLength * sizeof(CHAR));
			memcpy(szValue, szSource + iFirst, iLength-iFirst);
			pArray->Add(szValue);
		}
	}while(iSecond != -1);

	// 释放临时内存。
	free(szValue);
	szValue = NULL;

}

//	---------------------------------------------------------------------------
//
// Function:    CalculateMaxiumDivisor()
//
// Description:
//  计算最大公约数， 计算一组LONG变量的最大公约数。
//
//
// Input:
//	LongArray *pArray			--	要计算的最大公约数。
//
// Returns:
//  LONG                        --  计算后的结果。
//
//	---------------------------------------------------------------------------
LONG CUtility::CalculateMaxiumDivisor(LongArray *pArray)
{
	LONG lReturn = 1;
	
	// 如果数组中没有内容，那么返回1
	if (pArray->GetSize() == 0 )
	{
		return 1;
	}
	
	// 如果只有一个数字，那么它的最大公约数就是它自己。
	if (pArray->GetSize() == 1 )
	{
		return pArray->GetAt(0);
	}

	// 先取的数组中第一个数字的值。
	LONG lValue = pArray->GetAt(0);
	LONG lMargin  = 1 , lNextValue = 0;
	//使用转转相减法来取得最大公约数。
	for (int iLoop = 1 ; iLoop < pArray->GetSize(); iLoop++)
	{
		lNextValue = pArray->GetAt(iLoop);
		
		//如果A和B的最大公约数是C，那么abs(A-B)和min(A,B)的最大公约数肯定也是C.
		while(lMargin != 0 )
		{
			lMargin = abs(lNextValue - lValue);
			// 读取一个较小的值
			lNextValue = (lNextValue < lValue?lNextValue : lValue );
			if (lMargin != 0  )	lValue = lMargin;
		}

		lMargin = lValue;
	}

	return lMargin;
}

//	---------------------------------------------------------------------------
//
// Function:    Encode()
//
// Description:
//  简单的加密和编码函数，按照字节把转换成可以识别的大写的英文字母的形式。一个字节转换成２个英文字母。
//
//
// Input:
//  BYTE *pBuf              --  要转换的对象。
//  LONG lSize　　　        --  要转换的对象的长度。
//
// Returns:
//  CString                 --  转换后的字符串。
//
//	---------------------------------------------------------------------------
CString CUtility::Encode(BYTE *pBuf, LONG lSize)
{
	//加密密码是：２０８７５４，通过循环异或的方式来加密。
	BYTE bKey[] = {0x2, 0xF, 0x0,0xA, 0x8,0xD, 0x7,0xB, 0x5,0x3, 0x4, 0x9};

	BYTE *bTemp = (BYTE *) malloc ( lSize);
	memcpy(bTemp, pBuf,lSize);
	int keyLoop = 0;
	for(int jLoop =0 ;jLoop < lSize ; jLoop++)
	{
		bTemp[jLoop] = bTemp[jLoop] ^ bKey[keyLoop];
		keyLoop ++;
		keyLoop ++;
		if( keyLoop >= sizeof(bKey)) keyLoop = 0;
	}

	CString szReturn = "";

	//开始编码
	BYTE *pValue = (BYTE *)malloc((lSize * 2 + 1 )* sizeof(BYTE) );
	memset(pValue,0,(lSize * 2 + 1) *sizeof(BYTE));

	int iTemp = 0, kLoop = 0;
	for(int iLoop =0 ; iLoop <lSize; iLoop++)
	{
		// 取字节的高位，＋６５表示转换成A开始的字母。
		iTemp = bTemp[iLoop] /16;
		pValue[kLoop] = iTemp + 65;
		kLoop ++;
		// 取字节的低位
		iTemp = bTemp[iLoop] %16;
		pValue[kLoop] = iTemp + 65;
		kLoop ++;
	}

	szReturn = (char *) pValue;
	free(pValue);
	
	free(bTemp);
	return szReturn;
}

//	---------------------------------------------------------------------------
//
// Function:    Decode()
//
// Description:
//  先把字符串反编码成对应的字节数组，然后再用循环异或解密。
//
// Input:
//  LPCTSTR szValue         --  解密对象
//  BYTE *pReturn           --  转换后的结果。
//  LONG *plSize　　　      --  转换后的结果的长度。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::Decode(LPCTSTR szValue, BYTE *pReturn, LONG *plSize)
{
	ASSERT(pReturn != NULL);

	int iLength = (int)strlen(szValue);
	BYTE *pValue =(BYTE *) szValue;
	*plSize = iLength /2;


	BYTE *pBuff = (BYTE *) malloc( iLength  * sizeof(BYTE));
	memset(pBuff, 0, iLength * sizeof(BYTE));

	BYTE *bTemp = (BYTE *) malloc ( (*plSize) * sizeof(BYTE) );
	memset(bTemp, 0, (*plSize) * sizeof(BYTE));
	int kLoop = 0;

	for(int iLoop =0 ; iLoop < iLength; iLoop++)
	{
		pBuff[iLoop] = pValue[iLoop]- 65;

		//接下来把２个字节合并成一个字节。
		if( iLoop % 2 == 1)
		{
			bTemp[kLoop] = pBuff[iLoop]  + pBuff[iLoop -1] * 16;
			kLoop ++;
		}
	}


	//加密密码是：２０８７５４，通过循环异或的方式来加密。
	BYTE bKey[] = {0x2, 0xF, 0x0,0xA, 0x8,0xD, 0x7,0xB, 0x5,0x3, 0x4, 0x9};

	int keyLoop = 0;
	for(int jLoop =0 ;jLoop < (*plSize) ; jLoop++)
	{
		bTemp[jLoop] = bTemp[jLoop] ^ bKey[keyLoop];
		keyLoop ++;
		keyLoop ++;
		if( keyLoop >= sizeof(bKey)) keyLoop = 0;
	}

	memcpy(pReturn, bTemp, (*plSize) * sizeof(BYTE) );
	free (bTemp);

	free( pBuff);

	return;
}
//	---------------------------------------------------------------------------
//
// Function:    FindBytes()
//
// Description:
//  查找字符串，在指定的目标字符串中查找一个子字符串，多用在双字节的字符串中。
//
//
// Input:
//	TCHAR *szSrc			--	目标字符串。
//  BYTE *bGoal             --  查找对象。
//  int nGoalLength         --  查找对象的长度。
//
// Returns:
//  int                     --  -1,如果没有找到，如果找到了，那么返回它的位置。
//
//	---------------------------------------------------------------------------
int CUtility::FindBytes(TCHAR *szSrc, BYTE *bGoal, int nGoalLength)
{
	BOOL bFound = FALSE;
	int iPos = 0;

	BYTE  *bSrc = (BYTE *)szSrc;
	while (*bSrc != '\0')
	{
		if (*bSrc == *bGoal )
		{
			if (memcmp(bSrc,bGoal,nGoalLength) ==0 )
			{
				bFound = TRUE;
				break;
			}
		}

		iPos ++;
		bSrc ++;
	}
	
	if( !bFound) iPos = -1;

	return iPos ;

}

//	---------------------------------------------------------------------------
//
// Function:    FormatIPAddress()
//
// Description:
//  格式化IP地址字符串，把9.13.255.4样式的IP地址字符串格式化成009.013.255.004
//
//
// Input:
//	CString &szIPAddress			--	输入/输出的IP地址字符串。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::FormatIPAddress(CString &szIPAddress)
{
	CString szInput = szIPAddress;

	CStringArray szSection;
	SplitString(szInput,".",&szSection);

	CString szNew = "";
	for(int iLoop =0 ; iLoop<szSection.GetSize(); iLoop++)
	{
		//如果是1位的数字，那么前面加上00
		if(szSection[iLoop].GetLength() <=1)
		{
			szSection[iLoop] = "00" + szSection[iLoop];
		}
		// 如果是2位的数字，那么前面加上1个0
		else if(szSection[iLoop].GetLength() ==2 )
		{
			szSection[iLoop] = "0" + szSection[iLoop];
		}
		
		szNew += szSection[iLoop];
		szNew += ".";
	}

	szNew.TrimRight(".");
	
	szIPAddress = szNew;
	return;
}


//	---------------------------------------------------------------------------
//
// Function:    GetHostIPAddress()
//
// Description:
//  取得当前电脑的IP地址。
//
//
// Input:
//	Nothing
//
// Returns:
//  CString 		--	取得的IP地址
//
//	---------------------------------------------------------------------------
CString CUtility::GetHostIPAddress()
{
	

	//取了一次了就不要重复的调用多次。
	if(m_szHostIPAddress.GetLength() == 0)
	{
		m_szHostIPAddress = "127.000.000.001";

		WORD wVersionRequested = 0; 
		WSADATA wsaData = {0}; 

		// 初始化Socket
		int nErr = 0; 
		wVersionRequested = MAKEWORD( 2, 2 ); 
		nErr = WSAStartup( wVersionRequested, &wsaData ); 
		if ( nErr != 0 ) 
		{ 
			return m_szHostIPAddress; 
		} 

		//取得当前计算机的名称
		CHAR szHostName[256] = {0}; 
		int nStatus = ::gethostname(szHostName, sizeof(szHostName)); 
		if (nStatus == SOCKET_ERROR ) 
		{ 
			return m_szHostIPAddress; 
		} 

		//	取得所有的IP地址列表。
		HOSTENT *host = ::gethostbyname(szHostName); 
		if (host != NULL) 
		{ 
			for ( int iLoop =0; ; iLoop++ ) 
			{ 
				m_szHostIPAddress = inet_ntoa( *(IN_ADDR*) host->h_addr_list[iLoop] ) ; 

				if ( host->h_addr_list[iLoop] + host->h_length >= host->h_name ) 
				{
					break; 
				}
			} 
		}	//end if(host != NULL) 

		WSACleanup(); 
		
		FormatIPAddress(m_szHostIPAddress);
	}	// end if (m_szHostIPAddress.IsEmpty())

	return m_szHostIPAddress;
}

//	---------------------------------------------------------------------------
//
// Function:    IsNumeric()
//
// Description:
//  检查输入的字符串是不是数值,带小数点的也是数值.按照ASCII标准来检查的，如果是UNICODE，那么
//  本程序要重新写过。
//
//
// Input:
//	LPCTSTR lpszText       --  待检查的字符串。
//
// Returns:
//  BOOL                   -- TRUE : 数字
//                            FALSE: 不是数字
//
//	---------------------------------------------------------------------------
BOOL CUtility::IsNumeric(LPCTSTR lpszText)
{
	CHAR  cValue;

	BOOL	bRet = TRUE;
	int		iTimes = 0;

	const CHAR  Code_Dot = 46, Code_9 = 57, Code_0 = 48, code_Minus = 45;

	for( unsigned int iLoop =0; iLoop < strlen(lpszText); iLoop++)
	{
		cValue = lpszText[iLoop];
		
		// 如果第一位是 负号， 那么继续查找下一位。
		if( iLoop == 0 && cValue == code_Minus )
			continue;

		if( cValue == Code_Dot)
		{
			iTimes ++;
			if ( iTimes > 1 )		// 如果出现2个或者2个以上的小数点"."， 那么返回FALSE.
			{
				bRet = FALSE;
				break;
			}
		}else
		{
			// 如果值>9，或者<0,那么返回FALSE.
			if ( cValue > Code_9 || cValue < Code_0 )
			{
				bRet = FALSE;
				break;
			}
		}
	}

	return bRet;
}

//	---------------------------------------------------------------------------
//
// Function:    IsInteger()
//
// Description:
//  检查输入的字符串是不是整数。按照ASCII标准来检查的，如果是UNICODE，那么
//  本程序要重新写过。
//
//
// Input:
//	LPCTSTR lpszText       --  待检查的字符串。
//
// Returns:
//  BOOL                   -- TRUE : 是
//                            FALSE: 不是
//
//	---------------------------------------------------------------------------
BOOL CUtility::IsInteger(LPCTSTR lpszText)
{
	CHAR  cValue;

	BOOL	bRet = TRUE;

	const CHAR  Code_Dot = 46, Code_9 = 57, Code_0 = 48 , code_Minus = 45;

	for( unsigned int iLoop =0; iLoop < strlen(lpszText); iLoop++)
	{
		cValue = lpszText[iLoop];

		// 如果第一位是 负号， 那么继续查找下一位。
		if( iLoop == 0 && cValue == code_Minus )
			continue;


		// 如果值>9，或者<0,那么返回FALSE.
		if ( cValue > Code_9 || cValue < Code_0 )
		{
			bRet = FALSE;
			break;
		}
	}

	return bRet;
}

//	---------------------------------------------------------------------------
//
// Function:    IncludeSymbol()
//
// Description:
//  检查输入的字符串中包含不包含符号（./;'等），减号也是符号。按照ASCII标准来检查的，如果是UNICODE，那么
//  本程序要重新写过。
//
//
// Input:
//	LPCTSTR lpszText       --  待检查的字符串。
//
// Returns:
//  BOOL                   -- TRUE : 含
//                            FALSE: 不含
//
//	---------------------------------------------------------------------------
BOOL CUtility::IncludeSymbol(LPCTSTR lpszText)
{
	CHAR  cValue;

	BOOL	bRet = FALSE;

	const CHAR  Code_Dot = 46, Code_9 = 57, Code_0 = 48, Code_A = 65, Code_Z = 90;
	const CHAR	Code_a = 97, Code_z = 122;

	for( unsigned int iLoop =0; iLoop <= strlen(lpszText) ; iLoop++)
	{
		cValue = lpszText[iLoop];

		// 如果值<9，或者>0,那么就是数字，标志位设置成FALSE，继续下一个检查字符.
		if ( cValue <= Code_9 && cValue >= Code_0 )
		{
			bRet = FALSE;
			continue;
		}
		// 大写的英文字母。
		if( cValue <= Code_Z && cValue >= Code_A)
		{
			bRet = FALSE;
			continue;
		}
		
		// 小写的英文字母。
		if( cValue <= Code_z && cValue >= Code_a)
		{
			bRet = FALSE;
			continue;
		}
		
	
		//如果 执行到了这里，说明要检查的字符串不是数字和字母，那么就认为是
		//是特殊字符，退出循环。
		bRet = TRUE;
		break;
	}

	return bRet;
}

//	---------------------------------------------------------------------------
//
// Function:    GetMacAddress()
//
// Description:
//  读取指定网卡的Mac地址，这个地址不是EPROM里面的真实地址，可以通过驱动程序来修改的。
//
//
// Input:
//	int NetCardNo            --   网卡编号,一般地,从0开始,但在Windows 2000中并不一定是连续分配的 
//  CString &szMacAddr       --   输出的Mac地址.
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::GetMacAddress (int NetCardNo, CString &szMacAddr) 
{

	// 定义一个存放返回网卡信息的变量 
	ASTAT Adapter;

	NCB ncb; 

	// 首先对选定的网卡发送一个NCBRESET命令,以便进行初始化 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = NetCardNo ;					// 指定网卡号 

	UCHAR uRetCode = ::Netbios(&ncb);

	// 接着,可以发送NCBASTAT命令以获取网卡的信息 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT;		
	ncb.ncb_lana_num = NetCardNo;					// 指定网卡号 
	strcpy_s((char *)ncb.ncb_callname,sizeof("*"), "*" ); 

	ncb.ncb_buffer = (unsigned char *) &Adapter;	// 指定返回的信息存放的变量 
	ncb.ncb_length = sizeof(Adapter); 

	uRetCode = ::Netbios( &ncb ); 
	if ( uRetCode == 0 ) 
	{
		// 把网卡MAC地址格式化成常用的16进制形式,如0010A4E45802 
		szMacAddr.Format("%02X%02X%02X%02X%02X%02X%02X", 
							NetCardNo, 
							Adapter.adapt.adapter_address[0], 
							Adapter.adapt.adapter_address[1], 
							Adapter.adapt.adapter_address[2], 
							Adapter.adapt.adapter_address[3], 
							Adapter.adapt.adapter_address[4], 
							Adapter.adapt.adapter_address[5] ); 
	}

} 
//	---------------------------------------------------------------------------
//
// Function:    GetFirstEffectiveAddr()
//
// Description:
//  读取第一个有效网卡的Mac地址，这里的有效是指Mac地址里面没有好多个（12个以下）0。
//
//
// Input:
//  CString &szpAddr       --   输出的Mac地址。
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------

void CUtility::GetFirstEffectiveAddr(CString &szpAddr)
{
	NCB ncb; 

	UCHAR uRetCode; 
	LANA_ENUM lana_enum; 

	// 向网卡发送NCBENUM命令,以获取当前机器的网卡信息,如有多少个网卡、每张网卡的编号等 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBENUM; 
	ncb.ncb_buffer = (unsigned char *) &lana_enum; 
	ncb.ncb_length = sizeof(lana_enum); 

	uRetCode = ::Netbios( &ncb ); 

	CString szMacAddr;
	if ( uRetCode == 0 ) 
	{ 
		// 对每一张网卡,以其网卡编号为输入编号,获取其MAC地址 
		for ( int iLoop=0; iLoop<lana_enum.length; ++iLoop) 
		{
			GetMacAddress( lana_enum.lana[iLoop], szMacAddr ); 
		
			//If there is many 0 in szMacAddr, we think it is not effective address.
			TCHAR *szTemp = (LPSTR) (LPCTSTR)szMacAddr;
			int iCount  = 0; 
			while (*szTemp != '\0')
			{
				if( *szTemp == '0') iCount ++;
				szTemp ++;
			}
			// if there count of 0 is less 12, we think it's effective.
			if( iCount <= 12) 
			{
				szpAddr = szMacAddr;
				break;
			}
		}
	}
}
