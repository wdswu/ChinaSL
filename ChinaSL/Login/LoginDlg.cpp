// LoginDlg.cpp : 实现文件
//
#include "stdafx.h"
#include "../stdafx.h"

#include "../ChinaSL.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "../WDS_COMMON.h"

//BSPHP全局变量，程序各地方都使用同一个网络验证，全程序唯一。
extern CAppen g_BSPHP;
//BSP登录用户名密码，在登录时获取，供程序其他地方使用，全程序唯一。
CString g_BspUser;
CString g_BspPwd;

//本程序版本号
extern CString g_strVer;
//配置文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strCfgName;


// CLoginDlg 对话框

IMPLEMENT_DYNAMIC(CLoginDlg, CDialogEx)

CLoginDlg::CLoginDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_LOGIN, pParent)
	, m_UserName(_T(""))
	, m_UserPwd(_T(""))
{

}

CLoginDlg::~CLoginDlg()
{
}

void CLoginDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_LOGIN_USER, m_UserName);
	DDX_Text(pDX, IDC_EDT_LOGIN_PWD, m_UserPwd);
}


BEGIN_MESSAGE_MAP(CLoginDlg, CDialogEx)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_LOGIN, &CLoginDlg::OnBnClickedBtnLogin)
	ON_BN_CLICKED(IDC_BTN_MACHINE_CODE, &CLoginDlg::OnBnClickedBtnMachineCode)
	ON_STN_CLICKED(IDC_STATIC_LOGIN_YZ_PIC, &CLoginDlg::OnStnClickedStaticLoginYzPic)
	ON_BN_CLICKED(IDC_BTN_LOGIN_REGISTRATION, &CLoginDlg::OnBnClickedBtnLoginRegistration)
	ON_BN_CLICKED(IDC_BTN_LOGIN_VIPCHONG, &CLoginDlg::OnBnClickedBtnLoginVipchong)
	ON_BN_CLICKED(IDC_BTN_LOGIN_JIEKEY, &CLoginDlg::OnBnClickedBtnLoginJiekey)
	ON_BN_CLICKED(IDC_BTN_VIEW_USER_INFO, &CLoginDlg::OnBnClickedBtnViewUserInfo)
	ON_BN_CLICKED(IDC_BTN_LOGIN_CONNECT_TEST, &CLoginDlg::OnBnClickedBtnLoginConnectTest)
END_MESSAGE_MAP()


// CLoginDlg 消息处理程序


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	CString strDebug = _T("【登录】足彩赔率库 By 紫血软件 ");
	CString strRelease = _T("【登录】足彩赔率库 By 紫血软件 ");
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
		//strTimeBuf.Format(_T("%s %4d%02d%02d %02d:%02d:%02d"), g_strVer, st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond);
		strTimeBuf = g_strVer;

#ifdef _UNICODE
		strDebug = strDebug + strTimeBuf;
		strRelease = strRelease + strTimeBuf;
#else
		strDebug = strDebug + strTimeBuf;
		strRelease = strRelease + strTimeBuf;
#endif
	}
#ifdef	_DEBUG
	SetWindowText(strDebug);
#else
	SetWindowText(strRelease);
#endif

	CString strWebUrl = g_BSPHP.AppEn_GetWebUrl(); 
	CString strUrl = g_BSPHP.AppEn_GetUrl();
	CString strLoginInfo;
	strLoginInfo.Format(_T("1、%s\r\n2、%s"), strWebUrl, strUrl);
	//显示软件描述信息
	strLoginInfo = g_BSPHP.AppEn_GetMiaoShu();
	SetDlgItemText(IDC_EDT_LOGIN_WEB_URL, strLoginInfo);

	//ini文件中的配置
	CConfigIni cfgIni(g_strCfgName);
	//读取帐号
	TCHAR name[50] = { 0 };
	cfgIni.GetIniString("USERINFO", "userName", name);
	SetDlgItemText(IDC_EDT_LOGIN_USER, name);
	//读取密码
	TCHAR pwd[50] = { 0 };
	cfgIni.GetIniString("USERINFO", "userPwd", pwd);
	SetDlgItemText(IDC_EDT_LOGIN_PWD, pwd);

	//程序运行时加载一次验证码
	ShowVerifyCodePic();


	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
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


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	hbr = CreateSolidBrush(RGB(0, 120, 215));

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CLoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: 在此处添加消息处理程序代码

	//要在OnPaint()中重绘，所以在销毁窗口再调用
	g_BSPHP.m_AppEn_imga.Free();
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	//重绘验证码
	CDC *pDC = GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC)->GetDC();
	g_BSPHP.m_AppEn_imga.Render(pDC, CRect(0, 0, 91, 32));
	ReleaseDC(pDC);
}


//显示验证码图片
void CLoginDlg::ShowVerifyCodePic()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowVerifyCodePic()");
	BOOL bRet = g_BSPHP.Appen_getImage();
	if (bRet)
	{
		//设置静态控件的样式，使其可以使用位图，并使位图显示居中
		//((CStatic*)GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC))->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
		//绘制1次验证码
		CDC *pDC = GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC)->GetDC();
		g_BSPHP.m_AppEn_imga.Render(pDC, CRect(0, 0, 91, 32));
		ReleaseDC(pDC);
	}
	VMProtectEnd();
}

