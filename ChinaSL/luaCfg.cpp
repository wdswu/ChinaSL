
#include "stdafx.h"
#include "luaCfg.h"



DWORD color_���� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_�Ͷ� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_�÷� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_ȫ�� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_��ȫ�� = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_����ֵ = 0xFF00;
DWORD color_ĩ�� = 0xFF00;
DWORD color_ĩ��ֵ = 0xFF00;
DWORD color_�ó��� = 0xFF00;
DWORD color_�ó���ֵ = 0xFF00;
DWORD color_��ĩ�� = 0xFF00;
DWORD color_��ĩ��ֵ = 0xFF00;
DWORD color_����ˮ = 0xFF00;
DWORD color_���� = 0xFF00;
DWORD color_����ˮ = 0xFF00;
DWORD color_��ˮλ = 0xFF00;
DWORD color_������ = 0xFF00;
DWORD color_��ˮλ = 0xFF00;

DWORD g_Ĭ������ = 30;


//������ҳ�������ʼ��
//UINT g_iAnalysisStartLine;
//��ȡ������Ϣ��URL
CString g_SaiShiUrl;
//�������������ҳ
CString g_FootballURL;
//����ҳ��
CString g_SaiGuoUrl;
//����ҳ��
CString g_YaPanUrl;

static int ColorToInt(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		int a = lua_tointeger(L, 2);
		int b = lua_tointeger(L, 3);
		int c = lua_tointeger(L, 4);

		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "�Ͷ�") color_�Ͷ� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "�÷�") color_�÷� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "ȫ��") color_ȫ�� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "��ȫ��") color_��ȫ�� = RGB(a, b, c);
		if (str == "����") color_���� = RGB(a, b, c);
		if (str == "����ֵ")color_����ֵ = RGB(a, b, c);
		if (str == "ĩ��") color_ĩ�� = RGB(a, b, c);
		if (str == "ĩ��ֵ")color_ĩ��ֵ = RGB(a, b, c);
		if (str == "�ó���") color_�ó��� = RGB(a, b, c);
		if (str == "�ó���ֵ")color_�ó���ֵ = RGB(a, b, c);
		if (str == "��ĩ��") color_��ĩ�� = RGB(a, b, c);
		if (str == "��ĩ��ֵ")color_��ĩ��ֵ = RGB(a, b, c);
		if (str == "����ˮ") color_����ˮ = RGB(a, b, c);
		if (str == "����")color_���� = RGB(a, b, c);
		if (str == "����ˮ")color_����ˮ = RGB(a, b, c);
		if (str == "��ˮλ")color_��ˮλ = RGB(a, b, c);
		if (str == "������")color_������ = RGB(a, b, c);
		if (str == "��ˮλ") color_��ˮλ = RGB(a, b, c);
		if (str == "Ĭ������") g_Ĭ������ = a;
	}
	return 0;
}
static int GetInt(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		int a = lua_tointeger(L, 2);

		if (str == "Ĭ������") g_Ĭ������ = a;
		//if (str == "����������ʼ��") g_iAnalysisStartLine = (UINT)a;
	}
	return 0;
}
static int GetString(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		CString str2 = lua_tostring(L, 2);

		if (str == "�������������ҳ") g_FootballURL = str2;
		if (str == "����url") g_SaiShiUrl = str2;
		if (str == "����ҳ��") g_SaiGuoUrl = str2;
		if (str == "����ҳ��") g_YaPanUrl = str2;
	}
	return 0;
}

//ͳһ��ע�ắ��
int luaL_regfun(lua_State *L)
{
	lua_register(L, "��ȡ��ɫ", ColorToInt);
	lua_register(L, "��ȡ����", GetInt);
	lua_register(L, "��ȡ�ִ�", GetString);

	return 0;
}
void ReadLuaColorCfg()
{
	lua_State *L = luaL_newstate();
	luaL_regfun(L);
	luaL_dofile(L, "colorCfg.lua");
	lua_close(L);
}
void ReadLuaReptileCfg()
{
	lua_State *L = luaL_newstate();
	luaL_regfun(L);
	luaL_dofile(L, "reptileCfg.lua");
	lua_close(L);
}
