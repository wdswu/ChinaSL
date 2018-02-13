// LoginDlg.cpp : ʵ���ļ�
//
#include "stdafx.h"
#include "../stdafx.h"

#include "../ChinaSL.h"
#include "LoginDlg.h"
#include "afxdialogex.h"

#include "../WDS_COMMON.h"

//BSPHPȫ�ֱ�����������ط���ʹ��ͬһ��������֤��ȫ����Ψһ��
extern CAppen g_BSPHP;
//BSP��¼�û������룬�ڵ�¼ʱ��ȡ�������������ط�ʹ�ã�ȫ����Ψһ��
CString g_BspUser;
CString g_BspPwd;

//������汾��
extern CString g_strVer;
//�����ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strCfgName;


// CLoginDlg �Ի���

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


// CLoginDlg ��Ϣ�������


BOOL CLoginDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CString strDebug = _T("����¼��������ʿ� By ��Ѫ��� ");
	CString strRelease = _T("����¼��������ʿ� By ��Ѫ��� ");
	//��ȡ�ļ��޸�ʱ��
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
	strLoginInfo.Format(_T("1��%s\r\n2��%s"), strWebUrl, strUrl);
	//��ʾ���������Ϣ
	strLoginInfo = g_BSPHP.AppEn_GetMiaoShu();
	SetDlgItemText(IDC_EDT_LOGIN_WEB_URL, strLoginInfo);

	//ini�ļ��е�����
	CConfigIni cfgIni(g_strCfgName);
	//��ȡ�ʺ�
	TCHAR name[50] = { 0 };
	cfgIni.GetIniString("USERINFO", "userName", name);
	SetDlgItemText(IDC_EDT_LOGIN_USER, name);
	//��ȡ����
	TCHAR pwd[50] = { 0 };
	cfgIni.GetIniString("USERINFO", "userPwd", pwd);
	SetDlgItemText(IDC_EDT_LOGIN_PWD, pwd);

	//��������ʱ����һ����֤��
	ShowVerifyCodePic();


	SetForegroundWindow();

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL CLoginDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)// ����ESC
			return TRUE;
		if (pMsg->wParam == VK_RETURN)// ���ػس�
		{
			CWnd* wnd = GetFocus();

			if (wnd != NULL)
			{
				TCHAR str[260];
				CString ClassName = _T("Button");// ��ť��
				GetClassName(wnd->m_hWnd, str, 260);// ȡ�ÿؼ�������
				UINT id = wnd->GetDlgCtrlID();// �õ��ؼ�ID��
				if (ClassName == str)
				{// �����ǰ�����ǰ�ť������WM_COMMAND��Ϣ����ð�ť
					SendMessage(WM_COMMAND, id, (LPARAM)wnd->m_hWnd);
					return TRUE;
				}
			}

			// �����ǰ���㲻������ؼ������������뽹��Ϊ��һ���ؼ�
			CWnd *mwnd = GetNextDlgTabItem(wnd);
			if (mwnd)
			{
				mwnd->SetFocus();// ���ý���Ϊ��һ��TABѭ��Ŀؼ�
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CLoginDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	pDC->SetTextColor(RGB(255, 255, 255));
	pDC->SetBkMode(TRANSPARENT);
	hbr = CreateSolidBrush(RGB(0, 120, 215));

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CLoginDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������

	//Ҫ��OnPaint()���ػ棬���������ٴ����ٵ���
	g_BSPHP.m_AppEn_imga.Free();
}


void CLoginDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: �ڴ˴������Ϣ����������
					   // ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
	//�ػ���֤��
	CDC *pDC = GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC)->GetDC();
	g_BSPHP.m_AppEn_imga.Render(pDC, CRect(0, 0, 91, 32));
	ReleaseDC(pDC);
}


//��ʾ��֤��ͼƬ
void CLoginDlg::ShowVerifyCodePic()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowVerifyCodePic()");
	BOOL bRet = g_BSPHP.Appen_getImage();
	if (bRet)
	{
		//���þ�̬�ؼ�����ʽ��ʹ�����ʹ��λͼ����ʹλͼ��ʾ����
		//((CStatic*)GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC))->ModifyStyle(0xF, SS_BITMAP | SS_CENTERIMAGE);
		//����1����֤��
		CDC *pDC = GetDlgItem(IDC_STATIC_LOGIN_YZ_PIC)->GetDC();
		g_BSPHP.m_AppEn_imga.Render(pDC, CRect(0, 0, 91, 32));
		ReleaseDC(pDC);
	}
	VMProtectEnd();
}

