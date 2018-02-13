//================================================================
// 文件名:VerificationData.h
// 说明:wdswu自定义游戏关键验证数据类，其解析xml文本获取关键数据
// 作者:晓梦迷蝶 wdswu@163.com
// 创建时间:2016.07.12
// 最后更新时间：2016.07.12
//================================================================

#pragma once

//关键验证数据结构，需要从服务器获取来
typedef struct _GAME_KEY_DATA
{
	CString server;
	CString database;
	CString uid;
	CString pwd;
}GAME_KEY_DATA, *PGAME_KEY_DATA;

//定义游戏关键数据个数--与BSPHP服务器同步
#define GAME_KEY_DATA_NUM 30

//---------------------验证数据类------------------------
//调用类方法ParseVerificationData()解析xml数据后，
//就可以使用类成员m_pKeyData.的关键数据
class CParseVerificationData
{
public:
	CParseVerificationData()
	{
		VMProtectBeginUltra("WDS_VMBEGIN OF CParseVerificationData()");
		m_pKeyData = new GAME_KEY_DATA;
		VMProtectEnd();
	}
	~CParseVerificationData()
	{
		if (m_pKeyData)
		{
			delete m_pKeyData;
			m_pKeyData = NULL;
		}
	}

public:
	//关键验证数据，输出外部
	GAME_KEY_DATA *m_pKeyData;

public:
	//解析XML验证数据--若都输入，则文本xmlText优先
	void ParseVerificationData(CString xmlText = _T(""), CString xmlFileName = _T(""));

private:
	//用于解析xml时，临时保存结点名字和数据
	typedef struct _PARSE_XML_DATA
	{
		CString strNodeName;//XML结点名字
		CString strNodeData;//XML结点数据
	}PARSE_XML_DATA, *PPARSE_XML_DATA;
	//临时保存xml解析出的验证数据
	PARSE_XML_DATA *m_pXmlData;

	//解析XML文本或文件--若都输入，则文本xmlText优先
	void ParseXML(CString xmlText = _T(""), CString xmlFileName = _T(""));

	//根据服务器的xml文本各结点的名字，取出m_pXmlData数组中的关键数据，存入m_pKeyData结构
	void GetGameKeyData();

	//根据名字,从m_pBsphpXmlData数组取数据
	CString GetData(CString strName);

};