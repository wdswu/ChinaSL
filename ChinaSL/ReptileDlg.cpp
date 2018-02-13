// ReptileDlg.cpp : 实现文件
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

//日志文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strLogName;

//配置文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strCfgName;

//打开网页耗时
DWORD g_httpCostMS = 0;
//CInternetSession g_session("HttpClient");


HWND g_hwndDlg;

DWORD g_dwStartID;
DWORD g_dwEndID;


//配置文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strCfgName;
//创建界面配置文件
void CreateUiCfgFile()
{
	CConfigIni cfgIni(g_strCfgName);

	// 打开文件并关闭，用于测试文件是否存在
	HANDLE  File = CreateFile(
		cfgIni.m_FilePath,
		0,// 此参数为0，则测试文件后，将释放
		FILE_SHARE_READ, NULL,
		OPEN_EXISTING, // 测试文件是否存在
		FILE_ATTRIBUTE_NORMAL, NULL);
	if (File == INVALID_HANDLE_VALUE)
	{	// 如果文件不存在，则创建一个配置文件
		cfgIni.WriteIniString("ADOConnect", "server", "localhost");
		cfgIni.WriteIniString("ADOConnect", "database", "ChinaSL");
		cfgIni.WriteIniString("ADOConnect", "uid", "000");
		cfgIni.WriteIniString("ADOConnect", "pwd", "000");
	}
}



// CReptileDlg 对话框

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


// CReptileDlg 消息处理程序
HWND g_hMsgEdit;
BOOL CReptileDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	g_hwndDlg = GetSafeHwnd();

	//设置文本框最大字符长度
	((CEdit*)GetDlgItem(IDC_EDT_REPTILE_MSG))->SetLimitText(UINT_MAX);

	g_hMsgEdit = ::GetDlgItem(g_hwndDlg, IDC_EDT_REPTILE_MSG);
	ASSERT(g_hMsgEdit);

	//读取分析网页的lua配置
	ReadLuaReptileCfg();

	//没有配置文件则创建
	CreateUiCfgFile();
	//读取配置文件并显示
	CConfigIni cfgIni(g_strCfgName);
	TCHAR server[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "server", server);
	TCHAR database[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "database", database);
	TCHAR uid[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "uid", uid);
	TCHAR pwd[50] = { 0 };
	cfgIni.GetIniString("ADOConnect", "pwd", pwd);

	//获取加密字符
	//CString str11 = EncryptPsw(pwd);
	//MessageBox(str11);

	SetDlgItemText(IDC_EDT_SERVER_IP, server);
	SetDlgItemText(IDC_EDT_DATABASE_NAME, database);
	SetDlgItemText(IDC_EDT_DATABASE_USER, uid);
	SetDlgItemText(IDC_EDT_DATABASE_PWD, pwd);

#if WDS_ADMIN	
	VMProtectBeginUltra("WDS_VMBEGIN OF CReptileDlg::OnInitDialog");
	//------程序注册检测------后期可专门做个窗口，注册后才跳转主界面
	// 读取注册表中存的KEY。未读到，直接跳转到程序限制运行，
	// 读取到，则计算本机硬件信息，生成key，与读取的对比，不同就限制运行
	// 相同程序正常运行。
	CString machine_code = CalculateMachineCode();//计算得到机器码
	SetDlgItemText(IDC_EDT_MAC_CODE, machine_code);

	TCHAR keyInReg[100] = { 0 };
	bool bReg = ReadRegInfo(keyInReg);
	if (!bReg)
	{
		MessageBox(TEXT("程序还未注册过，功能受限"));
		//禁用功能按钮
		GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
		GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);
	}
	else
	{
		//获取注册表中的授权key
		GetDlgItem(IDC_EDT_REG_CODE)->SetWindowText(keyInReg);
		//根据机器码计算授权Key
		CString license_key = CalculateLicenseKey(machine_code);
		//对比获取的注册表授权和计算的授权
		if (keyInReg == license_key)
		{
			//OnBnClickedBtnReadFile();
			//MessageBox(TEXT("注册成功"));
		}
		else
		{
			MessageBox(TEXT("注册码不正确，请重新注册"));
			GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
			GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);
		}
	}

	VMProtectEnd();
#endif
	return TRUE;  // return TRUE unless you set the focus to a control
					  // 异常: OCX 属性页应返回 FALSE
}

void CReptileDlg::OnPaint()
{
	// device context for painting
	CPaintDC dc(this);
	// TODO: 在此处添加消息处理程序代码

	//static bool flag = true;
	//if (flag)
	//{
	//	flag = false;
	//	OnBnClickedBtnRefreshId();
	//}

	// 不为绘图消息调用 CDialogEx::OnPaint()
}


void CReptileDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	m_ThreadData.beExitThread = true;
	Sleep(100);

}
//增加文本到编辑框末尾
void AddMsgEditCtrlString(CString lpStr, bool bLinefeed = true)
{
	static int i = 0;
	i++;
	if (i == 5000)
	{
		i = 0;
		SendMessage(g_hMsgEdit, WM_SETTEXT, 0, (LPARAM)(LPCTSTR)"");
	}
	//SDK版本
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

	//MFC版本--用于mfc类中
	//CEdit *pEdit= GetDlgItem(g_hwndDlg, IDC_EDT_REPTILE_MSG);
	//ASSERT(pEdit&&pEdit->GetSafeHwnd());
	//pEdit->SetSel(-1, -1);
	//pEdit->ReplaceSel(lpStr);

	CStdioFile file;
	if (file.Open(g_strLogName, CFile::modeCreate | CFile::modeNoTruncate/*存在则不删除内容*/ | CFile::modeWrite))
	{
		file.SeekToEnd();
		file.WriteString(lpStr);
	}
	file.Close();
}
//刷新ID
void CReptileDlg::OnBnClickedBtnRefreshId()
{
	GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(0);

	CCslAdo cslAdo;
	//连接数据库 
	if (!cslAdo.InitADOConnect())
	{
		GetDlgItem(IDC_BTN_REFRESH_ID)->EnableWindow(1);
		return;
	}

	//VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnRefreshId");
	//-----从数据库获取最大的ID-100作为开始ID------
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
	if (cslAdo.m_pRecordset->adoBOF)//如果数据库不存在此记录
	{
		g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
		g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	}
	else
	{
		_variant_t TheValue;//
		try
		{
			//VARIANT数据类型,得到字段的值
			TheValue = cslAdo.m_pRecordset->GetCollect(_T("id"));
		}
		catch (_com_error e)
		{
			cslAdo.dump_com_error(e);
		}

		//获取数据库最大的一个ID
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
//winnet获取网页
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
			//当超时时间到达后，recv会返回错误，也就是-1，而此时的错误码是EAGAIN或者EWOULDBLOCK，POSIX.1-2001上允许两个任意一个出现都行，所以建议在判断错误码上两个都写上。
			//如果socket是被对方用linger为0的形式关掉，也就是直接发RST的方式关闭的时候，recv也会返回错误，错误码是ENOENT
			//还有一种经常在代码中常见的错误码，那就是EINTER，意思是系统在接收的时候因为收到其他中断信号而被迫返回，不算socket故障，应该继续接收。
			//if (/*(Number < 0) &&*/ (errno == EAGAIN || errno == EWOULDBLOCK || errno == EINTR))
			//{
			//	continue;//继续接收数据
			//}

			break;
		}
	}

	DWORD startMS2 = (GetTickCount() - StartMS1) / 1000;
	CString strMsg;
	strMsg.Format("\r\nReadFile:-----%d-----秒  %d次  size:%dbyte", startMS2, n, strBuffer->GetLength());
	AddMsgEditCtrlString(strMsg);

	InternetCloseHandle(hURL);
	hURL = NULL;
	InternetCloseHandle(hSession);
	hSession = NULL;

	return true;
}

