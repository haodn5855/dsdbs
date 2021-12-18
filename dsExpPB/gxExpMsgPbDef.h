#ifndef GX_EXP_MSG_DEF_2021_09_16
#define GX_EXP_MSG_DEF_2021_09_16

#include <Windows.h>

#define EXP_MSG_PB_REQ_SELECT      51001
#define EXP_MSG_PB_ACK_SELECT      51002

#define EXP_MSG_PB_REQ_INSERT      51003
#define EXP_MSG_PB_ACK_INSERT      51004

#define EXP_MSG_PB_REQ_UPDATE      51005
#define EXP_MSG_PB_ACK_UPDATE      51006

#define EXP_MSG_PB_REQ_DELETE      51007
#define EXP_MSG_PB_ACK_DELETE      51008

typedef struct tagExpHead
{
	int MsgFlag;
	int MsgID;
	int MsgLen;
	int MsgSerial;
public:
	void hton()
	{
		MsgFlag		=htonl(MsgFlag) ;
		MsgID		=htonl(MsgID) ;
		MsgLen		=htonl(MsgLen) ;
		MsgSerial	=htonl(MsgSerial) ;
	}
	void ntoh()
	{
		MsgFlag		=ntohl(MsgFlag) ;
		MsgID		=ntohl(MsgID) ;
		MsgLen		=ntohl(MsgLen) ;
		MsgSerial	=ntohl(MsgSerial) ;
	}
}EXP_HEAD;


#endif