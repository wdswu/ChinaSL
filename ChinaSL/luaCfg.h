#pragma once



//lua头文件  
#ifdef __cplusplus
extern "C"
{
#include "lualib\lua.h"
#include "lualib\lauxlib.h"
#include "lualib\lualib.h"
#include "lualib\luaconf.h"
}
#else
#include "lualib\lua.h"
#include "lualib\lauxlib.h"
#include "lualib\lualib.h"
#include "lualib\luaconf.h"
#endif

#pragma comment(lib,"lualib\\lua.lib")

extern DWORD color_主队;
extern DWORD color_赛事;
extern DWORD color_日期;
extern DWORD color_客队;
extern DWORD color_赛果;
extern DWORD color_让分;
extern DWORD color_让球;
extern DWORD color_全场;
extern DWORD color_进球;
extern DWORD color_半全场;
extern DWORD color_初赔;
extern DWORD color_初赔值;
extern DWORD color_末赔;
extern DWORD color_末赔值;
extern DWORD color_让初赔;
extern DWORD color_让初赔值;
extern DWORD color_让末赔;
extern DWORD color_让末赔值;
extern DWORD color_初主水;
extern DWORD color_初盘;
extern DWORD color_初客水;
extern DWORD color_主水位;
extern DWORD color_终亚盘;
extern DWORD color_客水位;

extern DWORD g_默认条数;


//分析网页代码的起始行
//extern UINT g_iAnalysisStartLine;
//获取赛事信息的URL
extern CString g_SaiShiUrl;
//足彩赛果开奖主页
extern CString g_FootballURL;
//赛果页面
extern CString g_SaiGuoUrl;
//亚盘页面
extern CString g_YaPanUrl;

//统一的注册函数
int luaL_regfun(lua_State *L);
void ReadLuaColorCfg();
void ReadLuaReptileCfg();