//#include <winsock2.h>
//#pragma comment(lib, "ws2_32.lib")
//socket获取网页
bool SocketGetWebSrcCode(CString* strBuffer, CString Url)
{
	// 初始化WSA
	WSADATA wsadata;
	if (WSAStartup(MAKEWORD(2, 2), &wsadata))
	{
		AddMsgEditCtrlString("初始化SOCKET出错！");
		return false;
	}
	//创建套接字
	SOCKET hsocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (hsocket == INVALID_SOCKET)
	{
		//int error = WSAGetLastError();
		AddMsgEditCtrlString("创建socket error");
		WSACleanup();
		return false;
	}

	//绑定IP、端口
	int i = Url.Find("/");
	CString strHostName = Url.Mid(0, i);
	CString strRequestUrl = Url.Mid(i, Url.GetLength());
	LPHOSTENT lphostent = gethostbyname(strHostName);//"i.sporttery.cn";
	if (lphostent == NULL)
	{
		AddMsgEditCtrlString("主机名为空！");
		WSACleanup();
		return false;
	}
	SOCKADDR_IN saServer;
	saServer.sin_family = AF_INET;
	//saServer.sin_addr.S_un.S_addr = INADDR_ANY;//服务端用
	saServer.sin_addr = *((LPIN_ADDR)*lphostent->h_addr_list);
	saServer.sin_port = htons(80);

	// 利用SOCKET连接
	int nRet = connect(hsocket, (LPSOCKADDR)&saServer, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("建立连接时出错！");
		closesocket(hsocket);
		WSACleanup();
		return false;
	}

	//默认send和recv是阻塞的，这里设置超时则返回
	//int timeout = 3000; //3s
	//int ret = setsockopt(hsocket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));

	CString req =
		"GET " + strRequestUrl + " HTTP/1.1\r\n"
		"HOST:" + strHostName + "\r\n"
		"User-Agent: Mozilla/5.0 (compatible; MSIE 8.0; Windows NT 5.1; Trident/4.0; .NET CLR 1.1.4322; .NET CLR //2.0.50727; .NET4.0C; .NET4.0E; .NET CLR 3.0.4506.2152; .NET CLR 3.5.30729)\r\n"
		"Connection: close"//
		"\r\n\r\n";

	// 利用SOCKET发送
	nRet = send(hsocket, req, strlen(req), 0);
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("发送数据包时出错！");
		closesocket(hsocket);
		WSACleanup();
		return false;
	}

	// 没有数据发送后，就关闭连接，这之后不能执行关闭的操作，但是不会释放socket资源
	// 比如SD_SEND，会关闭send，但是已经放入缓冲区的数据会被协议发送，只是后面不能用send了。
	// 这里关闭是为了解决后面recv时，最后一份数据等待太久，，原因不明
	//nRet = shutdown(hsocket, SD_SEND);
	//if (nRet == SOCKET_ERROR)
	//{
	//	AddMsgEditCtrlString("关闭连接出错！");
	//	closesocket(hsocket);
	//	WSACleanup();
	//	return false;
	//}

	//int n = 0;

	char Dest[MAXBLOCKSIZE] = { 0 };

	//DWORD StartMS1 = GetTickCount();

	//接收代码有问题:
	//1、如果使用Keep-Alive,你必须小心处理接收：分两步走：接收并解释头，然后接收实体，在解释头数据中，要解析出实体数据的长度（Content-Length 或chunked）,然后接收精确长度的字节数，然后go on...
	//2、或者使用"Connection: close"请求，也应该按照header+body的数据接收+同步处理，而不是一个简单的循环recv,但这解决了卡的问题
	do {
		//n++;
		//DWORD StartMS3 = GetTickCount();

		// 接收返回数据包
		nRet = recv(hsocket, (LPSTR)Dest, sizeof(Dest) - 1, 0);

		//DWORD startMS4 = (GetTickCount() - StartMS3) / 1000;
		//CString strMsg;
		//strMsg.Format("recv第%d次\t耗时%d秒\t大小%d byte\r\n", n, startMS4, strlen(Dest));
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
	//strMsg.Format("recv():共-----%d-----秒  %d次  size:%dbyte\r\n", startMS2, n, strlen(*strBuffer));
	//AddMsgEditCtrlString(strMsg);

	//去掉http返回的头部（头部结尾是"\r\n\r\n"）
	i = strBuffer->Find("\r\n\r\n");
	int iStart = i + strlen("\r\n\r\n");
	int iEnd = strBuffer->GetLength();
	*strBuffer = strBuffer->Mid(iStart, iEnd);

	//关闭并释放资源
	nRet = closesocket(hsocket);
	if (nRet == SOCKET_ERROR)
	{
		AddMsgEditCtrlString("关闭socket出错");
	}
	WSACleanup();

	return true;
}
//CHttpFile获取网页
//bool MFCGetWebSrcCode()

