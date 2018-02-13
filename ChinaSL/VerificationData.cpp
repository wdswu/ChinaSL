//================================================================
// 文件名:VerificationData.cpp
// 说明:wdswu自定义游戏关键验证数据类，其解析xml文本获取关键数据
// 作者:晓梦迷蝶 wdswu@163.com
// 创建时间:2016.07.12
// 最后更新时间：2016.07.12
//================================================================
#include "stdafx.h"

#include "VerificationData.h"

#import "msxml3.dll"
using namespace MSXML2;

//显示状态信息对话框UI线程的ID（唯一WuXia.cpp）
extern DWORD g_dwShowStateThreadID;

//关键验证数据,唯一VerificationData.cpp
CParseVerificationData g_VfcData;

//取BSPHP后台验证数据--若都输入，则文本xmlText优先
void CParseVerificationData::ParseVerificationData(CString xmlText, CString xmlFileName)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF GetVerificationData()");

	CString strData;
	if (xmlText != _T(""))
	{
		//BSPHP后台验证文本不能输入xml标记，所以这里添加上,并将Element头放这里 encoding=\"GB2312\"
		strData.Format(_T("<?xml version='1.0' encoding='GB2312'?><DataRoot>%s</DataRoot>"), xmlText);
	}

	//new一个，在ParseXML()内部使用
	m_pXmlData = new PARSE_XML_DATA[GAME_KEY_DATA_NUM];

	ParseXML(strData, xmlFileName);

	GetGameKeyData();

	delete[]m_pXmlData;

	VMProtectEnd();
}

//解析XML文本或文件到new出的m_pXmlData数组--若都输入，则文本xmlText优先
void CParseVerificationData::ParseXML(/*_bstr_t*/CString xmlText, CString xmlFileName)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF ParseXML()");

	MSXML2::IXMLDOMDocumentPtr xmldoc;
	HRESULT hr;
	hr = CoInitialize(NULL);//必须初始化，否则后面的CreateInstance创建文档要失败
	if (FAILED(hr))//此处最好用FAILED或者SUCCEEDED来判断，否则可能引起未知错误
	{
		AfxMessageBox("初始化COM失败");
	}
	hr = xmldoc.CreateInstance(__uuidof(MSXML2::DOMDocument));
	if (FAILED(hr))
	{
		AfxMessageBox("创建xml文档对象失败");
	}

	if (xmlText != _T(""))
	{
		//解析XML文本
		xmldoc->loadXML((_bstr_t)xmlText);
	}
	else if (xmlFileName != _T(""))
	{
		//解析XML文件
		xmldoc->load((_bstr_t)xmlFileName);
	}
	else
	{
		AfxMessageBox("请输入XML文本，或XML文件路径");
	}

	MSXML2::IXMLDOMElementPtr root;
	MSXML2::IXMLDOMNodeListPtr nodelist;
	MSXML2::IXMLDOMNodeListPtr childnodelist;

	root = xmldoc->GetdocumentElement();
	//root->GettagName()可以获取根结点名字,如"DataRoot"
	//AfxMessageBox("1"+root->GettagName());//根结点名
	nodelist = root->GetchildNodes();

	MSXML2::IXMLDOMNodePtr node;
	MSXML2::IXMLDOMNodePtr childnode;

	for (int i = 0; i < nodelist->length; i++)
	{
		node = nodelist->Getitem(i);
		//node->GetbaseName()获取数据结点名称，如"dmKey"
		m_pXmlData[i].strNodeName = (LPCTSTR)node->GetbaseName();
		childnodelist = node->GetchildNodes();
		for (int j = 0; j < childnodelist->length; j++)
		{
			childnode = childnodelist->Getitem(j);
			//获取数据结点的文本数据
			//AfxMessageBox(childnode->Gettext());
			m_pXmlData[i].strNodeData = (LPCTSTR)childnode->Gettext();
		}
	}
	//::CoUninitialize();

	VMProtectEnd();
}

//根据服务器的xml文本各结点的名字，取出m_pXmlData数组中的关键数据，存入m_pKeyData结构
void CParseVerificationData::GetGameKeyData()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF GetGameKeyData()");

	//将取得的数据复制到相应的变量
	m_pKeyData->server = GetData(_T("server"));
	m_pKeyData->database = GetData(_T("database"));
	m_pKeyData->uid = GetData(_T("uid"));
	m_pKeyData->pwd = GetData(_T("pwd"));

	//AfxMessageBox(m_pKeyData->dmKey +_T("\r\n")+ m_pKeyData->dmKeypad);

	VMProtectEnd();
}
//根据名字从数组取关键数据
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
