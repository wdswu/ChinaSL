//================================================================
// 文件名:WDS_COMMON.cpp
// 说明:wdswu自定义的一些以WDS_开头的函数
// 作者:晓梦迷蝶 wdswu@163.com
// 创建时间：2015.08.07
// 最后更新时间：2017.11.18
//================================================================
#include "stdafx.h"

#include "WDS_COMMON.h"


#include <math.h>//用于WDS_strHexToInt()




//==========================后台消息相关函数====================================
//指定窗口，指定位置，鼠标左键单击消息
void WDS_LBtnClickMsg(HWND hWnd, WORD x, WORD y)
{
	PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
	Sleep(50);
	PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
	Sleep(50);
}
//指定窗口，指定位置，鼠标左键双击消息
void WDS_LBtnDbClickMsg(HWND hWnd, WORD x, WORD y)
{
	PostMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(x, y));
	Sleep(50);
}
//使用向指定窗口发消息的方式输入字符串（部分窗口中文字符也可）
//hWnd----接收消息的窗口句柄
//lpStr----要发送的字符串
//Delay----每个字符之间的发送延时
void WDS_StrInputMsg(HWND hWnd, LPCTSTR lpStr, long Delay)
{
	while (*lpStr != 0)
	{
		PostMessage(hWnd, WM_CHAR, *lpStr, 0);
		lpStr++;
		Sleep(Delay);
	}
}
//指定窗口，指定位置，鼠标单击并输入字符串
void WDS_LBtnClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay)
{
	WDS_LBtnClickMsg(hWnd, x, y);
	WDS_StrInputMsg(hWnd, lpStr, Delay);
}
//指定窗口，指定位置，鼠标双击并输入字符串
void WDS_LBtnDbClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay)
{
	WDS_LBtnDbClickMsg(hWnd, x, y);
	WDS_StrInputMsg(hWnd, lpStr, Delay);
}


//==========================前台按键相关函数====================================