//解析json赛果数据
bool ParseJsonFromString_SaiGuo(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader将Json字符串解析到root，root将包含Json里所有子元素
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------解析json数据--------------------------------
		int code = root["status"]["code"].asInt();//访问成功，message=="",或者code="0"
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
			if (bMember)//判断是否是成员
			{
				strSPF = root["result"]["pool_rs"]["had"]["prs_name"].asString();//赛果
			}
			stRecord->strSPF = strSPF.c_str();//赛果
			if (stRecord->strSPF == "")
				return false;

			if (root["result"]["pool_rs"].isMember("hhad"))
			{
				strRangFen = root["result"]["pool_rs"]["hhad"]["goalline"].asString();//让分
				strRangQiuSPF = root["result"]["pool_rs"]["hhad"]["prs_name"].asString();//让球
			}
			if (root["result"]["pool_rs"].isMember("crs"))
			{
				strBiFen = root["result"]["pool_rs"]["crs"]["prs_name"].asString();//全场比分
			}
			if (root["result"]["pool_rs"].isMember("ttg"))
			{
				strZongJQS = root["result"]["pool_rs"]["ttg"]["prs_name"].asString();//进球
			}
			if (root["result"]["pool_rs"].isMember("hafu"))
			{
				strQBCSPF = root["result"]["pool_rs"]["hafu"]["prs_name"].asString();//半全场胜平负
			}
		}
		else
			return false;

		string strFBPLB_S, strFBPLB_P, strFBPLB_F;//初赔值S/P/F
		string strZZPLB_S, strZZPLB_P, strZZPLB_F;//末赔值S/P/F
		string strRQFBPLB_S, strRQFBPLB_P, strRQFBPLB_F;//让初赔值S/P/F		
		string strRQZZPLB_S, strRQZZPLB_P, strRQZZPLB_F;//让末赔值S/P/F
		nnn = root["result"]["odds_list"].size();
		if (nnn > 0)
		{
			int num;
			if (root["result"]["odds_list"].isMember("had"))
			{
				num = root["result"]["odds_list"]["had"]["odds"].size();//求个数，并取头和尾
				if (num > 0)
				{
					//初赔值S/P/F
					strFBPLB_S = root["result"]["odds_list"]["had"]["odds"][0]["h"].asString();
					strFBPLB_P = root["result"]["odds_list"]["had"]["odds"][0]["d"].asString();
					strFBPLB_F = root["result"]["odds_list"]["had"]["odds"][0]["a"].asString();
					if (num > 1)
					{
						//末赔值S/P/F
						strZZPLB_S = root["result"]["odds_list"]["had"]["odds"][num - 1]["h"].asString();
						strZZPLB_P = root["result"]["odds_list"]["had"]["odds"][num - 1]["d"].asString();
						strZZPLB_F = root["result"]["odds_list"]["had"]["odds"][num - 1]["a"].asString();
					}
				}
			}

			if (root["result"]["odds_list"].isMember("hhad"))
			{
				num = root["result"]["odds_list"]["hhad"]["odds"].size();//求个数，并取头和尾
				if (num > 0)
				{
					//让初赔值S/P/F
					strRQFBPLB_S = root["result"]["odds_list"]["hhad"]["odds"][0]["h"].asString();
					strRQFBPLB_P = root["result"]["odds_list"]["hhad"]["odds"][0]["d"].asString();
					strRQFBPLB_F = root["result"]["odds_list"]["hhad"]["odds"][0]["a"].asString();
					if (num > 1)
					{
						//让末赔值S/P/F
						strRQZZPLB_S = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["h"].asString();
						strRQZZPLB_P = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["d"].asString();
						strRQZZPLB_F = root["result"]["odds_list"]["hhad"]["odds"][num - 1]["a"].asString();
					}
				}
			}
		}
		else
			return false;

		//-----------------------------转换数据--------------------------------
		stRecord->strRangFen = strRangFen.c_str();//让分
		stRecord->strRangFen = "(" + stRecord->strRangFen + ")";

		stRecord->strRangQiuSPF = strRangQiuSPF.c_str();//让球
		stRecord->strBiFen = strBiFen.c_str();//全场比分
		stRecord->strZongJQS = strZongJQS.c_str();//进球
		stRecord->strQBCSPF = strQBCSPF.c_str();//半全场胜平负

		//初赔值S/P/F
		stRecord->strFBPLB_S = strFBPLB_S.c_str();
		stRecord->strFBPLB_P = strFBPLB_P.c_str();
		stRecord->strFBPLB_F = strFBPLB_F.c_str();
		int S = atoi(stRecord->strFBPLB_S);//计算
		int P = atoi(stRecord->strFBPLB_P);
		int F = atoi(stRecord->strFBPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			return false;
		}
		stRecord->strChuPei.Format("%d%d%d", S, P, F);

		//末赔值S/P/F
		stRecord->strZZPLB_S = strZZPLB_S.c_str();
		stRecord->strZZPLB_P = strZZPLB_P.c_str();
		stRecord->strZZPLB_F = strZZPLB_F.c_str();
		S = atoi(stRecord->strZZPLB_S);//计算
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

		//让初赔值S/P/F
		stRecord->strRQFBPLB_S = strRQFBPLB_S.c_str();
		stRecord->strRQFBPLB_P = strRQFBPLB_P.c_str();
		stRecord->strRQFBPLB_F = strRQFBPLB_F.c_str();
		S = atoi(stRecord->strRQFBPLB_S);//计算让球发布赔率--让初赔
		P = atoi(stRecord->strRQFBPLB_P);
		F = atoi(stRecord->strRQFBPLB_F);
		if (S == 0 || P == 0 || F == 0)
		{
			return false;
		}
		stRecord->strRangChuPei.Format("%d%d%d", S, P, F);

		//让末赔值S/P/F
		stRecord->strRQZZPLB_S = strRQZZPLB_S.c_str();
		stRecord->strRQZZPLB_P = strRQZZPLB_P.c_str();
		stRecord->strRQZZPLB_F = strRQZZPLB_F.c_str();
		S = atoi(stRecord->strRQZZPLB_S);//计算让末赔
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
//解析json赛事数据
bool ParseJsonFromString_SaiShi(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader将Json字符串解析到root，root将包含Json里所有子元素
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------解析json数据--------------------------------
		string msg = root["status"]["message"].asString();//访问成功，message=="",或者code="0"
		int bRet = strcmp(msg.c_str(), "");
		if (bRet)
			return false;

		if (root["result"].size() > 0)
		{
			string strRiQi = root["result"]["date_cn"].asString();//日期
			string strSaiShi = root["result"]["l_cn"].asString();//赛事全称
			string strZhuDui = root["result"]["h_cn"].asString();//主队
			string strKeDui = root["result"]["a_cn"].asString();//客队

			//-----------------------------转换数据--------------------------------
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
//解析json亚盘数据
bool ParseJsonFromString_YaPan(CString str, SPORT_RECORD_LINE_DATA *stRecord)
{
	Json::Reader reader;
	Json::Value root;
	// reader将Json字符串解析到root，root将包含Json里所有子元素
	if (reader.parse(str.GetBuffer(), root))
	{
		str.ReleaseBuffer();

		//-----------------------------解析json数据--------------------------------
		string msg = root["status"]["message"].asString();//访问成功，message=="",或者code="0"
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
					if (id == "229")//找到澳门ID
					{
						//初盘
						string strChuZSW = root["result"]["data"][i]["o1_s"].asString();
						string strChuYaPan = root["result"]["data"][i]["o3_s"].asString();
						string strChuKSW = root["result"]["data"][i]["o2_s"].asString();
						//即时盘
						string strJsZSW = root["result"]["data"][i]["o1"].asString();
						string strJsYaPan = root["result"]["data"][i]["o3"].asString();
						string strJsKSW = root["result"]["data"][i]["o2"].asString();

						//-----------------------------转换数据-------------------------------- 
						//---初亚盘信息---
						stRecord->strChuZSW = strChuZSW.c_str();//初盘主水
						stRecord->strChuYaPan = strChuYaPan.c_str();//初盘亚盘
						stRecord->strChuKSW = strChuKSW.c_str();//初盘客水

						//---终亚盘信息---
						stRecord->strJsZSW = strJsZSW.c_str();//即时盘主水
						stRecord->strJsYaPan = strJsYaPan.c_str();//即时盘亚盘
						stRecord->strJsKSW = strJsKSW.c_str();//即时盘客水

						return true;
					}
				}
			}
		}
	}

	return false;
}
//解析web赛果信息
bool Analysis_SaiGuo(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString SaiGuoUrl;
	SaiGuoUrl.Format("%s%d", g_SaiGuoUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//SaiGuoUrl = "http://" + SaiGuoUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, SaiGuoUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, SaiGuoUrl);

	//计算打开网页耗时
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//解析json数据
		return ParseJsonFromString_SaiGuo(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//解析web赛事信息
bool Analysis_SaiShi(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString SaiShiUrl;
	SaiShiUrl.Format("%s%d", g_SaiShiUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//SaiShiUrl = "http://" + SaiShiUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, SaiShiUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, SaiShiUrl);

	//计算打开网页耗时
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//解析json数据
		return ParseJsonFromString_SaiShi(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//解析web亚盘信息
bool Analysis_YanPan(DWORD ID, SPORT_RECORD_LINE_DATA *stRecord)
{
	CString YaPanUrl;
	YaPanUrl.Format("%s%d", g_YaPanUrl, ID);

	DWORD StartMS = GetTickCount();

	CString strBuffer;
	//YaPanUrl = "http://" + YaPanUrl;
	//bool bRet = GetWebSrcCode(&strBuffer, YaPanUrl);
	bool bRet = SocketGetWebSrcCode(&strBuffer, YaPanUrl);

	//计算打开网页耗时
	g_httpCostMS += GetTickCount() - StartMS;

	if (bRet)
	{
		//解析json数据
		return ParseJsonFromString_YaPan(strBuffer, stRecord);
	}
	else
	{
		return false;
	}
}
//线程函数
UINT ThreadReptile(LPVOID lParam)
{
	THREADDATA *pTrdData = (THREADDATA*)lParam;

	CString writeTemp;
	CString strID;
	CString strSQL;
	SPORT_RECORD_LINE_DATA *pstRecord;

	CCslAdo cslAdo;
	//连接数据库 
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
		//从网页获取一条比赛信息
		bool bRet = Analysis_SaiShi(ID, pstRecord);//获取赛事
		if (bRet)
		{
			bRet = Analysis_SaiGuo(ID, pstRecord);//获取赛果

			if (bRet)
			{
				//获取亚盘
				bRet = Analysis_YanPan(ID, pstRecord);

				//1、先查询是否存在此ID的记录
				strSQL = "select id from PeiLv_Info where id=" + strID;
				try
				{
					cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
				}
				catch (_com_error e)
				{
					cslAdo.dump_com_error(e);
					AddMsgEditCtrlString("【失败】:查询记录,ID：" + strID);
					delete pstRecord;
					continue;
				}
				if (cslAdo.m_pRecordset->adoBOF)
				{
					//2、如果数据库不存在此ID的记录，则新增数据写入到数据库
					strSQL = "insert into PeiLv_Info(id,ZhuDui,SaiShi,RiQi,KeDui,SPF,RangFen,RangQiuSPF,BiFen,ZongJQS,QBCSPF,RQFBPLB_S,RQFBPLB_P,RQFBPLB_F,RangChuPei,RangMoPei,RQZZPLB_S,RQZZPLB_P,RQZZPLB_F,FBPLB_S,FBPLB_P,FBPLB_F,ChuPei,MoPei,ZZPLB_S,ZZPLB_P,ZZPLB_F,ChuZSW,ChuYaPan,ChuKSW,JsZSW,JsYaPan,JsKSW) values(" + strID + ",'" + pstRecord->strZhuDui + "','" + pstRecord->strSaiShi + "','" + pstRecord->strRiQi + "','" + pstRecord->strKeDui + "','" + pstRecord->strSPF + "','" + pstRecord->strRangFen + "','" + pstRecord->strRangQiuSPF + "','" + pstRecord->strBiFen + "','" + pstRecord->strZongJQS + "','" + pstRecord->strQBCSPF + "','" + pstRecord->strRQFBPLB_S + "','" + pstRecord->strRQFBPLB_P + "','" + pstRecord->strRQFBPLB_F + "','" + pstRecord->strRangChuPei + "','" + pstRecord->strRangMoPei + "','" + pstRecord->strRQZZPLB_S + "','" + pstRecord->strRQZZPLB_P + "','" + pstRecord->strRQZZPLB_F + "','" + pstRecord->strFBPLB_S + "','" + pstRecord->strFBPLB_P + "','" + pstRecord->strFBPLB_F + "','" + pstRecord->strChuPei + "','" + pstRecord->strMoPei + "','" + pstRecord->strZZPLB_S + "','" + pstRecord->strZZPLB_P + "','" + pstRecord->strZZPLB_F + "','" + pstRecord->strChuZSW + "','" + pstRecord->strChuYaPan + "','" + pstRecord->strChuKSW + "','" + pstRecord->strJsZSW + "','" + pstRecord->strJsYaPan + "','" + pstRecord->strJsKSW + "')";
					try
					{
						cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
					}
					catch (_com_error e)
					{
						cslAdo.dump_com_error(e);
						AddMsgEditCtrlString("【失败】:写入记录,ID：" + strID);
						delete pstRecord;
						continue;
					}
					AddMsgEditCtrlString("[成功]:写入记录,ID：" + strID);
				}
				else
				{
					//3、如果数据库存在此ID的记录，则更新数据到数据库
					strSQL = "update PeiLv_Info set ZhuDui='" + pstRecord->strZhuDui + "',SaiShi='" + pstRecord->strSaiShi + "',RiQi='" + pstRecord->strRiQi + "',KeDui='" + pstRecord->strKeDui + "',SPF='" + pstRecord->strSPF + "',RangFen='" + pstRecord->strRangFen + "',RangQiuSPF='" + pstRecord->strRangQiuSPF + "',BiFen='" + pstRecord->strBiFen + "',ZongJQS='" + pstRecord->strZongJQS + "',QBCSPF='" + pstRecord->strQBCSPF + "',RQFBPLB_S='" + pstRecord->strRQFBPLB_S + "',RQFBPLB_P='" + pstRecord->strRQFBPLB_P + "',RQFBPLB_F='" + pstRecord->strRQFBPLB_F + "',RangChuPei='" + pstRecord->strRangChuPei + "',RangMoPei='" + pstRecord->strRangMoPei + "',RQZZPLB_S='" + pstRecord->strRQZZPLB_S + "',RQZZPLB_P='" + pstRecord->strRQZZPLB_P + "',RQZZPLB_F='" + pstRecord->strRQZZPLB_F + "',FBPLB_S='" + pstRecord->strFBPLB_S + "',FBPLB_P='" + pstRecord->strFBPLB_P + "',FBPLB_F='" + pstRecord->strFBPLB_F + "',ChuPei='" + pstRecord->strChuPei + "',MoPei='" + pstRecord->strMoPei + "',ZZPLB_S='" + pstRecord->strZZPLB_S + "',ZZPLB_P='" + pstRecord->strZZPLB_P + "',ZZPLB_F='" + pstRecord->strZZPLB_F + "',ChuZSW='" + pstRecord->strChuZSW + "',ChuYaPan='" + pstRecord->strChuYaPan + "',ChuKSW='" + pstRecord->strChuKSW + "',JsZSW='" + pstRecord->strJsZSW + "',JsYaPan='" + pstRecord->strJsYaPan + "',JsKSW='" + pstRecord->strJsKSW + "' where id=" + strID;
					//AfxMessageBox(strSQL);
					try
					{
						cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
					}
					catch (_com_error e)
					{
						cslAdo.dump_com_error(e);
						AddMsgEditCtrlString("【失败】:更新记录,ID：" + strID);
						delete pstRecord;
						continue;
					}
					AddMsgEditCtrlString("[成功]:更新记录,ID：" + strID);
				}
			}
			else
			{
				AddMsgEditCtrlString("【失败】:未解析出必要数据,ID：" + strID);
			}
		}
		//cslAdo.m_pRecordset->Close();
		delete pstRecord;
	}
	//VMProtectEnd();

	DWORD DifMS = (GetTickCount() - startMS) / 1000;
	CString strMsg;
	strMsg.Format("爬取完毕%d个,共耗时%d秒,平均%.3f秒/个,获取网页耗时%d秒",
		n, DifMS, (float)DifMS / (float)n, g_httpCostMS / 1000);
	AddMsgEditCtrlString(strMsg);

	g_httpCostMS = 0;

	cslAdo.ExitADOConnect();

	return 0;
}

//开始爬数据按钮
void CReptileDlg::OnBnClickedBtnReptileData()
{
	g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
	g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	if (g_dwStartID == 0 || g_dwEndID == 0)
	{
		MessageBox("请输入ID");
		return;
	}

	GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(0);

	m_ThreadData.beExitThread = false;
	m_ThreadData.hwnd = GetSafeHwnd();
	CWinThread *m_pWinThread = AfxBeginThread(ThreadReptile, &m_ThreadData, 0, 0, CREATE_SUSPENDED, NULL);
	if (m_pWinThread)
	{
		m_pWinThread->m_bAutoDelete = FALSE;
		//"线程创建成功"
		m_pWinThread->ResumeThread();
	}
	else
	{
		GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(1);
		//"线程创建失败"
		return;
	}

	DWORD dwRet = 0;
	while (m_ThreadData.beExitThread != true)
	{
		//超时判断,这里先不作,用INFINITE参数表示永不超时
		//dwRet = MsgWaitForMultipleObjectsEx(1, &(winThread->m_hThread), INFINITE, QS_ALLINPUT, NULL);
		//可等待事件，还可等待消息
		dwRet = MsgWaitForMultipleObjects(1, &(m_pWinThread->m_hThread), FALSE, INFINITE, QS_ALLINPUT);
		switch (dwRet)
		{
			//case 0xffffffff://使用AfxBeginThread()在线程函数执行完毕后，dwRet会返回0xffffffff而不是WAIT_OBJECT_0，而_beginthreadex()没有问题
			//但是_beginthreadex()是CRT优化的，最好不要用在MFC中。

		case WAIT_OBJECT_0://返回数据到达
			if (m_ThreadData.beExitThread)
			{
				AddMsgEditCtrlString(_T("Thread状态:线程终止"));
			}
			else
			{
				AddMsgEditCtrlString(_T("Thread状态:线程成功返回"));
			}
			break;//只终止最近的匹配，即switch而不是while
		case WAIT_OBJECT_0 + 1://新消息
			MSG msg;
			//进入消息循环，dispatch消息到特定窗口
			if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))//不能响应最小化后的消息
			//while (GetMessage(&msg, NULL, 0, 0) && m_ThreadData.beExitThread != true)
			//GetMessage(&msg, NULL, 0, 0);
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
				continue;//跳过本次循环（while）的余下语句，再次执行while
			}
			//break;//这里不能break，不然就直接跳出while了
		case WAIT_TIMEOUT:
			continue;
		default:
			DWORD err = GetLastError();
			if (ERROR_INVALID_HANDLE == err)
			{
				AddMsgEditCtrlString(_T("Thread状态:线程句柄无效,可能线程退出后等不到！"));
			}
			else
			{
				AddMsgEditCtrlString(_T("Thread状态:线程错误"));
				AfxMessageBox(_T("线程信号返回数据获取失败，未知错误!"), MB_OK | MB_ICONERROR);
			}
			break;
		}
		break;//关键啊，这个是终止while的
	}
	m_pWinThread->m_bAutoDelete = TRUE;
	//m_pWinThread->Delete();
	m_pWinThread = NULL;



	GetDlgItem(IDC_BTN_REPTILE_DATA)->EnableWindow(1);
}
//查询不良记录线程
unsigned __stdcall ThreadFindBadRecord(void *param)
{
	AddMsgEditCtrlString("【提示】:现在查询不良记录--------------");

	CCslAdo cslAdo;
	//连接数据库 
	if (!cslAdo.InitADOConnect())
	{
		return 0;
	}
	CString strID, strSQL;
	for (DWORD ID = g_dwStartID; ID <= g_dwEndID; ID++)
	{
		strID.Format("%d", ID);
		//1、先查询是否存在此ID的记录
		strSQL = "select id,jsYaPan,ChuYaPan from PeiLv_Info where id=" + strID;
		try
		{
			cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo.dump_com_error(e);
			AddMsgEditCtrlString("【失败】:查询记录,id: " + strID);
			continue;
		}
		if (cslAdo.m_pRecordset->adoBOF)//没有查到
		{
			AddMsgEditCtrlString("【提示】:不良记录:不存在的id: " + strID);
			continue;
		}
		else
		{
			//1、没有 亚赔
			CString strjsYaPan = cslAdo.m_pRecordset->GetCollect("jsYaPan");
			if (strjsYaPan == "")
			{
				AddMsgEditCtrlString("【提示】:不良记录:亚赔不存在id: " + strID);
			}
			//2、没有 欧赔
			CString strChuYaPan = cslAdo.m_pRecordset->GetCollect("ChuYaPan");
			if (strChuYaPan == "")
			{
				AddMsgEditCtrlString("【提示】:不良记录:欧赔不存在id: " + strID);
			}
		}
	}

	AddMsgEditCtrlString("【提示】:查询不良记录end--------------");
	cslAdo.ExitADOConnect();

	return 0;
}
//查询不良记录
void CReptileDlg::OnBnClickedBtnBadRecord()
{
	g_dwStartID = GetDlgItemInt(IDC_EDT_reptile_START_ID);
	g_dwEndID = GetDlgItemInt(IDC_EDT_reptile_END_ID);
	if (g_dwStartID == 0 || g_dwEndID == 0)
	{
		MessageBox("请输入ID");
		return;
	}
	GetDlgItem(IDC_BTN_BAD_RECORD)->EnableWindow(0);

	_beginthreadex(0, NULL, ThreadFindBadRecord, 0, 0, 0);

	GetDlgItem(IDC_BTN_BAD_RECORD)->EnableWindow(1);
}

//停止取数据
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

	// TODO:  在此更改 DC 的任何特性

	 //设置字体颜色
	//pDC->SetTextColor(RGB(255, 255, 255));
	//设置背景色
	//pDC->SetBkMode(TRANSPARENT);
	//*色
	//hbr = CreateSolidBrush(RGB(0xFF, 0xFF, 0xFF));
	//hbr = CreateSolidBrush(RGB(0x80, 0xB0, 0x90));

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


BOOL CReptileDlg::PreTranslateMessage(MSG* pMsg)
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

//保存服务器配置
void CReptileDlg::OnBnClickedBtnSaveServerCfg()
{
	//写入界面服务器配置到文件
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

//获取某字段值的汇总
void CReptileDlg::OnBnClickedBtnGetValueGather()
{
	CCslAdo cslAdo;
	//连接数据库 
	if (!cslAdo.InitADOConnect())
	{
		return;
	}

	//************以后可改为从数据库获取字段到界面，根据界面选择的字段来进行查询***************
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
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "查询出错");
		cslAdo.ExitADOConnect();
		return;
	}
	if (cslAdo.m_pRecordset->adoBOF)
	{
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "无数据");
		cslAdo.ExitADOConnect();
		return;
	}

	cslAdo.m_pRecordset->MoveFirst();

	//long iRstCount = cslAdo.m_pRecordset->GetRecordCount();要用Open()方法的 adOpenStatic 游标，否则返回-1

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

//更新值到某字段
void CReptileDlg::OnBnClickedBtnUpdateValue()
{
	//获取原值和更新的值
	CString originValue, newValue;
	GetDlgItemText(IDC_EDT_ORIGIN_VALUE, originValue);
	GetDlgItemText(IDC_EDT_NEW_VALUE, newValue);
	if (originValue == "" || newValue == "")
	{
		CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDT_REPTILE_MSG);
		pEdit->SetSel(-1, -1);
		pEdit->ReplaceSel("不能为空\r\n");
		return;
	}
	//originValue.Trim();//万一本来就带空格的数据呢
	//newValue.Trim();

	CCslAdo cslAdo;
	//连接数据库 
	if (!cslAdo.InitADOConnect())
	{
		return;
	}

	VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnUpdateValue");
	//************以后可改为从数据库获取字段到界面，根据界面选择的字段来进行更新***************
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
		SetDlgItemText(IDC_EDT_REPTILE_MSG, "更新出错");
		cslAdo.ExitADOConnect();
		return;
	}

	VMProtectEnd();
	SetDlgItemText(IDC_EDT_REPTILE_MSG, "更新完成");

	cslAdo.ExitADOConnect();
}

