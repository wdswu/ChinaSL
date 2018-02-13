#pragma once


// CLoginDlg 对话框

class CLoginDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLoginDlg)

public:
	CLoginDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoginDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_LOGIN };
#endif
	
public:
	//----WDS----
	//显示验证码图片
	void ShowVerifyCodePic();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedBtnMachineCode();
	CString m_UserName;
	CString m_UserPwd;
	afx_msg void OnStnClickedStaticLoginYzPic();
	afx_msg void OnBnClickedBtnLoginRegistration();
	afx_msg void OnBnClickedBtnLoginVipchong();
	afx_msg void OnBnClickedBtnLoginJiekey();
	afx_msg void OnBnClickedBtnViewUserInfo();
	afx_msg void OnBnClickedBtnLoginConnectTest();
};
