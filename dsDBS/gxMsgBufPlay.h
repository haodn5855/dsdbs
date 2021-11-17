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