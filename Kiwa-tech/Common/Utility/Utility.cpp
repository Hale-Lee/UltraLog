///////////////////////////////////////////////////////////////////////////////
//	
//	Copyright (C) Li Haolai 2012 - All Rights Reserved
//	
//	Author:		Li HaoLai (lihaolai0000@163.com, QQ:3883140)
//	File:		CUtility
//	Version: 	1.0
//	Date: 		2012-01-09
//	
//	Purpose:	������,��װ��һЩ���õĹ���,�����ַ����Ĳ���, ��ʽ������.
//				ע��: �����Ǿ�̬��,���еĳ�Ա�������Ǿ�̬��.
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
//  ��һ���ַ����ָ��һ���ַ������飬����ָ���ķָ���š�
//
//
// Input:
//	LPCTSTR szSource			--	Ҫ�ָ�Ķ���
//	LPCTSTR szToken				--	�ָ����
//  CStringArray *pArray        --  �ָ����ַ������顣
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::SplitString(LPCTSTR szSource, LPCTSTR szToken, CStringArray *pArray)
{
	// ����һ����ʱ�ڴ�	
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
			// ����ҵ��ˣ� ��ô���ҵ�����һ���ַ�����������ʱ�ڴ���ȥ��
			memset(szValue,0,iLength * sizeof(CHAR));
			memcpy(szValue , szSource + iFirst, iSecond-iFirst);
			
			// ���������ʱ�ڴ�ֵ�������ȥ��ͬʱ��ʼ��һ��Ѱ�ҡ�
			pArray->Add(szValue);
			iFirst = iSecond + (int)strlen(szToken);

		}else
		{
			// ����Ѿ�����ĩβ�ˣ� ��ô�������е����ݵ���ʱ�ڴ���ȥ��
			memset(szValue,0,iLength * sizeof(CHAR));
			memcpy(szValue, szSource + iFirst, iLength-iFirst);
			pArray->Add(szValue);
		}
	}while(iSecond != -1);

	// �ͷ���ʱ�ڴ档
	free(szValue);
	szValue = NULL;

}

