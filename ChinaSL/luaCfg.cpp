
#include "stdafx.h"
#include "luaCfg.h"



DWORD color_主队 = 0xFF00;
DWORD color_赛事 = 0xFF00;
DWORD color_日期 = 0xFF00;
DWORD color_客队 = 0xFF00;
DWORD color_赛果 = 0xFF00;
DWORD color_让分 = 0xFF00;
DWORD color_让球 = 0xFF00;
DWORD color_全场 = 0xFF00;
DWORD color_进球 = 0xFF00;
DWORD color_半全场 = 0xFF00;
DWORD color_初赔 = 0xFF00;
DWORD color_初赔值 = 0xFF00;
DWORD color_末赔 = 0xFF00;
DWORD color_末赔值 = 0xFF00;
DWORD color_让初赔 = 0xFF00;
DWORD color_让初赔值 = 0xFF00;
DWORD color_让末赔 = 0xFF00;
DWORD color_让末赔值 = 0xFF00;
DWORD color_初主水 = 0xFF00;
DWORD color_初盘 = 0xFF00;
DWORD color_初客水 = 0xFF00;
DWORD color_主水位 = 0xFF00;
DWORD color_终亚盘 = 0xFF00;
DWORD color_客水位 = 0xFF00;

DWORD g_默认条数 = 30;


//分析网页代码的起始行
//UINT g_iAnalysisStartLine;
//获取赛事信息的URL
CString g_SaiShiUrl;
//足彩赛果开奖主页
CString g_FootballURL;
//赛果页面
CString g_SaiGuoUrl;
//亚盘页面
CString g_YaPanUrl;

static int ColorToInt(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		int a = lua_tointeger(L, 2);
		int b = lua_tointeger(L, 3);
		int c = lua_tointeger(L, 4);

		if (str == "主队") color_主队 = RGB(a, b, c);
		if (str == "赛事") color_赛事 = RGB(a, b, c);
		if (str == "日期") color_日期 = RGB(a, b, c);
		if (str == "客队") color_客队 = RGB(a, b, c);
		if (str == "赛果") color_赛果 = RGB(a, b, c);
		if (str == "让分") color_让分 = RGB(a, b, c);
		if (str == "让球") color_让球 = RGB(a, b, c);
		if (str == "全场") color_全场 = RGB(a, b, c);
		if (str == "进球") color_进球 = RGB(a, b, c);
		if (str == "半全场") color_半全场 = RGB(a, b, c);
		if (str == "初赔") color_初赔 = RGB(a, b, c);
		if (str == "初赔值")color_初赔值 = RGB(a, b, c);
		if (str == "末赔") color_末赔 = RGB(a, b, c);
		if (str == "末赔值")color_末赔值 = RGB(a, b, c);
		if (str == "让初赔") color_让初赔 = RGB(a, b, c);
		if (str == "让初赔值")color_让初赔值 = RGB(a, b, c);
		if (str == "让末赔") color_让末赔 = RGB(a, b, c);
		if (str == "让末赔值")color_让末赔值 = RGB(a, b, c);
		if (str == "初主水") color_初主水 = RGB(a, b, c);
		if (str == "初盘")color_初盘 = RGB(a, b, c);
		if (str == "初客水")color_初客水 = RGB(a, b, c);
		if (str == "主水位")color_主水位 = RGB(a, b, c);
		if (str == "终亚盘")color_终亚盘 = RGB(a, b, c);
		if (str == "客水位") color_客水位 = RGB(a, b, c);
		if (str == "默认条数") g_默认条数 = a;
	}
	return 0;
}
static int GetInt(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		int a = lua_tointeger(L, 2);

		if (str == "默认条数") g_默认条数 = a;
		//if (str == "赛果分析起始行") g_iAnalysisStartLine = (UINT)a;
	}
	return 0;
}
static int GetString(lua_State *L)
{
	if (lua_isstring(L, 1))
	{
		CString str = lua_tostring(L, 1);
		CString str2 = lua_tostring(L, 2);

		if (str == "足彩赛果开奖主页") g_FootballURL = str2;
		if (str == "赛事url") g_SaiShiUrl = str2;
		if (str == "赛果页面") g_SaiGuoUrl = str2;
		if (str == "亚盘页面") g_YaPanUrl = str2;
	}
	return 0;
}

//统一的注册函数
int luaL_regfun(lua_State *L)
{
	lua_register(L, "获取颜色", ColorToInt);
	lua_register(L, "获取整数", GetInt);
	lua_register(L, "获取字串", GetString);

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
