
// dsExpSvr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdsExpSvrApp:
// �йش����ʵ�֣������ dsExpSvr.cpp
//

class CdsExpSvrApp : public CWinApp
{
public:
	CdsExpSvrApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdsExpSvrApp theApp;