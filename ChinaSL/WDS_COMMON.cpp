//================================================================
// �ļ���:WDS_COMMON.cpp
// ˵��:wdswu�Զ����һЩ��WDS_��ͷ�ĺ���
// ����:�����Ե� wdswu@163.com
// ����ʱ�䣺2015.08.07
// ������ʱ�䣺2017.11.18
//================================================================
#include "stdafx.h"

#include "WDS_COMMON.h"


#include <math.h>//����WDS_strHexToInt()




//==========================��̨��Ϣ��غ���====================================
//ָ�����ڣ�ָ��λ�ã�������������Ϣ
void WDS_LBtnClickMsg(HWND hWnd, WORD x, WORD y)
{
	PostMessage(hWnd, WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(x, y));
	Sleep(50);
	PostMessage(hWnd, WM_LBUTTONUP, 0, MAKELPARAM(x, y));
	Sleep(50);
}
//ָ�����ڣ�ָ��λ�ã�������˫����Ϣ
void WDS_LBtnDbClickMsg(HWND hWnd, WORD x, WORD y)
{
	PostMessage(hWnd, WM_LBUTTONDBLCLK, MK_LBUTTON, MAKELPARAM(x, y));
	Sleep(50);
}
//ʹ����ָ�����ڷ���Ϣ�ķ�ʽ�����ַ��������ִ��������ַ�Ҳ�ɣ�
//hWnd----������Ϣ�Ĵ��ھ��
//lpStr----Ҫ���͵��ַ���
//Delay----ÿ���ַ�֮��ķ�����ʱ
void WDS_StrInputMsg(HWND hWnd, LPCTSTR lpStr, long Delay)
{
	while (*lpStr != 0)
	{
		PostMessage(hWnd, WM_CHAR, *lpStr, 0);
		lpStr++;
		Sleep(Delay);
	}
}
//ָ�����ڣ�ָ��λ�ã���굥���������ַ���
void WDS_LBtnClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay)
{
	WDS_LBtnClickMsg(hWnd, x, y);
	WDS_StrInputMsg(hWnd, lpStr, Delay);
}
//ָ�����ڣ�ָ��λ�ã����˫���������ַ���
void WDS_LBtnDbClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay)
{
	WDS_LBtnDbClickMsg(hWnd, x, y);
	WDS_StrInputMsg(hWnd, lpStr, Delay);
}


//==========================ǰ̨������غ���====================================

//ģ��ǰ̨�����ĳ���Ե㰴��
void WDS_LBtnClick(DWORD x, DWORD y)
{
	//ʹ�þ�������,��Ȼλ������Ե�ǰ������ڵ�
	//mouse_event(MOUSEEVENTF_ABSOLUTE | MOUSEEVENTF_MOVE, x, y, 0, 0);
	////���������������º��ɿ������¼�����ϼ�һ�ε�����
	//mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);

	//mouse_event(MOUSEEVENTF_LEFTDOWN, x * 65536 / 1280, y * 65536 / 800, 0, 0);
	//mouse_event(MOUSEEVENTF_LEFTUP, x * 65536 / 1280, y * 65536 / 800, 0, 0);

	SetCursorPos(x, y);
	mouse_event(MOUSEEVENTF_LEFTDOWN | MOUSEEVENTF_LEFTUP, 0, 0, 0, 0);
}


