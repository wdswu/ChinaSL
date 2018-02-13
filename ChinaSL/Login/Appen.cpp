// Appen.cpp: implementation of the CAppen class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "../stdafx.h"

#include "../ChinaSL.h"
#include "Appen.h"

#include "../GetMachineCode\GetMachineCode.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#define new DEBUG_NEW
#endif

//定义bsphp的api名是否加密
#define BSPHP_API_ENCRYPTION 1

//-------定义各个api别名的CString变量-------
//----api加密密码：wdswu2016----
#if BSPHP_API_ENCRYPTION
//赋值加密名字
CString g_getdate_ic = _T("QwcFD8Zadn5CHEree0WoLw|MH171C|MH171C");
CString g_getinfo_ic = _T("OqyW1OerShdCHEree0WoLw|MH171C|MH171C");
CString g_login_ic = _T("96kxLw8f|TJS7EZov55eQENHNig|MH171C|MH171C");
CString g_pointsdeduction_ic = _T("JOMYOi7sLpS1mbstzKFkAkIcSt57Ragv");
CString g_timeout_ic = _T("ZDeKJ1T8gSFCHEree0WoLw|MH171C|MH171C");
CString g_AddCardFeatures_key_ic = _T("70H55eOCTWhCF9nFsKi0FHZ3CvM515ad");
CString g_BSphpSeSsL_in = _T("QyHGE9qO4tAhUz1QSHbo|TJS7Eg|MH171C|MH171C");
CString g_GetPleaseregister_lg = _T("kUSEDePAcxKH7sJboeUmafuZobRxRrPj");
CString g_Perfect_lg = _T("8QuOs72cxHcRnwpZ9smNIQ|MH171C|MH171C");
CString g_SetAppRemarks_ic = _T("Bbc9RfTGGzGr|TJS7EVDydJKdti|TJS7Enl5AQ0c2K");
CString g_SetAppRemarks_lg = _T("Bbc9RfTGGzFgDqeU3tmb7C|TJS7Enl5AQ0c2K");
CString g_appbadpush_in = _T("L2KTlOAHLpOUBUKn8tzXoA|MH171C|MH171C");
CString g_backto_lg = _T("YFxHJMyxIYZaMe1PQVRyIQ|MH171C|MH171C");
CString g_balancededuction_lg = _T("UVIY6e6oDmrfQkbqDGM9X6DH2odnc|H370Jr|TJS7E");
CString g_bang_lg = _T("u4|H370Jz25Q2ub4|MH171C");
CString g_cancellation_ic = _T("niBxn7YIMKtUgpkTOBfvFQ|MH171C|MH171C");
CString g_cancellation_lg = _T("niBxn7YIMKttwbFVnZDuUQ|MH171C|MH171C");
CString g_chong_lg = _T("UYy1vYW9FaYv55eQENHNig|MH171C|MH171C");
CString g_date_in = _T("1uSeD0uH0UU|MH171C");
CString g_get_polog = _T("xbGawbiphzRaMe1PQVRyIQ|MH171C|MH171C");
CString g_getdata_ic = _T("tPOUVRMzc|TJS7ERCHEree0WoLw|MH171C|MH171C");
//CString g_getdate_ic=_T("QwcFD8Zadn5CHEree0WoLw|MH171C|MH171C");
//CString g_getinfo_ic=_T("OqyW1OerShdCHEree0WoLw|MH171C|MH171C");
CString g_getinfo_lg = _T("OqyW1OerShcRnwpZ9smNIQ|MH171C|MH171C");
CString g_getlkinfo_ic = _T("MTuaOwG4LEUhkL|H370Jb9c4pQQ|MH171C|MH171C");
CString g_getnotkey_lg = _T("JnkhL1rI0jvGYoroDsHCNQ|MH171C|MH171C");
CString g_getsetimag_in = _T("8Tlm0yv4foYZoJrvgyzn1w|MH171C|MH171C");
CString g_getsetini_in = _T("a|H370Jd00Ztj6elRnd76v6U8SA|MH171C|MH171C");
CString g_getuserinfo_lg = _T("UtW6KOkFy7O|H370JktzRhG0pFw|MH171C|MH171C");
CString g_gg_in = _T("D2Hy|TJS7En2PDq0|MH171C");
CString g_globalinfo_in = _T("KOuRHzkqOfnUY4tnqtOhAA|MH171C|MH171C");
CString g_imga_in = _T("IwaCJ7SuBss|MH171C");
CString g_internet_in = _T("oO|H370JYJXFK|TJS7El|TJS7ENEnHuk36eMQ|MH171C|MH171C");
CString g_jiekey_lg = _T("s|TJS7EoTCt8bYJhaMe1PQVRyIQ|MH171C|MH171C");
CString g_lginfo_lg = _T("5jKSJRvFY9JaMe1PQVRyIQ|MH171C|MH171C");
CString g_lgkey_lg = _T("JdPnHhJ9gUkv55eQENHNig|MH171C|MH171C");
CString g_liuyan_in = _T("qLEOV9JsbWbRBgZ81qO3jQ|MH171C|MH171C");
CString g_logica_in = _T("E|TJS7E86qgKEc|TJS7EbRBgZ81qO3jQ|MH171C|MH171C");
CString g_logicb_in = _T("1TRGCFxwLvPRBgZ81qO3jQ|MH171C|MH171C");
CString g_logicinfoa_in = _T("SR3GnioSg4wxYVpARr4RtA|MH171C|MH171C");
CString g_logicinfob_in = _T("SR3GnioSg4xTuPAxKaYXyw|MH171C|MH171C");
//CString g_login_ic=_T("96kxLw8f|TJS7EZov55eQENHNig|MH171C|MH171C");
CString g_login_lg = _T("fp9hXUhidFMv55eQENHNig|MH171C|MH171C");
CString g_miao_in = _T("r|TJS7E8BuzorLzs|MH171C");
CString g_password_lg = _T("Wu8mvmpjVdKgx9qHZ3Pq|TJS7Ew|MH171C|MH171C");
CString g_pushaddmoney_in = _T("8OP6mH4mBVW3ps8UazZkKQ|MH171C|MH171C");
CString g_registration_lg = _T("rYd5QSZcEAFtwbFVnZDuUQ|MH171C|MH171C");
CString g_remotecancellation_ic = _T("TnstrLJc8o|TJS7E6OOwBg5fU743Is6s11aYv");
CString g_remotecancellation_lg = _T("TnstrLJc8o|TJS7E6OOwBg5fU7|TJS7EJG8jvtS5j2");
CString g_setcarnot_ic = _T("DBMtmZeqIwVrrpxn|H370JBxYtg|MH171C|MH171C");
CString g_setcaron_ic = _T("XXNvJuuM7KL0DYob4h20aw|MH171C|MH171C");
CString g_setcode_lg = _T("NN9mWWQEy7IRnwpZ9smNIQ|MH171C|MH171C");
CString g_setini_in = _T("mj6m0Zedri3RBgZ81qO3jQ|MH171C|MH171C");
CString g_setpushdate_ic = _T("YkOiqd3n1lGsU6dGlzBs8A|MH171C|MH171C");
CString g_setthesecuritycode_lg = _T("jEng0u2|H370J|TJS7EeQ1GKSJlvXC7u54vz6KUfxs");
//CString g_timeout_ic=_T("ZDeKJ1T8gSFCHEree0WoLw|MH171C|MH171C");
CString g_timeout_lg = _T("ZDeKJ1T8gSERnwpZ9smNIQ|MH171C|MH171C");
CString g_url_in = _T("UA0OrE0m658|MH171C");
CString g_userkey_lg = _T("0KxxQoV8owERnwpZ9smNIQ|MH171C|MH171C");
CString g_v_in = _T("ZbXYWOWnzd4|MH171C");
CString g_vipdate_lg = _T("8t78NVn2CLwRnwpZ9smNIQ|MH171C|MH171C");
CString g_weburl_in = _T("YP1FgFbycuXRBgZ81qO3jQ|MH171C|MH171C");
//CString g_BSphpSeSsL_in=_T("QyHGE9qO4tAhUz1QSHbo|TJS7Eg|MH171C|MH171C");
//CString g_cancellation_ic=_T("niBxn7YIMKtUgpkTOBfvFQ|MH171C|MH171C");
//CString g_cancellation_lg=_T("niBxn7YIMKttwbFVnZDuUQ|MH171C|MH171C");
//CString g_chong_lg=_T("UYy1vYW9FaYv55eQENHNig|MH171C|MH171C");
//CString g_date_in=_T("1uSeD0uH0UU|MH171C");
//CString g_get_polog=_T("xbGawbiphzRaMe1PQVRyIQ|MH171C|MH171C");
//CString g_getuserinfo_lg=_T("UtW6KOkFy7O|H370JktzRhG0pFw|MH171C|MH171C");
//CString g_jiekey_lg=_T("s|TJS7EoTCt8bYJhaMe1PQVRyIQ|MH171C|MH171C");
//CString g_lgkey_lg=_T("JdPnHhJ9gUkv55eQENHNig|MH171C|MH171C");
//CString g_login_lg=_T("fp9hXUhidFMv55eQENHNig|MH171C|MH171C");
CString g_pointsdeduction_lg = _T("JOMYOi7sLpS1mbstzKFkAhGfCln2yY0h");
//CString g_timeout_lg=_T("ZDeKJ1T8gSERnwpZ9smNIQ|MH171C|MH171C");
//CString g_vipdate_lg=_T("8t78NVn2CLwRnwpZ9smNIQ|MH171C|MH171C");

