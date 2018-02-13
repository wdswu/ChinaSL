// ReptileDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "ReptileDlg.h"
#include "afxdialogex.h"

#include "WDS_COMMON.h"
#include "ReptileFun.h"

#include "luaCfg.h"

#include ".\json\reader.h"
#pragma comment(lib,"lib_json.lib")

//��־�ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strLogName;

//�����ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strCfgName;

//����ҳ��ʱ
DWORD g_httpCostMS = 0;
//CInternetSession g_session("HttpClient");


HWND g_hwndDlg;

DWORD g_dwStartID;
DWORD g_dwEndID;


//�����ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strCfgName;
//�������������ļ�
void CreateUiCfgFile()
{
	CConfigIni cfgIni(g_strCfgName);

	// ���ļ����رգ����ڲ����ļ��Ƿ����
	HANDLE  File = CreateFile(
		cfgIni.m_FilePath,
		0,// �˲���Ϊ0��������ļ��󣬽��ͷ�
		FILE_SHARE_READ, NULL,
		OPEN_EXISTING, // �����ļ��Ƿ����
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (File == INVALID_HANDLE_VALUE)
	{	// ����ļ������ڣ��򴴽�һ�������ļ�
		cfgIni.WriteIniString("ADOConnect", "server", "localhost");
		cfgIni.WriteIniString("ADOConnect", "database", "ChinaSL");
		cfgIni.WriteIniString("ADOConnect", "uid", "000");
		cfgIni.WriteIniString("ADOConnect", "pwd", "000");
	}
}



// CReptileDlg �Ի���

IMPLEMENT_DYNAMIC(CReptileDlg, CDialogEx)

CReptileDlg::CReptileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_REPTILE, pParent)
{

}

CReptileDlg::~CReptileDlg()
{
}

void CReptileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CReptileDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_REPTILE_DATA, &CReptileDlg::OnBnClickedBtnReptileData)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BTN_REFRESH_ID, &CReptileDlg::OnBnClickedBtnRefreshId)
	ON_BN_CLICKED(IDC_BTN_STOP_REPTILE_DATA, &CReptileDlg::OnBnClickedBtnStopReptileData)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_CLEAR_REPTILE_MSG, &CReptileDlg::OnBnClickedBtnClearReptileMsg)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BTN_SAVE_SERVER_CFG, &CReptileDlg::OnBnClickedBtnSaveServerCfg)
	ON_BN_CLICKED(IDC_BTN_GET_VALUE_GATHER, &CReptileDlg::OnBnClickedBtnGetValueGather)
	ON_BN_CLICKED(IDC_BTN_UPDATE_VALUE, &CReptileDlg::OnBnClickedBtnUpdateValue)
	ON_BN_CLICKED(IDC_BTN_REG_SOFTWARE, &CReptileDlg::OnBnClickedBtnRegSoftware)
	ON_BN_CLICKED(IDC_BTN_BAD_RECORD, &CReptileDlg::OnBnClickedBtnBadRecord)
END_MESSAGE_MAP()


// CReptileDlg ��Ϣ�������
HWND g_hMsgEdit;
BOOL CReptileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	g_hwndDlg = GetSafeHwnd();

	//�����ı�������ַ�����
	((CEdit*)GetDlgItem(IDC_EDT_REPTILE_MSG))->SetLimitText(UINT_MAX);

	g_hMsgEdit = ::GetDlgItem(g_hwndDlg, IDC_EDT_REPTILE_MSG);
	ASSERT(g_hMsgEdit);

	//��ȡ������ҳ��lua����
	ReadLuaReptileCfg();

	//û�������ļ��򴴽�
	CreateUiCfgFile();
	//��ȡ�����ļ�����ʾ
	CConfigIni cfgIni(g_strCfgName);
	TCHAR server[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "server", server);
	TCHAR database[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "database", database);
	TCHAR uid[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "uid", uid);
	TCHAR pwd[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "pwd", pwd);

	//��ȡ�����ַ�
	//CString str11 = EncryptPsw(pwd);
	//MessageBox(str11);

	SetDlgItemText(IDC_EDT_SERVER_IP, server);
	SetDlgItemText(IDC_EDT_DATABASE_NAME, database);
	SetDlgItemText(IDC_EDT_DATABASE_USER, uid);
	SetDlgItemText(IDC_EDT_DATABASE_PWD, pwd);

#if WDS_ADMIN	
	VMProtectBeginUltra("WDS_VMBEGIN OF CReptileDlg::OnInitDialog");
	//------����ע����------���ڿ�ר���������ڣ�ע������ת������
	// ��ȡע����д��KEY��δ������ֱ����ת�������������У�
	// ��ȡ��������㱾��Ӳ����Ϣ������key�����ȡ�ĶԱȣ���ͬ����������
	// ��ͬ�����������С�
	CString machine_code = CalculateMachineCode();//����õ�������
	SetDlgItemText(IDC_EDT_MAC_CODE, machine_code);

	TCHAR keyInReg[100] = { 0 };
	bool bReg = ReadRegInfo(keyInReg);
	if (!bReg)
	{
		MessageBox(TEXT("����δע�������������"));
		//���ù��ܰ�ť
		GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
		GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);
	}
	else
	{
		//��ȡע����е���Ȩkey
		GetDlgItem(IDC_EDT_REG_CODE)->SetWindowText(keyInReg);
		//���ݻ����������ȨKey
		CString license_key = CalculateLicenseKey(machine_code);
		//�ԱȻ�ȡ��ע�����Ȩ�ͼ������Ȩ
		if (keyInReg == license_key)
		{
			//OnBnClickedBtnReadFile();
			//MessageBox(TEXT("ע��ɹ�"));
		}
		else
		{
			MessageBox(TEXT("ע���벻��ȷ��������ע��"));
			GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
			GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);
		}
	}

	VMProtectEnd();
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
					  // �쳣: OCX ����ҳӦ���� FALSE
}

void CReptileDlg::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);
	// TODO: �ڴ˴������Ϣ����������

	//static bool flag = true;
	//if (flag)
	//{
	//	flag = false;
	//	OnBnClickedBtnRefreshId();
	//}

	// ��Ϊ��ͼ��Ϣ���� CDialogEx::OnPaint()
}


void CReptileDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: �ڴ˴������Ϣ����������
	m_ThreadData.beExitThread = true;
	Sleep(100);

}
//�����ı����༭��ĩβ
void AddMsgEditCtrlString(CString lpStr, bool bLinefeed = true)
{
	static int i = 0;
	i++;
	if (i == 5000)
	{
		i = 0;
		SendMessage(g_hMsgEdit, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"");
	}
	//SDK�汾
	//HWND hEdit = GetDlgItem(g_hwndDlg, IDC_EDT_REPTILE_MSG);
	//ASSERT(hEdit);
	if (bLinefeed)
	{
		SendMessage(g_hMsgEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
		SendMessage(g_hMsgEdit, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)(lpStr + "\r\n"));
	}
	else
	{
		SendMessage(g_hMsgEdit, EM_SETSEL, LOWORD(-1), HIWORD(-1));
		SendMessage(g_hMsgEdit, EM_REPLACESEL, 0, (LPARAM)(LPCTSTR)(lpStr));
	}

	//MFC�汾--����mfc����
	//CEdit *pEdit= GetDlgItem(g_hwndDlg, IDC_EDT_REPTILE_MSG);
	//ASSERT(pEdit&&pEdit->GetSafeHwnd());
	//pEdit->SetSel(-1, -1);
	//pEdit->ReplaceSel(lpStr);

	CStdioFile file;
	if (file.Open(g_strLogName, CFile::modeCreate | CFile::modeNoTruncate/*������ɾ������*/ | CFile::modeWrite))
	{
		file.SeekToEnd();
		file.WriteString(lpStr);
	}
	file.Close();
}
//ˢ��ID
void CReptileDlg::OnBnClickedBtnRefreshId()
{
	GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(0);

	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(1);
		return;
	}

	//VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnRefreshId");
	//-----�����ݿ��ȡ����ID-100��Ϊ��ʼID------
	CString strSQL = "select max(id) as id from PeiLv_Info";
	try
	{
		cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cslAdo.dump_com_error(e);
		SetDlgItemInt(IDC_EDT_reptile_START_ID, 0);
		SetDlgItemInt(IDC_EDT_reptile_END_ID, 0);
		g_dwStartID = 0;
		g_dwEndID = 0;
		GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(1);
		return;
	}
	if (cslAdo.m_pRecordset->adoBOF)//������ݿⲻ���ڴ˼�¼
	{
		g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
		g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	}
	else
	{
		_variant_t TheValue;//
		try
		{
			//VARIANT��������,�õ��ֶε�ֵ
			TheValue = cslAdo.m_pRecordset->GetCollect(_T("id"));
		}
		catch (_com_error e)
		{
			cslAdo.dump_com_error(e);
		}

		//��ȡ���ݿ�����һ��ID
		//g_dwStartID = atoi((LPCTSTR)(TheValue.bstrVal)) + 1;
		g_dwStartID = TheValue.intVal - 100;
		if (g_dwStartID >= 1305562737)
		{
			g_dwStartID = 1;
		}
		g_dwEndID = GetWebMaxID();// TheValue.intVal + 50;

	}

	//VMProtectEnd();

	SetDlgItemInt(IDC_EDT_reptile_START_ID, g_dwStartID);
	SetDlgItemInt(IDC_EDT_reptile_END_ID, g_dwEndID);

	cslAdo.ExitADOConnect();

	GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(1);
}

void UTF_8ToUnicode(wchar_t* pOut, char *pText)
{
	char* uchar = (char *)pOut;
	uchar[1] = ((pText[0] & 0x0F) << 4) + ((pText[1] >> 2) & 0x0F);
	uchar[0] = ((pText[1] & 0x03) << 6) + (pText[2] & 0x3F);
}
void Gb2312ToUnicode(wchar_t* pOut, char *gbBuffer)
{
	::MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, gbBuffer, 2, pOut, 1);
}
void UnicodeToGB2312(char* pOut, wchar_t uData)
{
	WideCharToMultiByte(CP_ACP, NULL, &uData, 1, pOut, sizeof(wchar_t), NULL, NULL);
}
void UTF_8ToGB2312(char*pOut, char *pText, int pLen)
{
	char Ctemp[4];
	memset(Ctemp, 0, 4);
	int i = 0, j = 0;
	while (i < pLen)
	{
		if (pText[i] >= 0)
		{
			pOut[j++] = pText[i++];
		}
		else
		{
			WCHAR Wtemp;
			UTF_8ToUnicode(&Wtemp, pText + i);
			UnicodeToGB2312(Ctemp, Wtemp);
			pOut[j] = Ctemp[0];
			pOut[j + 1] = Ctemp[1];
			i += 3;
			j += 2;
		}
	}
	pOut[j] = '\n';
	return;
}

