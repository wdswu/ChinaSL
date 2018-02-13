//================================================================
// �ļ���:WDS_COMMON.h
// ˵��:wdswu�Զ����һЩ��WDS_��ͷ�ĺ���
// ����:�����Ե� wdswu@163.com
// ����ʱ�䣺2015.08.07
// ������ʱ�䣺2017.10.19
//================================================================

#pragma once
#include "Mmsystem.h"


//==========================��̨��Ϣ��غ���====================================

//������������Ϣ
void WDS_LBtnClickMsg(HWND hwnd, WORD x, WORD y);
//������˫����Ϣ
void WDS_LBtnDbClickMsg(HWND hWnd, WORD x, WORD y);
//ʹ����ָ�����ڷ���Ϣ�ķ�ʽ�����ַ�����֧�ֲ��ִ��������ַ���
void WDS_StrInputMsg(HWND hWnd, LPCTSTR lpStr, long Delay = 100);
//ָ�����ڣ�ָ��λ�ã���굥���������ַ���
void WDS_LBtnClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay = 100);
//ָ�����ڣ�ָ��λ�ã����˫���������ַ���
void WDS_LBtnDbClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay = 100);



//==========================ǰ̨������غ���====================================
//ģ��ǰ̨�����ĳ���Ե㰴��
void WDS_LBtnClick(DWORD x, DWORD y);
bool WDS_SetClipBoardText(LPCTSTR text, HWND hWnd = NULL);
//����keybd_event��ʵ��ctrl+v��ճ������
void WDS_kbevt_Paste();
//�Զ��������ճ�����Ա������ַ�����Ϸ,���������ַ�
void WDS_ClipBoard_Paste(LPCTSTR text, HWND hWnd);
//����keybd_event��ʵ�ּ��̵�������---Ŀǰ��������
//����iKeyΪ��д��ĸ��ascii��ֵ,bCapsΪ��д����
void WDS_kbevt_PressKey(int iKey, bool bCaps = false);
//����keybd_event��ʵ�ּ�����������---Ŀǰ���������
//lpstr----����İ����ַ�����delay----������ʱ
//����Ϊ�����ַ����������ַ���Ӧ��ֵΪASCII��ֵ
void WDS_kbevt_PressKeyStr(LPCTSTR lpstr, long delay = 100);



//==========================����������غ���====================================

//����
void WDS_MessageBox(int lvar);
void WDS_MessageBox(unsigned long lvar);
//˫���ȸ�����
void WDS_MessageBox(double lvar);
//������
void WDS_MessageBox(float lvar);
//�ַ���
void WDS_MessageBox(LPCTSTR lpStr);
// ��[�ַ�������]�ġ�16���ơ�תΪ[int����]�ġ�10���ơ�
int WDS_strHexToInt(TCHAR* strSource);
//----------------------------------------------------------------------------
// WDS_RegReadKeyValue()��KEY_ALL_ACCESSȨ�޶�ȡע����еļ�ֵ
//hKey----Ҫ�򿪵�������, 
//lpSubKey----Ҫ�򿪵��Ӽ���
//lpValueName----����
//lpValueData----��ֵ
//����ֵ��-1Ϊʧ�ܣ�����Ϊ�����洢����������(Registry Value Types:REG_SZ��)
int WDS_RegReadKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValueData);
//ģ�����Ҵ���
HWND WDS_FindWndFuzzy(LPCTSTR strClassName, LPCTSTR strTitle);



//��������
void playmusic(LPCTSTR strFile);
extern UINT g_ssDeviceID;

//------------------------------------------------------------------------------
// �����ļ��������
class CConfigIni
{
public:
	CString m_FilePath;
public:
	CConfigIni(LPCTSTR fileName = "config.ini");
public:
	DWORD GetIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPSTR lpReturnedString);
	UINT GetIniInt(LPCSTR lpAppName, LPCSTR lpKeyName);
	BOOL WriteIniString(LPCTSTR lpAppName, LPCTSTR lpKeyName, LPCTSTR lpString);
	BOOL WriteIniInt(LPCTSTR lpAppName, LPCTSTR lpKeyName, INT Value);
};




CString EncryptPsw(char *str);
CString DecodePsw(char *str);