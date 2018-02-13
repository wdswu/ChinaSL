//================================================================
// �ļ���:VerificationData.cpp
// ˵��:wdswu�Զ�����Ϸ�ؼ���֤�����࣬�����xml�ı���ȡ�ؼ�����
// ����:�����Ե� wdswu@163.com
// ����ʱ��:2016.07.12
// ������ʱ�䣺2016.07.12
//================================================================
#include "stdafx.h"

#include "VerificationData.h"

#import "msxml3.dll"
using namespace MSXML2;

//��ʾ״̬��Ϣ�Ի���UI�̵߳�ID��ΨһWuXia.cpp��
extern DWORD g_dwShowStateThreadID;

//�ؼ���֤����,ΨһVerificationData.cpp
CParseVerificationData g_VfcData;

//ȡBSPHP��̨��֤����--�������룬���ı�xmlText����
void CParseVerificationData::ParseVerificationData(CString xmlText, CString xmlFileName)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF GetVerificationData()");

	CString strData;
	if (xmlText != _T(""))
	{
		//BSPHP��̨��֤�ı���������xml��ǣ��������������,����Elementͷ������ encoding=\"GB2312\"
		strData.Format(_T("<?xml version='1.0' encoding='GB2312'?><DataRoot>%s</DataRoot>"), xmlText);
	}

	//newһ������ParseXML()�ڲ�ʹ��
	m_pXmlData = new PARSE_XML_DATA[GAME_KEY_DATA_NUM];

	ParseXML(strData, xmlFileName);

	GetGameKeyData();

	delete[]m_pXmlData;

	VMProtectEnd();
}

//����XML�ı����ļ���new����m_pXmlData����--�������룬���ı�xmlText����
void CParseVerificationData::ParseXML(/*_bstr_t*/CString xmlText, CString xmlFileName)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF ParseXML()");

	MSXML2::IXMLDOMDocumentPtr xmldoc;
	HRESULT hr;
	hr = CoInitialize(NULL);//�����ʼ������������CreateInstance�����ĵ�Ҫʧ��
	if (FAILED(hr))//�˴������FAILED����SUCCEEDED���жϣ������������δ֪����
	{
		AfxMessageBox("��ʼ��COMʧ��");
	}
	hr = xmldoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if (FAILED(hr))
	{
		AfxMessageBox("����xml�ĵ�����ʧ��");
	}

	if (xmlText != _T(""))
	{
		//����XML�ı�
		xmldoc->loadXML((_bstr_t)xmlText);
	}
	else if (xmlFileName != _T(""))
	{
		//����XML�ļ�
		xmldoc->load((_bstr_t)xmlFileName);
	}
	else
	{
		AfxMessageBox("������XML�ı�����XML�ļ�·��");
	}

	MSXML2::IXMLDOMElementPtr root;
	MSXML2::IXMLDOMNodeListPtr nodelist;
	MSXML2::IXMLDOMNodeListPtr childnodelist;

	root = xmldoc->GetdocumentElement();
	//root->GettagName()���Ի�ȡ���������,��"DataRoot"
	//AfxMessageBox("1"+root->GettagName());//�������
	nodelist = root->GetchildNodes();

	MSXML2::IXMLDOMNodePtr node;
	MSXML2::IXMLDOMNodePtr childnode;

	for (int i = 0; i < nodelist->length; i++)
	{
		node = nodelist->Getitem(i);
		//node->GetbaseName()��ȡ���ݽ�����ƣ���"dmKey"
		m_pXmlData[i].strNodeName = (LPCTSTR)node->GetbaseName();
		childnodelist = node->GetchildNodes();
		for (int j = 0; j < childnodelist->length; j++)
		{
			childnode = childnodelist->Getitem(j);
			//��ȡ���ݽ����ı�����
			//AfxMessageBox(childnode->Gettext());
			m_pXmlData[i].strNodeData = (LPCTSTR)childnode->Gettext();
		}
	}
	//::CoUninitialize();

	VMProtectEnd();
}

//���ݷ�������xml�ı����������֣�ȡ��m_pXmlData�����еĹؼ����ݣ�����m_pKeyData�ṹ
void CParseVerificationData::GetGameKeyData()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF GetGameKeyData()");

	//��ȡ�õ����ݸ��Ƶ���Ӧ�ı���
	m_pKeyData->server = GetData(_T("server"));
	m_pKeyData->database = GetData(_T("database"));
	m_pKeyData->uid = GetData(_T("uid"));
	m_pKeyData->pwd = GetData(_T("pwd"));

	//AfxMessageBox(m_pKeyData->dmKey +_T("\r\n")+ m_pKeyData->dmKeypad);

	VMProtectEnd();
}
//�������ִ�����ȡ�ؼ�����
CString CParseVerificationData::GetData(CString strName)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF GetData()");

	for (int i = 0; i < GAME_KEY_DATA_NUM; i++)
	{
		if (m_pXmlData[i].strNodeName == strName)
		{
			return m_pXmlData[i].strNodeData;
		}
	}
	VMProtectEnd();
	
	return _T("");
}
