
// ChinaSL.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "ChinaSLDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "Login/LoginDlg.h"//������û���¼�Ի���

#include "WDS_COMMON.h"

//���桢��ť��ʹ��ϵͳ���
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")

//---------------------���ø����汾������������------------------
// ��������Release�汾ʱ�����ɲ�ͬ���ֵĳ���
// ����Debug�汾����ʱ������TD.exe����ʱTD��Ҫ����Ŀ����-�������õ�һ��
// Ҫ����Release����Ҫ��NDEBUG��Ϊδ����,���ſ�#else
//#ifdef NDEBUG//if1
//#pragma warning(disable: 4070)

#if WDS_ADMIN//if1.1
//#pragma comment(linker, "/OUT:\"../Release/ChinaSL_Admin.exe\"")
#pragma comment(linker, "/OUT:\"../Release/ChinaSL.exe\"")
#else
#pragma comment(linker, "/OUT:\"../Release/������ʿ�.exe\"")
#endif//if1.1

//#else//if1
//����Releaseʱ�ſ�
//#pragma comment(linker, "/OUT:\"../Bin/TD.exe\"")
//#endif//if1

//BSPHPȫ�ֱ�����������ط���ʹ��ͬһ��������֤��ΨһWuXia.cpp��
CAppen g_BSPHP;

//�ؼ���֤����,ΨһVerificationData.cpp
extern CParseVerificationData g_VfcData;

//����dpi����
float g_dpi_rate = 1.0;

//------------------------------------------------------------------
// ����ע����е�ע���������ֵ
// pSaveKeyToReg---�����Ѿ��ȽϺ���Ч��ע����
bool CreateRegInfo(TCHAR *pSaveKeyToReg)
{
	HKEY phkResult;
	DWORD dwDisposition;// ����ֵ�������µĻ�򿪴��ڵ�REG_CREATED_NEW_KEY(1),REG_OPENED_EXISTING_KEY(2) 
						// ����ָ����ע����������Ѵ��ڣ���򿪡�
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
// ��ȡע����е�ע����
// keyInReg---�����ȡ��ע����
bool ReadRegInfo(TCHAR *keyInReg)
{
	DWORD dwtype, sizeBuff;
	HKEY phkResult;// �õ��Ľ�Ҫ�򿪼��ľ��
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


// CChinaSLApp ����

CChinaSLApp::CChinaSLApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CChinaSLApp ����

CChinaSLApp theApp;


// CChinaSLApp ��ʼ��

BOOL CChinaSLApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();


	AfxEnableControlContainer();

	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	//����ϵͳUI���ű���
	HDC hdc = ::GetDC(NULL);
	g_dpi_rate = GetDeviceCaps(hdc, LOGPIXELSX);
	//WDS_MessageBox(g_dpi_rate);
	g_dpi_rate /= 96;


	VMProtectBeginUltra("WDS_VMBEGIN OF g_BSPHP.init()");
#if !WDS_ADMIN
	//::PostThreadMessage(g_dwShowStateThreadID, UM_DLG_SHOW_STATE, (WPARAM)_T("�������������Ե�..."), 0);
	//��ʼ��ȫ�ֱ���BSPHP
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

	//��ȡBsphp��������֤����
	CString BsphpData = g_BSPHP.AppEn_GetVerification();
	//AfxMessageBox(BsphpData);
	//������֤����
	g_VfcData.ParseVerificationData(BsphpData);// _T(""), "vrfdata.xml");
#endif
	VMProtectEnd();




	CChinaSLDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}

