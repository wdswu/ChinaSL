
// ChinaSLDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "ChinaSLDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "WDS_COMMON.h"

#define UM_MAIN_DLG_END				WM_USER+104 //֪ͨ���Ի����˳�����Ϣ��BSP����߳�֪ͨ

CString g_strVer = "v18.01.24";

//��־�ļ�����Ψһ(ChinaSLDlg.CPP)
CString g_strLogName = "Log.txt";
//�����ļ�����Ψһ(ChinaSLDlg.CPP)
CString g_strCfgName = "CslConfig.dat";

//����dpi����
extern float g_dpi_rate;

//BSPHP��֤�̵߳��˳���־
bool g_bExitThreadBsphp = false;
//BSPHPȫ�ֱ�����������ط���ʹ��ͬһ��������֤��ȫ����Ψһ��
extern CAppen g_BSPHP;
//BSP��¼�û������룬�ڵ�¼ʱ��ȡ�������������ط�ʹ�ã�ȫ����Ψһ��
extern CString g_BspUser;
extern CString g_BspPwd;


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���


class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

	// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// CChinaSLDlg �Ի���



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


// CChinaSLDlg ��Ϣ�������


//----------------����û�BSP��̨�Ƿ��ڵ��߳�--------------------
UINT __cdecl ThreadCheckBSPHP(LPVOID param)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF ThreadCheckBSPHP()");

	//��ʾ1���˻���Ϣ
	//CString strUserInfo;
	//strUserInfo.Format(_T("��¼�û�: %s\r\n"), g_BspUser);
	//strUserInfo += g_BSPHP.AppEn_GetUserInfo("��¼��IP: UserLogInIp\r\n����ʱ��: UserVipDate");
	////PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)(LPCTSTR)strUserInfo, 0);
	//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, strUserInfo);


	//��ʾ���BSP��̨�Ĺ���
	//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, g_BSPHP.AppEn_GetGongGao());

	//----------------------------------------��ʼwhile��֤---------------------------------------------
	DWORD dwDelayMS;
	DWORD dwDelayCount;
	int iCountError = 0;
	while (!g_bExitThreadBsphp)
	{
		//----��֤��g_BSPHP�����Ƿ���Ч----
		if (g_BSPHP.Version == _T(""))
		{
			//���g_BSPHP�İ汾��Ϊ�գ���رճ���
			::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("��֤ʧ��"), 0);
			break;
		}

		//----��֤���Ƿ񱣳ֵ�¼״̬----
		CString strLogin = g_BSPHP.AppEn_GetLoginInfo();
		if (strLogin != _T("1"))
		{
			//�ۼ�ʧ�ܴ���
			iCountError++;
			//----������֤������״�����ã���س��򣬼��50����----
			if (iCountError > 10)
			{
				//�����δ��¼״̬����رճ���
				::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("��֤ʧ��"), 0);
				MessageBox(*(HWND*)param, strLogin, "", 0);
				break;
			}
			//CString strInfo;
			//strInfo.Format(_T("%s��%d/10�ε�¼A��֤ʧ�ܣ��������"), strLogin, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------���ʧ�ܴ���<10�����ȵȴ�5���ӣ��ټ��--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//��ʱ����
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			//�ٴε��ü��
			continue;
		}

		//========================��ȡ������״̬=============================
		CString Sult = g_BSPHP.AppEn_timeout();
		//AfxMessageBox(Sult);//*******���ﵽ�ں��ȻҲ�Ƿ���5031********
		//-------------------�������������ֵ--------------------
		if (Sult.Find("5031") > -1)//��������
		{
			iCountError = 0;
			//ÿ3Сʱ��ȡһ��BSP��̨�Ĺ���
			static DWORD startMs = GetTickCount();
			if (GetTickCount() - startMs > 3 * 60 * 60 * 1000)
			{
				startMs = GetTickCount();
				//SetDlgItemText(*(HWND*)param, IDC_EDT_BSP_GONGGAO, g_BSPHP.AppEn_GetGongGao());
			}

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)(LPCTSTR)_T("��������"), 0);
#else
			//-------------�ɹ�����ʱ10Сʱ���ټ��----------------
			dwDelayMS = 10 * 60 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			continue;
		}
		else if (Sult.Find("5030") > -1)//�����ʺŵ���
		{
			//�رճ���
			::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("�ʺŵ���"), 0);
			break;
		}

		//===========���¶Է�����2������������д���=============

		//----������֤������״�����ã���س��򣬼��50����----
		if (iCountError < 10)//����ʧ��
		{
			//�ۼ�ʧ�ܴ���
			iCountError++;

			//CString strInfo;
			//strInfo.Format(_T("%s��%d/10������ʧ�ܣ��������"), Sult, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------���ʧ�ܴ���<10�����ȵȴ�5���ӣ��ټ��--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//��ʱ����
			for (DWORD i = 0; i < dwDelayCount && !g_bExitThreadBsphp; i++)
			{
				Sleep(500);
			}

			//�ٴε��ü��
			continue;
		}

		//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)_T("��֤ʧ�ܣ��������"), 0);
		AfxMessageBox(_T("��֤ʧ�ܣ��������"));
		//�رճ���
		::SendMessage(*(HWND*)param, UM_MAIN_DLG_END, (WPARAM)_T("��֤ʧ��"), 0);

		break;
	}//end while

	 //PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T(".....................��֤Trd����....................."), 0);

	VMProtectEnd();

	return 0;
}

