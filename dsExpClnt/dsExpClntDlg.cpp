
// dsExpClntDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "dsExpClnt.h"
#include "dsExpClntDlg.h"
#include "afxdialogex.h"

#include "gxThreadClient.h"
#include <iostream>
#include "gxExpMsgDef.h"

extern unsigned __int64 hton64(const unsigned __int64 *_pInput);

#pragma warning(disable:4996)
#pragma warning(disable:4305)

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CdsExpClntDlg 对话框




CdsExpClntDlg::CdsExpClntDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdsExpClntDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdsExpClntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdsExpClntDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdsExpClntDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdsExpClntDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CdsExpClntDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CdsExpClntDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CdsExpClntDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CdsExpClntDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CdsExpClntDlg 消息处理程序

BOOL CdsExpClntDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CdsExpClntDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CdsExpClntDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CgxThreadClient g_Client ;

void CdsExpClntDlg::OnBnClickedButton1()
{
	g_Client.m_Wnd = m_hWnd ;
	g_Client.StartThread() ;
}
void CdsExpClntDlg::OnBnClickedButton2()
{

}
void CdsExpClntDlg::ExpInsert()
{
	EXP_INSERT_REQ Req ;
	EXP_INSERT_ACK Ack;
	memset(&Req, 0 , sizeof(Req)) ;
	memset(&Ack , 0,sizeof(Ack)) ;
	Req.Head.MsgFlag = 0x01020506;
	Req.Head.MsgID = EXP_MSG_REQ_INSERT;
	Req.Head.MsgLen = sizeof(Req) ; 
	Req.Head.MsgSerial = rand() %1000;
	strcpy(Req.szTestName,"TestNameOn");
	char szTmp[128];
	memset(szTmp , 0, sizeof(szTmp)) ;
	Req.Data.CharType	= 1;
	Req.Data.ShortType	= 1000 + rand() %10;
	Req.Data.IntType	= Req.Data.ShortType + 1000;
	Req.Data.Int64Type	= 1234567890123456;
	Req.Data.FloatType	= 10.135;
	Req.Data.DoubleType = 1000.246;
	sprintf(szTmp , "StringType%I64d , %d" ,Req.Data.Int64Type,Req.Data.ShortType) ;
	strcpy(Req.Data.StringType,szTmp);
	Req.Data.SuffixType.dwOffSet = sizeof(Req) -sizeof(EXP_HEAD) ;
	Req.Data.SuffixType.dwSize	= strlen(szTmp)+1 ;
	Req.Head.MsgLen += strlen(szTmp)+1 ;

	Req.Head.MsgFlag	= htonl(Req.Head.MsgFlag);
	Req.Head.MsgID		= htonl(Req.Head.MsgID		) ;
	Req.Head.MsgLen		= htonl(Req.Head.MsgLen		) ;
	Req.Head.MsgSerial	= htonl(Req.Head.MsgSerial) ;
	Req.Data.ShortType	= htons(Req.Data.ShortType) ;
	Req.Data.IntType	= htonl(Req.Data.IntType) ;
	Req.Data.SuffixType.dwOffSet	= htonl(Req.Data.SuffixType.dwOffSet) ;
	Req.Data.SuffixType.dwSize		= htonl(Req.Data.SuffixType.dwSize) ;
	Req.Data.Int64Type  = hton64((unsigned __int64 *)&Req.Data.Int64Type) ;

	char szSendBuf[2048];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ;
	memcpy(szSendBuf ,&Req , sizeof(Req)) ;
	int SendLen = sizeof(Req) ;

	char * pTmp = szSendBuf ;
	pTmp +=SendLen ;
	szTmp[0] = 'S' ;
	szTmp[1] = 'u' ;
	szTmp[2] = 'f' ;
	strcpy(pTmp , szTmp ) ;
	SendLen +=strlen(szTmp) +1 ;
	g_Client.SendMsg(szSendBuf , SendLen) ;
}
void CdsExpClntDlg::ExpSelect()
{
	EXP_SELECT_REQ Req ;
	EXP_SELECT_ACK Ack;
	memset(&Req, 0 , sizeof(Req)) ;
	memset(&Ack , 0,sizeof(Ack)) ;
	Req.Head.MsgFlag	= 0x01020506;
	Req.Head.MsgID		= EXP_MSG_REQ_SELECT;
	Req.Head.MsgLen		= sizeof(Req) ; 
	Req.Head.MsgSerial	= rand() %1000;
	Req.IntType			= 2001;

	Req.Head.MsgFlag	= htonl(Req.Head.MsgFlag);
	Req.Head.MsgID		= htonl(Req.Head.MsgID		) ;
	Req.Head.MsgLen		= htonl(Req.Head.MsgLen		) ;
	Req.Head.MsgSerial	= htonl(Req.Head.MsgSerial) ;
	Req.IntType			= htonl(Req.IntType) ;
	strcpy(Req.szTestName , "TestNameOn") ;

	g_Client.SendMsg((char*)&Req , sizeof(Req)) ;
}
void CdsExpClntDlg::ExpUpdate()
{
	EXP_UPDATE_REQ Req ;
	EXP_UPDATE_ACK Ack;
	memset(&Req, 0 , sizeof(Req)) ;
	memset(&Ack , 0,sizeof(Ack)) ;
	Req.Head.MsgFlag = 0x01020506;
	Req.Head.MsgID = EXP_MSG_REQ_UPDATE;
	Req.Head.MsgLen = sizeof(Req) ; 
	Req.Head.MsgSerial = rand() %1000;
	strcpy(Req.szTestName,"TestNameUpdate");
	char szTmp[128];
	memset(szTmp , 0, sizeof(szTmp)) ;
	Req.Data.CharType = 1;
	Req.Data.ShortType = 1000 + rand() %10;
	Req.Data.Int64Type = 1234567890123456;
	Req.Data.FloatType = 10.135;
	Req.Data.DoubleType = 3000.246;
	Req.Data.SuffixType.dwOffSet = sizeof(Req) -sizeof(EXP_HEAD); 
	Req.Data.SuffixType.dwSize = strlen("Update")  +1 ;

	Req.Head.MsgLen += strlen("Update")  +1 ;
	sprintf(szTmp , "StringType%I64d , %d" ,Req.Data.Int64Type,Req.Data.ShortType) ;
	strcpy(Req.Data.StringType,szTmp);

	Req.Head.MsgFlag	= htonl(Req.Head.MsgFlag);
	Req.Head.MsgID		= htonl(Req.Head.MsgID	) ;
	Req.Head.MsgLen		= htonl(Req.Head.MsgLen	) ;
	Req.Head.MsgSerial	= htonl(Req.Head.MsgSerial) ;
	Req.Data.ShortType	= htons(Req.Data.ShortType) ;
	Req.Data.SuffixType.dwOffSet	= htonl(Req.Data.SuffixType.dwOffSet) ;
	Req.Data.SuffixType.dwSize		= htonl(Req.Data.SuffixType.dwSize) ; 

	char szSendBuf[1024];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ; 
	memcpy(szSendBuf , &Req, sizeof(Req)) ;
	int SendLen = sizeof(Req) ; 
	char * p = szSendBuf + SendLen;
	strcpy(p, "Update") ;
	SendLen += strlen("Update")  +1 ;
	g_Client.SendMsg(szSendBuf , SendLen) ;
}
void CdsExpClntDlg::ExpDelete()
{
	EXP_DELETE_REQ Req ;
	EXP_DELETE_ACK Ack;
	memset(&Req, 0 , sizeof(Req)) ;
	memset(&Ack , 0,sizeof(Ack)) ;
	Req.Head.MsgFlag = 0x01020506;
	Req.Head.MsgID = EXP_MSG_REQ_DELETE;
	Req.Head.MsgLen = sizeof(Req) ; 
	Req.Head.MsgSerial = rand() %1000;
	strcpy(Req.szTestName,"TestNameOn");
	char szTmp[128];
	memset(szTmp , 0, sizeof(szTmp)) ;
	Req.Data.CharType = 1;
	Req.Data.ShortType = 1000 + rand() %10;
	Req.Data.Int64Type = 1234567890123456;
	Req.Data.FloatType = 10.135;
	Req.Data.DoubleType = 1000.246;
	Req.Data.SuffixType.dwOffSet = sizeof(Req) -sizeof(EXP_HEAD) ; 
	Req.Data.SuffixType.dwSize = strlen("Delete")  +1 ;

	Req.Head.MsgLen += strlen("Delete")  +1 ;

	sprintf(szTmp , "StringType%I64d , %d" ,Req.Data.Int64Type,Req.Data.ShortType) ;
	strcpy(Req.Data.StringType,szTmp);

	Req.Head.MsgFlag	= htonl(Req.Head.MsgFlag);
	Req.Head.MsgID		= htonl(Req.Head.MsgID		) ;
	Req.Head.MsgLen		= htonl(Req.Head.MsgLen		) ;
	Req.Head.MsgSerial	= htonl(Req.Head.MsgSerial) ;
	Req.Data.ShortType	= htons(Req.Data.ShortType) ;

	Req.Data.SuffixType.dwOffSet	= htonl(Req.Data.SuffixType.dwOffSet) ;
	Req.Data.SuffixType.dwSize		= htonl(Req.Data.SuffixType.dwSize) ; 

	char szSendBuf[1024];
	memset(szSendBuf , 0 , sizeof(szSendBuf)) ; 
	memcpy(szSendBuf , &Req, sizeof(Req)) ;
	int SendLen = sizeof(Req) ; 
	char * p = szSendBuf + SendLen;
	strcpy(p, "Delete") ;
	SendLen += strlen("Delete")  +1 ;
	g_Client.SendMsg(szSendBuf , SendLen) ;
}
void CdsExpClntDlg::OnBnClickedButton3()
{
	ExpInsert();
}
void CdsExpClntDlg::OnBnClickedButton4()
{
	ExpSelect() ;
}
void CdsExpClntDlg::OnBnClickedButton5()
{
	ExpUpdate() ;
}
void CdsExpClntDlg::OnBnClickedButton6()
{
	ExpDelete() ;
}
