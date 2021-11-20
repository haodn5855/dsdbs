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

#include "gxDBSysMsgPlay.h"
#include "gxDBPlay.h"
#pragma warning(disable:4996)
#include "gxDBExecOdbc.h"
#include "gxDSKernel.h"

CgxDBSysMsgWhere::CgxDBSysMsgWhere()
{
	m_nDSWhereID=0;
	m_nDSMsgPlayID=0;
	memset(m_szWhereStr,0,sizeof(m_szWhereStr)) ;
	memset(m_szAckMember,0,sizeof(m_szAckMember)) ;
	memset(m_szReqMember,0,sizeof(m_szReqMember)) ;
	m_nColIdx=0;

	m_nAckMember=-1;
	m_nAckAdd=-1;
	m_nAckSub=-1;
	m_nReqMember=-1;
	m_nReqAdd=-1;
	m_nReqSub=-1;
}
CgxDBSysMsgWhere::~CgxDBSysMsgWhere()
{
}
int CgxDBSysMsgWhere::MemberStrToInt()
{
	if( strlen(m_szAckMember) >2)
	{
		if( GetMemberAddSub(m_szAckMember,m_nAckMember,m_nAckAdd,m_nAckSub) <0)
			gxWriteLog("MsgCol:AckMember%s 格式错误! DSWhereID[%d]", m_szAckMember,m_nDSWhereID) ;
	}
	if( strlen(m_szReqMember) >2)
	{
		if(GetMemberAddSub(m_szReqMember,m_nReqMember,m_nReqAdd,m_nReqSub) <0)
			gxWriteLog("MsgCol:ReqMember%s 格式错误! DSWhereID[%d]", m_szReqMember,m_nDSWhereID) ;
	}
return 0 ;
}
CgxDBSysMsgCol::CgxDBSysMsgCol()
{
	m_nDSColID=0;
	m_nDSMsgPlayID=0;
	memset(m_szColName,0,sizeof(m_szColName));
	m_nColType=0;
	m_nColLen=0;
	memset(m_szAckMember,0,sizeof(m_szAckMember));
	memset(m_szReqMember,0,sizeof(m_szReqMember));
	m_nSetAckFlag=0;
	m_nColIdx=0;

	memset(m_szFmtStr, 0 ,sizeof(m_szFmtStr)) ;

	m_nAckMember=-1;
	m_nAckAdd=-1;
	m_nAckSub=-1;
	m_nReqMember=-1;
	m_nReqAdd=-1;
	m_nReqSub=-1;
}
CgxDBSysMsgCol::~CgxDBSysMsgCol()
{
}
int CgxDBSysMsgCol::MemberStrToInt()
{
	if( strlen(m_szAckMember) >2)
	{
		if( GetMemberAddSub(m_szAckMember,m_nAckMember,m_nAckAdd,m_nAckSub) <0)
			gxWriteLog("MsgCol:AckMember%s 格式错误! DSColID[%d]",m_szAckMember,m_nDSColID) ;
	}
	if( strlen(m_szReqMember) >2)
	{
		if(GetMemberAddSub(m_szReqMember,m_nReqMember,m_nReqAdd,m_nReqSub) <0)
			gxWriteLog("MsgCol:ReqMember%s 格式错误! DSColID[%d]", m_szReqMember,m_nDSColID) ;
	}
return 0 ;
}
CgxDBSysMsgPlay::CgxDBSysMsgPlay()
{
	m_nDSMsgPlayID=0;
	memset(m_szMsgPlayName,0,sizeof(m_szMsgPlayName));
	m_nAckID=0;
	m_nReqID=0;
	m_nDSDBSID=0;
	memset(m_szTableName,0,sizeof(m_szTableName));
	memset(m_szAddTableName,0,sizeof(m_szAddTableName));
	m_nAddTableNameMode=0;
	memset(m_szTableNameSufID,0,sizeof(m_szTableNameSufID));
	m_nTableNameSufType=0;
	m_nSqlOperType=0;
	memset(m_szWhereStr,0,sizeof(m_szWhereStr));
	memset(m_szGroupStr,0,sizeof(m_szGroupStr));
	memset(m_szOrderStr,0,sizeof(m_szOrderStr));
	memset(m_szRetMember,0,sizeof(m_szRetMember));

	m_nAddTabMember=-1;
	m_nAddTabAdd=-1;
	m_nAddTabSub=-1;
	m_nTabSufMember=-1;
	m_nTabSufAdd=-1;
	m_nTabSufSub=-1;
	m_nRetMember=-1;
	m_nRetAdd=-1;
	m_nRetSub=-1;

	m_AckMsgIDMember[0]=-1;  
	m_AckMsgIDMember[1]=-1;  
	m_AckMsgIDMember[2]=-1;  
	m_AckMsgIDMember[3]=-1;  
	m_AckMsgLenMember[0]=-1;
	m_AckMsgLenMember[1]=-1;
	m_AckMsgLenMember[2]=-1;
	m_AckMsgLenMember[3]=-1;
	m_AckMsgSerialMember[0]=-1;
	m_AckMsgSerialMember[1]=-1;
	m_AckMsgSerialMember[2]=-1;
	m_AckMsgSerialMember[3]=-1;
}
CgxDBSysMsgPlay::~CgxDBSysMsgPlay()
{

}
int CgxDBSysMsgPlay::MemberStrToInt()
{
	if( strlen(m_szAddTableName) >2)
	{
		if( GetMemberAddSub(m_szAddTableName,m_nAddTabMember,m_nAddTabAdd,m_nAddTabSub) <0)
			gxWriteLog("CgxDBSysMsgPlay:m_szAddTableName%s 格式错误! DSMsgPlayID[%d]", m_szAddTableName,m_nDSMsgPlayID) ;
	}
	if( strlen(m_szTableNameSufID) >2)
	{
		if(GetMemberAddSub(m_szTableNameSufID,m_nTabSufMember,m_nTabSufAdd,m_nTabSufSub) <0)
			gxWriteLog("CgxDBSysMsgPlay:m_szTableNameSufID%s 格式错误! DSMsgPlayID[%d]", m_szTableNameSufID,m_nDSMsgPlayID) ;
	}
	if( strlen(m_szRetMember) >2)
	{
		if(GetMemberAddSub(m_szRetMember,m_nRetMember,m_nRetAdd,m_nRetSub) <0)
			gxWriteLog("CgxDBSysMsgPlay:m_szRetMember%s 格式错误! DSMsgPlayID[%d]", m_szTableNameSufID,m_nDSMsgPlayID) ;
	}
return 0 ;
}
int CgxDBSysMsgPlay::GetMsgColCount() 
{
return m_MsgColItems.GetCount() ;
}
int CgxDBSysMsgPlay::GetMsgWhereCount()
{
return m_MsgWhereItems.GetCount();
}
int CgxDBSysMsgPlay::AddMsgCol(CgxDBSysMsgCol *_pMsgCol) 
{
	if(_pMsgCol == NULL ) 
		return -1 ;
	m_MsgColItems.AddTail(_pMsgCol) ;
return 0 ;
}
int CgxDBSysMsgPlay::AddMsgWhere(CgxDBSysMsgWhere *_pMsgWhere)
{
	if( _pMsgWhere==NULL)
		return -1;
	m_MsgWhereItems.AddTail(_pMsgWhere);
return 0 ;
}
CgxDBSysMsgCol * CgxDBSysMsgPlay::GetMsgCol(int _nIdx)
{
return m_MsgColItems.GetAt(_nIdx);
}
CgxDBSysMsgWhere *CgxDBSysMsgPlay::GetMsgWhere(int _nIdx) 
{
return m_MsgWhereItems.GetAt(_nIdx);
}
int CgxDBSysMsgPlay::Play(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf)
{
	if(_pDBSvrInfo == NULL ) 
		return -1;
	switch (m_nSqlOperType)
	{
	case 1 :
		return MsgPlaySelect(_pDBSvrInfo , _ReqBuf , _AckBuf) ; 
		break; 
	case 2 :
		return MsgPlayInsert(_pDBSvrInfo , _ReqBuf , _AckBuf) ; 
		break; 
	case 3 :
		return MsgPlayUpdate(_pDBSvrInfo , _ReqBuf , _AckBuf) ; 
		break; 
	case 4:
		return MsgPlayDelete(_pDBSvrInfo , _ReqBuf , _AckBuf) ; 
		break; 
	case 5:
		return -3;
		break; 
	default :
		return -5;
	}
return -7 ; 
}
int CgxDBSysMsgPlay::MsgPlayInsert(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf)
{
	if(_pDBSvrInfo == NULL ) 
		return -1 ;

	char szTableName[2048];
	memset(szTableName , 0 , sizeof(szTableName)) ;
	GetTableName(szTableName ,&_ReqBuf) ;

	char szSql[2048];
	memset(szSql , 0, sizeof(szSql)) ;
	char szColName[1024];
	char szValue[2048];
	memset(szColName , 0 , sizeof(szColName)) ;
	memset(szValue, 0, sizeof(szValue)) ;

	int ColCount = m_MsgColItems.GetCount() ;
	CgxDBSysMsgCol *pMsgCol  = NULL ;
	char szTmp[1024];
	char szDataTmp[1024];
	memset(szDataTmp , 0 , sizeof(szDataTmp)) ;
	for( int i = 0 ;i < ColCount; i ++ ) 
	{
		pMsgCol = m_MsgColItems.GetAt(i) ; 
		if( pMsgCol == NULL ) 
			continue ; 
		if( pMsgCol->m_nSetAckFlag)
			continue;
		if( i)
		{
			strcat(szColName , ",") ; 
			strcat(szValue , ",") ; 
		}

		strcat(szColName,pMsgCol->m_szColName) ;
		memset(szTmp , 0 , sizeof(szTmp)) ;
		_ReqBuf.GetDataStr(szTmp , sizeof(szTmp) ,pMsgCol->m_szReqMember);
		if( strlen(pMsgCol->m_szFmtStr)>1)
		{
 
			memset(szDataTmp , 0, sizeof(szDataTmp)) ;
			sprintf(szDataTmp , pMsgCol->m_szFmtStr , szTmp) ;
			strcat(szValue , szDataTmp) ;
		}else
			strcat(szValue , szTmp) ;
	}

	sprintf( szSql , "insert into  %s (%s) values(%s)",szTableName,szColName,szValue); 
	CgxDBPlay Exec(_pDBSvrInfo->GetDBPool(),_pDBSvrInfo->m_nDBType );
	int nRet = Exec.Exec(szSql) ; 
	SetDBRet(nRet,_AckBuf) ;
	SetAckMsg(_ReqBuf,_AckBuf) ;

	SetAckMsgHeadLen(_AckBuf) ;
	_AckBuf.MakeBuf() ;
	if(nRet <0) 
	{
		gxWriteLog("Insert Err MsgID = %d(%08X) ,TableName=%s",
			_ReqBuf.m_dwMsgID,
			_ReqBuf.m_dwMsgID ,
			szTableName ) ;
	}
return TRUE;
}
int CgxDBSysMsgPlay::MsgPlaySelect(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf,CgxMsgBufPlay &_AckBuf)
{
	if(_pDBSvrInfo == NULL ) 
		return -1 ;
	char szWhere[2048];
	memset(szWhere , 0 , sizeof(szWhere)) ;
	GetMsgWhereStr(szWhere,&_ReqBuf) ;

	DBTABLEINFO * pTableInfo = NULL ;
	pTableInfo = GetTableColInfo(&_ReqBuf , &_AckBuf) ;

	char szTableName[2048];
	memset(szTableName , 0 , sizeof(szTableName)) ;
	GetTableName(szTableName ,&_ReqBuf) ;
	
	CgxDBPlay Exec(_pDBSvrInfo->GetDBPool(),_pDBSvrInfo->m_nDBType );
	int nExec = Exec.ExeSqlEx(szTableName ,pTableInfo,szWhere) ; 
	int AddCount = 0 ; 
	BOOL bRet = FALSE;
	if( nExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		CgxDBSysMsgCol * pMsgCol = NULL ; 
		int ColCount = m_MsgColItems.GetCount() ; 
		for(AddCount = 0 ;(pInfo = Exec.GetNextRecord())!=NULL;AddCount++)
		{
			for( int j = 0 ;j< ColCount ;j ++ ) 
			{
				pMsgCol = m_MsgColItems.GetAt(j ) ;
				if( pMsgCol == NULL ) 
					continue; 
				if( pMsgCol->m_nSetAckFlag)
					continue;
				SetSelectMsgData(pMsgCol,&pInfo[j],&_AckBuf,AddCount) ; 
			}
		}
		bRet = TRUE;
	}
	SetDBRet(bRet,_AckBuf) ;
	SetAckMsg(_ReqBuf,_AckBuf) ;
	_AckBuf.EndAddData(AddCount) ; 

	SetAckMsgHeadLen(_AckBuf) ;
	_AckBuf.MakeBuf() ;

	if( pTableInfo !=NULL) 
	{
		delete[] pTableInfo ; 
		pTableInfo = NULL ;
	}
return bRet;
}
int CgxDBSysMsgPlay::MsgPlayUpdate(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf , CgxMsgBufPlay &_AckBuf)
{
	if(_pDBSvrInfo == NULL ) 
		return -1 ;

	char szTableName[2048];
	memset(szTableName , 0 , sizeof(szTableName)) ;
	GetTableName(szTableName ,&_ReqBuf) ;

	char szWhere[2048];
	memset(szWhere , 0 , sizeof(szWhere)) ;
	GetMsgWhereStr(szWhere,&_ReqBuf) ;

	char szCol[2048];
	memset(szCol, 0 , sizeof(szCol)) ;
	GetMsgUpdateColStr(szCol,&_ReqBuf) ;

	char szSql[2048];
	memset(szSql , 0, sizeof(szSql)) ; 
	_snprintf(szSql , sizeof(szSql) , "update %s set %s  ",szTableName,szCol,szWhere); 

	if( strlen(szWhere) > 2 )
	{
		strcat(szSql , " where " ) ;
		strcat(szSql , szWhere);
	}

	CgxDBPlay Exec(_pDBSvrInfo->GetDBPool(),_pDBSvrInfo->m_nDBType );
	int nRet = Exec.Exec(szSql) ; 
	SetDBRet(nRet,_AckBuf) ;
	SetAckMsg(_ReqBuf,_AckBuf) ;
	SetAckMsgHeadLen(_AckBuf) ;

	_AckBuf.MakeBuf() ;
return 0;
}
int CgxDBSysMsgPlay::MsgPlayDelete(CgxDBSysDBSvrInfo *_pDBSvrInfo,CgxMsgBufPlay &_ReqBuf , CgxMsgBufPlay &_AckBuf)
{
	if(_pDBSvrInfo == NULL ) 
		return -1 ;

	char szTableName[2048];
	memset(szTableName , 0 , sizeof(szTableName)) ;
	GetTableName(szTableName ,&_ReqBuf) ;

	char szWhere[2048];
	memset(szWhere , 0 , sizeof(szWhere)) ;
	GetMsgWhereStr(szWhere,&_ReqBuf) ;

	char szSql[2048];
	memset(szSql , 0, sizeof(szSql)) ; 
	_snprintf(szSql , sizeof(szSql) , "delete from %s  " , szTableName  ) ; 
	if( strlen(szWhere) > 2 )
	{
		strcat(szSql , " where " ) ;
		strcat(szSql , szWhere);
	}
	CgxDBPlay Exec(_pDBSvrInfo->GetDBPool() ,_pDBSvrInfo->m_nDBType);
	int nRet = Exec.Exec(szSql) ; 
	SetDBRet(nRet,_AckBuf) ;
	SetAckMsg(_ReqBuf,_AckBuf) ;
	SetAckMsgHeadLen(_AckBuf) ;

	_AckBuf.MakeBuf() ;
return 0;
}
int CgxDBSysMsgPlay::SetAckMsg(CgxMsgBufPlay &_Req,CgxMsgBufPlay &_Ack)
{
	int Count = m_MsgColItems.GetCount() ; 
	CgxDBSysMsgCol * pMsgCol = NULL ; 
	for( int i = 0 ;i < Count ;i ++ ) 
	{
		pMsgCol = m_MsgColItems.GetAt(i) ; 
		if( pMsgCol == NULL ) 
			continue ;
		if( pMsgCol->m_nSetAckFlag)
		{
			void * pData = NULL ; 
			int DataLen = 0 ;
			pData =_Req.GetData(DataLen ,pMsgCol->m_nReqMember,pMsgCol->m_nReqAdd,pMsgCol->m_nReqSub) ;
			if((pData!=NULL )||(DataLen>0))
				_Ack.SetData(pData,DataLen,pMsgCol->m_nAckMember,pMsgCol->m_nAckAdd,pMsgCol->m_nAckSub) ;
		}
	}
	
	switch(m_AckMsgIDMember[3])
	{
	case GX_MSGBUF_DATATYPE_BYTE:
	case GX_MSGBUF_DATATYPE_CHAR:
		{
			BYTE Tmp = (BYTE)m_nAckID;
			_Ack.SetData(&Tmp,sizeof(Tmp) , m_AckMsgIDMember[0],m_AckMsgIDMember[1],m_AckMsgIDMember[2]);
		}
		break;
	case GX_MSGBUF_DATATYPE_SWORD:
	case GX_MSGBUF_DATATYPE_WORD:
		_Ack.SetShortData((short)m_nAckID, m_AckMsgIDMember[0],m_AckMsgIDMember[1],m_AckMsgIDMember[2]);
		break;
	case GX_MSGBUF_DATATYPE_INT:
	case GX_MSGBUF_DATATYPE_UINT:
		_Ack.SetIntData(m_nAckID, m_AckMsgIDMember[0],m_AckMsgIDMember[1],m_AckMsgIDMember[2]);
		break;
	case GX_MSGBUF_DATATYPE_LONGLONG:
		_Ack.SetInt64Data(m_nAckID, m_AckMsgIDMember[0],m_AckMsgIDMember[1],m_AckMsgIDMember[2]);
		break;
	default:
		gxWriteLog("消息处理应答头信息配置不正确！DSMsgPlayID[%8d],AckID[%8d],ReqID[%8d]",
			m_nDSMsgPlayID,m_nAckID,m_nReqID) ;
		break;
	}
return 0 ; 
}
int CgxDBSysMsgPlay::SetAckMsgHeadLen(CgxMsgBufPlay &_Ack)
{
	int Len = _Ack.GetDataLen() ;
	if( _Ack.SetHeadLen(Len , m_AckMsgLenMember[0],m_AckMsgLenMember[1],m_AckMsgLenMember[2],m_AckMsgLenMember[3])  <0)
		gxWriteLog("消息处理设置信息头长度出错，请检查配置！DSMsgPlayID[%8d],AckID[%8d],ReqID[%8d] Len[%8d]",
			m_nDSMsgPlayID,m_nAckID,m_nReqID,Len) ;
return 0 ; 
}
int CgxDBSysMsgPlay::SetSelectMsgData(CgxDBSysMsgCol *_pMsgCol,DBCOLINFO * _pInfo,CgxMsgBufPlay *_pMsgBuf,int _nAddIdx)
{
	if(( _pMsgCol == NULL ) || (_pInfo == NULL) || (_pMsgBuf == NULL))
		return -1 ; 

	int Type = 	_pMsgBuf->GetDataType(_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub ) ; 
	switch( Type) 
	{
	case GX_MSGBUF_DATATYPE_CHAR :
	case GX_MSGBUF_DATATYPE_BYTE :
		_pMsgBuf->SetData(_pInfo->pVar ,sizeof(BYTE),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_WORD :
	case GX_MSGBUF_DATATYPE_SWORD :
		_pMsgBuf->SetShortData(*((short*)_pInfo->pVar),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		_pMsgBuf->SetIntData(*((int*)_pInfo->pVar),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_LONGLONG :
		_pMsgBuf->SetInt64Data(*((__int64*)_pInfo->pVar),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_STR :
		_pMsgBuf->SetData(_pInfo->pVar ,_pInfo->nRetLen ,_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_FLOAT :
		_pMsgBuf->SetFloatData(*((float*)_pInfo->pVar),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_DOUBLE :
		_pMsgBuf->SetDoubleData(*((double*)_pInfo->pVar),_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub) ; 
		break ;
	case GX_MSGBUF_DATATYPE_SUFFIX :
		{
			int Len = _pInfo->nRetLen; 
			if( Len >0 )
			{
				char * pTmp = new char[Len +1];
				memset(pTmp , 0 , sizeof(char) * (Len +1) ) ; 
				strncpy(pTmp ,(char*) _pInfo->pVar , Len) ; 
				_pMsgBuf->SetSuffixData(pTmp ,Len +1 ,_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub ) ; 
				delete []pTmp ; 
			}
		}
		break; 
	default :
		gxWriteLog("未知数据类型(%d)！MAS=(%d ,%d,%d)" ,
			Type,_pMsgCol->m_nAckMember,_nAddIdx,_pMsgCol->m_nAckSub ) ;
	}
return 0 ; 
}
int CgxDBSysMsgPlay::GetTypeSize(int _nType)
{
	int Len = 0 ; 
	switch(_nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR	:
	case GX_MSGBUF_DATATYPE_BYTE	:
		Len = sizeof(char) ;
		break; 
	case GX_MSGBUF_DATATYPE_SWORD	:
		Len = sizeof(short) ;
		break; 
	case GX_MSGBUF_DATATYPE_WORD	:
		Len = sizeof(WORD) ;
		break; 
	case GX_MSGBUF_DATATYPE_INT		:
	case GX_MSGBUF_DATATYPE_UINT	:
		Len = sizeof(int) ;
		break; 
	case GX_MSGBUF_DATATYPE_LONGLONG:
		Len = sizeof(__int64) ;
		break; 
	case GX_MSGBUF_DATATYPE_FLOAT	:
		Len = sizeof(float) ;
		break; 
	case GX_MSGBUF_DATATYPE_DOUBLE	:
		Len = sizeof(double) ;
		break; 
	default :
		break; 
	}
return Len ;
}
int CgxDBSysMsgPlay::GetMsgUpdateColStr(char *_pCol,CgxMsgBufPlay *_pReq)
{
	if(( _pCol == NULL) || (_pReq == NULL ))
		return -1 ; 
	char szTmp[1024];
	char szCol[1024];
	int Count = m_MsgColItems.GetCount() ; 
	CgxDBSysMsgCol *pMsgCol = NULL; 
	for( int i = 0 ;i < Count;i ++ ) 
	{
		pMsgCol = m_MsgColItems.GetAt(i) ; 
		if( pMsgCol == NULL ) 
			continue; 
		if( pMsgCol->m_nSetAckFlag)
			continue;
		memset(szCol , 0 , sizeof(szCol)) ; 
		memset(szTmp , 0 , sizeof(szTmp)) ;
		GetUpdateMemberStr(szTmp , pMsgCol,_pReq) ;
		_snprintf(szCol , sizeof(szCol) , " %s = %s " ,pMsgCol->m_szColName,szTmp) ;
		strcat(_pCol , szCol) ; 
		if( i != Count -1 )
			strcat(_pCol , ", ") ;
	}
return 0 ;
}
int CgxDBSysMsgPlay::GetUpdateMemberStr(char *_pBuf,CgxDBSysMsgCol *_pMsgCol,CgxMsgBufPlay *_pReq)
{
	if( (_pBuf == NULL ) || (_pMsgCol == NULL)|| ( _pReq == NULL))
		return -1; 
	int nType  = _pReq->GetDataType( _pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub ) ;
	char szValue[2024];
	memset(szValue,0,sizeof(szValue)) ;
	switch(nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR :
		_snprintf(szValue , sizeof(szValue), "%d",_pReq->GetData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub)) ;
		break;
	case GX_MSGBUF_DATATYPE_BYTE :
		_snprintf(szValue , sizeof(szValue), "%u",_pReq->GetData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub)) ;
		break;
	case GX_MSGBUF_DATATYPE_SWORD :
		_snprintf(szValue , sizeof(szValue), "%d",_pReq->GetShortData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub)) ;
		break;
	case GX_MSGBUF_DATATYPE_WORD :
		_snprintf(szValue , sizeof(szValue), "%u",_pReq->GetShortData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub)) ;
		break; 
	case GX_MSGBUF_DATATYPE_INT	:
		_snprintf(szValue , sizeof(szValue), "%d",_pReq->GetIntData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub )) ;
		break; 
	case GX_MSGBUF_DATATYPE_UINT :
		_snprintf(szValue , sizeof(szValue), "%u",_pReq->GetIntData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub )) ;
		break; 
	case GX_MSGBUF_DATATYPE_LONGLONG :
		_snprintf(szValue , sizeof(szValue), "%I64d",_pReq->GetInt64Data(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub )) ;
		break; 
	case GX_MSGBUF_DATATYPE_STR :
		{
			int Len = 0 ;
			char *p =(char*) _pReq->GetData(Len, _pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub );
			if( Len >0) 
				strncpy(szValue,p,Len) ;
		}
		break; 
	case GX_MSGBUF_DATATYPE_FLOAT :
		_snprintf(szValue , sizeof(szValue), "%10.8f",_pReq->GetFloatData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub )) ;
		break; 
	case GX_MSGBUF_DATATYPE_DOUBLE :
		_snprintf(szValue , sizeof(szValue), "%10.8f",_pReq->GetDoubleData(_pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub )) ;
		break; 
	case GX_MSGBUF_DATATYPE_SUFFIX :
		{
			int Len = 0 ;
			char *p = (char*)_pReq->GetData(Len, _pMsgCol->m_nReqMember,_pMsgCol->m_nReqAdd,_pMsgCol->m_nReqSub );
			if( Len >0) 
				strncpy(szValue , p ,Len) ;
		}
		break; 
	}
	if( _pMsgCol->m_nColType ==GX_DBDATATYPE_STR )
		_snprintf(_pBuf , 1024 ,"'%s'" , szValue) ;
	else
		_snprintf(_pBuf , 1024 ,"%s" , szValue) ;
