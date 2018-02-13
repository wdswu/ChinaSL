
// ChinaSL.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "ChinaSLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Login/LoginDlg.h"//本软件用户登录对话框

#include "WDS_COMMON.h"

//界面、按钮等使用系统风格
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//---------------------设置各个版本程序的输出名字------------------
// 正常编译Release版本时，生成不同名字的程序
// 编译Debug版本程序时，生成TD.exe，这时TD需要和项目属性-常规配置的一样
// 要调试Release，需要把NDEBUG改为未定义,并放开#else
//#ifdef NDEBUG//if1
//#pragma warning(disable: 4070)

#if WDS_ADMIN//if1.1
//#pragma comment(linker, "/OUT:\"../Release/ChinaSL_Admin.exe\"")
#pragma comment(linker, "/OUT:\"../Release/ChinaSL.exe\"")
#else
#pragma comment(linker, "/OUT:\"../Release/足彩赔率库.exe\"")
#endif//if1.1

//#else//if1
//调试Release时放开
//#pragma comment(linker, "/OUT:\"../Bin/TD.exe\"")
//#endif//if1

//BSPHP全局变量，程序各地方都使用同一个网络验证（唯一WuXia.cpp）
CAppen g_BSPHP;

//关键验证数据,唯一VerificationData.cpp
extern CParseVerificationData g_VfcData;

//界面dpi比率
float g_dpi_rate = 1.0;

//------------------------------------------------------------------
// 创建注册表中的注册码键、键值
// pSaveKeyToReg---输入已经比较后，有效的注册码
bool CreateRegInfo(TCHAR *pSaveKeyToReg)
{
	HKEY phkResult;
	DWORD dwDisposition;// 反回值代表创建新的或打开存在的REG_CREATED_NEW_KEY(1),REG_OPENED_EXISTING_KEY(2) 
						// 创建指定的注册表键，如果已存在，则打开。
	LONG err = RegCreateKeyEx(HKEY_CURRENT_USER, SOFTWARE_CHINASL, 0, NULL, REG_OPTION_NON_VOLATILE,
		KEY_CREATE_SUB_KEY | KEY_ALL_ACCESS, NULL, &phkResult, &dwDisposition);
	if (ERROR_SUCCESS == err)
	{
		RegSetValueEx(phkResult, TEXT("RegKey"), 0, REG_SZ, (BYTE*)pSaveKeyToReg, _tcslen(pSaveKeyToReg) * sizeof(TCHAR));
		err = RegSetValueEx(phkResult, TEXT("RegKey"), 0, REG_SZ, (BYTE*)pSaveKeyToReg, _tcslen(pSaveKeyToReg) * sizeof(TCHAR));
		if (ERROR_SUCCESS != err)
		{
			RegCloseKey(phkResult);
			return false;
		}
	}
	else
	{
		return false;
	}

	RegCloseKey(phkResult);
	return true;
}


//----------------------------------------------------------------------------
// 读取注册表中的注册码
// keyInReg---输出读取的注册码
bool ReadRegInfo(TCHAR *keyInReg)
{
	DWORD dwtype, sizeBuff;
	HKEY phkResult;// 得到的将要打开键的句柄
	if (ERROR_SUCCESS == RegOpenKeyEx(HKEY_CURRENT_USER, SOFTWARE_CHINASL, 0, KEY_READ, &phkResult))
	{
		RegQueryValueEx(phkResult, TEXT("RegKey"), 0, &dwtype, (BYTE*)keyInReg, &sizeBuff);
		if (ERROR_SUCCESS != RegQueryValueEx(phkResult, TEXT("RegKey"), 0, &dwtype, (BYTE*)keyInReg, &sizeBuff))
		{
			RegCloseKey(phkResult);
			return false;
		}
	}
	else
	{
		return false;
	}

	RegCloseKey(phkResult);
	return true;
}


// CChinaSLApp

BEGIN_MESSAGE_MAP(CChinaSLApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CChinaSLApp 构造

CChinaSLApp::CChinaSLApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CChinaSLApp 对象

CChinaSLApp theApp;


// CChinaSLApp 初始化

BOOL CChinaSLApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	//计算系统UI缩放比率
	HDC hdc = ::GetDC(NULL);
	g_dpi_rate = GetDeviceCaps(hdc, LOGPIXELSX);
	//WDS_MessageBox(g_dpi_rate);
	g_dpi_rate /= 96;


	VMProtectBeginUltra("WDS_VMBEGIN OF g_BSPHP.init()");
#if !WDS_ADMIN
	//::PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("正在启动，请稍等..."), 0);
	//初始化全局变量BSPHP
	if (!g_BSPHP.init())
	{
		return FALSE;
	}
	CLoginDlg LoginDlg;
	INT_PTR nRet = LoginDlg.DoModal();
	if (nRet != 8888)
	{
		return FALSE;
	}

	//获取Bsphp服务器验证数据
	CString BsphpData = g_BSPHP.AppEn_GetVerification();
	//AfxMessageBox(BsphpData);
	//解析验证数据
	g_VfcData.ParseVerificationData(BsphpData);// _T(""), "vrfdata.xml");
#endif
	VMProtectEnd();




	CChinaSLDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}

