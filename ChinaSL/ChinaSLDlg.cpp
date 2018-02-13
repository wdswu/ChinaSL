
// ChinaSLDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "ChinaSLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "WDS_COMMON.h"

#define UM_MAIN_DLG_END				WM_USER+104 //通知主对话框退出的消息，BSP检测线程通知

CString g_strVer = "v18.01.24";

//日志文件名，唯一(ChinaSLDlg.CPP)
CString g_strLogName = "Log.txt";
//配置文件名，唯一(ChinaSLDlg.CPP)
CString g_strCfgName = "CslConfig.dat";

//界面dpi比率
extern float g_dpi_rate;

//BSPHP验证线程的退出标志
bool g_bExitThreadBsphp = false;
//BSPHP全局变量，程序各地方都使用同一个网络验证，全程序唯一。
extern CAppen g_BSPHP;
//BSP登录用户名密码，在登录时获取，供程序其他地方使用，全程序唯一。
extern CString g_BspUser;
extern CString g_BspPwd;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CChinaSLDlg 对话框



CChinaSLDlg::CChinaSLDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_CHINASL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CChinaSLDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB_MAIN, m_MainTab);
}

BEGIN_MESSAGE_MAP(CChinaSLDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
	ON_WM_CTLCOLOR()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CChinaSLDlg::OnTcnSelchangeTabMain)
	ON_WM_DESTROY()
	ON_MESSAGE(UM_MAIN_DLG_END, &CChinaSLDlg::OnMainDialogEnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CChinaSLDlg 消息处理程序


//----------------检测用户BSP后台是否到期的线程--------------------
UINT __cdecl ThreadCheckBSPHP(LPVOID param)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF ThreadCheckBSPHP()");

	//显示1次账户信息
	//CString strUserInfo;
	//strUserInfo.Format(_T("登录用户: %s\r\n"), g_BspUser);
	//strUserInfo += g_BSPHP.AppEn_GetUserInfo("登录的IP: UserLogInIp\r\n到期时间: UserVipDate");
	////PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)(LPCTSTR)strUserInfo, 0);
	//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, strUserInfo);


	//显示软件BSP后台的公告
	//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, g_BSPHP.AppEn_GetGongGao());

	//----------------------------------------开始while验证---------------------------------------------
	DWORD dwDelayMS;
	DWORD dwDelayCount;
	int iCountError = 0;
	while (!g_bExitThreadBsphp)
	{
		//----验证：g_BSPHP对象是否有效----
		if (g_BSPHP.Version == _T(""))
		{
			//如果g_BSPHP的版本号为空，则关闭程序
			::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("验证失败"), 0);
			break;
		}

		//----验证：是否保持登录状态----
		CString strLogin = g_BSPHP.AppEn_GetLoginInfo();
		if (strLogin != _T("1"))
		{
			//累计失败次数
			iCountError++;
			//----避免验证服务器状况不好，误关程序，检测50分钟----
			if (iCountError > 10)
			{
				//如果是未登录状态，则关闭程序
				::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("验证失败"), 0);
				MessageBox(*(HWND*)param, strLogin, "", 0);
				break;
			}
			//CString strInfo;
			//strInfo.Format(_T("%s第%d/10次登录A验证失败，继续检测"), strLogin, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------如果失败次数<10，则先等待5分钟，再检测--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//延时次数
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			//再次调用检测
			continue;
		}

		//========================获取心跳包状态=============================
		CString Sult = g_BSPHP.AppEn_timeout();
		//AfxMessageBox(Sult);//*******这里到期后居然也是返回5031********
		//-------------------检测心跳包返回值--------------------
		if (Sult.Find("5031") > -1)//心跳正常
		{
			iCountError = 0;
			//每3小时获取一次BSP后台的公告
			static DWORD startMs = GetTickCount();
			if (GetTickCount() - startMs > 3 * 60 * 60 * 1000)
			{
				startMs = GetTickCount();
				//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, g_BSPHP.AppEn_GetGongGao());
			}

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)(LPCTSTR)_T("心跳正常"), 0);
#else
			//-------------成功就延时10小时，再检测----------------
			dwDelayMS = 10 * 60 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			continue;
		}
		else if (Sult.Find("5030") > -1)//心跳帐号到期
		{
			//关闭程序
			::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("帐号到期"), 0);
			break;
		}

		//===========以下对非上面2种正常情况进行处理=============

		//----避免验证服务器状况不好，误关程序，检测50分钟----
		if (iCountError < 10)//心跳失败
		{
			//累计失败次数
			iCountError++;

			//CString strInfo;
			//strInfo.Format(_T("%s第%d/10次心跳失败，继续检测"), Sult, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------如果失败次数<10，则先等待5分钟，再检测--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//延时次数
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			//再次调用检测
			continue;
		}

		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)_T("验证失败，程序结束"), 0);
		AfxMessageBox(_T("验证失败，程序结束"));
		//关闭程序
		::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("验证失败"), 0);

		break;
	}//end while

	 //PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T(".....................验证Trd结束....................."), 0);

	VMProtectEnd();

	return 0;
}

