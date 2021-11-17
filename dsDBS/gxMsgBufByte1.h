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
#ifndef GX_MSGBUF_BYTE1_2021_01_01
#define GX_MSGBUF_BYTE1_2021_01_01

#include "gxMsgBufBase.h"
#include "gxDBSysMsgList.h"
#include "gxByte1Msg.h"

#define MSGBUF_BYTE1_SIZE	2048


class CgxMemBuf
{
public:
	CgxMemBuf() ; 
	virtual ~CgxMemBuf() ;
	char *GetData() ;
	int AddData(void *_pData,int _nLen);
	BOOL ReallocMem(int _nAddSize=1024);
	int GetBufLen() ;
	int GetDataLen() ;
	int InitDataLen(int _nLen) ;
private:
	char *m_pData;
	int m_nDataLen;						
	int m_nBufLen;					
};

class CgxMsgBufByte1 :	public CgxMsgBufBase
{
public:
	CgxMsgBufByte1();
	virtual ~CgxMsgBufByte1(void);
public:
	virtual int InitProtocol(CgxDBSysMsgList *_pSysMsgList) ; 	
	virtual int InitMsg(int _nMsgID,char *_pMsgBuf,int _nMsgLen) ;
	virtual void ReleaseAll() ;
	virtual int GetHeadLen()  ;		virtual int WriteDatalog(char*_pBuf,int _nBufLen);					virtual int SetData(void *_pData, int _nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual int GetDataType(int _nMember,int _nAdd,int _nSub);
	
	virtual int SetIntData(int _nData ,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetShortData(short _nData,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) ;
	virtual int SetInt64Data(__int64 _Data ,int _nMember,int _nAdd,int _nSub)  ;
	virtual int SetSuffixData(void *_pData ,int _nDataLen,int _nMember,int _nAdd,int _nSub);

	virtual int GetIntData(int _nMember,int _nAdd,int _nSub);
	virtual short GetShortData(int _nMember,int _nAdd,int _nSub);
	virtual float GetFloatData(int _nMember,int _nAdd,int _nSub);
	virtual double GetDoubleData(int _nMember,int _nAdd,int _nSub) ;
	virtual __int64 GetInt64Data(int _nMember,int _nAdd,int _nSub);
	virtual void* GetSuffixData(int _nMember,int _nAdd,int _nSub);
	virtual int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) ;
	virtual void* GetSuffixData(int &_nSuffixLen ,int _nMember,int _nAdd,int _nSub);
	virtual int GetDataStr(char *_pstrValue ,int &_nValueLen,int _nMember,int _nAdd,int _nSub); 

	virtual int GetHeadInfo(CgxMsgHeadInfo &_Info) ;

	virtual int BeginAddData();
	virtual int EndAddData(int _nAddCount);
	virtual int GetDataLen();
	virtual void * GetData();
	MSG_SUFFIX *GetSuffix(int _nMember,int _nAdd,int _nSub);
public:
	static int Init(CgxDBSysMsgList *_pSysMsgList);
public:
	static int WriteByte1MsgListLog();
	int WriteDataLog();
	int GetLogStr(char *_pBuf,void *_pItem,int _nMember,int _nAdd,int _nSub);
	int WriteDataLog(CgxByte1Struct *_pStruct);
private:
	static CgxByte1MsgList m_Byte1MsgItems;
	CgxByte1Msg *m_pByte1Msg;
	CgxMemBuf m_Buf ;
	CgxMemBuf m_AddBuf;					
	CgxMemBuf m_SuffixBuf ;				
	int CheckAddBufLen(int _nAddIdx);
};

#endif