//登录
void CLoginDlg::OnBnClickedBtnLogin()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.Login()");

	UpdateData(true);
	if (m_UserName == _T("") || m_UserPwd == _T(""))
	{
		MessageBox("帐号密码不能为空", "登录");
		return;
	}

	//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("正在登录，请稍等..."), 0);
	SetDlgItemText(IDC_BTN_LOGIN, _T("正在登录.."));
	GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(0);

	//保存登录的用户，供程序其他地方使用
	g_BspUser = m_UserName;
	g_BspPwd = m_UserPwd;

	//踢掉原先登录的帐号****会导致没法从后台控制机器数，但是会让之前登录的T掉，然后下线*******
	g_BSPHP.AppEn_LoginOff(m_UserName, m_UserPwd);

	//获取用户输入的验证码
	CString strCode;
	GetDlgItemText(IDC_EDT_LOGIN_YZ_CODE, strCode);

	//调用登录接口
	CString Sult;
	Sult = g_BSPHP.AppEn_LogIn(m_UserName, m_UserPwd, strCode, GetMachineCode());

	if (Sult.Find("1011") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("登录成功！请稍等..."), 0);
		//ini文件中的配置
		CConfigIni cfgIni(g_strCfgName);
		cfgIni.WriteIniString("USERINFO", "userName", g_BspUser);
		cfgIni.WriteIniString("USERINFO", "userPwd", g_BspPwd);

		//此返回结果8888用于主对话框判断是否成功登录
		EndDialog(8888);
		return;
	}
	else if (Sult.Find("9908") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("登录失败"), 0);
		MessageBox("您的账号已到期，请续费", "登录");
	}
	else if (Sult.Find("验证码") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("登录失败"), 0);
		MessageBox(Sult, "登录");
	}
	else if (Sult.Find("还没有绑定") > -1)
	{
		//绑定本机特征码
		Sult = g_BSPHP.AppEn_BindkeyByUser(m_UserName, m_UserPwd, GetMachineCode());
		if (Sult.Find("5013") > -1)
		{
			MessageBox("首次登录绑定成功，请重新登录", "登录");
		}
	}
	else
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("登录失败"), 0);
		MessageBox(Sult, "登录");
	}

	VMProtectEnd();
	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetWindowText("");

	SetDlgItemText(IDC_BTN_LOGIN, _T("用户登录"));
	GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(1);

}


//显示本机特征码
void CLoginDlg::OnBnClickedBtnMachineCode()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowMacCode()");

	// TODO: 在此添加控件通知处理程序代码
	CString str = GetMachineCode();
	WDS_SetClipBoardText(str);
	MessageBox(str, _T("本机特征码，已复制到剪贴板"));

	VMProtectEnd();
}


//验证码单击消息
void CLoginDlg::OnStnClickedStaticLoginYzPic()
{
	ShowVerifyCodePic();
}

//注册账号
void CLoginDlg::OnBnClickedBtnLoginRegistration()
{
	CString strUser, strPwd, strPwdb, strQQ, strMail, strCoode, strMobile, strWenti, strDaan;

	//先获取用户输入的信息
	GetDlgItemText(IDC_EDT_LOGIN_REG_USER, strUser);
	GetDlgItemText(IDC_EDT_LOGIN_REG_PWD, strPwd);
	GetDlgItemText(IDC_EDT_LOGIN_REG_PWDB, strPwdb);
	GetDlgItemText(IDC_EDT_LOGIN_REG_QQ, strQQ);
	GetDlgItemText(IDC_EDT_LOGIN_REG_MAIL, strMail);
	GetDlgItemText(IDC_EDT_LOGIN_REG_COODE, strCoode);
	GetDlgItemText(IDC_EDT_LOGIN_REG_MOBILE, strMobile);
	GetDlgItemText(IDC_EDT_LOGIN_REG_WENTI, strWenti);
	GetDlgItemText(IDC_EDT_LOGIN_REG_DAAN, strDaan);

	if (strUser == _T("") || strPwd == _T("") || strPwdb == _T("") || strQQ == _T("") || strMail == _T("") || strCoode == _T("") || strWenti == _T("") || strDaan == _T(""))
	{
		MessageBox(_T("您有未输入的信息，请填写完整！"));
		return;
	}
	if (strUser.GetLength() < 5 || strUser.GetLength() > 15)
	{
		MessageBox(_T("帐号限制5-15个字符！"));
		return;
	}
	if (strPwd != strPwdb)
	{
		MessageBox(_T("两次输入的密码不一样！"));
		return;
	}
	if (strPwd.GetLength() < 4)
	{
		MessageBox(_T("密码不能小于4位！"));
		return;
	}

	//再调用BSPHP接口
	CString strRegRet = g_BSPHP.AppEn_registration(strUser, strPwd, strPwdb, strQQ, strMail, NULL, strCoode, strMobile, strWenti, strDaan);
	//重新取验证码
	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();
	MessageBox(strRegRet);

	//清空用户输入的信息-防恶意注册
	SetDlgItemText(IDC_EDT_LOGIN_REG_USER, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_PWD, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_PWDB, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_QQ, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_MAIL, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_COODE, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_MOBILE, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_WENTI, _T(""));
	SetDlgItemText(IDC_EDT_LOGIN_REG_DAAN, _T(""));
}