#else
//赋值原始名字
CString g_getdate_ic = _T("getdate.ic");
CString g_getinfo_ic = _T("getinfo.ic");
CString g_login_ic = _T("login.ic");
CString g_pointsdeduction_ic = _T("pointsdeduction.ic");
CString g_timeout_ic = _T("timeout.ic");
CString g_AddCardFeatures_key_ic = _T("AddCardFeatures.key.ic");
CString g_BSphpSeSsL_in = _T("BSphpSeSsL.in");
CString g_GetPleaseregister_lg = _T("GetPleaseregister.lg");
CString g_Perfect_lg = _T("Perfect.lg");
CString g_SetAppRemarks_ic = _T("SetAppRemarks.ic");
CString g_SetAppRemarks_lg = _T("SetAppRemarks.lg");
CString g_appbadpush_in = _T("appbadpush.in");
CString g_backto_lg = _T("backto.lg");
CString g_balancededuction_lg = _T("balancededuction.lg");
CString g_bang_lg = _T("bang.lg");
CString g_cancellation_ic = _T("cancellation.ic");
CString g_cancellation_lg = _T("cancellation.lg");
CString g_chong_lg = _T("chong.lg");
CString g_date_in = _T("date.in");
CString g_get_polog = _T("get.polog");
CString g_getdata_ic = _T("getdata.ic");
//CString g_getdate_ic = _T("getdate.ic");
//CString g_getinfo_ic = _T("getinfo.ic");
CString g_getinfo_lg = _T("getinfo.lg");
CString g_getlkinfo_ic = _T("getlkinfo.ic");
CString g_getnotkey_lg = _T("getnotkey.lg");
CString g_getsetimag_in = _T("getsetimag.in");
CString g_getsetini_in = _T("getsetini.in");
CString g_getuserinfo_lg = _T("getuserinfo.lg");
CString g_gg_in = _T("gg.in");
CString g_globalinfo_in = _T("globalinfo.in");
CString g_imga_in = _T("imga.in");
CString g_internet_in = _T("internet.in");
CString g_jiekey_lg = _T("jiekey.lg");
CString g_lginfo_lg = _T("lginfo.lg");
CString g_lgkey_lg = _T("lgkey.lg");
CString g_liuyan_in = _T("liuyan.in");
CString g_logica_in = _T("logica.in");
CString g_logicb_in = _T("logicb.in");
CString g_logicinfoa_in = _T("logicinfoa.in");
CString g_logicinfob_in = _T("logicinfob.in");
//CString g_login_ic = _T("login.ic");
CString g_login_lg = _T("login.lg");
CString g_miao_in = _T("miao.in");
CString g_password_lg = _T("password.lg");
CString g_pushaddmoney_in = _T("pushaddmoney.in");
CString g_registration_lg = _T("registration.lg");
CString g_remotecancellation_ic = _T("remotecancellation.ic");
CString g_remotecancellation_lg = _T("remotecancellation.lg");
CString g_setcarnot_ic = _T("setcarnot.ic");
CString g_setcaron_ic = _T("setcaron.ic");
CString g_setcode_lg = _T("setcode.lg");
CString g_setini_in = _T("setini.in");
CString g_setpushdate_ic = _T("setpushdate.ic");
CString g_setthesecuritycode_lg = _T("setthesecuritycode.lg");
//CString g_timeout_ic = _T("timeout.ic");
CString g_timeout_lg = _T("timeout.lg");
CString g_url_in = _T("url.in");
CString g_userkey_lg = _T("userkey.lg");
CString g_v_in = _T("v.in");
CString g_vipdate_lg = _T("vipdate.lg");
CString g_weburl_in = _T("weburl.in");
//CString g_BSphpSeSsL_in = _T("BSphpSeSsL.in");
//CString g_cancellation_ic = _T("cancellation.ic");
//CString g_cancellation_lg = _T("cancellation.lg");
//CString g_chong_lg = _T("chong.lg");
//CString g_date_in = _T("date.in");
//CString g_get_polog = _T("get.polog");
//CString g_getuserinfo_lg = _T("getuserinfo.lg");
//CString g_jiekey_lg = _T("jiekey.lg");
//CString g_lgkey_lg = _T("lgkey.lg");
//CString g_login_lg = _T("login.lg");
CString g_pointsdeduction_lg = _T("pointsdeduction.lg");
//CString g_timeout_lg = _T("timeout.lg");
//CString g_vipdate_lg = _T("vipdate.lg");

