#pragma once



//luaͷ�ļ�  
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

extern DWORD color_����;
extern DWORD color_����;
extern DWORD color_����;
extern DWORD color_�Ͷ�;
extern DWORD color_����;
extern DWORD color_�÷�;
extern DWORD color_����;
extern DWORD color_ȫ��;
extern DWORD color_����;
extern DWORD color_��ȫ��;
extern DWORD color_����;
extern DWORD color_����ֵ;
extern DWORD color_ĩ��;
extern DWORD color_ĩ��ֵ;
extern DWORD color_�ó���;
extern DWORD color_�ó���ֵ;
extern DWORD color_��ĩ��;
extern DWORD color_��ĩ��ֵ;
extern DWORD color_����ˮ;
extern DWORD color_����;
extern DWORD color_����ˮ;
extern DWORD color_��ˮλ;
extern DWORD color_������;
extern DWORD color_��ˮλ;

extern DWORD g_Ĭ������;


//������ҳ�������ʼ��
//extern UINT g_iAnalysisStartLine;
//��ȡ������Ϣ��URL
extern CString g_SaiShiUrl;
//�������������ҳ
extern CString g_FootballURL;
//����ҳ��
extern CString g_SaiGuoUrl;
//����ҳ��
extern CString g_YaPanUrl;

//ͳһ��ע�ắ��
int luaL_regfun(lua_State *L);
void ReadLuaColorCfg();
void ReadLuaReptileCfg();