//充值续期
void CLoginDlg::OnBnClickedBtnLoginVipchong()
{
	CString strCardID, strCardPwd;
	GetDlgItemText(IDC_EDT_LOGIN_CHONGZHI_COODE, strCardID);
	GetDlgItemText(IDC_EDT_LOGIN_CHONGZHI_KEY, strCardPwd);
	if (strCardID == _T("") || strCardPwd == _T(""))
	{
		MessageBox(_T("请填写完整充值卡号、卡密！"));
		return;
	}
	CString strUser, strPwd;
	GetDlgItemText(IDC_EDT_LOGIN_USER, strUser);
	GetDlgItemText(IDC_EDT_LOGIN_PWD, strPwd);
	if (strUser == _T("") || strPwd == _T(""))
	{
		MessageBox(_T("请填写完整 用户登录处 的帐号、密码！"));
		return;
	}
	//调用充值
	CString strCZRet = g_BSPHP.AppEn_VipChong(strUser, strPwd, strCardID, strCardPwd, _T("1"));
	MessageBox(strCZRet);
}

//解绑帐号
void CLoginDlg::OnBnClickedBtnLoginJiekey()
{
	CString strUser, strPwd;
	GetDlgItemText(IDC_EDT_LOGIN_USER, strUser);
	GetDlgItemText(IDC_EDT_LOGIN_PWD, strPwd);
	if (strUser == _T("") || strPwd == _T(""))
	{
		MessageBox(_T("请填写完整 用户登录处 的帐号、密码！"));
		return;
	}
	//获取转绑定扣除秒数
	CString strTurn = g_BSPHP.AppEn_GlobalInfo("GLOBAL_TURN");
	//转为小时
	int iHours = _ttoi(strTurn) / 3600;
	CString strMsg;
	strMsg.Format(_T("解绑定需扣除%d小时帐号使用时间！\r\n\r\n解绑成功后，下一次登录会自动绑定！\r\n\r\n是否确定解绑？"), iHours);
	int retBox = MessageBox(strMsg, "注意！！！", MB_YESNO | MB_ICONWARNING);
	if (retBox == IDNO)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("取消解绑定"), 0);
		return;
	}
	//调用解绑定接口
	CString strUnBindRet = g_BSPHP.AppEn_UnBindKey(strUser, strPwd);
	//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("帐号解绑定成功"), 0);
	MessageBox(_T("帐号解绑定成功,") + strUnBindRet);
}

//查看帐号信息
void CLoginDlg::OnBnClickedBtnViewUserInfo()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowUserInfo");

	UpdateData(true);
	//获取用户输入的验证码
	CString strCode;
	GetDlgItemText(IDC_EDT_LOGIN_YZ_CODE, strCode);
	CString Sult;
	Sult = g_BSPHP.AppEn_LogIn(m_UserName, m_UserPwd, strCode, GetMachineCode());
	CString info = g_BSPHP.AppEn_GetUserInfo("激活时间:UserReDate\n激活时Ip:UserReIp\t\n用户状态:UserIsLock\n登录时间:UserLogInDate\n登录Ip:UserLogInIp\n到期时间:UserVipDate\n绑定特征:UserKey\n账号余额:UserRMB\n用户QQ:UserQQ\n用户邮箱:UserMAIL\n密保问题:Usermibao_wenti");
	//("激活时间:UserReDate\n激活时Ip:UserReIp\t\n用户状态:UserIsLock\n登录时间:UserLogInDate\n登录Ip:UserLogInIp\n到期时间:UserVipDate\n绑定特征:UserKey\n用户分组名称:Class_Nane\n用户分组别名:Class_Mark\n用户QQ:UserQQ\n用户邮箱:UserMAIL\n购卡折扣:UserPayZhe\n是否代理:1:UserTreasury\n电话:UserMobile\n账号金额:UserRMB\n账号积分:UserPoint\n密保问题:Usermibao_wenti\nVIP到期时间:UserVipDate\nvip是否到期没有到期返回1到期返回2:UserVipWhether");
	MessageBox(info, "用户信息");

	//踢掉刚才登录的帐号
	g_BSPHP.AppEn_LoginOff(m_UserName, m_UserPwd);

	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();

	VMProtectEnd();
}


//连接测试
void CLoginDlg::OnBnClickedBtnLoginConnectTest()
{
	if (g_BSPHP.AppEn_Internet() == "1")
		MessageBox("连接服务器成功！", "连接测试");
	else
		MessageBox("连接服务器失败！", "连接测试");
}
