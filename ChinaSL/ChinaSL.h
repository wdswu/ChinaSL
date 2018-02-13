
// ChinaSL.h : PROJECT_NAME 应用程序的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include "CCslAdo.h"//数据库类


//1供管理员采集数据界面出来,0供发布给客户
#define WDS_ADMIN 1
//用于去掉验证
#define WDS_PROGRAM_TEST 0


//------------------------------------------------------------------
// 创建注册表中的注册码键、键值
// pSaveKeyToReg---输入已经比较后，有效的注册码
bool CreateRegInfo(TCHAR *pSaveKeyToReg);
//----------------------------------------------------------------------------
// 读取注册表中的注册码
// keyInReg---输出读取的注册码
bool ReadRegInfo(TCHAR *keyInReg);


// CChinaSLApp: 
// 有关此类的实现，请参阅 ChinaSL.cpp
//

class CChinaSLApp : public CWinApp
{
public:
	CChinaSLApp();

// 重写
public:
	virtual BOOL InitInstance();

// 实现

	DECLARE_MESSAGE_MAP()
};

extern CChinaSLApp theApp;