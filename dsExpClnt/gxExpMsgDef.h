#ifndef GX_EXP_MSG_DEF_2021_08_01
#define GX_EXP_MSG_DEF_2021_08_01

#include <Windows.h>

#define EXP_MSG_REQ_INSERT      10001
#define EXP_MSG_REQ_SELECT      10002
#define EXP_MSG_REQ_UPDATE      10003
#define EXP_MSG_REQ_DELETE      10004

#define EXP_MSG_ACK_INSERT      20001
#define EXP_MSG_ACK_SELECT      20002
#define EXP_MSG_ACK_UPDATE      20003
#define EXP_MSG_ACK_DELETE      20004

typedef struct tagDSMsgSuffix
{
	DWORD dwOffSet;
	DWORD dwSize;
}MSG_SUFFIX;

typedef struct tagExpHead
{
	int MsgFlag;
	int MsgID;
	int MsgLen;
	int MsgSerial;
}EXP_HEAD;

typedef struct tagExpData
{
	char CharType;
	short ShortType;
	int IntType;
	__int64 Int64Type;
	float FloatType;
	double DoubleType;
	char StringType[128];
	MSG_SUFFIX SuffixType;
}EXP_DATA;

typedef struct tagExpInsertReq
{
	EXP_HEAD Head;
	char szTestName[128];
	EXP_DATA Data;
}EXP_INSERT_REQ;

typedef struct tagExpInsertAck
{
	EXP_HEAD Head;
	int nRet;
}EXP_INSERT_ACK;

typedef struct tagExpSelectReq
{
	EXP_HEAD Head;
	char szTestName[128];
	int IntType;
}EXP_SELECT_REQ;

typedef struct tagExpSelectAck
{
	EXP_HEAD Head;
	int nRetCount;
}EXP_SELECT_ACK;
//后缀nRetCount个EXP_DATA结构;

typedef struct tagExpUpdateReq
{
	EXP_HEAD Head;
	char szTestName[128];
	EXP_DATA Data;
}EXP_UPDATE_REQ;

typedef struct tagExpUpdateAck
{
	EXP_HEAD Head;
	int nRetCount;
}EXP_UPDATE_ACK;

typedef struct tagExpDeleteReq
{
	EXP_HEAD Head;
	char szTestName[128];
	EXP_DATA Data;
}EXP_DELETE_REQ;

typedef struct tagExpDeleteAck
{
	EXP_HEAD Head;
	int nRetCount;
}EXP_DELETE_ACK;

#endif