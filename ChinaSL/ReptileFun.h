#pragma once

#include <stdio.h>
#include <afxinet.h>
#include <locale.h>


#include <iostream>
#include <string>
#include <string.h>
using namespace std;
#include <atlconv.h>
//可转换全为GB2312编码的字符串为中文字符串，如果含有非"\uxxxx"编码，则会出错
string GB2312ToCN(char* p_unicode_escape_chars);
//将英文、数字、GB2312(\uxxxx)编码混合的字符串，其中的GB2312编码，转为中文字符
CString UnEscape(CString strBuffer, CString strFind);


//通过网页获取当前最大ID
DWORD GetWebMaxID();
//从获取的web文本行中取赛果
CString FindHttpFileLineStr(CString strFindTemp, CString strFindStart = ">", CString strFindEnd = "<", int startAdd = 0, int iMaxLen = 100);
//从获取的web文本中取赛事信息
CString FindHttpFileStr(CString strFindTemp, CString strFindStr, CString strFind1 = "\":\"", CString strFind2 = "\",");
