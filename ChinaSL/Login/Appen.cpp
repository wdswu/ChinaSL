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

//����bsphp��api���Ƿ����
#define BSPHP_API_ENCRYPTION 1

//-------�������api������CString����-------
//----api�������룺wdswu2016----
#if BSPHP_API_ENCRYPTION
//��ֵ��������
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
//��ֵԭʼ����
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
	//5�����ò���,�û���Ҫ���ݹ����̨����Ӧ�����������������漸����޸�

	//�������֤��·��,��Ϊ��һ�����������������ﲻͬ�涼һ��
	AppEn_ImgaUrl = "http://host4771495.xincache5.cn/index.php?m=coode&sessl=";

	//--�й��������ʿ�
	AppEn_HOST_KEY = "6edd99ff578771d753dbf06f72548897"; //ͨ����֤Key
	AppEn_password = "ERh8XRlMhBBcPDG57J";//ͨѶ����
	AppEn_Url = "http://host4771495.xincache5.cn/AppEn.php?appid=80000005&md5=f5f35682d30003f0697736f3d21eff7c";

	//-------------------------------------------------------------------------------------------

	AppEn_MoShi = "POST";//����ģʽ��д POST  GET
	Version = "v1.0";//���ڴ�ŵ�ǰ������İ汾��

	//����AppEn.dll
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	//������������
	if (AppEn_Internet() != "1")
	{
		AfxMessageBox("���ӷ�����ʧ�ܣ�����������");
		FreeLibrary(m_Hint);
		return FALSE;
	}
	//��Ա����	
	AppEn_KEY = GetMachineCode();//�Լ�����Ļ���Ӳ��������// = AppEn_GetCpuId();//���ڴ�Ż����룬�û����Ը����Լ���Ҫ���ı���㷽��
	AppEn_MD5 = AppEn_FileMD5(); //���ڴ�������ļ�MD5
	AppEn_BSphpSeSsl = AppEn_GetBSphpSeSsL();//���ӱ�ǣ�����Cookies��ͨ��  AppEn_GetBSphpSeSsL() ��ȡ

	//��ѯ�Ƿ���ʾ��֤��--[�������]-[ͨ�Ű�ȫ]-[API��֤��]�������������Щ������Ҫ��֤��
	m_ImgaSet = AppEn_GetsetImage("��¼INGES_LOGIN|ע��INGES_RE|�һظ���INGES_MACK|����INGES_SAY");
	//Appen_getImage();//��Ҫ��ʱ����ȡ

	FreeLibrary(m_Hint);

	VMProtectEnd();

	return TRUE;
}