BOOL CChinaSLDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	CString strDebug = _T("������ʿ�") + g_strVer + " By ��Ѫ���";
	CString strRelease = _T("������ʿ�") + g_strVer + " By ��Ѫ���";
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


	//��ʼ���������ؼ�λ��
	InitAllControl();
	SetControlRect();

#if !WDS_ADMIN
	g_bExitThreadBsphp = false;
	//���ü��BSP��̨��֤���̣߳�������ʱҪС�ڷ�������30����
	m_pBsphpThread = AfxBeginThread(ThreadCheckBSPHP, &m_hWnd);
	//���ü��BSP��̨��֤�Ķ�ʱ�������ʱ��Ҫ���ڼ�����ʱ���ɵ��ڷ�������30����-�Ѹ�Ϊ9Сʱ
	SetTimer(1, 9 * 60 * 60 * 1000, NULL);
#endif


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CChinaSLDlg::OnPaint()
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
HCURSOR CChinaSLDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BOOL CChinaSLDlg::PreTranslateMessage(MSG* pMsg)
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

HBRUSH CChinaSLDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//TCHAR str[260];
	//GetClassName(pWnd->m_hWnd, str, 260);// ȡ�ÿؼ�������
	//CString ClassName = _T("Edit");// �༭����
	////UINT id = pWnd->GetDlgCtrlID();// �õ��ؼ�ID��
	////if (ClassName != str)
	//{//���༭���⣬�����ܱ�Ķ���ɫ
	//	pDC->SetTextColor(RGB(255, 255, 255)); //����������ɫ
	//	//pDC->SelectObject(&m_Font); //����������ʽ

	//	//���ñ���ɫ
	//	pDC->SetBkMode(TRANSPARENT);
	//	//hbr = CreateSolidBrush(RGB(0, 120, 215));//��ɫ
	//	hbr = CreateSolidBrush(RGB(0x6C, 0x6F, 0x85));//ɫ
	//}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CChinaSLDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ

	CDialogEx::OnClose();
}

void CChinaSLDlg::InitAllControl()
{
	//��һҳTAB
	m_MainTab.InsertItem(0, _T("���ʱ�"));
	//���öԻ���ĸ�����Ϊ��TAB�ؼ� //GetDlgItem(IDC_TAB_MAIN));
	m_Dlg_cslPeilv.Create(IDD_DLG_CSL_PEILV, &m_MainTab);
#if WDS_ADMIN
	m_MainTab.InsertItem(1, _T("���ݿ�"));
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

	//��ȡtab������
	CRect tabRect;
	GetClientRect(&tabRect);
	//tabRect.left *= g_dpi_rate;//=0
	//tabRect.top *= g_dpi_rate;//=0
	//tabRect.right *= g_dpi_rate;
	//tabRect.bottom *= g_dpi_rate;
	m_MainTab.MoveWindow(&tabRect);

	//����tab��ǩ�Ŀ��
	CSize size;
	//size.cx *= g_dpi_rate;//=0
	size.cy = 28 * g_dpi_rate;
	m_MainTab.SetItemSize(size);
	m_MainTab.SetItemState(0, TCM_SETITEM, TCIS_HIGHLIGHTED);

	//���ø����ӶԻ���Ĵ����С��Χ���Ա�����tab�ı�ǩ�����߿�
	//tabRect.left *= g_dpi_rate;;//TAB��ǩ�����=0
	tabRect.top += size.cy;//TAB��ǩ�ĸ߶�0+=
	//tabRect.right /= g_dpi_rate;;//TAB��ǩ���ұ�
	//tabRect.bottom /= g_dpi_rate;;//TAB��ǩ�ĵױ�

	//���öԻ����СΪTABControl��С��ȥ��ǩ
	m_Dlg_cslPeilv.MoveWindow(&tabRect);
	m_Dlg_cslPeilv.ShowWindow(TRUE);

#if WDS_ADMIN
	m_reptileDlg.MoveWindow(&tabRect);
#endif

	tabRect.top -= size.cy;//List���Ҫ��ȥTAB��ǩ�ĸ߶�
	//tabRect.right *= g_dpi_rate;
	tabRect.bottom -= (115 + 60) * g_dpi_rate*1.1;//List�ĵױ�Ҫ��������������ť��λ��
	m_Dlg_cslPeilv.m_listCtrl.MoveWindow(&tabRect);
}