#define MAXBLOCKSIZE 1024+1
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")
//winnet��ȡ��ҳ
bool GetWebSrcCode(CString* strBuffer, CString Url)
{
	HINTERNET hSession = InternetOpen("GetWebSrcCode", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession == NULL)
	{
		AddMsgEditCtrlString("InternetOpen return NULL");
		return false;
	}
	HINTERNET hURL = InternetOpenUrl(hSession, Url, NULL, 0, INTERNET_FLAG_DONT_CACHE | INTERNET_FLAG_PRAGMA_NOCACHE | INTERNET_FLAG_RELOAD, 0);// 
	if (hURL == NULL)
	{
		AddMsgEditCtrlString("InternetOpenUrl return NULL");
		return false;
	}

	char Temp[MAXBLOCKSIZE] = { 0 };
	ULONG Number = 0;

	//int timeout = 3000; //3s
	//int i = InternetSetOption(hSession, INTERNET_OPTION_RECEIVE_TIMEOUT, (char*)&timeout, sizeof(timeout));
	//CString str;
	int n = 0;

	DWORD StartMS1 = GetTickCount();
	while (1)
	{
		n++;
		ZeroMemory(Temp, MAXBLOCKSIZE);

		DWORD StartMS1 = GetTickCount();

		//Number = 0;
		InternetReadFile(hURL, Temp, MAXBLOCKSIZE - 1, &Number);

		DWORD startMS2 = (GetTickCount() - StartMS1) / 1000;
		CString strMsg;
		strMsg.Format("%d:%d:%d\t", n, startMS2, strlen(Temp));
		AddMsgEditCtrlString(strMsg, false);

		if (Number > 0)
		{
			*strBuffer += Temp;
		}
		else
		{
			//����ʱʱ�䵽���recv�᷵�ش���Ҳ����-1������ʱ�Ĵ�������EAGAIN����EWOULDBLOCK��POSIX.1-2001��������������һ�����ֶ��У����Խ������жϴ�������������д�ϡ�
			//���socket�Ǳ��Է���lingerΪ0����ʽ�ص���Ҳ����ֱ�ӷ�RST�ķ�ʽ�رյ�ʱ��recvҲ�᷵�ش��󣬴�������ENOENT
			//����һ�־����ڴ����г����Ĵ����룬�Ǿ���EINTER����˼��ϵͳ�ڽ��յ�ʱ����Ϊ�յ������ж��źŶ����ȷ��أ�����socket���ϣ�Ӧ�ü������ա�
			//if (/*(Number < 0) &&*/ (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
			//{
			//	continue;//������������
			//}

			break;
		}
	}

	DWORD startMS2 = (GetTickCount() - StartMS1) / 1000;
	CString strMsg;
	strMsg.Format("\r\nReadFile:-----%d-----��  %d��  size:%dbyte", startMS2, n, strBuffer->GetLength());
	AddMsgEditCtrlString(strMsg);

	InternetCloseHandle(hURL);
	hURL = NULL;
	InternetCloseHandle(hSession);
	hSession = NULL;

	return true;
}

//#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")
//socket��ȡ��ҳ
bool SocketGetWebSrcCode(CString* strBuffer, CString Url)
{
	// ��ʼ��WSA
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		AddMsgEditCtrlString("��ʼ��SOCKET����");
		return false;
	}
	//�����׽���
	SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hsocket == INVALID_SOCKET)
	{
		//int error = WSAGetLastError();
		AddMsgEditCtrlString("����socket error");
		WSACleanup();
		return false;
	}

	//��IP���˿�
	int i = Url.Find("/");
	CString strHostName = Url.Mid(0, i);
	CString strRequestUrl = Url.Mid(i, Url.GetLength());
	LPHOSTENT lphostent = gethostbyname(strHostName);//"i.sporttery.cn";
	if (lphostent == NULL)
	{
		AddMsgEditCtrlString("������Ϊ�գ�");
		WSACleanup();
		return false;
	}
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	//saServer.sin_addr.S_un.S_addr = INADDR_ANY;//�������
	saServer.sin_addr = *((LPIN_ADDR)*lphostent->h_addr_list);
	saServer.sin_port = htons(80);

	// ����SOCKET����
	int nRet = connect(hsocket, (LPSOCKADDR)&saServer, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("��������ʱ����");
		closesocket(hsocket);
		WSACleanup();
		return false;
	}

	//Ĭ��send��recv�������ģ��������ó�ʱ�򷵻�
	//int timeout = 3000; //3s
	//int ret = setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

	CString req =
		"GET " + strRequestUrl + " HTTP/1.1\r\n"
		"HOST:" + strHostName + "\r\n"
		"User-Agent: Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR //2.0.50727; .NET4.0C; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\n"
		"Connection: close"//
		"\r\n\r\n";

	// ����SOCKET����
	nRet = send(hsocket, req, strlen(req), 0);
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("�������ݰ�ʱ����");
		closesocket(hsocket);
		WSACleanup();
		return false;
	}

	// û�����ݷ��ͺ󣬾͹ر����ӣ���֮����ִ�йرյĲ��������ǲ����ͷ�socket��Դ
	// ����SD_SEND����ر�send�������Ѿ����뻺���������ݻᱻЭ�鷢�ͣ�ֻ�Ǻ��治����send�ˡ�
	// ����ر���Ϊ�˽������recvʱ�����һ�����ݵȴ�̫�ã���ԭ����
	//nRet = shutdown(hsocket, SD_SEND);
	//if (nRet == SOCKET_ERROR)
	//{
	//	AddMsgEditCtrlString("�ر����ӳ���");
	//	closesocket(hsocket);
	//	WSACleanup();
	//	return false;
	//}

	//int n = 0;

	char Dest[MAXBLOCKSIZE] = { 0 };

	//DWORD StartMS1 = GetTickCount();

	//���մ���������:
	//1�����ʹ��Keep-Alive,�����С�Ĵ�����գ��������ߣ����ղ�����ͷ��Ȼ�����ʵ�壬�ڽ���ͷ�����У�Ҫ������ʵ�����ݵĳ��ȣ�Content-Length ��chunked��,Ȼ����վ�ȷ���ȵ��ֽ�����Ȼ��go on...
	//2������ʹ��"Connection: close"����ҲӦ�ð���header+body�����ݽ���+ͬ������������һ���򵥵�ѭ��recv,�������˿�������
	do {
		//n++;
		//DWORD StartMS3 = GetTickCount();

		// ���շ������ݰ�
		nRet = recv(hsocket, (LPSTR)Dest, sizeof(Dest) - 1, 0);

		//DWORD startMS4 = (GetTickCount() - StartMS3) / 1000;
		//CString strMsg;
		//strMsg.Format("recv��%d��\t��ʱ%d��\t��С%d byte\r\n", n, startMS4, strlen(Dest));
		//AddMsgEditCtrlString(strMsg);

		if (nRet > 0)
		{
			Dest[nRet] = 0;
			//char sDest[MAXBLOCKSIZE] = { 0 };
			//UTF_8ToGB2312(sDest, Dest, nRet);
			*strBuffer += Dest;
		}
		//else
		//	Dest[0] = 0;
	} while (nRet > 0);

	//DWORD startMS2 = (GetTickCount() - StartMS1) / 1000;
	//CString strMsg;
	//strMsg.Format("recv():��-----%d-----��  %d��  size:%dbyte\r\n", startMS2, n, strlen(*strBuffer));
	//AddMsgEditCtrlString(strMsg);

	//ȥ��http���ص�ͷ����ͷ����β��"\r\n\r\n"��
	i = strBuffer->Find("\r\n\r\n");
	int iStart = i + strlen("\r\n\r\n");
	int iEnd = strBuffer->GetLength();
	*strBuffer = strBuffer->Mid(iStart, iEnd);

	//�رղ��ͷ���Դ
	nRet = closesocket(hsocket);
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("�ر�socket����");
	}
	WSACleanup();

	return true;
}
//CHttpFile��ȡ��ҳ
//bool MFCGetWebSrcCode()