CString CAppen::Bsphp_API_Open(char* v1, char* v2, char* v3, char* v4) ////GET��ȡ����
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	typedef char* (__stdcall BsphpOpen)(char*, char*, char*, char*);
	BsphpOpen *BsphpAPIOpen;
	BsphpAPIOpen = (BsphpOpen*)GetProcAddress(m_Hint, "Bsphp_API_Open"); //GET��ȡ����
	if (BsphpAPIOpen == NULL)
	{
		AfxMessageBox("δ�ҵ�Bsphp_API_Open����!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*BsphpAPIOpen)(v1, v2, v3, v4);
	FreeLibrary(m_Hint);
	return strTemp;
}

CString CAppen::AppEn_FileMD5() ////GET��ȡ����
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	typedef char* (appenFileMD5)();
	appenFileMD5 *AppEnFileMD5;
	AppEnFileMD5 = (appenFileMD5*)GetProcAddress(m_Hint, "AppEn_FileMD5"); //GET��ȡ����
	if (AppEnFileMD5 == NULL)
	{
		AfxMessageBox("δ�ҵ�AppEn_FileMD5����!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnFileMD5)();
	FreeLibrary(m_Hint);
	return strTemp;

}
CString CAppen::AppEn_GetDate() ////GET��ȡ����
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	typedef char* (appenGetDate)();
	appenGetDate *AppEnGetDate;
	AppEnGetDate = (appenGetDate*)GetProcAddress(m_Hint, "AppEn_GetDate"); //GET��ȡ����
	if (AppEnGetDate == NULL)
	{
		AfxMessageBox("δ�ҵ�AppEn_GetDate����!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnGetDate)();
	FreeLibrary(m_Hint);
	return strTemp;
}
CString CAppen::AppEn_GetCpuId() //--������ʹ���Լ���
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	typedef char* (appenGetCpuId)();
	appenGetCpuId *AppEnGetCpuId;
	AppEnGetCpuId = (appenGetCpuId*)GetProcAddress(m_Hint, "AppEn_GetCpuId"); //GET��ȡ����
	if (AppEnGetCpuId == NULL)
	{
		AfxMessageBox("δ�ҵ�AppEn_GetCpuId����!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*AppEnGetCpuId)();
	FreeLibrary(m_Hint);
	return strTemp;

}
////��ȡ��֤��ͼƬ�����أ�����ͼƬ����·��
CString CAppen::AppEn_Pur_Imges_File(char* v1)
{
	CString strTemp;
	strTemp = "";
	m_Hint = LoadLibrary("AppEn.dll");
	if (m_Hint == NULL)
	{
		AfxMessageBox("AppEn.dllʧ��!");
		return FALSE;
	}
	typedef char* (__stdcall BsphpOpen)(char*);
	BsphpOpen *BsphpAPIOpen;
	BsphpAPIOpen = (BsphpOpen*)GetProcAddress(m_Hint, "AppEn_Pur_Imges_File"); //GET��ȡ����
	if (BsphpAPIOpen == NULL)
	{
		AfxMessageBox("δ�ҵ�AppEn_Pur_Imges_File����!");
		FreeLibrary(m_Hint);
		return strTemp;
	}
	strTemp = (*BsphpAPIOpen)(v1);
	FreeLibrary(m_Hint);
	return strTemp;
}
//��ȡ��֤��ͼƬ
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
				AfxMessageBox("Load()��֤��ͼƬʧ��");
			}
			DeleteFile(imgaPath);
		}
	}
	return bRet;
}
//����Э�麯��     
//�������ݵ��ڲ���API�ӿڲ�����Ϣ
// ����Э�� POSTģʽ����
CString CAppen::AppEn_Http(CString data) ////GET��ȡ����
{
	CString str1, str2, date;
	date = AppEn_GetDate();//ʱ��

	VMProtectBeginUltra("WDS_VMBEGIN OF CAppen.AppEn_Http()");

	//��װAPI�ӿ�
	str1 = "&BSphpSeSsL=" + AppEn_BSphpSeSsl + "&mutualkey=" + AppEn_HOST_KEY + "&md5=" + AppEn_MD5 + "&date=" + date + data;
	//�����������ݰ�
	str2 = Bsphp_API_Open((LPTSTR)(LPCTSTR)AppEn_Url, (LPTSTR)(LPCTSTR)str1, (LPTSTR)(LPCTSTR)AppEn_password, (LPTSTR)(LPCTSTR)AppEn_MoShi);

	VMProtectEnd();

	str2.TrimRight();
	str2.TrimLeft();

	return str2;
}
//ȡ��֤���Ƿ���
CString CAppen::AppEn_GetsetImage(CString type)
{
	return AppEn_Http("&api=" + g_getsetimag_in + "&type=" + type);
}

//��¼
CString CAppen::AppEn_LogIn(CString User, CString pwd, CString strImage,CString maxoror)
{
	VMProtectBeginUltra("WDS_VMBEGIN OF CAppen.AppEn_LogIn()");
	//&maxoror=�޿�����,&coode=��֤��
	return AppEn_Http("&api=" + g_login_lg + "&user=" + User + "&pwd=" + pwd + "&key=" + AppEn_KEY + "&maxoror=" + maxoror + "&coode=" + strImage);

	VMProtectEnd();
}

//Զ��ע����¼״̬,���2���˺�������ע��2���˺ţ�
void CAppen::AppEn_LoginOff(CString user, CString pwd)
{
	AppEn_Http("&api=" + g_remotecancellation_lg + "&user=" + user + "&pwd=" + pwd);
}

//��ȡ��ǰ��¼�ʺ�״̬��Ϣ,�ж��Ƿ��¼
CString CAppen::AppEn_GetLoginInfo()
{
	return AppEn_Http("&api=" + g_lginfo_lg);
}
//ȡ��ǰ��¼�û��Ļ�����Ϣ
CString CAppen::AppEn_GetUserInfo(CString info)
{
	return AppEn_Http("&api=" + g_getuserinfo_lg + "&info=" + info);
}
//ȡ��֤����
CString CAppen::AppEn_GetVerification()
{
	return AppEn_Http("&api=" + g_lgkey_lg + "&key=" + AppEn_KEY);
}

//����
CString CAppen::AppEn_GetGongGao()
{
	return AppEn_Http("&api=" + g_gg_in);
}

//�ж�����״̬
CString CAppen::AppEn_Internet()
{
	return AppEn_Http("&api=" + g_internet_in);
}

//ȡ�������
CString CAppen::AppEn_GetMiaoShu()
{
	return AppEn_Http("&api=" + g_miao_in);
}

//ȡVIP����ʱ��
CString CAppen::AppEn_GetVipDieDate()
{
	return AppEn_Http("&api=" + g_vipdate_lg);
}

//ȡ�����ҳ
CString CAppen::AppEn_GetWebUrl()
{
	return AppEn_Http("&api=" + g_weburl_in);
}
//ȡ���������ַ
CString CAppen::AppEn_GetUrl()
{
	return AppEn_Http("&api=" + g_url_in);
}
//ȡ��key
CString CAppen::AppEn_GetUserKey()
{
	return AppEn_Http("&api=" + g_userkey_lg);
}

