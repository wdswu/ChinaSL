#pragma once
#include "afxcmn.h"

#import "msado15.dll" no_namespace rename("EOF","adoEOF")rename("BOF","adoBOF") 

//#include "SkinListCtrl\SkinListCtrl.h"

// CCSLPeilv 对话框

class CCSLPeilv : public CDialogEx
{
	DECLARE_DYNAMIC(CCSLPeilv)

public:
	CCSLPeilv(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCSLPeilv();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_CSL_PEILV };
#endif

public://-------------------------wds------------------------
	void ShowRecordset();

	CMenu m_menu;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_listCtrl;
	//CSkinListCtrl m_listCtrl;
	afx_msg void OnBnClickedBtnPeilvQuery();
	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnNMRClickLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMenuOpenUrl();
	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnLvnGetdispinfoLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult);
};
