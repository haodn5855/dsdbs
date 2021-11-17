/******************************************************************************
**************************鼎石数据*********************************************
*******************************************************************************
	*版权：2013-2021 郝多年 版权所有 
	*版本：1.8
	*作者：郝多年
	*微信：19843355855
	*邮箱：han_dn@sohu.com
	*免责及版权声明：
	1、使用本开源产品时必须完整保留此版权及免责声明，不能对此版权及免责声明进行
	任何增加、修改、删除等操作。
	2、使用本产品或享受相关技术支持服务等需遵守中国相关法律及所在国法律。若将产
	 品或服务用于非法目地与作者无关。作者不负连带责任，所有责任由使用者自行承担。
	3、本产品无法做到零Bug。故不承担因本产品Bug或缺陷导致的任何后果。因此,在使
	 用本产品前请务必做好相关测试。
	4、本开源产品现在或将来会提供与多个开源产品如My Sql,protobuf,redis等的接入示
	例，若接入示例存在对相应开源产品侵权请及时告知，将进行相应修改或删除;使用者
	在使用本产品时用到相应的开源产品，请自行获得相关产品的使用许可。
	5、公司或组织少于100人且服务器少于50台以及个人使用者在符合中国相关法律及所
	 在国法律，并保留本产品完整免责及版权声明。可任意复制、修改、使用、学习相关
	 代码。
	6、公司或组织大于100人或服务器大于50台的使用本产品均需获得使用许可，不论是
	直接或间接使用或何种渠道从第三方获得含有本产品的源代码或经过编译发布的产品
	均需获得使用许可。
*******************************************************************************
**************************鼎石数据*********************************************
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