//��¼
void CLoginDlg::OnBnClickedBtnLogin()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.Login()");

	UpdateData(true);
	if (m_UserName == _T("") || m_UserPwd == _T(""))
	{
		MessageBox("�ʺ����벻��Ϊ��", "��¼");
		return;
	}

	//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("���ڵ�¼�����Ե�..."), 0);
	SetDlgItemText(IDC_BTN_LOGIN, _T("���ڵ�¼.."));
	GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(0);

	//�����¼���û��������������ط�ʹ��
	g_BspUser = m_UserName;
	g_BspPwd = m_UserPwd;

	//�ߵ�ԭ�ȵ�¼���ʺ�****�ᵼ��û���Ӻ�̨���ƻ����������ǻ���֮ǰ��¼��T����Ȼ������*******
	g_BSPHP.AppEn_LoginOff(m_UserName, m_UserPwd);

	//��ȡ�û��������֤��
	CString strCode;
	GetDlgItemText(IDC_EDT_LOGIN_YZ_CODE, strCode);

	//���õ�¼�ӿ�
	CString Sult;
	Sult = g_BSPHP.AppEn_LogIn(m_UserName, m_UserPwd, strCode, GetMachineCode());

	if (Sult.Find("1011") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("��¼�ɹ������Ե�..."), 0);
		//ini�ļ��е�����
		CConfigIni cfgIni(g_strCfgName);
		cfgIni.WriteIniString("USERINFO", "userName", g_BspUser);
		cfgIni.WriteIniString("USERINFO", "userPwd", g_BspPwd);

		//�˷��ؽ��8888�������Ի����ж��Ƿ�ɹ���¼
		EndDialog(8888);
		return;
	}
	else if (Sult.Find("9908") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("��¼ʧ��"), 0);
		MessageBox("�����˺��ѵ��ڣ�������", "��¼");
	}
	else if (Sult.Find("��֤��") > -1)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("��¼ʧ��"), 0);
		MessageBox(Sult, "��¼");
	}
	else if (Sult.Find("��û�а�") > -1)
	{
		//�󶨱���������
		Sult = g_BSPHP.AppEn_BindkeyByUser(m_UserName, m_UserPwd, GetMachineCode());
		if (Sult.Find("5013") > -1)
		{
			MessageBox("�״ε�¼�󶨳ɹ��������µ�¼", "��¼");
		}
	}
	else
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("��¼ʧ��"), 0);
		MessageBox(Sult, "��¼");
	}

	VMProtectEnd();
	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetWindowText("");

	SetDlgItemText(IDC_BTN_LOGIN, _T("�û���¼"));
	GetDlgItem(IDC_BTN_LOGIN)->EnableWindow(1);

}


//��ʾ����������
void CLoginDlg::OnBnClickedBtnMachineCode()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowMacCode()");

	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString str = GetMachineCode();
	WDS_SetClipBoardText(str);
	MessageBox(str, _T("���������룬�Ѹ��Ƶ�������"));

	VMProtectEnd();
}


//��֤�뵥����Ϣ
void CLoginDlg::OnStnClickedStaticLoginYzPic()
{
	ShowVerifyCodePic();
}

//ע���˺�
void CLoginDlg::OnBnClickedBtnLoginRegistration()
{
	CString strUser, strPwd, strPwdb, strQQ, strMail, strCoode, strMobile, strWenti, strDaan;

	//�Ȼ�ȡ�û��������Ϣ
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
		MessageBox(_T("����δ�������Ϣ������д������"));
		return;
	}
	if (strUser.GetLength() < 5 || strUser.GetLength() > 15)
	{
		MessageBox(_T("�ʺ�����5-15���ַ���"));
		return;
	}
	if (strPwd != strPwdb)
	{
		MessageBox(_T("������������벻һ����"));
		return;
	}
	if (strPwd.GetLength() < 4)
	{
		MessageBox(_T("���벻��С��4λ��"));
		return;
	}

	//�ٵ���BSPHP�ӿ�
	CString strRegRet = g_BSPHP.AppEn_registration(strUser, strPwd, strPwdb, strQQ, strMail, NULL, strCoode, strMobile, strWenti, strDaan);
	//����ȡ��֤��
	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();
	MessageBox(strRegRet);

	//����û��������Ϣ-������ע��
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

