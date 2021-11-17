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
#pragma once
#ifndef GX_SYS_INTERFACE_CFG_2021_01_01
#define GX_SYS_INTERFACE_CFG_2021_01_01

typedef void *(WINAPI *DSCreateDBLoad)(int _nID) ;
typedef void *(WINAPI *DSCreateDBExec)(int _nID,void*_pPool); 
typedef void *(WINAPI *DSCreateMsgBuf)(int _nID); 
class CgxDBLoadBase;
class CgxDBExecBase;
class CgxMsgBufBase;
class CgxSysInterfaceCfg
{
public:
	CgxSysInterfaceCfg(void);
	virtual ~CgxSysInterfaceCfg(void);
	int m_nInterfaceID;
	char m_szIFName[128];
	char m_szIFPath[256];
	int m_nIsEnable;
	int LoadDll() ;

	DSCreateDBLoad m_FuncCreateDBLoad;
	DSCreateDBExec m_FuncCreateDBExec;
	DSCreateMsgBuf m_FuncCreateMsgBuf;
	CgxDBLoadBase * m_pDBLoadBase;
	CgxDBExecBase * m_pDBExecBase;
	CgxMsgBufBase * m_pMsgBufBase;
private:
	HMODULE m_hDll ;
};
class CgxSysInterfaceCfgList
{
public:
	CgxSysInterfaceCfgList(void);
	virtual ~CgxSysInterfaceCfgList(void);
	int LoadSysInterfaceCfg(int _nDSSVRID,void *_pDBPool,int _nDBType) ;
	CgxSysInterfaceCfg * GetItemOfIdx(int _nIdx) ;
	CgxSysInterfaceCfg * FindItem(int _nID) ;
	int GetItemCount() ;

	CgxDBLoadBase * CreateDBLoad(int _nID) ;
	CgxDBExecBase * CreateDBExec(int _nID ,void *_pPool) ;
	CgxMsgBufBase * CreateMsgBuf(int _nID) ;
private:
	CgxList <CgxSysInterfaceCfg> m_SysIFItems;
	static int CompFuncSysIF(const void *_pItem1,const void *_pItem2);							};
#endif