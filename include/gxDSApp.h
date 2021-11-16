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

#ifndef GX_DS_APP_2021_07_15
#define GX_DS_APP_2021_07_15
#include <gxDBSDll.h>
#include <Windows.h>
#include <string>
using namespace std;

class DS_DBS_DLL CgxDSApp
{
public:
	CgxDSApp() ;
	~CgxDSApp();
	void ReleaseAll() ;
	int Init(int _nDBType,char *_pDBName,char *_pDsnName,char *_pIP,int _nPort,int _nCnntCount,char*_pUser,char *_pPass);
	int OnMsg(DWORD _dwMsgID ,string _Req,string &_Ack) ;				
	void SetDebug(BOOL _bEnable=TRUE);
	inline void SetDSSVRID(int _nDSSVRID);
	int GetHeadLen(DWORD _dwProtoType,DWORD _dwMsgID) ;
	int GetMsgLen(void *_pMsgHeadData,int _nDataLen , DWORD _dwProtoType,DWORD _dwMsgID) ;
	DWORD GetMsgType(void *_pMsgHeadData ,int _nDataLen , DWORD _dwProtoType,DWORD _dwMsgID) ;
private:
};

#endif