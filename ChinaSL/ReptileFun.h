#pragma once

#include <stdio.h>
#include <afxinet.h>
#include <locale.h>


#include <iostream>
#include <string>
#include <string.h>
using namespace std;
#include <atlconv.h>
//��ת��ȫΪGB2312������ַ���Ϊ�����ַ�����������з�"\uxxxx"���룬������
string GB2312ToCN(char* p_unicode_escape_chars);
//��Ӣ�ġ����֡�GB2312(\uxxxx)�����ϵ��ַ��������е�GB2312���룬תΪ�����ַ�
CString UnEscape(CString strBuffer, CString strFind);


//ͨ����ҳ��ȡ��ǰ���ID
DWORD GetWebMaxID();
//�ӻ�ȡ��web�ı�����ȡ����
CString FindHttpFileLineStr(CString strFindTemp, CString strFindStart = ">", CString strFindEnd = "<", int startAdd = 0, int iMaxLen = 100);
//�ӻ�ȡ��web�ı���ȡ������Ϣ
CString FindHttpFileStr(CString strFindTemp, CString strFindStr, CString strFind1 = "\":\"", CString strFind2 = "\",");
