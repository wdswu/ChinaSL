
// ChinaSL_Reg.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CChinaSL_RegApp: 
// �йش����ʵ�֣������ ChinaSL_Reg.cpp
//

class CChinaSL_RegApp : public CWinApp
{
public:
	CChinaSL_RegApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChinaSL_RegApp theApp;