BOOL CChinaSLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	CString strDebug = _T("足彩赔率库") + g_strVer + " By 紫血软件";
	CString strRelease = _T("足彩赔率库") + g_strVer + " By 紫血软件";
	//获取文件修改时间
	CWinApp* pApp = AfxGetApp();
	CString strAppName;
	if (NULL != pApp)
	{
		TCHAR szAppName[MAX_PATH];
		memset(szAppName, 0, MAX_PATH * sizeof(TCHAR));
		::GetModuleFileName(pApp->m_hInstance, szAppName, MAX_PATH);
		strAppName = szAppName;
		WIN32_FIND_DATA ffd;
		HANDLE hFind = FindFirstFile(szAppName, &ffd);
		SYSTEMTIME st = { 0 };
		FILETIME LocalFileTimel;
		FileTimeToLocalFileTime(&ffd.ftLastWriteTime, &LocalFileTimel);
		FileTimeToSystemTime(&LocalFileTimel, &st);
		CString strTimeBuf, strDmVer;
		strTimeBuf.Format(_T(" %4d%02d%02d %02d:%02d:%02d"), st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);

#ifdef _UNICODE
		strDebug = strDebug + " DEBUG UNICODE AT" + strTimeBuf;
		strRelease = strRelease + " RELEASE UNICODE AT" + strTimeBuf;
#else
		strDebug = strDebug + " DEBUG ANSI AT" + strTimeBuf;
		strRelease = strRelease + " RELEASE ANSI AT" + strTimeBuf;
#endif
	}
#ifdef	_DEBUG
	SetWindowText(strDebug);
#else
	SetWindowText(strRelease);
#endif


	//初始化并调整控件位置
	InitAllControl();
	SetControlRect();

#if !WDS_ADMIN
	g_bExitThreadBsphp = false;
	//设置检测BSP后台验证的线程，心跳延时要小于服务器的30分钟
	m_pBsphpThread = AfxBeginThread(ThreadCheckBSPHP, &m_hWnd);
	//设置检测BSP后台验证的定时器，间隔时间要大于检测的延时，可等于服务器的30分钟-已改为9小时
	SetTimer(1, 9 * 60 * 60 * 1000, NULL);
