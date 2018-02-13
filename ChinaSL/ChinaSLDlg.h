
// ChinaSLDlg.h : 头文件
//

#pragma once

#include "ReptileDlg.h"
#include "CSLPeilv.h"
#include "afxcmn.h"



// CChinaSLDlg 对话框
class CChinaSLDlg : public CDialogEx
{
// 构造
public:
	CChinaSLDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHINASL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public://------------wds--------------
	//初始化各个控件(各TAB对话框)
	void InitAllControl();
	//调整控件位置
	void SetControlRect();


	CTabCtrl m_MainTab;
	CReptileDlg m_reptileDlg;
	CCSLPeilv m_Dlg_cslPeilv;


	//bsphp验证检测线程
	CWinThread *m_pBsphpThread;


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
protected:
	afx_msg LRESULT OnMainDialogEnd(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnTcnSelchangeTabMain(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
