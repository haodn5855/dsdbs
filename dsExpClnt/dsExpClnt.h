
// dsExpClnt.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CdsExpClntApp:
// �йش����ʵ�֣������ dsExpClnt.cpp
//

class CdsExpClntApp : public CWinApp
{
public:
	CdsExpClntApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CdsExpClntApp theApp;