//��ֵ����
void CLoginDlg::OnBnClickedBtnLoginVipchong()
{
	CString strCardID, strCardPwd;
	GetDlgItemText(IDC_EDT_LOGIN_CHONGZHI_COODE, strCardID);
	GetDlgItemText(IDC_EDT_LOGIN_CHONGZHI_KEY, strCardPwd);
	if (strCardID == _T("") || strCardPwd == _T(""))
	{
		MessageBox(_T("����д������ֵ���š����ܣ�"));
		return;
	}
	CString strUser, strPwd;
	GetDlgItemText(IDC_EDT_LOGIN_USER, strUser);
	GetDlgItemText(IDC_EDT_LOGIN_PWD, strPwd);
	if (strUser == _T("") || strPwd == _T(""))
	{
		MessageBox(_T("����д���� �û���¼�� ���ʺš����룡"));
		return;
	}
	//���ó�ֵ
	CString strCZRet = g_BSPHP.AppEn_VipChong(strUser, strPwd, strCardID, strCardPwd, _T("1"));
	MessageBox(strCZRet);
}

//����ʺ�
void CLoginDlg::OnBnClickedBtnLoginJiekey()
{
	CString strUser, strPwd;
	GetDlgItemText(IDC_EDT_LOGIN_USER, strUser);
	GetDlgItemText(IDC_EDT_LOGIN_PWD, strPwd);
	if (strUser == _T("") || strPwd == _T(""))
	{
		MessageBox(_T("����д���� �û���¼�� ���ʺš����룡"));
		return;
	}
	//��ȡת�󶨿۳�����
	CString strTurn = g_BSPHP.AppEn_GlobalInfo("GLOBAL_TURN");
	//תΪСʱ
	int iHours = _ttoi(strTurn) / 3600;
	CString strMsg;
	strMsg.Format(_T("�����۳�%dСʱ�ʺ�ʹ��ʱ�䣡\r\n\r\n���ɹ�����һ�ε�¼���Զ��󶨣�\r\n\r\n�Ƿ�ȷ�����"), iHours);
	int retBox = MessageBox(strMsg, "ע�⣡����", MB_YESNO | MB_ICONWARNING);
	if (retBox == IDNO)
	{
		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("ȡ�����"), 0);
		return;
	}
	//���ý�󶨽ӿ�
	CString strUnBindRet = g_BSPHP.AppEn_UnBindKey(strUser, strPwd);
	//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("�ʺŽ�󶨳ɹ�"), 0);
	MessageBox(_T("�ʺŽ�󶨳ɹ�,") + strUnBindRet);
}

//�鿴�ʺ���Ϣ
void CLoginDlg::OnBnClickedBtnViewUserInfo()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF LoginDlg.ShowUserInfo");

	UpdateData(true);
	//��ȡ�û��������֤��
	CString strCode;
	GetDlgItemText(IDC_EDT_LOGIN_YZ_CODE, strCode);
	CString Sult;
	Sult = g_BSPHP.AppEn_LogIn(m_UserName, m_UserPwd, strCode, GetMachineCode());
	CString info = g_BSPHP.AppEn_GetUserInfo("����ʱ��:UserReDate\n����ʱIp:UserReIp\t\n�û�״̬:UserIsLock\n��¼ʱ��:UserLogInDate\n��¼Ip:UserLogInIp\n����ʱ��:UserVipDate\n������:UserKey\n�˺����:UserRMB\n�û�QQ:UserQQ\n�û�����:UserMAIL\n�ܱ�����:Usermibao_wenti");
	//("����ʱ��:UserReDate\n����ʱIp:UserReIp\t\n�û�״̬:UserIsLock\n��¼ʱ��:UserLogInDate\n��¼Ip:UserLogInIp\n����ʱ��:UserVipDate\n������:UserKey\n�û���������:Class_Nane\n�û��������:Class_Mark\n�û�QQ:UserQQ\n�û�����:UserMAIL\n�����ۿ�:UserPayZhe\n�Ƿ����:1:UserTreasury\n�绰:UserMobile\n�˺Ž��:UserRMB\n�˺Ż���:UserPoint\n�ܱ�����:Usermibao_wenti\nVIP����ʱ��:UserVipDate\nvip�Ƿ���û�е��ڷ���1���ڷ���2:UserVipWhether");
	MessageBox(info, "�û���Ϣ");

	//�ߵ��ղŵ�¼���ʺ�
	g_BSPHP.AppEn_LoginOff(m_UserName, m_UserPwd);

	ShowVerifyCodePic();
	GetDlgItem(IDC_EDT_LOGIN_YZ_CODE)->SetFocus();

	VMProtectEnd();
}


//���Ӳ���
void CLoginDlg::OnBnClickedBtnLoginConnectTest()
{
	if (g_BSPHP.AppEn_Internet() == "1")
		MessageBox("���ӷ������ɹ���", "���Ӳ���");
	else
		MessageBox("���ӷ�����ʧ�ܣ�", "���Ӳ���");
}