return 0 ; 
}
DBTABLEINFO *CgxDBSysMsgPlay::GetTableColInfo(CgxMsgBufPlay *_pReq ,CgxMsgBufPlay *_pAck)
{
	if(( _pReq == NULL) || (_pAck == NULL))
		return NULL ;
	int Count = m_MsgColItems.GetCount() ;
	DBTABLEINFO * pInfo = new DBTABLEINFO [Count +1];
	memset(pInfo , 0 , sizeof(DBTABLEINFO) * (Count +1));
	CgxDBSysMsgCol * pCol = NULL ;
	for( int i = 0 ;i < Count;i ++ ) 
	{
		pCol = m_MsgColItems.GetAt(i) ; 
		if( pCol == NULL ) 
			continue ; 
		strcpy(pInfo[i].ColName , pCol->m_szColName) ;
		pInfo[i].Type = pCol->m_nColType;
		pInfo[i].Size = pCol->m_nColLen;
		if( pInfo[i].Size <=0) 
		{
			pInfo[i].Size = GetTypeSize(pInfo[i].Type) ;
		}
	}
return pInfo ;
}
int CgxDBSysMsgPlay::GetMsgWhereStr(char *_pBuf ,CgxMsgBufPlay *_pReq)
{
	if(( _pReq == NULL ) ||  (_pBuf == NULL))
		return -1 ;
	strcat(_pBuf , m_szWhereStr ) ; 
	strcat(_pBuf , " " ) ; 

	char szSubWhere[2048];
	memset(szSubWhere , 0 , sizeof(szSubWhere)) ;
	int Count = m_MsgWhereItems.GetCount() ;

	char szTmp[1024];
	CgxDBSysMsgWhere *pMsgWhere = NULL ; 
	for( int i = 0 ;i < Count;i ++ ) 
	{
		pMsgWhere = m_MsgWhereItems.GetAt(i) ; 
		if( pMsgWhere == NULL ) 
			continue ; 
		memset(szTmp , 0 , sizeof(szTmp)) ; 

		int Type = _pReq->GetDataType(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ; 
		switch(Type) 
		{
		case GX_MSGBUF_DATATYPE_INT :
			_snprintf(szTmp,sizeof(szTmp),pMsgWhere->m_szWhereStr,_pReq->GetIntData(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ) ;
			break; 
		case GX_MSGBUF_DATATYPE_LONGLONG :
			_snprintf(szTmp ,sizeof(szTmp),pMsgWhere->m_szWhereStr,_pReq->GetInt64Data(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ) ;
			break; 
		case GX_MSGBUF_DATATYPE_SUFFIX:
		case GX_MSGBUF_DATATYPE_STR :
			_snprintf( szTmp , sizeof(szTmp) , pMsgWhere->m_szWhereStr , (char *)_pReq->GetData(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ) ;
			break; 
		case GX_MSGBUF_DATATYPE_FLOAT :
			_snprintf( szTmp , sizeof(szTmp) , pMsgWhere->m_szWhereStr , _pReq->GetFloatData(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ) ;
			break; 
		case GX_MSGBUF_DATATYPE_DOUBLE :
			_snprintf( szTmp , sizeof(szTmp) , pMsgWhere->m_szWhereStr , _pReq->GetDoubleData(pMsgWhere->m_nReqMember,pMsgWhere->m_nReqAdd,pMsgWhere->m_nReqSub) ) ;
			break; 
		}
		strcat ( szSubWhere , szTmp) ; 
		strcat( szSubWhere , " " ) ; 
	}
	if( strlen(szSubWhere) > 1 ) 
		strcat(_pBuf , szSubWhere) ; 
	if( strlen(m_szGroupStr ) >1 ) 
	{
		strcat (_pBuf , " group by ") ;
		strcat (_pBuf , m_szGroupStr) ;
	}
	if( strlen(m_szOrderStr ) >1 )
	{
		strcat(_pBuf , " order by " ) ; 
		strcat(_pBuf , m_szOrderStr) ; 
	}
return 0 ; 
}
int CgxDBSysMsgPlay::GetFirstAddTableName(char *_pTableName,CgxMsgBufPlay *_pReq)
{
	if(( _pTableName == NULL ) || (_pReq == NULL ) )
		return -1; 
	char szTmp[64]	;
	memset(szTmp , 0 , sizeof(szTmp)) ;
	if( m_nAddTableNameMode >0) 
	{
		int nValue = _pReq->GetIntData(m_nAddTabMember,m_nAddTabAdd,m_nAddTabSub) ;
		int nModeValue = nValue ;
		nModeValue = nValue % m_nAddTableNameMode ; 
		_snprintf(szTmp , sizeof(szTmp) , "_%d" , nModeValue) ;
	}else
	{
		int TmpLen = sizeof(szTmp);
		_pReq->GetDataStr(szTmp,TmpLen,m_nAddTabMember,m_nAddTabAdd,m_nAddTabSub);
	}
	strcpy(_pTableName , szTmp) ; 
return 0 ; 
}
int CgxDBSysMsgPlay::GetTableName(char *_pTableName,CgxMsgBufPlay *_pReq)
{
	if(( _pTableName ==  NULL ) || (_pReq == NULL))
		return -1 ;
	strcpy(_pTableName , m_szTableName) ;
	if( strlen(m_szAddTableName )>1  )
	{
		char szFirstName[256];
		memset(szFirstName , 0 , sizeof(szFirstName)) ; 
		GetFirstAddTableName(szFirstName , _pReq) ; 
		strcat(_pTableName , szFirstName) ; 
	}

	if(strlen(m_szTableNameSufID) <1 ) 
		return 0;
	char szTmp[256];
	memset(szTmp , 0, sizeof(szTmp)) ;
	int TmpLen = sizeof(szTmp);

	if( m_nTableNameSufType ==0 )
		_pReq->GetDataStr(szTmp , TmpLen,m_nTabSufMember,m_nTabSufAdd,m_nTabSufSub);
	else
	{
		time_t tTime =0;
		tTime =  _pReq->GetIntData(m_nTabSufMember,m_nTabSufAdd,m_nTabSufSub);
		if( tTime ==0)
			return -3;
		struct tm tmTime ;
		memset(&tmTime , 0 , sizeof(tmTime)) ;
		localtime_s(&tmTime, &tTime);
	
		switch(m_nTableNameSufType)
		{
		case 1 :
			_snprintf(szTmp , sizeof(szTmp) , "%04d%02d%02d",tmTime.tm_year +1900 ,tmTime.tm_mon +1,tmTime.tm_mday  ) ;
			break;
		case 3 :
			_snprintf(szTmp , sizeof(szTmp) , "%04d%02d",tmTime.tm_year +1900 , tmTime.tm_mon +1 ) ;
			break;
		case 5 :
			_snprintf(szTmp , sizeof(szTmp) , "%04d", tmTime.tm_year +1900   ) ;
			break;
		default :
			break;
		}
	}
	if( strlen(szTmp) >1) 
	{
		strcat(_pTableName , "_") ;
		strcat(_pTableName,szTmp) ;
	}
return 0 ; 
}
int CgxDBSysMsgPlay::SetDBRet(int _nRet,CgxMsgBufPlay &_AckBuf)
{
	if( strlen(m_szRetMember) <3)
		return 0 ;
	int nType = _AckBuf.GetDataType(m_nRetMember, m_nRetAdd,m_nRetSub) ;
	switch(nType)
	{
	case GX_MSGBUF_DATATYPE_CHAR :
	case GX_MSGBUF_DATATYPE_BYTE :
		{
			char Data = (char)_nRet ;
			_AckBuf.SetData(&Data,sizeof(Data) , m_nRetMember,m_nRetAdd,m_nRetSub) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_SWORD :
	case GX_MSGBUF_DATATYPE_WORD :
		{
			short Data = (short) _nRet ;
			_AckBuf.SetShortData(Data,m_nRetMember,m_nRetAdd, m_nRetSub) ;
			break;
		}
	case GX_MSGBUF_DATATYPE_INT :
	case GX_MSGBUF_DATATYPE_UINT :
		_AckBuf.SetIntData(_nRet,m_nRetMember,m_nRetAdd,m_nRetSub) ;
		break;
	}
return 0 ;
}
CgxDBSysMsgPlayList::CgxDBSysMsgPlayList()
{
}
CgxDBSysMsgPlayList::~CgxDBSysMsgPlayList()
{
	ReleaseAll();
}
void CgxDBSysMsgPlayList::ReleaseAll()
{
	m_MsgPlayItems.DeleteAll();
}
int CgxDBSysMsgPlayList::CompFuncDBSysMsgPlay(const void *_pItem1,const void *_pItem2)
{
	CgxDBSysMsgPlay * pItem1 = *(CgxDBSysMsgPlay **)_pItem1;
	CgxDBSysMsgPlay * pItem2 = *(CgxDBSysMsgPlay **)_pItem2;
	if(( pItem1 == NULL) || (pItem2 == NULL))
		return 0 ;
	__int64  Value = (__int64)pItem1->m_nReqID  - (__int64)pItem2->m_nReqID;
	if( Value >0) 
		return 1 ;
	if( Value <0 )
		return -1 ;
return 0 ;
}
int CgxDBSysMsgPlayList::GetMsgPlayCount()
{
return m_MsgPlayItems.GetCount();
}
CgxDBSysMsgPlay *CgxDBSysMsgPlayList::GetMsgPlayOfID(int _nID)
{
	CgxDBSysMsgPlay * pMsg = NULL ;
	int Count = m_MsgPlayItems.GetCount() ; 
	for( int i = 0;i < Count ;i ++ ) 
	{
		pMsg = m_MsgPlayItems.GetAt(i) ;
		if( pMsg == NULL ) 
			continue;
		if( pMsg->m_nDSMsgPlayID == _nID)
			return pMsg;
	}
return NULL ;
}
CgxDBSysMsgPlay *CgxDBSysMsgPlayList::GetMsgPlayOfReqID(int _nID)
{
	CgxDBSysMsgPlay * pMsg = NULL ;
	CgxDBSysMsgPlay Tmp ; 
	Tmp.m_nReqID = _nID ; 
		pMsg = m_MsgPlayItems.Find(&Tmp , CompFuncDBSysMsgPlay ) ;
return pMsg ;
}
CgxDBSysMsgPlay *CgxDBSysMsgPlayList::GetMsgPlayOfIdx(int _nIdx)
{
return m_MsgPlayItems.GetAt(_nIdx) ; ;
}
int CgxDBSysMsgPlayList::InsertMsgPlay(CgxDBSysMsgPlay * _pMsg)
{
	if( _pMsg == NULL ) 
		return -1;
	m_MsgPlayItems.Insert(_pMsg,CompFuncDBSysMsgPlay) ; 
return 0 ; 
}
int CgxDBSysMsgPlayList::AddMsgCol(CgxDBSysMsgCol *_pMsgCol,int _nReqID) 
{
	if(_pMsgCol == NULL ) 
		return -1;
	CgxDBSysMsgPlay *pMsgPlay = GetMsgPlayOfReqID(_nReqID) ;
	if( pMsgPlay == NULL ) 
		pMsgPlay = GetMsgPlayOfID(_pMsgCol->m_nDSMsgPlayID) ;
	if( pMsgPlay == NULL ) 
		return -2 ;
	if( pMsgPlay->m_nDSMsgPlayID== _pMsgCol->m_nDSMsgPlayID)
		pMsgPlay->AddMsgCol(_pMsgCol) ;
return 0;
}
int CgxDBSysMsgPlayList::AddMsgWhere(CgxDBSysMsgWhere *_pMsgWhere,int _nReqID) 
{
	if(_pMsgWhere ==NULL)
		return -1 ;
	CgxDBSysMsgPlay *pMsgPlay = GetMsgPlayOfReqID(_nReqID) ;
	if( pMsgPlay ==NULL ) 
		pMsgPlay = GetMsgPlayOfID(_pMsgWhere->m_nDSMsgPlayID) ;
	if( pMsgPlay == NULL ) 
		return -2 ;
	if( pMsgPlay->m_nDSMsgPlayID== _pMsgWhere->m_nDSMsgPlayID)
		pMsgPlay->AddMsgWhere(_pMsgWhere);
return 0;
}
int CgxDBSysMsgPlayList::LoadDBSys(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	if( LoadDBSysMsgPlay(_pDBPool, _nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("加载消息处理定义表失败！");
		return -5;
	}
	if( LoadDBSysMsgColPlay(_pDBPool, _nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("加载消息处理列定义表失败！");
		return -6;
	}
	if( LoadDBSysMsgWherePlay(_pDBPool, _nDBType,_nDSSVRID) <0)
	{
		gxWriteLog("加载消息处理条件定义表失败！");
		return -7;
	}
return 0;
}

int CgxDBSysMsgPlayList::LoadDBSysMsgPlay(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSMsgPlayID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"MsgPlayName"				,GX_DBDATATYPE_STR 		,NULL,128	,1,0} ,
		{"AckID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ReqID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DSDBSID"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"TableName"				,GX_DBDATATYPE_STR 		,NULL,512	,1,0} ,
		{"AddTableName"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"AddTableNameMode"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"TableNameSufID"			,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"TableNameSufType"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"SqlOperType"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"WhereStr"					,GX_DBDATATYPE_STR 		,NULL,512	,1,0} ,
		{"GroupStr"					,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"OrderStr"					,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"RetMember"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID!=-1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID=%d order by ReqID",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," 1 = 1 order by ReqID");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_MsgPlay") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysMsgPlay *pMsgPlay = new CgxDBSysMsgPlay() ;
			if( pMsgPlay==NULL)
				continue;
			pMsgPlay->m_nDSMsgPlayID				= *(int*) pInfo[0].pVar;
			strcpy(pMsgPlay->m_szMsgPlayName		, (char*) pInfo[1].pVar);
			pMsgPlay->m_nAckID						= *(int*) pInfo[2].pVar;
			pMsgPlay->m_nReqID						= *(int*) pInfo[3].pVar;
			pMsgPlay->m_nDSDBSID					= *(int*) pInfo[4].pVar;
			strcpy(pMsgPlay->m_szTableName			, (char*) pInfo[5].pVar);
			strcpy(pMsgPlay->m_szAddTableName		, (char*) pInfo[6].pVar);
			pMsgPlay->m_nAddTableNameMode			= *(int*) pInfo[7].pVar;
			strcpy(pMsgPlay->m_szTableNameSufID		, (char*) pInfo[8].pVar);
			pMsgPlay->m_nTableNameSufType			= *(int*) pInfo[9].pVar;
			pMsgPlay->m_nSqlOperType				= *(int*) pInfo[10].pVar;
			strcpy(pMsgPlay->m_szWhereStr			, (char*) pInfo[11].pVar);
			strcpy(pMsgPlay->m_szGroupStr			, (char*) pInfo[12].pVar);
			strcpy(pMsgPlay->m_szOrderStr			, (char*) pInfo[13].pVar);
			strcpy(pMsgPlay->m_szRetMember			, (char*) pInfo[14].pVar);

			pMsgPlay->MemberStrToInt() ;
			InsertMsgPlay(pMsgPlay) ;
		}
	}
return bExec ; 
}
int CgxDBSysMsgPlayList::LoadDBSysMsgColPlay(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSColID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DSMsgPlayID"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ColName"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"ColType"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ColLen"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"AckMember"			,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"ReqMember"			,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"SetAckFlag"			,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ColIdx"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ReqID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"FmtStr"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID!=-1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID = %d order by DSMsgPlayID,ReqID,ColIdx",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," 1 = 1 order by DSMsgPlayID,ReqID,ColIdx");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;

	_snprintf(szTableName, sizeof(szTableName) , "%s " ,  "DS_SYS_MsgCol") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysMsgCol *pMsgCol = new CgxDBSysMsgCol() ;
			if( pMsgCol==NULL)
				continue;
			int nReqID = 0 ;
			pMsgCol->m_nDSColID					= *(int*) pInfo[0].pVar;
			pMsgCol->m_nDSMsgPlayID				= *(int*) pInfo[1].pVar;
			strcpy(pMsgCol->m_szColName			, (char*) pInfo[2].pVar);
			pMsgCol->m_nColType					= *(int*) pInfo[3].pVar;
			pMsgCol->m_nColLen					= *(int*) pInfo[4].pVar;
			strcpy(pMsgCol->m_szAckMember		, (char*) pInfo[5].pVar);
			strcpy(pMsgCol->m_szReqMember		, (char*) pInfo[6].pVar);
			pMsgCol->m_nSetAckFlag				= *(int*) pInfo[7].pVar;
			pMsgCol->m_nColIdx					= *(int*) pInfo[8].pVar;
			nReqID								= *(int*) pInfo[9].pVar;
			strcpy(pMsgCol->m_szFmtStr			, (char*) pInfo[10].pVar);

			pMsgCol->MemberStrToInt();
			AddMsgCol(pMsgCol,nReqID) ;
		}
	}
return bExec ; 
}
int CgxDBSysMsgPlayList::LoadDBSysMsgWherePlay(void *_pDBPool,int _nDBType,int _nDSSVRID) 
{
	if( _pDBPool == NULL ) 
		return -1 ; 
	CgxDBPlay Exec( _pDBPool,_nDBType) ;
	DBTABLEINFO ColInfo[]= {
		{"DSWhereID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"DSMsgPlayID"				,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"WhereStr"					,GX_DBDATATYPE_STR 		,NULL,512	,1,0} ,
		{"AckMember"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"ReqMember"				,GX_DBDATATYPE_STR 		,NULL,256	,1,0} ,
		{"ColIdx"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"ReqID	"					,GX_DBDATATYPE_INT 		,NULL,8		,1,0} ,
		{"",0 ,NULL,0,0} 
	};
	char szWhere[2048];
	memset(szWhere,0,sizeof(szWhere)) ; 
	if( _nDSSVRID!=-1)
		_snprintf(szWhere,sizeof(szWhere)," DSSVRID = %d order by DSMsgPlayID,ReqID,ColIdx",_nDSSVRID);
	else
		_snprintf(szWhere,sizeof(szWhere)," 1 = 1 order by DSMsgPlayID,ReqID,ColIdx");

	char szTableName[1024];
	memset(szTableName ,0 , sizeof(szTableName)) ;
	_snprintf(szTableName, sizeof(szTableName) , "%s " ,"DS_SYS_MsgWhere") ;

	int bExec = Exec.ExeSqlEx(szTableName,ColInfo,szWhere) ; 
	if( bExec>=0)
	{
		DBCOLINFO * pInfo = NULL ; 
		for( ;(pInfo = Exec.GetNextRecord())!=NULL;)
		{
			CgxDBSysMsgWhere *pMsgWhere = new CgxDBSysMsgWhere() ;
			if( pMsgWhere==NULL)
				continue;
			int nReqID = 0 ;
			pMsgWhere->m_nDSWhereID					= *(int*) pInfo[0].pVar;
			pMsgWhere->m_nDSMsgPlayID				= *(int*) pInfo[1].pVar;
			strcpy(pMsgWhere->m_szWhereStr			, (char*) pInfo[2].pVar);
			strcpy(pMsgWhere->m_szAckMember			, (char*) pInfo[3].pVar);
			strcpy(pMsgWhere->m_szReqMember			, (char*) pInfo[4].pVar);
			pMsgWhere->m_nColIdx					= *(int*) pInfo[5].pVar;
			nReqID									= *(int*) pInfo[6].pVar;

			pMsgWhere->MemberStrToInt();
			AddMsgWhere(pMsgWhere,nReqID) ;
		}
	}
return bExec ; 
}
CgxDBSysDBSvrInfo::CgxDBSysDBSvrInfo()
{
	m_nDSDBSID=0;
	m_nDBType=0;
	memset(m_szDBName,0,sizeof(m_szDBName));
	memset(m_szDSNName,0,sizeof(m_szDSNName));
	memset(m_szIP,0,sizeof(m_szIP));
	m_nPort=0;
	m_nConnectCount=3;
	memset(m_szDBUserName , 0 , sizeof(m_szDBUserName)) ;
	memset(m_szDBPassword, 0 , sizeof(m_szDBPassword)) ;

	m_pDBLoadBase = NULL;
}
CgxDBSysDBSvrInfo::~CgxDBSysDBSvrInfo()
{
}
int CgxDBSysDBSvrInfo::InitDB()
{
	if( m_pDBLoadBase != NULL ) 
		delete m_pDBLoadBase;
	m_pDBLoadBase = NULL ;
	switch(m_nDBType) 
	{
	case DS_DB_TYPE_ODBC		:
		{
			CgxDBLoadOdbc *pOdbc = new CgxDBLoadOdbc() ;
			if( pOdbc == NULL )
				return -1;
			if( pOdbc->InitDB(m_szDSNName,m_szDBName,m_szDBUserName,m_szDBPassword,m_nConnectCount,m_nPort) <0)
				return -10;
			m_pDBLoadBase = pOdbc;
		}
		break;
	case DS_DB_TYPE_REDIS		:
		break;
	default :

		m_pDBLoadBase = CgxDSKernel::m_SysInterfaceCfgItems.CreateDBLoad(m_nDBType);
		if( m_pDBLoadBase!=NULL ) 
		{
			if(m_pDBLoadBase->InitDB(m_szDSNName,m_szDBName,m_szDBUserName,m_szDBPassword,m_nConnectCount,m_nPort) <0)
				return -10 ;
		}else
		{
			gxWriteLog("未知的数据库类型[%d]", m_nDBType) ;
			return -2 ;
		}
		break;
	}
return 0 ; 
}
void * CgxDBSysDBSvrInfo::GetDBPool()
{
	if( m_pDBLoadBase == NULL ) 
		return NULL ;
return m_pDBLoadBase->GetDBPool(0);
}
