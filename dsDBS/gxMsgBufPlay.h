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
#ifndef GX_MSGBUF_PLAY_2021_01_01
#define GX_MSGBUF_PLAY_2021_01_01

#include "gxMsgBufBase.h"
#include "gxMsgBufByte1.h"
class CgxDBSysMsg;
class CgxMsgBufPlay :public CgxMsgBufBase
{
public:
	CgxMsgBufPlay();
	CgxMsgBufPlay(DWORD _dwMsgID,void *_pMsg,int _nMsgLen,int _HeadLen=-1);
	virtual ~CgxMsgBufPlay(void);
	static int GetMsgHeadInfo(CgxMsgHeadInfo &_Info,DWORD _dwMsgHeadID,int _nProtoType);	CgxMsgBufBase* Init(DWORD _dwMsgID,CgxDBSysMsg *_pDBMsg,void *_pMsg,int _nMsgLen,int _HeadLen=-1);		CgxMsgBufBase* GetBasePtr() ;	
	CgxMsgBufBase* GetBaseOfType(int _nType);
	void ReleaseAll() ;
	int GetDataStr(char *_pstrValue,int _nValueLen,char *_pszMembers) ; private:
	CgxMsgBufBase *m_pMsgBufBase;
	CgxMsgBufBase *m_pHeadBufBase;
	int GetMsgTypeOfID(DWORD _dwMsgID);
public:
	int InitProtocol(CgxDBSysMsgList *_pSysMsgList)   ;			int InitMsg(int m_nMsgID,char *_pMsgBuf,int _nMsgLen) ;
	int GetHeadLen()  ;		int WriteDatalog(char*_pBuf,int _nBufLen);											int SetData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub) ;
	void* GetData(int _nMember,int _nAdd,int _nSub) ;
	void* GetData(int &_nDataLen,int _nMember,int _nAdd,int _nSub) ;
	int GetDataLen(int _nMember,int _nAdd,int _nSub) ;
	int GetDataType(int _nMember,int _nAdd,int _nSub);
	
	int SetIntData(int _nData,int _nMember,int _nAdd,int _nSub) ;
	int SetShortData(short _nData,int _nMember,int _nAdd,int _nSub) ;
	int SetFloatData(float _fData,int _nMember,int _nAdd,int _nSub) ;
	int SetDoubleData(double _Data,int _nMember,int _nAdd,int _nSub) ;
	int SetInt64Data(__int64 _Data,int _nMember,int _nAdd,int _nSub)  ;
	int SetSuffixData(void *_pData,int _nDataLen,int _nMember,int _nAdd,int _nSub);

	int GetIntData(int _nMember,int _nAdd,int _nSub);
	short GetShortData(int _nMember,int _nAdd,int _nSub);
	float GetFloatData(int _nMember,int _nAdd,int _nSub);
	double GetDoubleData(int _nMember,int _nAdd,int _nSub) ;
	__int64 GetInt64Data(int _nMember,int _nAdd,int _nSub);
	void* GetSuffixData(int _nMember,int _nAdd,int _nSub);
	int GetSuffixDataLen(int _nMember,int _nAdd,int _nSub) ;
	void* GetSuffixData(int &_nSuffixLen,int _nMember,int _nAdd,int _nSub);
	int GetDataStr(char *_pstrValue,int &_nValueLen,int _nMember,int _nAdd,int _nSub); 

	int GetHeadInfo(CgxMsgHeadInfo &_Info);

	int BeginAddData();
	int EndAddData(int _nAddCount);
	virtual int GetDataLen()  ;
	virtual void * GetData()  ;
	int MakeBuf() ;
	int SetHeadLen(int _nLen ,int _nMember,int _nAdd,int _nSub,int _nType) ;
	int DebugInfo() ;
	static int m_nDebugInfoFlag;
private:
	CgxMemBuf m_Buf ;
};

#endif