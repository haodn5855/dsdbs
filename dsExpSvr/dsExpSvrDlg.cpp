
// dsExpSvrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "dsExpSvr.h"
#include "dsExpSvrDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "gxDSApp.h"

#include "gxThread.h"
#include "gxThreadAccept.h"
#include "gxThreadRecv.h"
#include "gxVirBusiness.h"

// CdsExpSvrDlg �Ի���




CdsExpSvrDlg::CdsExpSvrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CdsExpSvrDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CdsExpSvrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CdsExpSvrDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CdsExpSvrDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CdsExpSvrDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CdsExpSvrDlg ��Ϣ�������

BOOL CdsExpSvrDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CdsExpSvrDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CdsExpSvrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

CgxDSApp g_DSApp;

CgxThreadRecv g_ThreadRecv ; 
CgxThreadAccept g_ThreadAccept ;
CgxListObj g_Recv ;	
CgxListObj g_Send ;	
int g_VirBusinCount =6;
CgxVirBusiness * g_Business= NULL ;

void CdsExpSvrDlg::OnBnClickedButton1()
{
	AfxOleInit() ;
	AfxSocketInit() ;
	g_Business = new CgxVirBusiness[g_VirBusinCount];
	for( int i = 0 ;i < g_VirBusinCount ;i ++) 
	{
		g_Business[i].SetList(NULL,&g_Recv,&g_Send) ;
		g_Business[i].StartThread() ; 
	}

	g_ThreadRecv.SetList(NULL,&g_Recv,&g_Send) ;
	g_ThreadAccept.m_Port = 6666 ; 
	g_ThreadAccept.SetThreadRecv(&g_ThreadRecv) ; 	

	g_ThreadRecv.StartThread() ;
	g_ThreadAccept.StartThread(); 

	// dsDBS Init
//	DSApp.SetDSSVRID(1);
	g_DSApp.SetDebug() ;
	int nRet = g_DSApp.Init(10001,"dssysdb","dssysdb","",3306,3,"dsuser","dspass123");
	if( nRet <0)
	{
		AfxMessageBox("��ʼ��ʧ�ܣ�");
		OnBnClickedButton2();
	}
}

void CdsExpSvrDlg::OnBnClickedButton2()
{
	if( g_Business!= NULL ) 
		for( int i = 0 ;i < g_VirBusinCount ;i ++) 
			g_Business[i].StopThread() ; 

	g_ThreadAccept.StopThread(0) ; 
	g_ThreadRecv.StopThread(0) ; 

	g_ThreadAccept.StopThread() ; 
	g_ThreadRecv.StopThread() ; 	
}

BOOL CdsExpSvrDlg::DestroyWindow()
{
	// TODO: �ڴ����ר�ô����/����û���
	OnBnClickedButton2();
	return CDialogEx::DestroyWindow();
}
