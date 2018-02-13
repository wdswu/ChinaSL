#include "stdafx.h"

#include "ChinaSL.h"
#include "CCslAdo.h"
#include "WDS_COMMON.h"

//日志文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strLogName;
//配置文件名，唯一(ChinaSLDlg.CPP)
extern CString g_strCfgName;

//关键验证数据,唯一VerificationData.cpp
extern CParseVerificationData g_VfcData;

CCslAdo::CCslAdo()
{
}

CCslAdo::~CCslAdo()
{
	//ExitADOConnect();
}

//打印调用ADO控件时产生的详细错误信息
void CCslAdo::dump_com_error(_com_error & e)
{
	CString ErrorStr;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format("\nADO Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s\n", e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription);
	//ErrorStr.Format("ADO Error:\tCode = %08lx\tCode meaning = %s\tSource = %s\tDescription = %s\r\n", e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription);
	//在调试窗口中打印错误信息,在Release版中可用DBGView查看错误信息
	::OutputDebugString((LPCTSTR)ErrorStr);

	CStdioFile file;
	if (file.Open(g_strLogName, CFile::modeCreate | CFile::modeNoTruncate/*存在则不删除内容*/ | CFile::modeWrite))
	{
		file.SeekToEnd();
		file.WriteString(ErrorStr);
	}
	file.Close();

#ifdef _DEBUG
	AfxMessageBox(ErrorStr, MB_OK | MB_ICONERROR);
#endif	
}

bool CCslAdo::InitADOConnect(long Options)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF InitADOConnect");

	if (FAILED(CoInitialize(NULL)))
	{
		AfxMessageBox("初始化COM失败!");
		return false;
	}
	//设置连接字符串
	CString strConnect;
#if WDS_ADMIN
	//读取ini文件中的配置
	CConfigIni cfgIni(g_strCfgName);
	cfgIni.GetIniString("ADOConnect", "server", m_server);
	cfgIni.GetIniString("ADOConnect", "database", m_database);
	cfgIni.GetIniString("ADOConnect", "uid", m_uid);
	char strPwd[256] = { 0 };
	cfgIni.GetIniString("ADOConnect", "pwd", strPwd);

	CString strpwd2 = DecodePsw(strPwd);
	memcpy(m_pwd, strpwd2, 49);

	strConnect.Format("Provider=SQLOLEDB.1;Server=%s;Database=%s;uid=%s;pwd=%s;", m_server, m_database, m_uid, m_pwd);
#else
	strConnect.Format("Provider=SQLOLEDB.1;Server=%s;Database=%s;uid=%s;pwd=%s;", g_VfcData.m_pKeyData->server, g_VfcData.m_pKeyData->database, g_VfcData.m_pKeyData->uid, g_VfcData.m_pKeyData->pwd);
#endif
	try
	{
		m_pRecordset.CreateInstance("ADO.Record");
		//创建连接对象实例 
		m_pConnection.CreateInstance("ADODB.Connection");
		//使用Open方法连接数据库
		m_pConnection->Open((_bstr_t)strConnect, "", "", Options);
	}
	catch (_com_error e)
	{
		AfxMessageBox("连接数据库失败");
		dump_com_error(e);
		return false;
	}

	VMProtectEnd();

	return true;

}

void CCslAdo::ExitADOConnect()
{
	/*关闭数据库并释放指针*/
	try
	{
		m_pRecordset->Close();     //关闭记录集
		m_pConnection->Close();//关闭数据库
		m_pRecordset.Release();//释放记录集对象指针
		m_pConnection.Release();//释放连接对象指针
	}
	catch (_com_error e)
	{
		dump_com_error(e);
	}

}
