//================================================================
// 文件名:WDS_COMMON.h
// 说明:wdswu自定义的一些以WDS_开头的函数
// 作者:晓梦迷蝶 wdswu@163.com
// 创建时间：2015.08.07
// 最后更新时间：2017.10.19
//================================================================

#pragma once
#include "Mmsystem.h"


//==========================后台消息相关函数====================================

//鼠标左键单击消息
void WDS_LBtnClickMsg(HWND hwnd, WORD x, WORD y);
//鼠标左键双击消息
void WDS_LBtnDbClickMsg(HWND hWnd, WORD x, WORD y);
//使用向指定窗口发消息的方式输入字符串（支持部分窗口中文字符）
void WDS_StrInputMsg(HWND hWnd, LPCTSTR lpStr, long Delay = 100);
//指定窗口，指定位置，鼠标单击并输入字符串
void WDS_LBtnClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay = 100);
//指定窗口，指定位置，鼠标双击并输入字符串
void WDS_LBtnDbClickStrInputMsg(HWND hWnd, LPCTSTR lpStr, WORD x, WORD y, long Delay = 100);



//==========================前台按键相关函数====================================
//模拟前台鼠标在某绝对点按下
void WDS_LBtnClick(DWORD x, DWORD y);
bool WDS_SetClipBoardText(LPCTSTR text, HWND hWnd = NULL);
//重做keybd_event，实现ctrl+v的粘贴按键
void WDS_kbevt_Paste();
//自定义剪贴板粘贴，以便输入字符到游戏,比如中文字符
void WDS_ClipBoard_Paste(LPCTSTR text, HWND hWnd);
//重做keybd_event，实现键盘单个按键---目前用于密码
//输入iKey为大写字母的ascii码值,bCaps为大写开关
void WDS_kbevt_PressKey(int iKey, bool bCaps = false);
//重做keybd_event，实现键盘连续按键---目前用于密码框
//lpstr----输入的按键字符串，delay----按键延时
//输入为按键字符串，单个字符对应的值为ASCII码值
void WDS_kbevt_PressKeyStr(LPCTSTR lpstr, long delay = 100);



//==========================其他辅助相关函数====================================

//整数
void WDS_MessageBox(int lvar);
void WDS_MessageBox(unsigned long lvar);
//双精度浮点数
void WDS_MessageBox(double lvar);
//浮点数
void WDS_MessageBox(float lvar);
//字符串
void WDS_MessageBox(LPCTSTR lpStr);
// 将[字符串类型]的【16进制】转为[int类型]的【10进制】
int WDS_strHexToInt(TCHAR* strSource);
//----------------------------------------------------------------------------
// WDS_RegReadKeyValue()以KEY_ALL_ACCESS权限读取注册表中的键值
//hKey----要打开的主键名, 
//lpSubKey----要打开的子键名
//lpValueName----键名
//lpValueData----键值
//返回值：-1为失败，其他为读出存储的数据类型(Registry Value Types:REG_SZ等)
int WDS_RegReadKeyValue(HKEY hKey, LPCTSTR lpSubKey, LPCTSTR lpValueName, LPCTSTR lpValueData);
//模糊查找窗口
HWND WDS_FindWndFuzzy(LPCTSTR strClassName, LPCTSTR strTitle);



//播放音乐
void playmusic(LPCTSTR strFile);
extern UINT g_ssDeviceID;

//------------------------------------------------------------------------------
// 配置文件类的申明
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