//模拟前台鼠标在某绝对点按下
void WDS_LBtnClick(DWORD x, DWORD y)
{
	//使用绝对坐标,不然位移是相对当前鼠标所在点
	//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0);
	////这里是鼠标左键按下和松开两个事件的组合即一次单击：
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	//mouse_event(MOUSEEVENTF_LEFTDOWN, x * 65536 / 1280, y * 65536 / 800, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTUP, x * 65536 / 1280, y * 65536 / 800, 0, 0);

	SetCursorPos(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


// 设置剪贴板文本
bool WDS_SetClipBoardText(LPCTSTR text, HWND hWnd)
{
	//	ASSERT(hWnd);
	// 装入中文键盘布局，解决拷贝剪贴板的乱码问题
	//LoadKeyboardLayout(_T("00000804"), KLF_ACTIVATE | KLF_SETFORPROCESS);//0x0804:Chinese (PRC), e004:智能ABC
	//打开剪贴板   
	if (!::OpenClipboard(hWnd))
		return false;
	//empties the clipboard and frees handles to data in the clipboard   
	if (!EmptyClipboard())
	{
		CloseClipboard();
		return false;
	}
	//get text length   
	int len = _tcslen(text);
	//After SetClipboardData is called, the system owns the object identified by the hMem parameter.    
	//The application can read the data, but must not free the handle or leave it locked. If the    
	//hMem parameter identifies a memory object, the object must have been allocated using the    
	//GlobalAlloc function with the GMEM_MOVEABLE and GMEM_DDESHARE flags.    
	HANDLE hClip = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, (len + 1) * sizeof(TCHAR));
	if (hClip == NULL)
	{
		CloseClipboard();
		return false;
	}
	//locks a global memory object and returns a pointer to the first byte of the object's memory block   
	TCHAR* pBuf = (TCHAR*)GlobalLock(hClip);
	if (pBuf == NULL)
	{
		GlobalFree(hClip);
		CloseClipboard();
		return false;
	}

	memcpy(pBuf, text, len * sizeof(TCHAR));
	pBuf[len] = NULL;

	GlobalUnlock(hClip);
#ifdef _UNICODE
	if (NULL == SetClipboardData(CF_UNICODETEXT, hClip))//CF_TEXT为ANSI文本格式
#else
	if (NULL == SetClipboardData(CF_TEXT, hClip))//CF_TEXT为ANSI文本格式
#endif
	{
		GlobalFree(hClip);
		CloseClipboard();
		return false;
	}

	CloseClipboard();

	// 拷贝剪贴板完毕后，需装入英文键盘布局
	//LoadKeyboardLayout(_T("00000409"), KLF_ACTIVATE | KLF_SETFORPROCESS);//0x0409:English
	return true;
}
//重做keybd_event，实现ctrl+v的粘贴按键
void WDS_kbevt_Paste()
{
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event('V', 0, 0, 0);
	keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}
//自定义剪贴板并前台按键粘贴，以便输入字符到指定窗口,比如中文字符
void WDS_ClipBoard_Paste(LPCTSTR text, HWND hWnd)
{
	WDS_SetClipBoardText(text, hWnd);
	WDS_kbevt_Paste();
}
//重做keybd_event，实现前台键盘单个按键---目前用于密码
//输入iKey为大写字母的ascii码值,bCaps为大写开关
void WDS_kbevt_PressKey(int iKey, bool bCaps)
{
	if (bCaps)
	{
		keybd_event(VK_SHIFT, 0, 0, 0);
		keybd_event(iKey, 0, 0, 0);
		keybd_event(iKey, 0, KEYEVENTF_KEYUP, 0);
		keybd_event(VK_SHIFT, 0, KEYEVENTF_KEYUP, 0);
	}
	else
	{
		keybd_event(iKey, 0, 0, 0);
		keybd_event(iKey, 0, KEYEVENTF_KEYUP, 0);
	}
}
//重做keybd_event，实现前台键盘连续按键---目前用于密码框
//lpstr----输入的按键字符串，delay----按键延时
//输入为按键字符串，单个字符对应的值为ASCII码值,
//但keybd_event输入的是虚拟键码（不是ASCII码）对应键盘按键：
//----横排的"0-9"键:48-57,同ASCII
//----键盘的"A-Z"键:65-90,同ASCII，但默认是小写（ASCII的小写"a-z":97-122)，大写要上档
//----数字小键盘的“0-9”：96-105，与ASCII不同
//----数字小键盘的其他字符：*：106，+：107，Enter：108，-：109，.：110，/：111
//----功能键F1-F12:112-113, 
//----其他键：Backspace：8，Tab：9，其他查资料吧
//----"`〓~〓192"  "1〓!〓49"   "2〓@〓50"    "3〓#〓51"     "4〓$〓52"
//----"5〓%〓53"   "6〓^〓54"    "7〓&〓55"     "8〓*〓56"   "9〓(〓57"
//----"0〓)〓48"   "-〓_〓189"   "=〓+〓187"    "[〓{〓219"  "]〓}〓221"
//----"\〓|〓220"  ";〓:〓186"   "'〓""〓222"   ",〓<〓188"  ".〓>〓190"  "/〓?〓191"
void WDS_kbevt_PressKeyStr(LPCTSTR lpstr, long delay)
{
	int n = _tcslen(lpstr);
	for (int i = 0; i < n; i++)
	{
		if (65 <= lpstr[i] && lpstr[i] <= 90)//ASCII大写字母范围,需上档
		{
			WDS_kbevt_PressKey(lpstr[i], true);
		}
		else if (97 <= lpstr[i] && lpstr[i] <= 122)//ASCII小写字母,是按大写字母的值按键
		{
			WDS_kbevt_PressKey(lpstr[i] - 32, false);
		}
		else if (48 <= lpstr[i] && lpstr[i] <= 57)//ASCII数字
		{
			WDS_kbevt_PressKey(lpstr[i], false);
		}
		else
		{
			switch (lpstr[i])//支持其他字符(这里switch的是ASCII，需要转换成虚拟键码)
			{
				//注释掉小键盘的，免得与大键盘冲突
				//case '*':// *  ASCII
				//	WDS_kbevt_PressKey(106, false);//转换成虚拟键码
				//	break;
				//case '+':// + 
				//	WDS_kbevt_PressKey(107, false);
				//	break;
				//case '-':// -
				//	WDS_kbevt_PressKey(109, false);
				//	break;
				//case '.':// .
				//	WDS_kbevt_PressKey(110, false);
				//	break;
				//case '/':// /
				//	WDS_kbevt_PressKey(111, false);
				//	break;
			case '!'://ASCII
				WDS_kbevt_PressKey(49, true);//虚拟键码
				break;
			case '@':
				WDS_kbevt_PressKey(50, true);
				break;
			case '#':
				WDS_kbevt_PressKey(51, true);
				break;
			case '$':
				WDS_kbevt_PressKey(52, true);
				break;
			case '%':
				WDS_kbevt_PressKey(53, true);
				break;
			case '^':
				WDS_kbevt_PressKey(54, true);
				break;
			case '&':
				WDS_kbevt_PressKey(55, true);
				break;
			case '*':
				WDS_kbevt_PressKey(56, true);
				break;
			case '(':
				WDS_kbevt_PressKey(57, true);
				break;
			case ')':
				WDS_kbevt_PressKey(48, true);
				break;
			case '-':
				WDS_kbevt_PressKey(189, false);
				break;
			case '_':
				WDS_kbevt_PressKey(189, true);
				break;
			case '+':
				WDS_kbevt_PressKey(187, true);
				break;
			case '=':
				WDS_kbevt_PressKey(187, false);
				break;
			case '[':
				WDS_kbevt_PressKey(219, false);
				break;
			case '{':
				WDS_kbevt_PressKey(219, true);
				break;
			case ']':
				WDS_kbevt_PressKey(221, false);
				break;
			case '}':
				WDS_kbevt_PressKey(221, true);
				break;
			case '\\':
				WDS_kbevt_PressKey(220, false);
				break;
			case '|':
				WDS_kbevt_PressKey(220, true);
				break;
			case ';':
				WDS_kbevt_PressKey(186, false);
				break;
			case ':':
				WDS_kbevt_PressKey(186, true);
				break;
			case '\'':
				WDS_kbevt_PressKey(222, false);
				break;
			case '"':
				WDS_kbevt_PressKey(222, true);
				break;
			case ',':
				WDS_kbevt_PressKey(188, false);
				break;
			case '<':
				WDS_kbevt_PressKey(188, true);
				break;
			case '.':
				WDS_kbevt_PressKey(190, false);
				break;
			case '>':
				WDS_kbevt_PressKey(190, true);
				break;
			case '/':
				WDS_kbevt_PressKey(191, false);
				break;
			case '?':
				WDS_kbevt_PressKey(191, true);
				break;
			default:
				break;
			}
		}
		Sleep(delay);
	}
}



//==========================其他辅助相关函数====================================

//整数
void WDS_MessageBox(int lvar)
{
	CString str;
	str.Format(_T("%d"), lvar);
	AfxMessageBox(str);
}
void WDS_MessageBox(unsigned long lvar)
{
	CString str;
	str.Format(_T("%d"), lvar);
	AfxMessageBox(str);
}
//浮点数
void WDS_MessageBox(float lvar)
{
	CString str;
	str.Format(_T("%f"), lvar);
	AfxMessageBox(str);
}
//双精度浮点数
void WDS_MessageBox(double lvar)
{
	CString str;
	str.Format(_T("%f"), lvar);
	AfxMessageBox(str);
}
//字符串
void WDS_MessageBox(LPCTSTR lpStr)
{
	AfxMessageBox(lpStr);
}

// 将[字符串类型]的【16进制】转为[int类型]的【10进制】
int WDS_strHexToInt(TCHAR* strSource)
{
	CString strTemp = strSource;
	for (char cGZ = 'G', cgz = 'g'; cGZ <= 'Z', cgz <= 'z'; cGZ++, cgz++)    //判断输入的字符串是否合法    
	{
		if (strTemp.Find(cGZ, 0) != -1 || strTemp.Find(cgz, 0) != -1)
		{
			MessageBox(NULL, _T("请输入正确的16进制字符串!"), _T("输入错误"), MB_ICONEXCLAMATION);
			return -1;
		}
	}
	int nTemp = 0;// 存放转换后的int类数据
	int strNum = _tcslen(strSource);
	for (int i = 0; i < strNum; i++)
	{
		int nDecNum;
		switch (strSource[i])
		{
		case 'a':
		case 'A':   nDecNum = 10;   break;
		case 'b':
		case 'B':   nDecNum = 11;   break;
		case 'c':
		case 'C':   nDecNum = 12;   break;
		case 'd':
		case 'D':   nDecNum = 13;   break;
		case 'e':
		case 'E':   nDecNum = 14;   break;
		case 'f':
		case 'F':   nDecNum = 15;   break;
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':    nDecNum = strSource[i] - '0';     break;
		default:     return 0;
		}
		nTemp += nDecNum * (int)pow((float)16, (int)(strNum - i - 1)); // pow(x,y)即x的y次幂
	}
	return nTemp;
}
//----------------------------------------------------------------------------
// 以KEY_ALL_ACCESS权限读取注册表中的键值
//hKey----要打开的主键名, 
//lpSubKey----要打开的子键名
//lpValueName----键名
//lpValueData----键值,out
//返回值：-1为失败，其他为读出存储的数据类型(Registry Value Types:REG_SZ等)
int WDS_RegReadKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValueData)
{
	DWORD dwtype, sizeBuff;
	HKEY phkResult;// 得到的将要打开键的句柄
	if (ERROR_SUCCESS == RegOpenKeyEx(hKey, lpSubKey, 0, KEY_ALL_ACCESS, &phkResult))
	{
		if (ERROR_SUCCESS != RegQueryValueEx(phkResult, lpValueName, 0, &dwtype, (BYTE*)lpValueData, &sizeBuff))
		{
			RegCloseKey(phkResult);
			return -1;
		}
	}
	else
	{
		return -1;
	}

	RegCloseKey(phkResult);
	return dwtype;
}


