// Appen.h: interface for the CAppen class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#if !defined(AFX_APPEN_H__2AEAB87C_47EC_499A_9BDE_8B66DAEDF7C2__INCLUDED_)
#define AFX_APPEN_H__2AEAB87C_47EC_499A_9BDE_8B66DAEDF7C2__INCLUDED_

#include "Picture.h"
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CAppen
{
public:
	CAppen();
	virtual ~CAppen();
public:
	CString Version;//���ڴ�ŵ�ǰ������İ汾��
	CString m_ImgaSet;//���ڴ����֤��ͼƬ�Ƿ���Ҫ��ʾ���ַ������
	CPicture m_AppEn_imga;//��֤��ͼƬ
private:
	//5�����ò���
	CString AppEn_ImgaUrl;  //��֤��·��
	CString AppEn_HOST_KEY; //ͨ����֤Key:
	CString AppEn_password;//ͨѶ����
	CString AppEn_Url;//��������ַ
	CString AppEn_MoShi;//����ģʽ��д POST  GET

	//��Ա����	
	CString AppEn_KEY;//���ڴ�Ż�����
	CString AppEn_MD5; //���ڴ�������ļ�MD5
	CString AppEn_BSphpSeSsl;//���ӱ�ǣ�����Cookies��ͨ��  AppEn_GetBSphpSeSsL() ��ȡ

	//DLL���
	HINSTANCE m_Hint;
public:
	BOOL init();

	void AppEn_LoginOff(CString user, CString pwd);//Զ��ע����¼״̬
	CString Bsphp_API_Open(char* v1, char* v2, char* v3, char* v4);
	CString AppEn_FileMD5();
	CString AppEn_GetDate();
	CString AppEn_GetCpuId();
	CString AppEn_VipChong(CString user, CString usepwd, CString kahao, CString kami, CString  userset);
	CString AppEn_UnBindKey(CString user, CString password);
	CString AppEn_BindkeyByLogined(CString key);
	CString AppEn_BindkeyByUser(CString user, CString pwd, CString key);
	CString AppEn_registration(CString user, CString pwd, CString pwdb, CString qq =NULL, CString mail = NULL, CString key = NULL, CString coode = NULL, CString mobile = NULL, CString mibao_wenti = NULL, CString mibao_daan = NULL);
	CString AppEn_GetBSphpSeSsL();
	CString AppEn_GetLogicText_B();
	CString AppEn_GetLogicText_A();
	CString AppEn_timeout();
	CString AppEn_GetLogic_A();
	CString AppEn_GetLogic_B();
	CString AppEn_GetVerification();
	CString AppEn_Feedback(CString name, CString pwd, CString biaoti, CString lianxi, CString leixing, CString neirong, CString imga);
	CString AppEn_GetUserInfo(CString info);
	CString AppEn_AddUserInfo(CString qq, CString mail, CString mobile);
	CString AppEn_GetLoginInfo();
	CString AppEn_GetUserKey();
	CString AppEn_GetVipDieDate();
	CString AppEn_GetMiaoShu();
	CString AppEn_Internet();
	CString AppEn_password_S(CString user, CString usepwd, CString pwda, CString pwdb, CString img);//�޸�����
	CString AppEn_backto(CString user, CString pwda, CString pwdb, CString quset, CString ans, CString img);//�һ�����
	CString AppEn_LogIn(CString User, CString pwd, CString strImage, CString maxoror);//��¼
	CString AppEn_GetGongGao();
	CString AppEn_Http(CString data);
	CString AppEn_Ver();
	CString AppEn_GetWebUrl();
	CString AppEn_GetUrl();
	CString AppEn_GetsetImage(CString type);
	//��ȡ��֤��ͼƬ
	BOOL Appen_getImage(); 
	//��ȡ��֤��ͼƬ�����أ�����ͼƬ����·��
	CString AppEn_Pur_Imges_File(char* v1);
	CString AppEn_GlobalInfo(CString info);
};

#endif // !defined(AFX_APPEN_H__2AEAB87C_47EC_499A_9BDE_8B66DAEDF7C2__INCLUDED_)