void CChinaSLDlg::OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	m_Dlg_cslPeilv.ShowWindow(FALSE);
#if WDS_ADMIN
	m_reptileDlg.ShowWindow(FALSE);
	m_MainTab.SetItemState(1, TCM_SETITEM, TCIS_BUTTONPRESSED);
#endif

	//����TAB��ǩ����
	m_MainTab.SetItemState(0, TCM_SETITEM, TCIS_BUTTONPRESSED);

	int iSelect = m_MainTab.GetCurSel();
	//����TAB��ǩ����
	m_MainTab.SetItemState(iSelect, TCM_SETITEM, TCIS_HIGHLIGHTED);
	CRect rect, rect2;
	switch (iSelect)
	{
	case 0://���ʿ�
		SetWindowPos(0, 0, 0, (1336 + 212 + 212) *  g_dpi_rate, (695 + 50) *  g_dpi_rate, SWP_NOMOVE);
		m_Dlg_cslPeilv.ShowWindow(TRUE);
		break;
#if WDS_ADMIN
	case 1://������
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

	// TODO: �ڴ˴������Ϣ����������
	//--------������֤�߳�--------
	g_bExitThreadBsphp = true;
	//if (m_pBsphpThread)
	{
		//DWORD dwRet = WaitForSingleObject(m_pBsphpThread->m_hThread, 5000);
	}
	//�ߵ���ǰ��¼���ʺ�
	g_BSPHP.AppEn_LoginOff(g_BspUser, g_BspPwd);
}
//----------------��Ӧ�Ի��������Ϣ����Ҫ����BSPHP��֤----------------
afx_msg LRESULT CChinaSLDlg::OnMainDialogEnd(WPARAM wParam, LPARAM lParam)
{
	g_bExitThreadBsphp = true;

	VMProtectBeginUltra("WDS_VMBEGIN OF CWuXiaDlg::OnMainDialogEnd()");

	//�ߵ�ԭ�ȵ�¼���ʺ�
	g_BSPHP.AppEn_LoginOff(g_BspUser, g_BspPwd);

	EndDialog(8887);
	//MessageBox("EndDialog()");
	if ((LPCTSTR)wParam == _T("�ʺŵ���"))
	{
		MessageBox(_T("���ʺ��ѹ��ڣ��������Զ��رա������ѣ�"));
	}
	else if ((LPCTSTR)wParam == _T("��֤ʧ��"))
	{
		MessageBox(_T("������֤ʧ�ܣ����Զ��رա�����ϵ����Ա��"));
	}
	VMProtectEnd();
	return 0;
}

void CChinaSLDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	int iCountError = 0;
	switch (nIDEvent)
	{
	case 1:
		DWORD dwDelayMS;
		DWORD dwDelayCount;
		while (!g_bExitThreadBsphp)
		{
			//----BSPHP��֤���Ƿ񱣳ֵ�¼״̬----
			//��ȡ��ǰ�ʺ�״̬��Ϣ,�ж��Ƿ��¼:��½״̬����1 ���򷵻�״̬��Ϣ
			CString strLogin = g_BSPHP.AppEn_GetLoginInfo();
			if (strLogin == _T("1"))//�ɹ���¼
			{
				iCountError = 0;
				break;//while
			}
			//δ�ɹ���¼�����ۼ�ʧ�ܴ���
			iCountError++;
			if (iCountError > 10)
			{
				//���ʧ�ܴ���>10����رճ���
				SendMessage(UM_MAIN_DLG_END, (WPARAM)_T("��֤ʧ��"), 0);
				//MessageBox(strLogin);
				break;
			}
			//CString strInfo;
			//strInfo.Format(_T("%s��%d/10�ε�¼B��֤ʧ�ܣ��������"), strLogin, iCountError);
			//PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE_ERROR, (WPARAM)(LPCTSTR)strInfo, 0);

#if WDS_PROGRAM_TEST
			dwDelayMS = 2 * 1000;
#else
			//---------���ʧ�ܴ���<10�����ȵȴ�2���ӣ��ټ�⣬����ʱ����>SetTimer��ʱ��--------
			dwDelayMS = 5 * 60 * 1000;
#endif
			dwDelayCount = dwDelayMS / 500;//��ʱ����
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
