//================================================================
// �ļ���:VerificationData.h
// ˵��:wdswu�Զ�����Ϸ�ؼ���֤�����࣬�����xml�ı���ȡ�ؼ�����
// ����:�����Ե� wdswu@163.com
// ����ʱ��:2016.07.12
// ������ʱ�䣺2016.07.12
//================================================================

#pragma once

//�ؼ���֤���ݽṹ����Ҫ�ӷ�������ȡ��
typedef struct _GAME_KEY_DATA
{
	CString server;
	CString database;
	CString uid;
	CString pwd;
}GAME_KEY_DATA, *PGAME_KEY_DATA;

//������Ϸ�ؼ����ݸ���--��BSPHP������ͬ��
#define GAME_KEY_DATA_NUM 30

//---------------------��֤������------------------------
//�����෽��ParseVerificationData()����xml���ݺ�
//�Ϳ���ʹ�����Աm_pKeyData.�Ĺؼ�����
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
	//�ؼ���֤���ݣ�����ⲿ
	GAME_KEY_DATA *m_pKeyData;

public:
	//����XML��֤����--�������룬���ı�xmlText����
	void ParseVerificationData(CString xmlText = _T(""), CString xmlFileName = _T(""));

private:
	//���ڽ���xmlʱ����ʱ���������ֺ�����
	typedef struct _PARSE_XML_DATA
	{
		CString strNodeName;//XML�������
		CString strNodeData;//XML�������
	}PARSE_XML_DATA, *PPARSE_XML_DATA;
	//��ʱ����xml����������֤����
	PARSE_XML_DATA *m_pXmlData;

	//����XML�ı����ļ�--�������룬���ı�xmlText����
	void ParseXML(CString xmlText = _T(""), CString xmlFileName = _T(""));

	//���ݷ�������xml�ı����������֣�ȡ��m_pXmlData�����еĹؼ����ݣ�����m_pKeyData�ṹ
	void GetGameKeyData();

	//��������,��m_pBsphpXmlData����ȡ����
	CString GetData(CString strName);

};