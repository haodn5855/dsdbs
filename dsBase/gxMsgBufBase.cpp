/******************************************************************************
**************************��ʯ����*********************************************
*******************************************************************************
	*��Ȩ��2013-2021 �¶��� ��Ȩ���� 
	*�汾��1.8
	*���ߣ��¶���
	*΢�ţ�19843355855
	*���䣺han_dn@sohu.com
	*���𼰰�Ȩ������
	1��ʹ�ñ���Դ��Ʒʱ�������������˰�Ȩ���������������ܶԴ˰�Ȩ��������������
	�κ����ӡ��޸ġ�ɾ���Ȳ�����
	2��ʹ�ñ���Ʒ��������ؼ���֧�ַ�����������й���ط��ɼ����ڹ����ɡ�������
	 Ʒ��������ڷǷ�Ŀ���������޹ء����߲����������Σ�����������ʹ�������ге���
	3������Ʒ�޷�������Bug���ʲ��е��򱾲�ƷBug��ȱ�ݵ��µ��κκ�������,��ʹ
	 �ñ���Ʒǰ�����������ز��ԡ�
	4������Դ��Ʒ���ڻ������ṩ������Դ��Ʒ��My Sql,protobuf,redis�ȵĽ���ʾ
	����������ʾ�����ڶ���Ӧ��Դ��Ʒ��Ȩ�뼰ʱ��֪����������Ӧ�޸Ļ�ɾ��;ʹ����
	��ʹ�ñ���Ʒʱ�õ���Ӧ�Ŀ�Դ��Ʒ�������л����ز�Ʒ��ʹ����ɡ�
	5����˾����֯����100���ҷ���������50̨�Լ�����ʹ�����ڷ����й���ط��ɼ���
	 �ڹ����ɣ�����������Ʒ�������𼰰�Ȩ�����������⸴�ơ��޸ġ�ʹ�á�ѧϰ���
	 ���롣
	6����˾����֯����100�˻����������50̨��ʹ�ñ���Ʒ������ʹ����ɣ�������
	ֱ�ӻ���ʹ�û���������ӵ�������ú��б���Ʒ��Դ����򾭹����뷢���Ĳ�Ʒ
	������ʹ����ɡ�
*******************************************************************************
**************************��ʯ����*********************************************
******************************************************************************/

#include "gxMsgBufBase.h"
#pragma warning(disable:4996)

int  GetMemberAndSubIdx(char *_pBuf , int &_nMemberIdx,int &_nSubIdx)
{
	if( _pBuf == NULL ) 
		return -1 ;
	if( strlen(_pBuf)  < 1) 
		return -2 ; 
	char szTmp[512];
	memset(szTmp , 0 , sizeof(szTmp)) ; 
	_snprintf(szTmp , sizeof(szTmp) , "%s" , _pBuf) ; 
	char * p = strstr(szTmp ,".") ;
	if( p == NULL ) 
		return -2 ;
	*p = 0 ;
	try
	{
		_nMemberIdx = atoi( szTmp) ; 
	}catch(...)
	{
	}
	p++ ; 
	try
	{
		_nSubIdx = atoi( p) ;
	}catch(...)
	{
	}
return 0 ; 
}

int  GetMemberAddSub(char *_pBuf , int &_nMember,int &_nAdd,int &_nSub)
{
	if( _pBuf == NULL ) 
		return -1 ;
	if( strlen(_pBuf)  < 1) 
		return -2 ; 
	char szTmp[512];
	memset(szTmp , 0 , sizeof(szTmp)) ; 
	_snprintf(szTmp , sizeof(szTmp) , "%s" , _pBuf) ; 
	char * p = strstr(szTmp ,".") ;
	if( p == NULL ) 
		return -2 ;
	*p = 0 ;
	try
	{
		_nMember = atoi( szTmp) ; 
	}catch(...)
	{
	}
	p++ ; 
	char * pTmp = strstr(p, ".");
	if( pTmp == NULL)
		return -3;
	*pTmp =0;
	try
	{
		_nAdd = atoi( p) ; 
	}catch(...)
	{
	}
	pTmp++;
	try
	{
		_nSub = atoi( pTmp) ; 
	}catch(...)
	{
	}
return 0 ;
}
CgxMsgHeadInfo::CgxMsgHeadInfo()
{
	m_dwMsgHeadID =0;
	m_dwMsgHeadProto=0;
	m_dwMsgBodyProto=0;
	memset(&m_MemberMsgID,0,sizeof(m_MemberMsgID));
	memset(&m_MemberMsgLen,0,sizeof(m_MemberMsgLen));
	memset(&m_MemberSerial,0,sizeof(m_MemberSerial));
	m_dwHeadLen=0;
}
CgxMsgHeadInfo::~CgxMsgHeadInfo()
{
}

CgxMsgBufBase::CgxMsgBufBase(void)
{
	m_dwMsgID=0;
	m_dwMsgHeadID=0 ;
	m_dwMsgHeadProto=0;
	m_dwMsgBodyProto=0;
}

CgxMsgBufBase::~CgxMsgBufBase(void)
{
}
