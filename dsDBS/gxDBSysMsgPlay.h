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

#include <gxFunc.h>
#include <gxList.h>
#include "gxDBExecBase.h"
#include "gxMsgBufPlay.h"
#pragma once
#ifndef GX_DBSYS_MSG_PLAY_2021_01_01
#define GX_DBSYS_MSG_PLAY_2021_01_01

class CgxDBSysDBSvrInfo;
class CgxDBSysMsgWhere
{
public:
	CgxDBSysMsgWhere() ;
	virtual ~CgxDBSysMsgWhere() ;
public:
	int m_nDSWhereID;
	int m_nDSMsgPlayID;
	char m_szWhereStr[512];
	char m_szAckMember[256];
	char m_szReqMember[256];
	int m_nColIdx;
public:
		int MemberStrToInt();
	int m_nAckMember;
	int m_nAckAdd;
	int m_nAckSub;
	int m_nReqMember;
	int m_nReqAdd;
	int m_nReqSub;
};

class CgxDBSysMsgCol
{
public:
	CgxDBSysMsgCol() ;
	virtual ~CgxDBSysMsgCol() ;
public:
	int m_nDSColID;
	int m_nDSMsgPlayID;
	char m_szColName[256];
	int m_nColType;
	int m_nColLen;
	char m_szAckMember[256];
	char m_szReqMember[256];
	int m_nSetAckFlag;
	int m_nColIdx;
	char m_szFmtStr[256];
public:
		int MemberStrToInt();
	int m_nAckMember;
	int m_nAckAdd;
	int m_nAckSub;
	int m_nReqMember;
	int m_nReqAdd;
	int m_nReqSub;
};

class CgxDBSysMsgPlay
{
public:
	CgxDBSysMsgPlay();
	virtual ~CgxDBSysMsgPlay();
public:
	int m_nDSMsgPlayID;
	char m_szMsgPlayName[256];
	int m_nAckID;
	int m_nReqID;
	int m_nDSDBSID;
	char m_szTableName[512];
	char m_szAddTableName[256];
	int m_nAddTableNameMode;
	char m_szTableNameSufID[256];
	int m_nTableNameSufType;
	int m_nSqlOperType;
	char m_szWhereStr[512];
	char m_szGroupStr[512];
	char m_szOrderStr[512];
	char m_szRetMember[256];
public:
		int MemberStrToInt();
	int m_nAddTabMember;
	int m_nAddTabAdd;
	int m_nAddTabSub;
	int m_nTabSufMember;
	int m_nTabSufAdd;
	int m_nTabSufSub;
	int m_nRetMember;
	int m_nRetAdd;
	int m_nRetSub;

	int m_AckMsgIDMember[4];  	
	int m_AckMsgLenMember[4];
	int m_AckMsgSerialMember[4];
public:
	int GetMsgColCount() ;
	int GetMsgWhereCount();
	int AddMsgCol(CgxDBSysMsgCol *_pMsgCol) ;
	int AddMsgWhere(CgxDBSysMsgWhere *_pMsgWhere) ;
	CgxDBSysMsgCol * GetMsgCol(int _nIdx);
	CgxDBSysMsgWhere *GetMsgWhere(int _nIdx) ;

	int Play(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf , CgxMsgBufPlay &_AckBuf);
private:
	CgxList <CgxDBSysMsgCol> m_MsgColItems;
	CgxList <CgxDBSysMsgWhere> m_MsgWhereItems;

	int MsgPlayInsert(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf);
	int MsgPlaySelect(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf);
	int MsgPlayUpdate(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf);
	int MsgPlayDelete(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf);

	int SetAckMsg(CgxMsgBufPlay &_Req,CgxMsgBufPlay &_Ack);
	int SetAckMsgHeadLen( CgxMsgBufPlay &_Ack);
	int GetTableName(char *_pTableName,CgxMsgBufPlay *_pReq );
	int GetFirstAddTableName(char *_pTableName , CgxMsgBufPlay *_pReq);
	DBTABLEINFO *CgxDBSysMsgPlay::GetTableColInfo(CgxMsgBufPlay *_pReq ,CgxMsgBufPlay *_pAck);
	int GetMsgWhereStr(char *_pBuf ,CgxMsgBufPlay *_pReq);
	int SetSelectMsgData(CgxDBSysMsgCol *_pMsgCol ,DBCOLINFO * _pInfo,CgxMsgBufPlay *_pMsgBuf,int _nAddIdx);
	int GetTypeSize(int _nType);

	int GetMsgUpdateColStr(char *_pCol,CgxMsgBufPlay *_pReqEx);
	int GetUpdateMemberStr(char *_pBuf,CgxDBSysMsgCol *_pMsgCol,CgxMsgBufPlay *_pReq);
	int SetDBRet(int _nRet,CgxMsgBufPlay &_AckBuf) ;
};

class CgxDBSysMsgPlayList
{
public:
	CgxDBSysMsgPlayList() ;
	virtual ~CgxDBSysMsgPlayList() ;
	void ReleaseAll();
	CgxDBSysMsgPlay *GetMsgPlayOfIdx(int _nIdx);
	CgxDBSysMsgPlay *GetMsgPlayOfReqID(int _nID);
	CgxDBSysMsgPlay *GetMsgPlayOfID(int _nID);
	int InsertMsgPlay(CgxDBSysMsgPlay * _pMsg);
	int GetMsgPlayCount() ;
public:
	int AddMsgCol(CgxDBSysMsgCol *_pMsgCol,int _nReqID) ;
	int AddMsgWhere(CgxDBSysMsgWhere *_pMsgWhere,int _nReqID) ;
private:
	CgxList <CgxDBSysMsgPlay> m_MsgPlayItems;
	static int CompFuncDBSysMsgPlay(const void *_pItem1,const void *_pItem2);							public:
	int LoadDBSys(void *_pDBPool,int _nDBType,int _nDSSVRID) ;
private:
	int LoadDBSysMsgPlay(void *_pDBPool,int _nDBType,int _nDSSVRID) ;
	int LoadDBSysMsgColPlay(void *_pDBPool,int _nDBType,int _nDSSVRID) ;
	int LoadDBSysMsgWherePlay(void *_pDBPool,int _nDBType,int _nDSSVRID) ;
};
class CgxDBSysDBSvrInfo
{
public:
	CgxDBSysDBSvrInfo();
	virtual ~CgxDBSysDBSvrInfo();
	int m_nDSDBSID;
	int m_nDBType;
	char m_szDBName[128];
	char m_szDSNName[128];
	char m_szIP[128];
	int m_nPort;
	int m_nConnectCount;
	char m_szDBUserName[128];
	char m_szDBPassword[128];
public:
	int InitDB() ;
	void * GetDBPool() ;
private:
	CgxDBLoadBase *m_pDBLoadBase;
};

#endif