//	---------------------------------------------------------------------------
//
// Function:    CalculateMaxiumDivisor()
//
// Description:
//  �������Լ���� ����һ��LONG���������Լ����
//
//
// Input:
//	LongArray *pArray			--	Ҫ��������Լ����
//
// Returns:
//  LONG                        --  �����Ľ����
//
//	---------------------------------------------------------------------------
LONG CUtility::CalculateMaxiumDivisor(LongArray *pArray)
{
	LONG lReturn = 1;
	
	// ���������û�����ݣ���ô����1
	if (pArray->GetSize() == 0 )
	{
		return 1;
	}
	
	// ���ֻ��һ�����֣���ô�������Լ���������Լ���
	if (pArray->GetSize() == 1 )
	{
		return pArray->GetAt(0);
	}

	// ��ȡ�������е�һ�����ֵ�ֵ��
	LONG lValue = pArray->GetAt(0);
	LONG lMargin  = 1 , lNextValue = 0;
	//ʹ��תת�������ȡ�����Լ����
	for (int iLoop = 1 ; iLoop < pArray->GetSize(); iLoop++)
	{
		lNextValue = pArray->GetAt(iLoop);
		
		//���A��B�����Լ����C����ôabs(A-B)��min(A,B)�����Լ���϶�Ҳ��C.
		while(lMargin != 0 )
		{
			lMargin = abs(lNextValue - lValue);
			// ��ȡһ����С��ֵ
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
//  �򵥵ļ��ܺͱ��뺯���������ֽڰ�ת���ɿ���ʶ��Ĵ�д��Ӣ����ĸ����ʽ��һ���ֽ�ת���ɣ���Ӣ����ĸ��
//
//
// Input:
//  BYTE *pBuf              --  Ҫת���Ķ���
//  LONG lSize������        --  Ҫת���Ķ���ĳ��ȡ�
//
// Returns:
//  CString                 --  ת������ַ�����
//
//	---------------------------------------------------------------------------
CString CUtility::Encode(BYTE *pBuf, LONG lSize)
{
	//���������ǣ���������������ͨ��ѭ�����ķ�ʽ�����ܡ�
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

	//��ʼ����
	BYTE *pValue = (BYTE *)malloc((lSize * 2 + 1 )* sizeof(BYTE) );
	memset(pValue,0,(lSize * 2 + 1) *sizeof(BYTE));

	int iTemp = 0, kLoop = 0;
	for(int iLoop =0 ; iLoop <lSize; iLoop++)
	{
		// ȡ�ֽڵĸ�λ����������ʾת����A��ʼ����ĸ��
		iTemp = bTemp[iLoop] /16;
		pValue[kLoop] = iTemp + 65;
		kLoop ++;
		// ȡ�ֽڵĵ�λ
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
//  �Ȱ��ַ���������ɶ�Ӧ���ֽ����飬Ȼ������ѭ�������ܡ�
//
// Input:
//  LPCTSTR szValue         --  ���ܶ���
//  BYTE *pReturn           --  ת����Ľ����
//  LONG *plSize������      --  ת����Ľ���ĳ��ȡ�
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

		//�������ѣ����ֽںϲ���һ���ֽڡ�
		if( iLoop % 2 == 1)
		{
			bTemp[kLoop] = pBuff[iLoop]  + pBuff[iLoop -1] * 16;
			kLoop ++;
		}
	}


	//���������ǣ���������������ͨ��ѭ�����ķ�ʽ�����ܡ�
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
//  �����ַ�������ָ����Ŀ���ַ����в���һ�����ַ�����������˫�ֽڵ��ַ����С�
//
//
// Input:
//	TCHAR *szSrc			--	Ŀ���ַ�����
//  BYTE *bGoal             --  ���Ҷ���
//  int nGoalLength         --  ���Ҷ���ĳ��ȡ�
//
// Returns:
//  int                     --  -1,���û���ҵ�������ҵ��ˣ���ô��������λ�á�
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
//  ��ʽ��IP��ַ�ַ�������9.13.255.4��ʽ��IP��ַ�ַ�����ʽ����009.013.255.004
//
//
// Input:
//	CString &szIPAddress			--	����/�����IP��ַ�ַ�����
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
		//�����1λ�����֣���ôǰ�����00
		if(szSection[iLoop].GetLength() <=1)
		{
			szSection[iLoop] = "00" + szSection[iLoop];
		}
		// �����2λ�����֣���ôǰ�����1��0
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
//  ȡ�õ�ǰ���Ե�IP��ַ��
//
//
// Input:
//	Nothing
//
// Returns:
//  CString 		--	ȡ�õ�IP��ַ
//
//	---------------------------------------------------------------------------
CString CUtility::GetHostIPAddress()
{
	

	//ȡ��һ���˾Ͳ�Ҫ�ظ��ĵ��ö�Ρ�
	if(m_szHostIPAddress.GetLength() == 0)
	{
		m_szHostIPAddress = "127.000.000.001";

		WORD wVersionRequested = 0; 
		WSADATA wsaData = {0}; 

		// ��ʼ��Socket
		int nErr = 0; 
		wVersionRequested = MAKEWORD( 2, 2 ); 
		nErr = WSAStartup( wVersionRequested, &wsaData ); 
		if ( nErr != 0 ) 
		{ 
			return m_szHostIPAddress; 
		} 

		//ȡ�õ�ǰ�����������
		CHAR szHostName[256] = {0}; 
		int nStatus = ::gethostname(szHostName, sizeof(szHostName)); 
		if (nStatus == SOCKET_ERROR ) 
		{ 
			return m_szHostIPAddress; 
		} 

		//	ȡ�����е�IP��ַ�б�
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
//  ���������ַ����ǲ�����ֵ,��С�����Ҳ����ֵ.����ASCII��׼�����ģ������UNICODE����ô
//  ������Ҫ����д����
//
//
// Input:
//	LPCTSTR lpszText       --  �������ַ�����
//
// Returns:
//  BOOL                   -- TRUE : ����
//                            FALSE: ��������
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
		
		// �����һλ�� ���ţ� ��ô����������һλ��
		if( iLoop == 0 && cValue == code_Minus )
			continue;

		if( cValue == Code_Dot)
		{
			iTimes ++;
			if ( iTimes > 1 )		// �������2������2�����ϵ�С����"."�� ��ô����FALSE.
			{
				bRet = FALSE;
				break;
			}
		}else
		{
			// ���ֵ>9������<0,��ô����FALSE.
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
//  ���������ַ����ǲ�������������ASCII��׼�����ģ������UNICODE����ô
//  ������Ҫ����д����
//
//
// Input:
//	LPCTSTR lpszText       --  �������ַ�����
//
// Returns:
//  BOOL                   -- TRUE : ��
//                            FALSE: ����
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

		// �����һλ�� ���ţ� ��ô����������һλ��
		if( iLoop == 0 && cValue == code_Minus )
			continue;


		// ���ֵ>9������<0,��ô����FALSE.
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
//  ���������ַ����а������������ţ�./;'�ȣ�������Ҳ�Ƿ��š�����ASCII��׼�����ģ������UNICODE����ô
//  ������Ҫ����д����
//
//
// Input:
//	LPCTSTR lpszText       --  �������ַ�����
//
// Returns:
//  BOOL                   -- TRUE : ��
//                            FALSE: ����
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

		// ���ֵ<9������>0,��ô�������֣���־λ���ó�FALSE��������һ������ַ�.
		if ( cValue <= Code_9 && cValue >= Code_0 )
		{
			bRet = FALSE;
			continue;
		}
		// ��д��Ӣ����ĸ��
		if( cValue <= Code_Z && cValue >= Code_A)
		{
			bRet = FALSE;
			continue;
		}
		
		// Сд��Ӣ����ĸ��
		if( cValue <= Code_z && cValue >= Code_a)
		{
			bRet = FALSE;
			continue;
		}
		
	
		//��� ִ�е������˵��Ҫ�����ַ����������ֺ���ĸ����ô����Ϊ��
		//�������ַ����˳�ѭ����
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
//  ��ȡָ��������Mac��ַ�������ַ����EPROM�������ʵ��ַ������ͨ�������������޸ĵġ�
//
//
// Input:
//	int NetCardNo            --   �������,һ���,��0��ʼ,����Windows 2000�в���һ������������� 
//  CString &szMacAddr       --   �����Mac��ַ.
//
// Returns:
//  Nothing
//
//	---------------------------------------------------------------------------
void CUtility::GetMacAddress (int NetCardNo, CString &szMacAddr) 
{

	// ����һ����ŷ���������Ϣ�ı��� 
	ASTAT Adapter;

	NCB ncb; 

	// ���ȶ�ѡ������������һ��NCBRESET����,�Ա���г�ʼ�� 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBRESET; 
	ncb.ncb_lana_num = NetCardNo ;					// ָ�������� 

	UCHAR uRetCode = ::Netbios(&ncb);

	// ����,���Է���NCBASTAT�����Ի�ȡ��������Ϣ 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBASTAT;		
	ncb.ncb_lana_num = NetCardNo;					// ָ�������� 
	strcpy_s((char *)ncb.ncb_callname,sizeof("*"), "*" ); 

	ncb.ncb_buffer = (unsigned char *) &Adapter;	// ָ�����ص���Ϣ��ŵı��� 
	ncb.ncb_length = sizeof(Adapter); 

	uRetCode = ::Netbios( &ncb ); 
	if ( uRetCode == 0 ) 
	{
		// ������MAC��ַ��ʽ���ɳ��õ�16������ʽ,��0010A4E45802 
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
//  ��ȡ��һ����Ч������Mac��ַ���������Ч��ָMac��ַ����û�кö����12�����£�0��
//
//
// Input:
//  CString &szpAddr       --   �����Mac��ַ��
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

	// ����������NCBENUM����,�Ի�ȡ��ǰ������������Ϣ,���ж��ٸ�������ÿ�������ı�ŵ� 
	memset( &ncb, 0, sizeof(ncb) ); 
	ncb.ncb_command = NCBENUM; 
	ncb.ncb_buffer = (unsigned char *) &lana_enum; 
	ncb.ncb_length = sizeof(lana_enum); 

	uRetCode = ::Netbios( &ncb ); 

	CString szMacAddr;
	if ( uRetCode == 0 ) 
	{ 
		// ��ÿһ������,�����������Ϊ������,��ȡ��MAC��ַ 
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