#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CAppen::CAppen()
{

}

CAppen::~CAppen()
{
	if (m_Hint != NULL)
		FreeLibrary(m_Hint);
}

BOOL CAppen::init()
{
	VMProtectBeginUltra("WDS_VMBEGIN OF CAppen.init()");
	//------------------------------------------------------------------------------------------
	//5个配置参数,用户需要根据管理后台中相应软件的设置项进行下面几项的修改

	//本软件验证码路径,因为是一个服务器，所以这里不同版都一样
	AppEn_ImgaUrl = "http://host4771495.xincache5.cn/index.php?m=coode&sessl=";

	//--中国竞彩赔率库
	AppEn_HOST_KEY = "6edd99ff578771d753dbf06f72548897"; //通信认证Key
	AppEn_password = "ERh8XRlMhBBcPDG57J";//通讯密码
	AppEn_Url = "http://host4771495.xincache5.cn/AppEn.php?appid=80000005&md5=f5f35682d30003f0697736f3d21eff7c";

	//-------------------------------------------------------------------------------------------

	AppEn_MoShi = "POST";//发包模式大写 POST  GET
	Version = "v1.0";//用于存放当前主程序的版本号

	//加载AppEn.dll
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	//测试网络连接
	if (AppEn_Internet() != "1")
	{
		AfxMessageBox("连接服务器失败，请重启程序！");
		FreeLibrary(m_Hint);
		return FALSE;
	}
	//成员变量	
	AppEn_KEY = GetMachineCode();//自己计算的机器硬件特征码// = AppEn_GetCpuId();//用于存放机器码，用户可以根据自己需要，改变计算方法
	AppEn_MD5 = AppEn_FileMD5(); //用于存放自身文件MD5
	AppEn_BSphpSeSsl = AppEn_GetBSphpSeSsL();//连接标记，类似Cookies，通过  AppEn_GetBSphpSeSsL() 获取

	//查询是否显示验证码--[软件配置]-[通信安全]-[API验证码]这里可以配置哪些函数需要验证码
	m_ImgaSet = AppEn_GetsetImage("登录INGES_LOGIN|注册INGES_RE|找回改密INGES_MACK|留言INGES_SAY");
	//Appen_getImage();//需要的时候再取

	FreeLibrary(m_Hint);

	VMProtectEnd();

	return TRUE;
}