//注册按钮
void CReptileDlg::OnBnClickedBtnRegSoftware()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF OnBnClickedBtnRegSoftware");
	//获取用户输入的授权Key
	CString user_Key;
	GetDlgItemText(IDC_EDT_REG_CODE, user_Key);//获得用户输入的Key
	if (user_Key == "")
	{
		MessageBox("授权码不能为空");
		return;
	}
	//计算得到机器码
	CString machine_code = CalculateMachineCode();
	//根据机器码计算得到授权Key
	CString license_key = CalculateLicenseKey(machine_code);
	if (user_Key == license_key)//
	{
		//写入注册表
		if (CreateRegInfo(license_key.GetBuffer()))
		{
			MessageBox(TEXT("注册成功,请重新打开"));
			exit(1);
		}
		license_key.ReleaseBuffer();
	}
	else
	{
		MessageBox(TEXT("注册失败，注册码不正确"));
		GetDlgItem(IDC_BTN_REPTILE_DATA)->ShowWindow(0);
		GetDlgItem(IDC_BTN_UPDATE_VALUE)->ShowWindow(0);

		//注册表写入错误的注册码--可能原来正确的注册码也被清空了
		TCHAR clearKey[100] = { 0 };
		_tcscpy_s(clearKey, user_Key);
		//写入错误的注册码
		CreateRegInfo(clearKey);
	}
	VMProtectEnd();

}

