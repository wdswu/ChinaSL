
// ChinaSL.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������

#include "CCslAdo.h"//���ݿ���


//1������Ա�ɼ����ݽ������,0���������ͻ�
#define WDS_ADMIN 1
//����ȥ����֤
#define WDS_PROGRAM_TEST 0


//------------------------------------------------------------------
// ����ע����е�ע���������ֵ
// pSaveKeyToReg---�����Ѿ��ȽϺ���Ч��ע����
bool CreateRegInfo(TCHAR *pSaveKeyToReg);
//----------------------------------------------------------------------------
// ��ȡע����е�ע����
// keyInReg---�����ȡ��ע����
bool ReadRegInfo(TCHAR *keyInReg);


// CChinaSLApp: 
// �йش����ʵ�֣������ ChinaSL.cpp
//

class CChinaSLApp : public CWinApp
{
public:
	CChinaSLApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CChinaSLApp theApp;