//�޸�����
CString CAppen::AppEn_password_S(CString user, CString usepwd, CString pwda, CString pwdb, CString img)
{
	return AppEn_Http("&api=" + g_password_lg + "&user=" + user + "&pwd=" + usepwd + "&pwda=" + pwda + "&pwdb=" + pwdb + "&coode=" + img);
}

//�һ�����
CString CAppen::AppEn_backto(CString user, CString pwda, CString pwdb, CString quset, CString ans, CString img)
{
	return AppEn_Http("&api=" + g_backto_lg + "&user=" + user + "&pwd=" + pwda + "&pwdb=" + pwdb + "&mibao=" + quset + "&daan=" + ans + "&coode=" + img);
}

//���Ƶ�ǰ��¼�˺���Ϣ QQ ���� �绰
CString CAppen::AppEn_AddUserInfo(CString qq, CString mail, CString mobile)
{
	return AppEn_Http("&api=" + g_Perfect_lg + "&qq=" + qq + "&mail=" + mail + "&mobile=" + mobile);
}
//�û�����  ����;����;��ϵ��ʽ;����;��֤��
CString CAppen::AppEn_Feedback(CString name, CString pwd, CString biaoti, CString lianxi, CString leixing, CString neirong, CString imga)
{
	return AppEn_Http("&api=" + g_liuyan_in + "&table=" + biaoti + "&leix=" + leixing + "&qq=" + lianxi + "&txt=" + neirong + "&coode=" + imga + "&user=" + name + "&pwd=" + pwd);
}
//ȡ�߼��ж�B
CString CAppen::AppEn_GetLogic_B()
{
	return AppEn_Http("&api=" + g_logicb_in);
}
//ȡ�߼��ж�A
CString CAppen::AppEn_GetLogic_A()
{
	return AppEn_Http("&api=" + g_logica_in);
}
//ȡ�߼��ж�A ����
CString CAppen::AppEn_GetLogicText_A()
{
	return AppEn_Http("&api=" + g_logicinfoa_in);
}
//ȡ�߼��ж�B ����
CString CAppen::AppEn_GetLogicText_B()
{
	return AppEn_Http("&api=" + g_logicinfob_in);
}

//������
CString CAppen::AppEn_timeout()
{
	return AppEn_Http("&api=" + g_timeout_lg);
}
//��ȡ BSphpSeSsL ��
CString CAppen::AppEn_GetBSphpSeSsL()
{
	return AppEn_Http("&api=" + g_BSphpSeSsL_in);
}
//ע���µ��˺ţ�����ע��״̬��Ϣ
CString CAppen::AppEn_registration(CString user, CString pwd, CString pwdb, CString qq, CString mail, CString key, CString coode, CString mobile, CString mibao_wenti, CString mibao_daan)
{
	return AppEn_Http("&api=" + g_registration_lg + "&user=" + user + "&pwd=" + pwd + "&pwdb=" + pwdb + "&qq=" + qq + "&mail=" + mail + "&key=" + key + "&coode=" + coode + "&mobile=" + mobile + "&mibao_wenti=" + mibao_wenti + "&mibao_daan=" + mibao_daan);
}
//����ǰ����½���˺Ű�����,���һ��������ֹ�ظ��󶨵���̨>�����������
CString CAppen::AppEn_BindkeyByLogined(CString key)
{
	return AppEn_Http("&api=" + g_bang_lg + "&key=" + key);
}
//����ǰ�����롿�˺Ű�����,���һ��������ֹ�ظ��󶨵���̨>�����������
CString CAppen::AppEn_BindkeyByUser(CString user, CString pwd, CString key)
{
	return AppEn_Http("&api=" + g_bang_lg + "&user=" + user + "&pwd=" + pwd + "&key=" + key);
}
//�����ǰ������,��۳���Ӧ����ʱ��
CString CAppen::AppEn_UnBindKey(CString user, CString password)
{
	return AppEn_Http("&api=" + g_jiekey_lg + "&user=" + user + "&pwd=" + password);
}
//��ֵ,userset�Ƿ���Ҫ��֤�û�����(1��Ҫ��֤����)
CString CAppen::AppEn_VipChong(CString user, CString usepwd, CString kahao, CString kami, CString  userset)
{
	return AppEn_Http("&api=" + g_chong_lg + "&user=" + user + "&userpwd=" + usepwd + "&ka=" + kahao + "&pwd=" + kami + "&userset=" + userset);
}
//ȡ����汾��Ϣ
CString CAppen::AppEn_Ver()
{
	return AppEn_Http("&api=" + g_v_in);
}
//ȡ���������Ϣ��
CString CAppen::AppEn_GlobalInfo(CString info)
{
	return AppEn_Http("&api=" + g_globalinfo_in + "&info=" + info);
}