#endif


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChinaSLDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CChinaSLDlg::OnPaint()
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
HCURSOR CChinaSLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChinaSLDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)// 拦截ESC
			return TRUE;
		if (pMsg->wParam == VK_RETURN)// 拦截回车
		{
			CWnd* wnd = GetFocus();
			if (wnd != NULL)
			{
				TCHAR str[260];
				CString ClassName = _T("Button");// 按钮类
				GetClassName(wnd->m_hWnd, str, 260);// 取得控件类型名
				UINT id = wnd->GetDlgCtrlID();// 得到控件ID号
				if (ClassName == str)
				{// 如果当前焦点是按钮，则发送WM_COMMAND消息点击该按钮
					SendMessage(WM_COMMAND, id, (LPARAM)wnd->m_hWnd);
					return TRUE;
				}
			}
			// 如果当前焦点不是上面控件，则设置输入焦点为下一个控件
			CWnd *mwnd = GetNextDlgTabItem(wnd);
			if (mwnd)
			{
				mwnd->SetFocus();// 设置焦点为下一个TAB循序的控件
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}

HBRUSH CChinaSLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//TCHAR str[260];
	//GetClassName(pWnd->m_hWnd, str, 260);// 取得控件类型名
	//CString ClassName = _T("Edit");// 编辑框类
	////UINT id = pWnd->GetDlgCtrlID();// 得到控件ID号
	////if (ClassName != str)
	//{//除编辑框外，背景能变的都变色
	//	pDC->SetTextColor(RGB(255, 255, 255)); //设置字体颜色
	//	//pDC->SelectObject(&m_Font); //设置字体样式

	//	//设置背景色
	//	pDC->SetBkMode(TRANSPARENT);
	//	//hbr = CreateSolidBrush(RGB(0, 120, 215));//蓝色
	//	hbr = CreateSolidBrush(RGB(0x6C, 0x6F, 0x85));//色
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CChinaSLDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}

void CChinaSLDlg::InitAllControl()
{
	//插一页TAB
	m_MainTab.InsertItem(0, _T("赔率表"));
	//设置对话框的父窗口为主TAB控件 //GetDlgItem(IDC_TAB_MAIN));
	m_Dlg_cslPeilv.Create(IDD_DLG_CSL_PEILV, &m_MainTab);
#if WDS_ADMIN
	m_MainTab.InsertItem(1, _T("数据库"));
	m_reptileDlg.Create(IDD_DLG_REPTILE, &m_MainTab);
#endif
}

void CChinaSLDlg::SetControlRect()
{
	CRect dlgRect;
	GetWindowRect(&dlgRect);
	dlgRect.right = (1336 + 212 + 212) * g_dpi_rate;
	dlgRect.bottom = (695 + 50) *  g_dpi_rate;
	MoveWindow(dlgRect);

	//获取tab的区域
	CRect tabRect;
	GetClientRect(&tabRect);
	//tabRect.left *= g_dpi_rate;//=0
	//tabRect.top *= g_dpi_rate;//=0
	//tabRect.right *= g_dpi_rate;
	//tabRect.bottom *= g_dpi_rate;
	m_MainTab.MoveWindow(&tabRect);

	//设置tab标签的宽高
	CSize size;
	//size.cx *= g_dpi_rate;//=0
	size.cy = 28 * g_dpi_rate;
	m_MainTab.SetItemSize(size);
	m_MainTab.SetItemState(0, TCM_SETITEM, TCIS_HIGHLIGHTED);

	//设置各个子对话框的窗体大小范围，以便留出tab的标签及各边框
	//tabRect.left *= g_dpi_rate;;//TAB标签的左边=0
	tabRect.top += size.cy;//TAB标签的高度0+=
	//tabRect.right /= g_dpi_rate;;//TAB标签的右边
	//tabRect.bottom /= g_dpi_rate;;//TAB标签的底边

	//设置对话框大小为TABControl大小减去标签
	m_Dlg_cslPeilv.MoveWindow(&tabRect);
	m_Dlg_cslPeilv.ShowWindow(TRUE);

#if WDS_ADMIN
	m_reptileDlg.MoveWindow(&tabRect);
#endif

	tabRect.top -= size.cy;//List起点要减去TAB标签的高度
	//tabRect.right *= g_dpi_rate;
	tabRect.bottom -= (115 + 60) * g_dpi_rate*1.1;//List的底边要留出下面搜索按钮的位置
	m_Dlg_cslPeilv.m_listCtrl.MoveWindow(&tabRect);
}

void CChinaSLDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	m_Dlg_cslPeilv.ShowWindow(FALSE);
#if WDS_ADMIN
	m_reptileDlg.ShowWindow(FALSE);
	m_MainTab.SetItemState(1, TCM_SETITEM, TCIS_BUTTONPRESSED);
#endif

	//设置TAB标签高亮
	m_MainTab.SetItemState(0, TCM_SETITEM, TCIS_BUTTONPRESSED);

	int iSelect = m_MainTab.GetCurSel();
	//设置TAB标签高亮
	m_MainTab.SetItemState(iSelect, TCM_SETITEM, TCIS_HIGHLIGHTED);
	CRect rect, rect2;
	switch (iSelect)
	{
	case 0://赔率库
		SetWindowPos(0, 0, 0, (1336 + 212 + 212) *  g_dpi_rate, (695 + 50) *  g_dpi_rate, SWP_NOMOVE);
		m_Dlg_cslPeilv.ShowWindow(TRUE);
		break;
#if WDS_ADMIN
	case 1://爬数据
		SetWindowPos(0, 0, 0, 920 * g_dpi_rate, 600 * g_dpi_rate, SWP_NOMOVE);
		m_reptileDlg.ShowWindow(TRUE);
		break;
#endif
	default:
		break;
	}
}

void CChinaSLDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	//--------结束验证线程--------
	g_bExitThreadBsphp = true;
	//if (m_pBsphpThread)
	{
		//DWORD dwRet = WaitForSingleObject(m_pBsphpThread->m_hThread, 5000);
	}
	//踢掉当前登录的帐号
	g_BSPHP.AppEn_LoginOff(g_BspUser, g_BspPwd);
}
//----------------响应对话框结束消息，主要用与BSPHP验证----------------
afx_msg LRESULT CChinaSLDlg::OnMainDialogEnd(WPARAM wParam, LPARAM lParam)
{
	g_bExitThreadBsphp = true;

	VMProtectBeginUltra("WDS_VMBEGIN OF CWuXiaDlg::OnMainDialogEnd()");

	//踢掉原先登录的帐号
	g_BSPHP.AppEn_LoginOff(g_BspUser, g_BspPwd);

	EndDialog(8887);
	//MessageBox("EndDialog()");
	if ((LPCTSTR)wParam == _T("帐号到期"))
	{
		MessageBox(_T("您帐号已过期，程序已自动关闭。请续费！"));
	}
	else if ((LPCTSTR)wParam == _T("验证失败"))
	{
		MessageBox(_T("程序验证失败，已自动关闭。请联系管理员！"));
	}
	VMProtectEnd();
	return 0;
}

void CChinaSLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	int iCountError = 0;
	switch (nIDEvent)
	{
	case 1:
		DWORD dwDelayMS;
		DWORD dwDelayCount;
		while (!g_bExitThreadBsphp)
		{
			//----BSPHP验证：是否保持登录状态----
			//获取当前帐号状态信息,判断是否登录:登陆状态返回1 否则返回状态信息
			CString strLogin = g_BSPHP.AppEn_GetLoginInfo();
			if (strLogin == _T("1"))//成功登录
			{
				iCountError = 0;
				break;//while
			}
			//未成功登录，先累计失败次数
			iCountError++;
			if (iCountError > 10)
			{
				//如果失败次数>10，则关闭程序
				SendMessage(UM_MAIN_DLG_END, (WPARAM)_T("验证失败"), 0);
				//MessageBox(strLogin);
				break;
			}
			//CString strInfo;
			//strInfo.Format(_T("%s第%d/10次登录B验证失败，继续检测"), strLogin, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------如果失败次数<10，则先等待2分钟，再检测，总延时不能>SetTimer的时间--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//延时次数
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}
		}//end while
		break;//case 1
	case 2:
		break;
	}//end switch
	CDialogEx::OnTimer(nIDEvent);
}