LPCTSTR g_WndTitleFuzzy = NULL;//指定模糊查找的窗口名
LPCTSTR g_WndClassFuzzy = NULL;//指定模糊查找的类名
//回调函数
BOOL CALLBACK lpEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	HWND *WndHwndFuzzy = (HWND*)lParam;
	if (g_WndTitleFuzzy == NULL && g_WndClassFuzzy == NULL)
	{
		return 0;//退出枚举
	}

	static TCHAR szTitle[260];
	if (g_WndTitleFuzzy != NULL && g_WndClassFuzzy == NULL)//只给定窗口名
	{
		::GetWindowText(hwnd, szTitle, sizeof(szTitle));//获取窗口名
		CString strTitleBuf = szTitle;
		strTitleBuf.MakeLower();//对找到的窗口名转为小写

		CString strTitleFuzzyBuf = g_WndTitleFuzzy;
		strTitleFuzzyBuf.MakeLower();//对指定的窗口名转小写

		if (0 <= strTitleBuf.Find(strTitleFuzzyBuf)) //在找到的窗口名中模糊查找给定的窗口名
		{
			*WndHwndFuzzy = hwnd;
			g_WndTitleFuzzy = NULL;//指定模糊查找的窗口名
			g_WndClassFuzzy = NULL;//指定模糊查找的类名
			return 0;//找到退出
		}
	}
	static TCHAR szClass[260];
	if (g_WndTitleFuzzy == NULL && g_WndClassFuzzy != NULL)//只给定类名
	{
		::RealGetWindowClass(hwnd, szClass, sizeof(szClass));//获取类名

		CString strClassBuf = szClass;
		strClassBuf.MakeLower();

		CString strClassFuzzyBuf = g_WndClassFuzzy;
		strClassFuzzyBuf.MakeLower();

		if (0 <= strClassBuf.Find(strClassFuzzyBuf))//在找到的类名中模糊查找给定的类名
		{
			*WndHwndFuzzy = hwnd;
			g_WndTitleFuzzy = NULL;//指定模糊查找的窗口名
			g_WndClassFuzzy = NULL;//指定模糊查找的类名
			return 0;//找到退出
		}
	}

	if (g_WndTitleFuzzy != NULL && g_WndClassFuzzy != NULL)//给定类名和窗口名
	{
		::GetWindowText(hwnd, szTitle, sizeof(szTitle));//获取窗口名

		CString strTitleBuf = szTitle;
		strTitleBuf.MakeLower();//对找到的窗口名转为小写

		CString strTitleFuzzyBuf = g_WndTitleFuzzy;
		strTitleFuzzyBuf.MakeLower();//对指定的窗口名转小写

		if (0 <= strTitleBuf.Find(strTitleFuzzyBuf)) //匹配窗口名
		{
			::RealGetWindowClass(hwnd, szClass, sizeof(szClass));//获取类名

			CString strClassBuf = szClass;
			strClassBuf.MakeLower();

			CString strClassFuzzyBuf = g_WndClassFuzzy;
			strClassFuzzyBuf.MakeLower();

			if (0 <= strClassBuf.Find(strClassFuzzyBuf))//匹配类名
			{
				*WndHwndFuzzy = hwnd;
				g_WndTitleFuzzy = NULL;//指定模糊查找的窗口名
				g_WndClassFuzzy = NULL;//指定模糊查找的类名
				return 0;//找到退出
			}
		}
	}
	return 1;
}
//模糊查找窗口名
HWND WDS_FindWndFuzzy(LPCTSTR strClassName, LPCTSTR strTitle)
{
	g_WndClassFuzzy = strClassName;
	g_WndTitleFuzzy = strTitle;

	//枚举窗口
	HWND WndHwndFuzzy = 0;//模糊查找的结果窗口句柄
	EnumWindows(lpEnumWindowsProc, (LPARAM)&WndHwndFuzzy);//直到回调函数返回失败，才停止枚举，不然一直到找完所有窗口

	g_WndClassFuzzy = NULL;
	g_WndTitleFuzzy = NULL;

	return WndHwndFuzzy;
}


