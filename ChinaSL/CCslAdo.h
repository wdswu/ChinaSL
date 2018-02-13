#pragma once


#import "msado15.dll" no_namespace rename("EOF","adoEOF") rename("BOF","adoBOF") 


//-------------------------数据库相关-----------------
class CCslAdo
{
public:
	CCslAdo();
	~CCslAdo();

public:
	// 智能指针
	_ConnectionPtr	m_pConnection;
	_RecordsetPtr	m_pRecordset;
	_CommandPtr		m_pCommand;
	_RecordsetPtr	m_pRecordsetCmd;

public:
	//连接属性
	TCHAR m_server[50] = { 0 } , m_database[50] = { 0 }, m_uid[50] = { 0 }, m_pwd[50] = { 0 };

public:
	//打印调用ADO控件时产生的详细错误信息
	void dump_com_error(_com_error &e);
	bool InitADOConnect(long Options = adModeUnknown);
	void ExitADOConnect();

};

