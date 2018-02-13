#pragma once

#include "afxcmn.h"
//#include "afxwin.h"



typedef struct _SPORT_RECORD_LINE_DATA
{
	DWORD id;
	CString strZhuDui;//主队
	CString strSaiShi;//赛事
	CString strRiQi;//日期
	CString strKeDui;//客队

	CString strSPF;//胜平负（赛果）
	CString strRangFen;//让球胜平负-让分
	CString strRangQiuSPF;//让球胜平负
	CString strBiFen;//全场比分
	CString strZongJQS;//总进球数
	CString strQBCSPF;//半全场胜平负

	CString strRQFBPLB_S;//让球发布赔率表_胜
	CString strRQFBPLB_P;//让球发布赔率表_平
	CString strRQFBPLB_F;//让球发布赔率表_负
	CString strRangChuPei;//让球发布赔率--让初赔
	CString strRangMoPei;//让球发布赔率--让末赔
	CString strRQZZPLB_S;//让球最终赔率表_胜
	CString strRQZZPLB_P;//让球最终赔率表_平
	CString strRQZZPLB_F;//让球最终赔率表_负

	CString strFBPLB_S;//发布赔率表_胜
	CString strFBPLB_P;//发布赔率表_平
	CString strFBPLB_F;//发布赔率表_负
	CString strChuPei;//非让球发布赔率--初赔
	CString strMoPei;//非让球发布赔率--末赔
	CString strZZPLB_S;//最终赔率表_胜
	CString strZZPLB_P;//最终赔率表_平
	CString strZZPLB_F;//最终赔率表_负

	//--------------亚盘---------------------
	//---初盘---
	CString strChuZSW;
	CString strChuYaPan;
	CString strChuKSW;
	//---即时受注盘---
	CString strJsZSW;//即时主队水位
	CString strJsYaPan;//即时亚盘
	CString strJsKSW;//即时客队水位
}SPORT_RECORD_LINE_DATA, *PSPORT_RECORD_LINE_DATA;



//用于线程参数的结构体
typedef struct _THREAD_DATA
{
	HWND hwnd;//主线程对话框
	bool beExitThread;//线程终止标志
}THREADDATA, *PTHREADDATA;


// CReptileDlg 对话框

class CReptileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReptileDlg)

public:
	CReptileDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CReptileDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_REPTILE };
#endif
public://-------------wds--------------
	CWinThread *m_pWinThread;
	THREADDATA m_ThreadData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnReptileData();
	virtual BOOL OnInitDialog();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedBtnRefreshId();
	afx_msg void OnBnClickedBtnStopReptileData();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnClearReptileMsg();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnBnClickedBtnSaveServerCfg();
	afx_msg void OnBnClickedBtnGetValueGather();
	afx_msg void OnBnClickedBtnUpdateValue();
	afx_msg void OnBnClickedBtnRegSoftware();
	afx_msg void OnBnClickedBtnBadRecord();
};
