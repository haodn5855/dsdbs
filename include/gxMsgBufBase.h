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

#pragma once
#ifndef GX_MSGBUFBASE_2021_01_01
#define GX_MSGBUFBASE_2021_01_01
#include <gxFunc.h>
#include <gxBaseDll.h>

#define		DS_MSG_TYPE_NONE					20000
#define		DS_MSG_TYPE_BYTE1					20001
#define		DS_MSG_TYPE_BYTE2					20002
#define		DS_MSG_TYPE_BYTE4					20004
#define		DS_MSG_TYPE_GOOGLE_PROTOBUF			20008
#define		DS_MSG_TYPE_XML						20016
#define		DS_MSG_TYPE_JOSN					20032
#define		DS_MSG_TYPE_JCE						20064

#define		DS_MSG_TYPE_MAX						29999


#define		GX_MSGBUF_DATATYPE_CHAR					0														
#define		GX_MSGBUF_DATATYPE_BYTE					1														
#define		GX_MSGBUF_DATATYPE_SWORD				2														
#define		GX_MSGBUF_DATATYPE_WORD					3														
#define		GX_MSGBUF_DATATYPE_INT					4														
#define		GX_MSGBUF_DATATYPE_UINT					5														
#define		GX_MSGBUF_DATATYPE_STR					6														
#define		GX_MSGBUF_DATATYPE_RAWSTR				7														
#define		GX_MSGBUF_DATATYPE_BINARY				8														
#define		GX_MSGBUF_DATATYPE_LONGLONG				9														
#define		GX_MSGBUF_DATATYPE_FLOAT				10														
#define		GX_MSGBUF_DATATYPE_DATE					11														
#define		GX_MSGBUF_DATATYPE_DECIMAL				12														
#define		GX_MSGBUF_DATATYPE_DOUBLE				15	
#define		GX_MSGBUF_DATATYPE_SUFFIX				31		

#define		GX_MSGBUF_DATATYPE_MAX					1000	


typedef struct tagDSMsgSuffix
{
	DWORD dwOffSet;
	DWORD dwSize;
}MSG_SUFFIX;
class DS_BASE_DLL CgxMsgHeadInfo
{
public:
	CgxMsgHeadInfo();
	virtual ~CgxMsgHeadInfo();
public:
	DWORD m_dwMsgHeadID ;
	DWORD m_dwMsgHeadProto;
	DWORD m_dwMsgBodyProto;
	MSG_SUFFIX m_MemberMsgID;
	MSG_SUFFIX m_MemberMsgLen;
	MSG_SUFFIX m_MemberSerial;
	DWORD m_dwHeadLen;
};
class DS_BASE_DLL CgxMsgBufLoadBase
{
public:
	CgxMsgBufLoadBase(){};
	virtual ~CgxMsgBufLoadBase(){};
};

extern DS_BASE_DLL int GetMemberAndSubIdx(char *_pBuf , int &_nMemberIdx,int &_nSubIdx);
extern DS_BASE_DLL int GetMemberAddSub(char *_pBuf , int &_nMember,int &_nAdd,int &_nSub);
class CgxDBSysMsgList;
class DS_BASE_DLL CgxMsgBufBase
{
public:
	DWORD m_dwMsgID;
	DWORD m_dwMsgHeadID ;
	DWORD m_dwMsgHeadProto;
	DWORD m_dwMsgBodyProto;
public:
	CgxMsgBufBase(CgxMsgBufLoadBase *_pLoad , DWORD _dwMsgType){};
	CgxMsgBufBase();
	virtual ~CgxMsgBufBase(void);
	virtual int InitProtocol(CgxDBSysMsgList *_pSysMsgList) = 0 ;		
	virtual int InitMsg(int m_nMsgID,char *_pMsgBuf,int _nMsgLen) =0;
	virtual void ReleaseAll() = 0 ;
	virtual int GetHeadLen() = 0 ;	
	virtual int WriteDatalog(char*_pBuf,int _nBufLen)=0;				
	virtual int SetData(void *_pData, int _nDataLen,int _nMember,int _nAdd,int _nSub) =0;
	virtual void* GetData(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int GetDataLen(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int GetDataType(int _nMember,int _nAdd,int _nSub)=0;
	
	
	virtual int SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetShortData(short _nData ,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) =0;
	virtual int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub)=0 ;
	virtual int SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub) =0 ;
	virtual int SetSuffixData(void *_pData , int _nDataLen ,int _nMember,int _nAdd,int _nSub)=0;

	virtual int GetIntData(int _nMember,int _nAdd,int _nSub)=0;
	virtual short GetShortData(int _nMember,int _nAdd,int _nSub)=0;
	virtual float GetFloatData(int _nMember,int _nAdd,int _nSub)=0;
	virtual double GetDoubleData(int _nMember,int _nAdd,int _nSub) =0;
	virtual __int64 GetInt64Data(int _nMember,int _nAdd,int _nSub)=0;
	virtual void* GetSuffixData(int _nMember,int _nAdd,int _nSub)=0;
	virtual int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub)=0 ;
	virtual void* GetSuffixData(int &_nSuffixLen ,int _nMember,int _nAdd,int _nSub)=0;

	virtual int GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub)=0; 
	virtual int GetHeadInfo(CgxMsgHeadInfo &_Info) =0;
	
	virtual int BeginAddData()=0;
	virtual int EndAddData(int _nAddCount)=0;
	virtual int GetDataLen() = 0 ;
	virtual void * GetData() = 0 ;
};

#endif