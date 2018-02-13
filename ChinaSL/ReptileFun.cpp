#include "stdafx.h"
#include "ReptileFun.h"

#include "luaCfg.h"

//通过网页获取当前最大ID
DWORD GetWebMaxID()
{
	CInternetSession session("HttpClient");
	CHttpFile *pHttpFile = (CHttpFile*)session.OpenURL(g_FootballURL);

	DWORD dwStatusCode;
	pHttpFile->QueryInfoStatusCode(dwStatusCode);

	CString strID;
	CString strLineTemp, strBuffer;
	if (dwStatusCode == HTTP_STATUS_OK)
	{
		//分析网页代码，提取数据
		int iCount = 0;
		while (pHttpFile->ReadString(strLineTemp))
		{
			//iCount++;
			//if (iCount == 162)
			//{
			//	strID = FindHttpFileLineStr(strLineTemp, "m=", "\"");
			//}
			int i = strLineTemp.Find("fb_match_info.php?m=");
			if (i != -1)
			{
				int k = i + strlen("fb_match_info.php?m=");
				int j = strLineTemp.Find("\"", i);
				strID = strLineTemp.Mid(k, j-k);
				//AfxMessageBox(strID);
				break;
			}
		}
	}
	session.Close();
	return _ttol(strID);
}


enum
{
	UNICODE_CALC_SIZE = 1,
	UNICODE_GET_BYTES = 2
};
//将unicode转义字符序列转换为内存中的unicode字符串
int unicode_bytes(char* p_unicode_escape_chars, wchar_t *bytes, int flag)
{
	int unicode_count = 0;
	int length = strlen(p_unicode_escape_chars);
	for (int char_index = 0; char_index < length; char_index++)
	{
		char unicode_hex[5];
		memset(unicode_hex, 0, 5);

		char ascii[2];
		memset(ascii, 0, 2);
		if (*(p_unicode_escape_chars + char_index) == '\\')
		{
			char_index++;
			if (char_index < length)
			{
				if (*(p_unicode_escape_chars + char_index) == 'u')
				{
					if (flag == UNICODE_GET_BYTES)
					{
						memcpy(unicode_hex, p_unicode_escape_chars + char_index + 1, 4);

						//sscanf不可以使用unsigned short类型
						//否则：Run-Time Check Failure #2 - Stack around the variable 'a' was corrupted.
						unsigned int a = 0;
						sscanf_s(unicode_hex, "%04x", &a);
						bytes[unicode_count++] = a;
					}
					else if (flag == UNICODE_CALC_SIZE)
					{
						unicode_count++;
					}
					char_index += 4;
				}
				else
				{
					if (flag == UNICODE_GET_BYTES)
					{
						bytes[unicode_count++] = *(p_unicode_escape_chars + char_index);
					}
					else if (flag == UNICODE_CALC_SIZE)
					{
						unicode_count++;
					}
					char_index += 1;
				}
			}
		}
		else
		{
			if (flag == UNICODE_GET_BYTES)
			{
				memcpy(ascii, p_unicode_escape_chars + char_index, 1);
				unsigned int a = 0;
				//sscanf_s(ascii,"%c", &a);
				sscanf_s(ascii, "%d", &a);//--wds--
				bytes[unicode_count++] = a;
			}
			else if (flag == UNICODE_CALC_SIZE)
			{
				unicode_count++;
			}
		}
	}
	return unicode_count;
}
//可转换全为GB2312编码的字符串为中文字符串，如果含有非"\uxxxx"编码，则会出错
string GB2312ToCN(char* p_unicode_escape_chars)
{
	int nBytes = unicode_bytes(p_unicode_escape_chars, NULL, UNICODE_CALC_SIZE);

	wchar_t *p_bytes = new wchar_t[nBytes + sizeof(wchar_t)];

	unicode_bytes(p_unicode_escape_chars, p_bytes, UNICODE_GET_BYTES);

	p_bytes[nBytes] = 0;

	USES_CONVERSION;

	string cs_return = W2A((wchar_t*)p_bytes);

	delete[] p_bytes;
	return cs_return;
}

//从获取的web文本中取赛事信息
CString FindHttpFileStr(CString strFindTemp, CString strFindStr, CString strFind1, CString strFind2)
{
	if (strFindStr == "")
	{
		return strFindTemp;
	}
	int i = strFindTemp.Find(strFindStr);
	int j = i + strlen(strFindStr) + strlen(strFind1);
	int k = strFindTemp.Find(strFind2, j);

	return strFindTemp.Mid(j, k - j);
}

//将英文、数字、GB2312(\uxxxx)编码混合的字符串，其中的GB2312编码，转为中文字符
CString UnEscape(CString strBuffer, CString strFind)
{
	//取得英文、数字、GB2312(\uxxxx)编码混合的字符串
	CString strRet = "";//保存最终转换的GB3212中文，英文和数字不转换
	CString strTemp0 = "";//临时用于转换的单个汉字GB2312编码
	CString strTemp1 = FindHttpFileStr(strBuffer, strFind);
	for (int i = 0; i < strTemp1.GetLength();)
	{
		TCHAR chAt1 = strTemp1.GetAt(i);
		TCHAR chAt2 = strTemp1.GetAt(i + 1);
		if (chAt1 == '\\')
		{
			if (chAt2 == 'u')
			{
				strTemp0 = strTemp1.Mid(i, 6);
				string string1 = GB2312ToCN(strTemp0.GetBuffer());
				strTemp0.ReleaseBuffer();
				strRet += string1.c_str();
				i += 6;
			}
			else //if (chAt2 != 'u')
			{
				strRet += chAt2;
				i += 2;

				//---下列也可---
				//strTemp0 = strTemp1.Mid(i, 2);
				//string string1 = GB2312ToCN(strTemp0.GetBuffer());
				//strTemp0.ReleaseBuffer();
				//strRet += string1.c_str();
				//i += 2;
			}
		}
		else //if (chAt1 != '\\')
		{
			strRet += chAt1;
			i++;
		}
	}
	return strRet;
}

//从获取的web文本行中取赛果
CString FindHttpFileLineStr(CString strFindTemp, CString strFindStart, CString strFindEnd, int startAdd, int iMaxLen)
{
	int iStart = strFindTemp.Find(strFindStart) + strnlen_s(strFindStart, 100) + startAdd;
	int iLen = strFindTemp.Find(strFindEnd, iStart) - iStart;
	if (iLen > iMaxLen)
	{
		iLen = iMaxLen;
	}
	return strFindTemp.Mid(iStart, iLen);
}