// ���ü������ı�
bool WDS_SetClipBoardText(LPCTSTR text, HWND hWnd)
{
	//	ASSERT(hWnd);
	// װ�����ļ��̲��֣�����������������������
	//LoadKeyboardLayout(_T("00000804"), KLF_ACTIVATE | KLF_SETFORPROCESS);//0x0804:Chinese (PRC), e004:����ABC
	//�򿪼�����   
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
	if (NULL == SetClipboardData(CF_UNICODETEXT, hClip))//CF_TEXTΪANSI�ı���ʽ
#else
	if (NULL == SetClipboardData(CF_TEXT, hClip))//CF_TEXTΪANSI�ı���ʽ
#endif
	{
		GlobalFree(hClip);
		CloseClipboard();
		return false;
	}

	CloseClipboard();

	// ������������Ϻ���װ��Ӣ�ļ��̲���
	//LoadKeyboardLayout(_T("00000409"), KLF_ACTIVATE | KLF_SETFORPROCESS);//0x0409:English
	return true;
}
//����keybd_event��ʵ��ctrl+v��ճ������
void WDS_kbevt_Paste()
{
	keybd_event(VK_CONTROL, 0, 0, 0);
	keybd_event('V', 0, 0, 0);
	keybd_event('V', 0, KEYEVENTF_KEYUP, 0);
	keybd_event(VK_CONTROL, 0, KEYEVENTF_KEYUP, 0);
}
//�Զ�������岢ǰ̨����ճ�����Ա������ַ���ָ������,���������ַ�
void WDS_ClipBoard_Paste(LPCTSTR text, HWND hWnd)
{
	WDS_SetClipBoardText(text, hWnd);
	WDS_kbevt_Paste();
}
//����keybd_event��ʵ��ǰ̨���̵�������---Ŀǰ��������
//����iKeyΪ��д��ĸ��ascii��ֵ,bCapsΪ��д����
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
//����keybd_event��ʵ��ǰ̨������������---Ŀǰ���������
//lpstr----����İ����ַ�����delay----������ʱ
//����Ϊ�����ַ����������ַ���Ӧ��ֵΪASCII��ֵ,
//��keybd_event�������������루����ASCII�룩��Ӧ���̰�����
//----���ŵ�"0-9"��:48-57,ͬASCII
//----���̵�"A-Z"��:65-90,ͬASCII����Ĭ����Сд��ASCII��Сд"a-z":97-122)����дҪ�ϵ�
//----����С���̵ġ�0-9����96-105����ASCII��ͬ
//----����С���̵������ַ���*��106��+��107��Enter��108��-��109��.��110��/��111
//----���ܼ�F1-F12:112-113, 
//----��������Backspace��8��Tab��9�����������ϰ�
//----"`��~��192"  "1��!��49"   "2��@��50"    "3��#��51"     "4��$��52"
//----"5��%��53"   "6��^��54"    "7��&��55"     "8��*��56"   "9��(��57"
//----"0��)��48"   "-��_��189"   "=��+��187"    "[��{��219"  "]��}��221"
//----"\��|��220"  ";��:��186"   "'��""��222"   ",��<��188"  ".��>��190"  "/��?��191"
void WDS_kbevt_PressKeyStr(LPCTSTR lpstr, long delay)
{
	int n = _tcslen(lpstr);
	for (int i = 0; i < n; i++)
	{
		if (65 <= lpstr[i] && lpstr[i] <= 90)//ASCII��д��ĸ��Χ,���ϵ�
		{
			WDS_kbevt_PressKey(lpstr[i], true);
		}
		else if (97 <= lpstr[i] && lpstr[i] <= 122)//ASCIIСд��ĸ,�ǰ���д��ĸ��ֵ����
		{
			WDS_kbevt_PressKey(lpstr[i] - 32, false);
		}
		else if (48 <= lpstr[i] && lpstr[i] <= 57)//ASCII����
		{
			WDS_kbevt_PressKey(lpstr[i], false);
		}
		else
		{
			switch (lpstr[i])//֧�������ַ�(����switch����ASCII����Ҫת�����������)
			{
				//ע�͵�С���̵ģ���������̳�ͻ
				//case '*':// *  ASCII
				//	WDS_kbevt_PressKey(106, false);//ת�����������
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
				WDS_kbevt_PressKey(49, true);//�������
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



//==========================����������غ���====================================

//����
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
//������
void WDS_MessageBox(float lvar)
{
	CString str;
	str.Format(_T("%f"), lvar);
	AfxMessageBox(str);
}
//˫���ȸ�����
void WDS_MessageBox(double lvar)
{
	CString str;
	str.Format(_T("%f"), lvar);
	AfxMessageBox(str);
}
//�ַ���
void WDS_MessageBox(LPCTSTR lpStr)
{
	AfxMessageBox(lpStr);
}

// ��[�ַ�������]�ġ�16���ơ�תΪ[int����]�ġ�10���ơ�
int WDS_strHexToInt(TCHAR* strSource)
{
	CString strTemp = strSource;
	for (char cGZ = 'G', cgz = 'g'; cGZ <= 'Z', cgz <= 'z'; cGZ++, cgz++)    //�ж�������ַ����Ƿ�Ϸ�    
	{
		if (strTemp.Find(cGZ, 0) != -1 || strTemp.Find(cgz, 0) != -1)
		{
			MessageBox(NULL, _T("��������ȷ��16�����ַ���!"), _T("�������"), MB_ICONEXCLAMATION);
			return -1;
		}
	}
	int nTemp = 0;// ���ת�����int������
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
		nTemp += nDecNum * (int)pow((float)16, (int)(strNum - i - 1)); // pow(x,y)��x��y����
	}
	return nTemp;
}
//----------------------------------------------------------------------------
// ��KEY_ALL_ACCESSȨ�޶�ȡע����еļ�ֵ
//hKey----Ҫ�򿪵�������, 
//lpSubKey----Ҫ�򿪵��Ӽ���
//lpValueName----����
//lpValueData----��ֵ,out
//����ֵ��-1Ϊʧ�ܣ�����Ϊ�����洢����������(Registry Value Types:REG_SZ��)
int WDS_RegReadKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValueData)
{
	DWORD dwtype, sizeBuff;
	HKEY phkResult;// �õ��Ľ�Ҫ�򿪼��ľ��
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


LPCTSTR g_WndTitleFuzzy = NULL;//ָ��ģ�����ҵĴ�����
LPCTSTR g_WndClassFuzzy = NULL;//ָ��ģ�����ҵ�����
//�ص�����
BOOL CALLBACK lpEnumWindowsProc(HWND hwnd, LPARAM lParam)
{
	HWND *WndHwndFuzzy = (HWND*)lParam;
	if (g_WndTitleFuzzy == NULL && g_WndClassFuzzy == NULL)
	{
		return 0;//�˳�ö��
	}

	static TCHAR szTitle[260];
	if (g_WndTitleFuzzy != NULL && g_WndClassFuzzy == NULL)//ֻ����������
	{
		::GetWindowText(hwnd, szTitle, sizeof(szTitle));//��ȡ������
		CString strTitleBuf = szTitle;
		strTitleBuf.MakeLower();//���ҵ��Ĵ�����תΪСд

		CString strTitleFuzzyBuf = g_WndTitleFuzzy;
		strTitleFuzzyBuf.MakeLower();//��ָ���Ĵ�����תСд

		if (0 <= strTitleBuf.Find(strTitleFuzzyBuf)) //���ҵ��Ĵ�������ģ�����Ҹ����Ĵ�����
		{
			*WndHwndFuzzy = hwnd;
			g_WndTitleFuzzy = NULL;//ָ��ģ�����ҵĴ�����
			g_WndClassFuzzy = NULL;//ָ��ģ�����ҵ�����
			return 0;//�ҵ��˳�
		}
	}
	static TCHAR szClass[260];
	if (g_WndTitleFuzzy == NULL && g_WndClassFuzzy != NULL)//ֻ��������
	{
		::RealGetWindowClass(hwnd, szClass, sizeof(szClass));//��ȡ����

		CString strClassBuf = szClass;
		strClassBuf.MakeLower();

		CString strClassFuzzyBuf = g_WndClassFuzzy;
		strClassFuzzyBuf.MakeLower();

		if (0 <= strClassBuf.Find(strClassFuzzyBuf))//���ҵ���������ģ�����Ҹ���������
		{
			*WndHwndFuzzy = hwnd;
			g_WndTitleFuzzy = NULL;//ָ��ģ�����ҵĴ�����
			g_WndClassFuzzy = NULL;//ָ��ģ�����ҵ�����
			return 0;//�ҵ��˳�
		}
	}

	if (g_WndTitleFuzzy != NULL && g_WndClassFuzzy != NULL)//���������ʹ�����
	{
		::GetWindowText(hwnd, szTitle, sizeof(szTitle));//��ȡ������

		CString strTitleBuf = szTitle;
		strTitleBuf.MakeLower();//���ҵ��Ĵ�����תΪСд

		CString strTitleFuzzyBuf = g_WndTitleFuzzy;
		strTitleFuzzyBuf.MakeLower();//��ָ���Ĵ�����תСд

		if (0 <= strTitleBuf.Find(strTitleFuzzyBuf)) //ƥ�䴰����
		{
			::RealGetWindowClass(hwnd, szClass, sizeof(szClass));//��ȡ����

			CString strClassBuf = szClass;
			strClassBuf.MakeLower();

			CString strClassFuzzyBuf = g_WndClassFuzzy;
			strClassFuzzyBuf.MakeLower();

			if (0 <= strClassBuf.Find(strClassFuzzyBuf))//ƥ������
			{
				*WndHwndFuzzy = hwnd;
				g_WndTitleFuzzy = NULL;//ָ��ģ�����ҵĴ�����
				g_WndClassFuzzy = NULL;//ָ��ģ�����ҵ�����
				return 0;//�ҵ��˳�
			}
		}
	}
	return 1;
}
//ģ�����Ҵ�����
HWND WDS_FindWndFuzzy(LPCTSTR strClassName, LPCTSTR strTitle)
{
	g_WndClassFuzzy = strClassName;
	g_WndTitleFuzzy = strTitle;

	//ö�ٴ���
	HWND WndHwndFuzzy = 0;//ģ�����ҵĽ�����ھ��
	EnumWindows(lpEnumWindowsProc, (LPARAM)&WndHwndFuzzy);//ֱ���ص���������ʧ�ܣ���ֹͣö�٣���Ȼһֱ���������д���

	g_WndClassFuzzy = NULL;
	g_WndTitleFuzzy = NULL;

	return WndHwndFuzzy;
}


#include "Mmsystem.h"
#pragma comment(lib, "WINMM.LIB")
#include "Digitalv.h"//˳������Mmsystem.h����
UINT g_ssDeviceID;
//�������֣�strFile��ȫ·����Ҳ��ֻ���ļ�����".\\Alarm.mp3";
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
	//MCI_DGV_PLAY_REPEAT, Ҫ #include "Digitalv.h"
	mciError = mciSendCommand(g_ssDeviceID, MCI_PLAY,/* MCI_WAIT | */MCI_DGV_PLAY_REPEAT,
		(DWORD)(LPMCI_PLAY_PARMS)&mciPlay);
	if (mciError)
	{
		mciGetErrorString(mciError, buf, 128);
		return;
	}
}




//------------------------------------------------------------------------------
// �����ļ����ʵ��

CConfigIni::CConfigIni(LPCTSTR fileName)
{
	// ȡ�õ�ǰ����Ŀ¼
	::GetCurrentDirectory(MAX_PATH, m_FilePath.GetBuffer(MAX_PATH));
	m_FilePath.ReleaseBuffer();// �����ͷŲ��ܽ�����������
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




//=============================BASE64�ַ��ӽ���================================
#include "GetMachineCode/BASE64/BASE64_API.h"
CString EncryptPsw(char *chPwd)//����
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
CString DecodePsw(char *chBase64)//����
{
	chBase64[3] += 3;

	char chPwd[256] = { 0 };
	BASE64_Decode(chBase64, strlen(chBase64), (BYTE*)chPwd);
	int i = strlen(chPwd) - strlen("2w5ds34wu");
	chPwd[i] = '\0';

	return chPwd;
}