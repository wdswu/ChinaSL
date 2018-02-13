#include "stdafx.h"

#include "ChinaSL.h"
#include "CCslAdo.h"
#include "WDS_COMMON.h"

//��־�ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strLogName;
//�����ļ�����Ψһ(ChinaSLDlg.CPP)
extern CString g_strCfgName;

//�ؼ���֤����,ΨһVerificationData.cpp
extern CParseVerificationData g_VfcData;

CCslAdo::CCslAdo()
{
}

CCslAdo::~CCslAdo()
{
	//ExitADOConnect();
}

//��ӡ����ADO�ؼ�ʱ��������ϸ������Ϣ
void CCslAdo::dump_com_error(_com_error & e)
{
	CString ErrorStr;
	_bstr_t bstrSource(e.Source());
	_bstr_t bstrDescription(e.Description());
	ErrorStr.Format("\nADO Error\nCode = %08lx\nCode meaning = %s\nSource = %s\nDescription = %s\n", e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription);
	//ErrorStr.Format("ADO Error:\tCode = %08lx\tCode meaning = %s\tSource = %s\tDescription = %s\r\n", e.Error(), e.ErrorMessage(), (LPCTSTR)bstrSource, (LPCTSTR)bstrDescription);
	//�ڵ��Դ����д�ӡ������Ϣ,��Release���п���DBGView�鿴������Ϣ
	::OutputDebugString((LPCTSTR)ErrorStr);

	CStdioFile file;
	if (file.Open(g_strLogName, CFile::modeCreate | CFile::modeNoTruncate/*������ɾ������*/ | CFile::modeWrite))
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
		AfxMessageBox("��ʼ��COMʧ��!");
		return false;
	}
	//���������ַ���
	CString strConnect;
#if WDS_ADMIN
	//��ȡini�ļ��е�����
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
		//�������Ӷ���ʵ�� 
		m_pConnection.CreateInstance("ADODB.Connection");
		//ʹ��Open�����������ݿ�
		m_pConnection->Open((_bstr_t)strConnect, "", "", Options);
	}
	catch (_com_error e)
	{
		AfxMessageBox("�������ݿ�ʧ��");
		dump_com_error(e);
		return false;
	}

	VMProtectEnd();

	return true;

}

void CCslAdo::ExitADOConnect()
{
	/*�ر����ݿⲢ�ͷ�ָ��*/
	try
	{
		m_pRecordset->Close();     //�رռ�¼��
		m_pConnection->Close();//�ر����ݿ�
		m_pRecordset.Release();//�ͷż�¼������ָ��
		m_pConnection.Release();//�ͷ����Ӷ���ָ��
	}
	catch (_com_error e)
	{
		dump_com_error(e);
	}

}
