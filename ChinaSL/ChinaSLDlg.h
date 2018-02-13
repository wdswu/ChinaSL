
// ChinaSLDlg.h : ͷ�ļ�
//

#pragma once

#include "ReptileDlg.h"
#include "CSLPeilv.h"
#include "afxcmn.h"



// CChinaSLDlg �Ի���
class CChinaSLDlg : public CDialogEx
{
// ����
public:
	CChinaSLDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHINASL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public://------------wds--------------
	//��ʼ�������ؼ�(��TAB�Ի���)
	void InitAllControl();
	//�����ؼ�λ��
	void SetControlRect();


	CTabCtrl m_MainTab;
	CReptileDlg m_reptileDlg;
	CCSLPeilv m_Dlg_cslPeilv;


	//bsphp��֤����߳�
	CWinThread *m_pBsphpThread;


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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