#include "Mmsystem.h"
#pragma comment(lib, "WINMM.LIB")
#include "Digitalv.h"//顺序需在Mmsystem.h下面
UINT g_ssDeviceID;
//播放音乐，strFile可全路径，也可只给文件名如".\\Alarm.mp3";
void playmusic(LPCTSTR strFile)
{
	TCHAR buf[128];

	MCI_OPEN_PARMS mciOpen;
	DWORD mciError;
	mciOpen.lpstrDeviceType = _T("mpegvideo");
	mciOpen.lpstrElementName = strFile;// ".\\Alarm.mp3";
	mciError = mciSendCommand(0, MCI_OPEN, MCI_OPEN_TYPE | MCI_OPEN_ELEMENT, (DWORD)&mciOpen);
	if (mciError)
	{
		mciGetErrorString(mciError, buf, 128);
		return;
	}
	g_ssDeviceID = mciOpen.wDeviceID;
	MCI_PLAY_PARMS   mciPlay;
	//MCI_DGV_PLAY_REPEAT, 要 #include "Digitalv.h"
	mciError = mciSendCommand(g_ssDeviceID, MCI_PLAY,/* MCI_WAIT | */MCI_DGV_PLAY_REPEAT,
		(DWORD)(LPMCI_PLAY_PARMS)&mciPlay);
	if (mciError)
	{
		mciGetErrorString(mciError, buf, 128);
		return;
	}
}