CString CAppen::Bsphp_API_Open(char* v1, char* v2, char* v3, char* v4) ////GET获取数据
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	typedef char* (__stdcall BsphpOpen)(char*, char*, char*, char*);
	BsphpOpen *BsphpAPIOpen;
	BsphpAPIOpen = (BsphpOpen*)GetProcAddress(m_Hint, "Bsphp_API_Open"); //GET获取数据
	if (BsphpAPIOpen == NULL)
	{
		AfxMessageBox("未找到Bsphp_API_Open函数!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*BsphpAPIOpen)(v1, v2, v3, v4);
	FreeLibrary(m_Hint);
	return strTemp;
}

CString CAppen::AppEn_FileMD5() ////GET获取数据
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	typedef char* (appenFileMD5)();
	appenFileMD5 *AppEnFileMD5;
	AppEnFileMD5 = (appenFileMD5*)GetProcAddress(m_Hint, "AppEn_FileMD5"); //GET获取数据
	if (AppEnFileMD5 == NULL)
	{
		AfxMessageBox("未找到AppEn_FileMD5函数!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnFileMD5)();
	FreeLibrary(m_Hint);
	return strTemp;

}
CString CAppen::AppEn_GetDate() ////GET获取数据
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	typedef char* (appenGetDate)();
	appenGetDate *AppEnGetDate;
	AppEnGetDate = (appenGetDate*)GetProcAddress(m_Hint, "AppEn_GetDate"); //GET获取数据
	if (AppEnGetDate == NULL)
	{
		AfxMessageBox("未找到AppEn_GetDate函数!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnGetDate)();
	FreeLibrary(m_Hint);
	return strTemp;
}
CString CAppen::AppEn_GetCpuId() //--废弃，使用自己的
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	typedef char* (appenGetCpuId)();
	appenGetCpuId *AppEnGetCpuId;
	AppEnGetCpuId = (appenGetCpuId*)GetProcAddress(m_Hint, "AppEn_GetCpuId"); //GET获取数据
	if (AppEnGetCpuId == NULL)
	{
		AfxMessageBox("未找到AppEn_GetCpuId函数!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnGetCpuId)();
	FreeLibrary(m_Hint);
	return strTemp;

}
////获取验证码图片到本地，返回图片完整路径
CString CAppen::AppEn_Pur_Imges_File(char* v1)
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dll失败!");
		return FALSE;
	}
	typedef char* (__stdcall BsphpOpen)(char*);
	BsphpOpen *BsphpAPIOpen;
	BsphpAPIOpen = (BsphpOpen*)GetProcAddress(m_Hint, "AppEn_Pur_Imges_File"); //GET获取数据
	if (BsphpAPIOpen == NULL)
	{
		AfxMessageBox("未找到AppEn_Pur_Imges_File函数!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*BsphpAPIOpen)(v1);
	FreeLibrary(m_Hint);
	return strTemp;
}
//获取验证码图片
BOOL CAppen::Appen_getImage()
{
	BOOL bRet = FALSE;
	if (m_ImgaSet.Find("checked") > 0)
	{
		CString imgaPath = AppEn_Pur_Imges_File((LPTSTR)(LPCTSTR)(AppEn_ImgaUrl + AppEn_BSphpSeSsl));
		if (imgaPath != "")
		{
			bRet = m_AppEn_imga.Load(imgaPath);
			if (!bRet)
			{
				AfxMessageBox("Load()验证码图片失败");
			}
			DeleteFile(imgaPath);
		}
	}
	return bRet;
}
//发送协议函数     
//参数内容等于部分API接口参数信息
// 发送协议 POST模式发送
CString CAppen::AppEn_Http(CString data) ////GET获取数据
{
	CString str1, str2, date;
	date = AppEn_GetDate();//时间

	VMProtectBeginUltra("WDS_VMBEGIN OF CAppen.AppEn_Http()");

	//组装API接口
	str1 = "&BSphpSeSsL=" + AppEn_BSphpSeSsl + "&mutualkey=" + AppEn_HOST_KEY + "&md5=" + AppEn_MD5 + "&date=" + date + data;
	//发送生成数据包
	str2 = Bsphp_API_Open((LPTSTR)(LPCTSTR)AppEn_Url, (LPTSTR)(LPCTSTR)str1, (LPTSTR)(LPCTSTR)AppEn_password, (LPTSTR)(LPCTSTR)AppEn_MoShi);

	VMProtectEnd();

	str2.TrimRight();
	str2.TrimLeft();

	return str2;
}
//取验证码是否开启
CString CAppen::AppEn_GetsetImage(CString type)
{
	return AppEn_Http("&api=" + g_getsetimag_in + "&type=" + type);
}

//登录
CString CAppen::AppEn_LogIn(CString User, CString pwd, CString strImage,CString maxoror)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF CAppen.AppEn_LogIn()");
	//&maxoror=限开机制,&coode=验证码
	return AppEn_Http("&api=" + g_login_lg + "&user=" + User + "&pwd=" + pwd + "&key=" + AppEn_KEY + "&maxoror=" + maxoror + "&coode=" + strImage);

	VMProtectEnd();
}

//远程注销登录状态,如果2个账号在线着注销2个账号！
void CAppen::AppEn_LoginOff(CString user, CString pwd)
{
	AppEn_Http("&api=" + g_remotecancellation_lg + "&user=" + user + "&pwd=" + pwd);
}

//获取当前登录帐号状态信息,判断是否登录
CString CAppen::AppEn_GetLoginInfo()
{
	return AppEn_Http("&api=" + g_lginfo_lg);
}
//取当前登录用户的基本信息
CString CAppen::AppEn_GetUserInfo(CString info)
{
	return AppEn_Http("&api=" + g_getuserinfo_lg + "&info=" + info);
}
//取验证数据
CString CAppen::AppEn_GetVerification()
{
	return AppEn_Http("&api=" + g_lgkey_lg + "&key=" + AppEn_KEY);
}

//公告
CString CAppen::AppEn_GetGongGao()
{
	return AppEn_Http("&api=" + g_gg_in);
}

//判断网络状态
CString CAppen::AppEn_Internet()
{
	return AppEn_Http("&api=" + g_internet_in);
}

//取软件描述
CString CAppen::AppEn_GetMiaoShu()
{
	return AppEn_Http("&api=" + g_miao_in);
}

//取VIP到期时间
CString CAppen::AppEn_GetVipDieDate()
{
	return AppEn_Http("&api=" + g_vipdate_lg);
}

//取软件主页
CString CAppen::AppEn_GetWebUrl()
{
	return AppEn_Http("&api=" + g_weburl_in);
}
//取软件升级地址
CString CAppen::AppEn_GetUrl()
{
	return AppEn_Http("&api=" + g_url_in);
}
//取绑定key
CString CAppen::AppEn_GetUserKey()
{
	return AppEn_Http("&api=" + g_userkey_lg);
}

//修改密码
CString CAppen::AppEn_password_S(CString user, CString usepwd, CString pwda, CString pwdb, CString img)
{
	return AppEn_Http("&api=" + g_password_lg + "&user=" + user + "&pwd=" + usepwd + "&pwda=" + pwda + "&pwdb=" + pwdb + "&coode=" + img);
}

//找回密码
CString CAppen::AppEn_backto(CString user, CString pwda, CString pwdb, CString quset, CString ans, CString img)
{
	return AppEn_Http("&api=" + g_backto_lg + "&user=" + user + "&pwd=" + pwda + "&pwdb=" + pwdb + "&mibao=" + quset + "&daan=" + ans + "&coode=" + img);
}

//完善当前登录账号信息 QQ 邮箱 电话
CString CAppen::AppEn_AddUserInfo(CString qq, CString mail, CString mobile)
{
	return AppEn_Http("&api=" + g_Perfect_lg + "&qq=" + qq + "&mail=" + mail + "&mobile=" + mobile);
}
//用户留言  标题;类型;联系方式;内容;验证码
CString CAppen::AppEn_Feedback(CString name, CString pwd, CString biaoti, CString lianxi, CString leixing, CString neirong, CString imga)
{
	return AppEn_Http("&api=" + g_liuyan_in + "&table=" + biaoti + "&leix=" + leixing + "&qq=" + lianxi + "&txt=" + neirong + "&coode=" + imga + "&user=" + name + "&pwd=" + pwd);
}
//取逻辑判断B
CString CAppen::AppEn_GetLogic_B()
{
	return AppEn_Http("&api=" + g_logicb_in);
}
//取逻辑判断A
CString CAppen::AppEn_GetLogic_A()
{
	return AppEn_Http("&api=" + g_logica_in);
}
//取逻辑判断A 内容
CString CAppen::AppEn_GetLogicText_A()
{
	return AppEn_Http("&api=" + g_logicinfoa_in);
}
//取逻辑判断B 内容
CString CAppen::AppEn_GetLogicText_B()
{
	return AppEn_Http("&api=" + g_logicinfob_in);
}

//心跳包
CString CAppen::AppEn_timeout()
{
	return AppEn_Http("&api=" + g_timeout_lg);
}
//获取 BSphpSeSsL 串
CString CAppen::AppEn_GetBSphpSeSsL()
{
	return AppEn_Http("&api=" + g_BSphpSeSsL_in);
}
//注册新的账号，返回注册状态信息
CString CAppen::AppEn_registration(CString user, CString pwd, CString pwdb, CString qq, CString mail, CString key, CString coode, CString mobile, CString mibao_wenti, CString mibao_daan)
{
	return AppEn_Http("&api=" + g_registration_lg + "&user=" + user + "&pwd=" + pwd + "&pwdb=" + pwdb + "&qq=" + qq + "&mail=" + mail + "&key=" + key + "&coode=" + coode + "&mobile=" + mobile + "&mibao_wenti=" + mibao_wenti + "&mibao_daan=" + mibao_daan);
}
//给当前【登陆】账号绑定特征,如果一个特征禁止重复绑定到后台>软件配置设置
CString CAppen::AppEn_BindkeyByLogined(CString key)
{
	return AppEn_Http("&api=" + g_bang_lg + "&key=" + key);
}
//给当前【输入】账号绑定特征,如果一个特征禁止重复绑定到后台>软件配置设置
CString CAppen::AppEn_BindkeyByUser(CString user, CString pwd, CString key)
{
	return AppEn_Http("&api=" + g_bang_lg + "&user=" + user + "&pwd=" + pwd + "&key=" + key);
}
//解除当前绑定特征,会扣除对应设置时间
CString CAppen::AppEn_UnBindKey(CString user, CString password)
{
	return AppEn_Http("&api=" + g_jiekey_lg + "&user=" + user + "&pwd=" + password);
}
//充值,userset是否需要验证用户密码(1即要验证密码)
CString CAppen::AppEn_VipChong(CString user, CString usepwd, CString kahao, CString kami, CString  userset)
{
	return AppEn_Http("&api=" + g_chong_lg + "&user=" + user + "&userpwd=" + usepwd + "&ka=" + kahao + "&pwd=" + kami + "&userset=" + userset);
}
//取软件版本信息
CString CAppen::AppEn_Ver()
{
	return AppEn_Http("&api=" + g_v_in);
}
//取软件配置信息段
CString CAppen::AppEn_GlobalInfo(CString info)
{
	return AppEn_Http("&api=" + g_globalinfo_in + "&info=" + info);
}