//����json��������
bool ParseJsonFromString_SaiGuo(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader��Json�ַ���������root��root������Json��������Ԫ��
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------����json����--------------------------------
		int code = root["status"]["code"].asInt();//���ʳɹ���message=="",����code="0"
		//int bRet = strcmp(msg.c_str(), "");
		if (code != 0)
			return false;

		string strSPF;
		string strRangFen;
		string strRangQiuSPF;
		string strBiFen;
		string strZongJQS;
		string strQBCSPF;
		int nnn = root["result"]["pool_rs"].size();
		if (nnn > 0)
		{
			bool bMember = root["result"]["pool_rs"].isMember("had");
			if (bMember)//�ж��Ƿ��ǳ�Ա
			{
				strSPF = root["result"]["pool_rs"]["had"]["prs_name"].asString();//����
			}
			stRecord->strSPF = strSPF.c_str();//����
			if (stRecord->strSPF == "")
				return false;

			if (root["result"]["pool_rs"].isMember("hhad"))
			{
				strRangFen = root["result"]["pool_rs"]["hhad"]["goalline"].asString();//�÷�
				strRangQiuSPF = root["result"]["pool_rs"]["hhad"]["prs_name"].asString();//����
			}
			if (root["result"]["pool_rs"].isMember("crs"))
			{
				strBiFen = root["result"]["pool_rs"]["crs"]["prs_name"].asString();//ȫ���ȷ�
			}
			if (root["result"]["pool_rs"].isMember("ttg"))
			{
				strZongJQS = root["result"]["pool_rs"]["ttg"]["prs_name"].asString();//����
			}
			if (root["result"]["pool_rs"].isMember("hafu"))
			{
				strQBCSPF = root["result"]["pool_rs"]["hafu"]["prs_name"].asString();//��ȫ��ʤƽ��
			}
		}
		else
			return false;

		string strFBPLB_S, strFBPLB_P, strFBPLB_F;//����ֵS/P/F
		string strZZPLB_S, strZZPLB_P, strZZPLB_F;//ĩ��ֵS/P/F
		string strRQFBPLB_S, strRQFBPLB_P, strRQFBPLB_F;//�ó���ֵS/P/F		
		string strRQZZPLB_S, strRQZZPLB_P, strRQZZPLB_F;//��ĩ��ֵS/P/F
		nnn = root["result"]["odds_list"].size();
		if (nnn > 0)
		{
			int num;
			if (root["result"]["odds_list"].isMember("had"))
			{
				num = root["result"]["odds_list"]["had"]["odds"].size();//���������ȡͷ��β
				if (num > 0)
				{
					//����ֵS/P/F
					strFBPLB_S = root["result"]["odds_list"]["had"]["odds"][0]["h"].asString();
					strFBPLB_P = root["result"]["odds_list"]["had"]["odds"][0]["d"].asString();
					strFBPLB_F = root["result"]["odds_list"]["had"]["odds"][0]["a"].asString();
					if (num > 1)
					{
						//ĩ��ֵS/P/F
						strZZPLB_S = root["result"]["odds_list"]["had"]["odds"][num - 1]["h"].asString();
						strZZPLB_P = root["result"]["odds_list"]["had"]["odds"][num - 1]["d"].asString();
						strZZPLB_F = root["result"]["odds_list"]["had"]["odds"][num - 1]["a"].asString();
					}
				}
			}

			if (root["result"]["odds_list"].isMember("hhad"))
			{
				num = root["result"]["odds_list"]["hhad"]["odds"].size();//���������ȡͷ��β
				if (num > 0)
				{
					//�ó���ֵS/P/F
					strRQFBPLB_S = root["result"]["odds_list"]["hhad"]["odds"][0]["h"].asString();
					strRQFBPLB_P = root["result"]["odds_list"]["hhad"]["odds"][0]["d"].asString();
					strRQFBPLB_F = root["result"]["odds_list"]["hhad"]["odds"][0]["a"].asString();
					if (num > 1)
					{
						//��ĩ��ֵS/P/F
						strRQZZPLB_S = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["h"].asString();
						strRQZZPLB_P = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["d"].asString();
						strRQZZPLB_F = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["a"].asString();
					}
				}
			}
		}
		else
			return false;

		//-----------------------------ת������--------------------------------
		stRecord->strRangFen = strRangFen.c_str();//�÷�
		stRecord->strRangFen = "(" + stRecord->strRangFen + ")";

		stRecord->strRangQiuSPF = strRangQiuSPF.c_str();//����
		stRecord->strBiFen = strBiFen.c_str();//ȫ���ȷ�
		stRecord->strZongJQS = strZongJQS.c_str();//����
		stRecord->strQBCSPF = strQBCSPF.c_str();//��ȫ��ʤƽ��

		//����ֵS/P/F
		stRecord->strFBPLB_S = strFBPLB_S.c_str();
		stRecord->strFBPLB_P = strFBPLB_P.c_str();
		stRecord->strFBPLB_F = strFBPLB_F.c_str();
		int S = atoi(stRecord->strFBPLB_S);//����
		int P = atoi(stRecord->strFBPLB_P);
		int F = atoi(stRecord->strFBPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			return false;
		}
		stRecord->strChuPei.Format("%d%d%d", S, P, F);

		//ĩ��ֵS/P/F
		stRecord->strZZPLB_S = strZZPLB_S.c_str();
		stRecord->strZZPLB_P = strZZPLB_P.c_str();
		stRecord->strZZPLB_F = strZZPLB_F.c_str();
		S = atoi(stRecord->strZZPLB_S);//����
		P = atoi(stRecord->strZZPLB_P);
		F = atoi(stRecord->strZZPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			stRecord->strMoPei = "";
		}
		else
		{
			stRecord->strMoPei.Format("%d%d%d", S, P, F);
		}

		//�ó���ֵS/P/F
		stRecord->strRQFBPLB_S = strRQFBPLB_S.c_str();
		stRecord->strRQFBPLB_P = strRQFBPLB_P.c_str();
		stRecord->strRQFBPLB_F = strRQFBPLB_F.c_str();
		S = atoi(stRecord->strRQFBPLB_S);//�������򷢲�����--�ó���
		P = atoi(stRecord->strRQFBPLB_P);
		F = atoi(stRecord->strRQFBPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			return false;
		}
		stRecord->strRangChuPei.Format("%d%d%d", S, P, F);

		//��ĩ��ֵS/P/F
		stRecord->strRQZZPLB_S = strRQZZPLB_S.c_str();
		stRecord->strRQZZPLB_P = strRQZZPLB_P.c_str();
		stRecord->strRQZZPLB_F = strRQZZPLB_F.c_str();
		S = atoi(stRecord->strRQZZPLB_S);//������ĩ��
		P = atoi(stRecord->strRQZZPLB_P);
		F = atoi(stRecord->strRQZZPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			stRecord->strRangMoPei = "";
		}
		else
		{
			stRecord->strRangMoPei.Format("%d%d%d", S, P, F);
		}
	}
	else
	{
		return false;
	}
	return true;
}
//����json��������
bool ParseJsonFromString_SaiShi(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader��Json�ַ���������root��root������Json��������Ԫ��
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------����json����--------------------------------
		string msg = root["status"]["message"].asString();//���ʳɹ���message=="",����code="0"
		int bRet = strcmp(msg.c_str(), "");
		if (bRet)
			return false;

		if (root["result"].size() > 0)
		{
			string strRiQi = root["result"]["date_cn"].asString();//����
			string strSaiShi = root["result"]["l_cn"].asString();//����ȫ��
			string strZhuDui = root["result"]["h_cn"].asString();//����
			string strKeDui = root["result"]["a_cn"].asString();//�Ͷ�

			//-----------------------------ת������--------------------------------
			stRecord->strRiQi = strRiQi.c_str();
			stRecord->strSaiShi = strSaiShi.c_str();
			stRecord->strZhuDui = strZhuDui.c_str();
			stRecord->strKeDui = strKeDui.c_str();
		}
	}
	else
	{
		return false;
	}
}
//����json��������
bool ParseJsonFromString_YaPan(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader��Json�ַ���������root��root������Json��������Ԫ��
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------����json����--------------------------------
		string msg = root["status"]["message"].asString();//���ʳɹ���message=="",����code="0"
		int bRet = strcmp(msg.c_str(), "");
		if (bRet)
			return false;

		if (root["result"].size() > 0)
		{
			int num = root["result"]["data"].size();
			for (size_t i = 0; i < num; i++)
			{
				if (root["result"]["data"][i].isMember("id"))
				{
					string id = root["result"]["data"][i]["id"].asString();
					if (id == "229")//�ҵ�����ID
					{
						//����
						string strChuZSW = root["result"]["data"][i]["o1_s"].asString();
						string strChuYaPan = root["result"]["data"][i]["o3_s"].asString();
						string strChuKSW = root["result"]["data"][i]["o2_s"].asString();
						//��ʱ��
						string strJsZSW = root["result"]["data"][i]["o1"].asString();
						string strJsYaPan = root["result"]["data"][i]["o3"].asString();
						string strJsKSW = root["result"]["data"][i]["o2"].asString();

						//-----------------------------ת������-------------------------------- 
						//---��������Ϣ---
						stRecord->strChuZSW = strChuZSW.c_str();//������ˮ
						stRecord->strChuYaPan = strChuYaPan.c_str();//��������
						stRecord->strChuKSW = strChuKSW.c_str();//���̿�ˮ

						//---��������Ϣ---
						stRecord->strJsZSW = strJsZSW.c_str();//��ʱ����ˮ
						stRecord->strJsYaPan = strJsYaPan.c_str();//��ʱ������
						stRecord->strJsKSW = strJsKSW.c_str();//��ʱ�̿�ˮ

						return true;
					}
				}
			}
		}
	}

	return false;
}
//����web������Ϣ
bool Analysis_SaiGuo(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString SaiGuoUrl;
	SaiGuoUrl.Format("%s%d", g_SaiGuoUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//SaiGuoUrl = "http://" + SaiGuoUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, SaiGuoUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, SaiGuoUrl);

	//�������ҳ��ʱ
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//����json����
		return ParseJsonFromString_SaiGuo(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//����web������Ϣ
bool Analysis_SaiShi(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString SaiShiUrl;
	SaiShiUrl.Format("%s%d", g_SaiShiUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//SaiShiUrl = "http://" + SaiShiUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, SaiShiUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, SaiShiUrl);

	//�������ҳ��ʱ
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//����json����
		return ParseJsonFromString_SaiShi(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//����web������Ϣ
bool Analysis_YanPan(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString YaPanUrl;
	YaPanUrl.Format("%s%d", g_YaPanUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//YaPanUrl = "http://" + YaPanUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, YaPanUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, YaPanUrl);

	//�������ҳ��ʱ
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//����json����
		return ParseJsonFromString_YaPan(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//�̺߳���
UINT ThreadReptile(LPVOID lParam)
{
	THREADDATA *pTrdData = (THREADDATA*)lParam;

	CString writeTemp;
	CString strID;
	CString strSQL;
	SPORT_RECORD_LINE_DATA *pstRecord;

	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		return 0;
	}

	DWORD startMS = GetTickCount();
	int n = 0;
	//VMProtectBeginUltra("WDS_VMBEGIN OF ThreadReptile");
	for (DWORD ID = g_dwStartID; ID <= g_dwEndID && !pTrdData->beExitThread; ID++)
	{
		n++;
		strID.Format("%d", ID);
		pstRecord = new SPORT_RECORD_LINE_DATA;
		//����ҳ��ȡһ��������Ϣ
		bool bRet = Analysis_SaiShi(ID, pstRecord);//��ȡ����
		if (bRet)
		{
			bRet = Analysis_SaiGuo(ID, pstRecord);//��ȡ����

			if (bRet)
			{
				//��ȡ����
				bRet = Analysis_YanPan(ID, pstRecord);

				//1���Ȳ�ѯ�Ƿ���ڴ�ID�ļ�¼
				strSQL = "select id from PeiLv_Info where id=" + strID;
				try
				{
					cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
				}
				catch (_com_error e)
				{
					cslAdo.dump_com_error(e);
					AddMsgEditCtrlString("��ʧ�ܡ�:��ѯ��¼,ID��" + strID);
					delete pstRecord;
					continue;
				}
				if (cslAdo.m_pRecordset->adoBOF)
				{
					//2��������ݿⲻ���ڴ�ID�ļ�¼������������д�뵽���ݿ�
					strSQL = "insert into PeiLv_Info(id,ZhuDui,SaiShi,RiQi,KeDui,SPF,RangFen,RangQiuSPF,BiFen,ZongJQS,QBCSPF,RQFBPLB_S,RQFBPLB_P,RQFBPLB_F,RangChuPei,RangMoPei,RQZZPLB_S,RQZZPLB_P,RQZZPLB_F,FBPLB_S,FBPLB_P,FBPLB_F,ChuPei,MoPei,ZZPLB_S,ZZPLB_P,ZZPLB_F,ChuZSW,ChuYaPan,ChuKSW,JsZSW,JsYaPan,JsKSW) values(" + strID + ",'" + pstRecord->strZhuDui + "','" + pstRecord->strSaiShi + "','" + pstRecord->strRiQi + "','" + pstRecord->strKeDui + "','" + pstRecord->strSPF + "','" + pstRecord->strRangFen + "','" + pstRecord->strRangQiuSPF + "','" + pstRecord->strBiFen + "','" + pstRecord->strZongJQS + "','" + pstRecord->strQBCSPF + "','" + pstRecord->strRQFBPLB_S + "','" + pstRecord->strRQFBPLB_P + "','" + pstRecord->strRQFBPLB_F + "','" + pstRecord->strRangChuPei + "','" + pstRecord->strRangMoPei + "','" + pstRecord->strRQZZPLB_S + "','" + pstRecord->strRQZZPLB_P + "','" + pstRecord->strRQZZPLB_F + "','" + pstRecord->strFBPLB_S + "','" + pstRecord->strFBPLB_P + "','" + pstRecord->strFBPLB_F + "','" + pstRecord->strChuPei + "','" + pstRecord->strMoPei + "','" + pstRecord->strZZPLB_S + "','" + pstRecord->strZZPLB_P + "','" + pstRecord->strZZPLB_F + "','" + pstRecord->strChuZSW + "','" + pstRecord->strChuYaPan + "','" + pstRecord->strChuKSW + "','" + pstRecord->strJsZSW + "','" + pstRecord->strJsYaPan + "','" + pstRecord->strJsKSW + "')";
					try
					{
						cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
					}
					catch (_com_error e)
					{
						cslAdo.dump_com_error(e);
						AddMsgEditCtrlString("��ʧ�ܡ�:д���¼,ID��" + strID);
						delete pstRecord;
						continue;
					}
					AddMsgEditCtrlString("[�ɹ�]:д���¼,ID��" + strID);
				}
				else
				{
					//3��������ݿ���ڴ�ID�ļ�¼����������ݵ����ݿ�
					strSQL = "update PeiLv_Info set ZhuDui='" + pstRecord->strZhuDui + "',SaiShi='" + pstRecord->strSaiShi + "',RiQi='" + pstRecord->strRiQi + "',KeDui='" + pstRecord->strKeDui + "',SPF='" + pstRecord->strSPF + "',RangFen='" + pstRecord->strRangFen + "',RangQiuSPF='" + pstRecord->strRangQiuSPF + "',BiFen='" + pstRecord->strBiFen + "',ZongJQS='" + pstRecord->strZongJQS + "',QBCSPF='" + pstRecord->strQBCSPF + "',RQFBPLB_S='" + pstRecord->strRQFBPLB_S + "',RQFBPLB_P='" + pstRecord->strRQFBPLB_P + "',RQFBPLB_F='" + pstRecord->strRQFBPLB_F + "',RangChuPei='" + pstRecord->strRangChuPei + "',RangMoPei='" + pstRecord->strRangMoPei + "',RQZZPLB_S='" + pstRecord->strRQZZPLB_S + "',RQZZPLB_P='" + pstRecord->strRQZZPLB_P + "',RQZZPLB_F='" + pstRecord->strRQZZPLB_F + "',FBPLB_S='" + pstRecord->strFBPLB_S + "',FBPLB_P='" + pstRecord->strFBPLB_P + "',FBPLB_F='" + pstRecord->strFBPLB_F + "',ChuPei='" + pstRecord->strChuPei + "',MoPei='" + pstRecord->strMoPei + "',ZZPLB_S='" + pstRecord->strZZPLB_S + "',ZZPLB_P='" + pstRecord->strZZPLB_P + "',ZZPLB_F='" + pstRecord->strZZPLB_F + "',ChuZSW='" + pstRecord->strChuZSW + "',ChuYaPan='" + pstRecord->strChuYaPan + "',ChuKSW='" + pstRecord->strChuKSW + "',JsZSW='" + pstRecord->strJsZSW + "',JsYaPan='" + pstRecord->strJsYaPan + "',JsKSW='" + pstRecord->strJsKSW + "' where id=" + strID;
					//AfxMessageBox(strSQL);
					try
					{
						cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
					}
					catch (_com_error e)
					{
						cslAdo.dump_com_error(e);
						AddMsgEditCtrlString("��ʧ�ܡ�:���¼�¼,ID��" + strID);
						delete pstRecord;
						continue;
					}
					AddMsgEditCtrlString("[�ɹ�]:���¼�¼,ID��" + strID);
				}
			}
			else
			{
				AddMsgEditCtrlString("��ʧ�ܡ�:δ��������Ҫ����,ID��" + strID);
			}
		}
		//cslAdo.m_pRecordset->Close();
		delete pstRecord;
	}
	//VMProtectEnd();

	DWORD DifMS = (GetTickCount() - startMS) / 1000;
	CString strMsg;
	strMsg.Format("��ȡ���%d��,����ʱ%d��,ƽ��%.3f��/��,��ȡ��ҳ��ʱ%d��",
		n, DifMS, (float)DifMS / (float)n, g_httpCostMS / 1000);
	AddMsgEditCtrlString(strMsg);

	g_httpCostMS = 0;

	cslAdo.ExitADOConnect();

	return 0;
}

//��ʼ�����ݰ�ť
void CReptileDlg::OnBnClickedBtnReptileData()
{
	g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
	g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	if (g_dwStartID == 0 || g_dwEndID == 0)
	{
		MessageBox("������ID");
		return;
	}

	GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(0);

	m_ThreadData.beExitThread = false;
	m_ThreadData.hwnd = GetSafeHwnd();
	CWinThread *m_pWinThread = AfxBeginThread(ThreadReptile, &m_ThreadData, 0, 0, CREATE_SUSPENDED, NULL);
	if (m_pWinThread)
	{
		m_pWinThread->m_bAutoDelete = FALSE;
		//"�̴߳����ɹ�"
		m_pWinThread->ResumeThread();
	}
	else
	{
		GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(1);
		//"�̴߳���ʧ��"
		return;
	}

	DWORD dwRet = 0;
	while (m_ThreadData.beExitThread != true)
	{
		//��ʱ�ж�,�����Ȳ���,��INFINITE������ʾ������ʱ
		//dwRet = MsgWaitForMultipleObjectsEx(1, &(winThread->m_hThread), INFINITE, QS_ALLINPUT, NULL);
		//�ɵȴ��¼������ɵȴ���Ϣ
		dwRet = MsgWaitForMultipleObjects(1, &(m_pWinThread->m_hThread), FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
			//case 0xffffffff://ʹ��AfxBeginThread()���̺߳���ִ����Ϻ�dwRet�᷵��0xffffffff������WAIT_OBJECT_0����_beginthreadex()û������
			//����_beginthreadex()��CRT�Ż��ģ���ò�Ҫ����MFC�С�

		case WAIT_OBJECT_0://�������ݵ���
			if (m_ThreadData.beExitThread)
			{
				AddMsgEditCtrlString(_T("Thread״̬:�߳���ֹ"));
			}
			else
			{
				AddMsgEditCtrlString(_T("Thread״̬:�̳߳ɹ�����"));
			}
			break;//ֻ��ֹ�����ƥ�䣬��switch������while
		case WAIT_OBJECT_0 + 1://����Ϣ
			MSG msg;
			//������Ϣѭ����dispatch��Ϣ���ض�����
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//������Ӧ��С�������Ϣ
			//while (GetMessage(&msg, NULL, 0, 0) && m_ThreadData.beExitThread != true)
			//GetMessage(&msg, NULL, 0, 0);
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;//��������ѭ����while����������䣬�ٴ�ִ��while
			}
			//break;//���ﲻ��break����Ȼ��ֱ������while��
		case WAIT_TIMEOUT:
			continue;
		default:
			DWORD err = GetLastError();
			if (ERROR_INVALID_HANDLE == err)
			{
				AddMsgEditCtrlString(_T("Thread״̬:�߳̾����Ч,�����߳��˳���Ȳ�����"));
			}
			else
			{
				AddMsgEditCtrlString(_T("Thread״̬:�̴߳���"));
				AfxMessageBox(_T("�߳��źŷ������ݻ�ȡʧ�ܣ�δ֪����!"), MB_OK | MB_ICONERROR);
			}
			break;
		}
		break;//�ؼ������������ֹwhile��
	}
	m_pWinThread->m_bAutoDelete = TRUE;
	//m_pWinThread->Delete();
	m_pWinThread = NULL;



	GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(1);
}
//��ѯ������¼�߳�
unsigned __stdcall ThreadFindBadRecord(void *param)
{
	AddMsgEditCtrlString("����ʾ��:���ڲ�ѯ������¼--------------");

	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		return 0;
	}
	CString strID, strSQL;
	for (DWORD ID = g_dwStartID; ID <= g_dwEndID; ID++)
	{
		strID.Format("%d", ID);
		//1���Ȳ�ѯ�Ƿ���ڴ�ID�ļ�¼
		strSQL = "select id,jsYaPan,ChuYaPan from PeiLv_Info where id=" + strID;
		try
		{
			cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo.dump_com_error(e);
			AddMsgEditCtrlString("��ʧ�ܡ�:��ѯ��¼,id: " + strID);
			continue;
		}
		if (cslAdo.m_pRecordset->adoBOF)//û�в鵽
		{
			AddMsgEditCtrlString("����ʾ��:������¼:�����ڵ�id: " + strID);
			continue;
		}
		else
		{
			//1��û�� ����
			CString strjsYaPan = cslAdo.m_pRecordset->GetCollect("jsYaPan");
			if (strjsYaPan == "")
			{
				AddMsgEditCtrlString("����ʾ��:������¼:���ⲻ����id: " + strID);
			}
			//2��û�� ŷ��
			CString strChuYaPan = cslAdo.m_pRecordset->GetCollect("ChuYaPan");
			if (strChuYaPan == "")
			{
				AddMsgEditCtrlString("����ʾ��:������¼:ŷ�ⲻ����id: " + strID);
			}
		}
	}

	AddMsgEditCtrlString("����ʾ��:��ѯ������¼end--------------");
	cslAdo.ExitADOConnect();

	return 0;
}
//��ѯ������¼
void CReptileDlg::OnBnClickedBtnBadRecord()
{
	g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
	g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	if (g_dwStartID == 0 || g_dwEndID == 0)
	{
		MessageBox("������ID");
		return;
	}
	GetDlgItem(IDC_BTN_BAD_RECORD)->EnableWindow(0);

	_beginthreadex(0, NULL, ThreadFindBadRecord, 0, 0, 0);

	GetDlgItem(IDC_BTN_BAD_RECORD)->EnableWindow(1);
}

//ֹͣȡ����
void CReptileDlg::OnBnClickedBtnStopReptileData()
{
	m_ThreadData.beExitThread = true;
	Sleep(200);
	GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(1);
}


void CReptileDlg::OnBnClickedBtnClearReptileMsg()
{
	SetDlgItemText(IDC_EDT_REPTILE_MSG, "");
}


HBRUSH CReptileDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	 //����������ɫ
	//pDC->SetTextColor(RGB(255, 255, 255));
	//���ñ���ɫ
	//pDC->SetBkMode(TRANSPARENT);
	//*ɫ
	//hbr = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
	//hbr = CreateSolidBrush(RGB(0x80, 0xB0, 0x90));

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


BOOL CReptileDlg::PreTranslateMessage(MSG* pMsg)
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

//�������������
void CReptileDlg::OnBnClickedBtnSaveServerCfg()
{
	//д�������������õ��ļ�
	CConfigIni cfgIni(g_strCfgName);

	CString server, database, uid, pwd;
	GetDlgItemText(IDC_EDT_SERVER_IP, server);
	GetDlgItemText(IDC_EDT_DATABASE_NAME, database);
	GetDlgItemText(IDC_EDT_DATABASE_USER, uid);
	GetDlgItemText(IDC_EDT_DATABASE_PWD, pwd);

	cfgIni.WriteIniString("ADOConnect", "server", server);
	cfgIni.WriteIniString("ADOConnect", "database", database);
	cfgIni.WriteIniString("ADOConnect", "uid", uid);
	cfgIni.WriteIniString("ADOConnect", "pwd", pwd);
}

//��ȡĳ�ֶ�ֵ�Ļ���
void CReptileDlg::OnBnClickedBtnGetValueGather()
{
	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		return;
	}

	//************�Ժ�ɸ�Ϊ�����ݿ��ȡ�ֶε����棬���ݽ���ѡ����ֶ������в�ѯ***************
	//CString strField = "SaiShi";

	//CString strSQL = "select distinct " + strField + " from PeiLv_Info order by " + strField;
	CString strSQL = "select * from SaiShi_Info";

	try
	{
		cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cslAdo.dump_com_error(e);
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "��ѯ����");
		cslAdo.ExitADOConnect();
		return;
	}
	if (cslAdo.m_pRecordset->adoBOF)
	{
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "������");
		cslAdo.ExitADOConnect();
		return;
	}

	cslAdo.m_pRecordset->MoveFirst();

	//long iRstCount = cslAdo.m_pRecordset->GetRecordCount();Ҫ��Open()������ adOpenStatic �α꣬���򷵻�-1

	CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDT_REPTILE_MSG);

	CString strValue;
	int i = 0;
	for (; !cslAdo.m_pRecordset->adoEOF; i++)
	{
		strValue += cslAdo.m_pRecordset->GetCollect("SaiShi");
		strValue += "====";
		strValue += cslAdo.m_pRecordset->GetCollect("SaiShiShort");
		strValue += "\r\n";

		cslAdo.m_pRecordset->MoveNext();
	}
	pEdit->SetSel(-1, -1);
	pEdit->ReplaceSel(strValue + "\r\n");

	cslAdo.ExitADOConnect();
}

//����ֵ��ĳ�ֶ�
void CReptileDlg::OnBnClickedBtnUpdateValue()
{
	//��ȡԭֵ�͸��µ�ֵ
	CString originValue, newValue;
	GetDlgItemText(IDC_EDT_ORIGIN_VALUE, originValue);
	GetDlgItemText(IDC_EDT_NEW_VALUE, newValue);
	if (originValue == "" || newValue == "")
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDT_REPTILE_MSG);
		pEdit->SetSel(-1, -1);
		pEdit->ReplaceSel("����Ϊ��\r\n");
		return;
	}
	//originValue.Trim();//��һ�����ʹ��ո��������
	//newValue.Trim();

	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		return;
	}

	VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnUpdateValue");
	//************�Ժ�ɸ�Ϊ�����ݿ��ȡ�ֶε����棬���ݽ���ѡ����ֶ������и���***************
	CString strField = "SaiShiShort";

	//CString strSQL = "update PeiLv_Info set " + strField + "='" + newValue + "' where " + strField + "='" + originValue + "'";
	//MessageBox(strSQL); 
	CString strSQL = "update SaiShi_Info set " + strField + "='" + newValue + "' where SaiShi='" + originValue + "'";

	try
	{
		cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cslAdo.dump_com_error(e);
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "���³���");
		cslAdo.ExitADOConnect();
		return;
	}

	VMProtectEnd();
	SetDlgItemText(IDC_EDT_REPTILE_MSG, "�������");

	cslAdo.ExitADOConnect();
}

//ע�ᰴť
void CReptileDlg::OnBnClickedBtnRegSoftware()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnRegSoftware");
	//��ȡ�û��������ȨKey
	CString user_Key;
	GetDlgItemText(IDC_EDT_REG_CODE, user_Key);//����û������Key
	if (user_Key == "")
	{
		MessageBox("��Ȩ�벻��Ϊ��");
		return;
	}
	//����õ�������
	CString machine_code = CalculateMachineCode();
	//���ݻ��������õ���ȨKey
	CString license_key = CalculateLicenseKey(machine_code);
	if (user_Key == license_key)//
	{
		//д��ע���
		if (CreateRegInfo(license_key.GetBuffer()))
		{
			MessageBox(TEXT("ע��ɹ�,�����´�"));
			exit(1);
		}
		license_key.ReleaseBuffer();
	}
	else
	{
		MessageBox(TEXT("ע��ʧ�ܣ�ע���벻��ȷ"));
		GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
		GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);

		//ע���д������ע����--����ԭ����ȷ��ע����Ҳ�������
		TCHAR clearKey[100] = { 0 };
		_tcscpy_s(clearKey, user_Key);
		//д������ע����
		CreateRegInfo(clearKey);
	}
	VMProtectEnd();

}