//------------------------------------------------------------------------------
// 配置文件类的实现

CConfigIni::CConfigIni(LPCTSTR fileName)
{
	// 取得当前程序目录
	::GetCurrentDirectory(MAX_PATH, m_FilePath.GetBuffer(MAX_PATH));
	m_FilePath.ReleaseBuffer();// 必须释放才能进行其他操作
	m_FilePath += "\\";
	m_FilePath += fileName;
}
DWORD CConfigIni::GetIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPSTR lpReturnedString)
{
	return GetPrivateProfileString(lpAppName, lpKeyName, NULL, lpReturnedString, MAX_PATH, m_FilePath);
}
UINT CConfigIni::GetIniInt(LPCSTR lpAppName, LPCSTR lpKeyName)
{
	return GetPrivateProfileInt(lpAppName, lpKeyName, -1, m_FilePath);
}
BOOL CConfigIni::WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString)
{
	return WritePrivateProfileString(lpAppName, lpKeyName, lpString, m_FilePath);
}
#include "Strsafe.h"
BOOL CConfigIni::WriteIniInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value)
{
	TCHAR ValBuf[16];
	StringCbPrintf(ValBuf, sizeof(ValBuf), TEXT("%i"), Value);
	return WritePrivateProfileString(lpAppName, lpKeyName, ValBuf, m_FilePath);
}




//=============================BASE64字符加解密================================
#include "GetMachineCode/BASE64/BASE64_API.h"
CString EncryptPsw(char *chPwd)//加密
{
	char Buffer[256] = { 0 };
	memcpy(Buffer, chPwd, strlen(chPwd));
	strcat_s(Buffer, 255, "2w5ds34wu"); 

	char chBase64[256] = { 0 };
	int i = strlen(Buffer);
	BASE64_Encode((BYTE*)Buffer,i, chBase64);

	chBase64[3] -= 3;
	return chBase64;
}
CString DecodePsw(char *chBase64)//解密
{
	chBase64[3] += 3;

	char chPwd[256] = { 0 };
	BASE64_Decode(chBase64, strlen(chBase64), (BYTE*)chPwd);
	int i = strlen(chPwd) - strlen("2w5ds34wu");
	chPwd[i] = '\0';

	return chPwd;
}