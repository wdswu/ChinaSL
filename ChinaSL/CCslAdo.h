#pragma once


#import "msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF") 


//-------------------------���ݿ����-----------------
class CCslAdo
{
public:
	CCslAdo();
	~CCslAdo();

public:
	// ����ָ��
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr	m_pRecordset;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordsetCmd;

public:
	//��������
	TCHAR m_server[50] = { 0 } , m_database[50] = { 0 }, m_uid[50] = { 0 }, m_pwd[50] = { 0 };

public:
	//��ӡ����ADO�ؼ�ʱ��������ϸ������Ϣ
	void dump_com_error(_com_error &e);
	bool InitADOConnect(long Options = adModeUnknown);
	void ExitADOConnect();

};

