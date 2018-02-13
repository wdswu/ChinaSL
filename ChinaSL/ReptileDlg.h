#pragma once

#include "afxcmn.h"
//#include "afxwin.h"



typedef struct _SPORT_RECORD_LINE_DATA
{
	DWORD id;
	CString strZhuDui;//����
	CString strSaiShi;//����
	CString strRiQi;//����
	CString strKeDui;//�Ͷ�

	CString strSPF;//ʤƽ����������
	CString strRangFen;//����ʤƽ��-�÷�
	CString strRangQiuSPF;//����ʤƽ��
	CString strBiFen;//ȫ���ȷ�
	CString strZongJQS;//�ܽ�����
	CString strQBCSPF;//��ȫ��ʤƽ��

	CString strRQFBPLB_S;//���򷢲����ʱ�_ʤ
	CString strRQFBPLB_P;//���򷢲����ʱ�_ƽ
	CString strRQFBPLB_F;//���򷢲����ʱ�_��
	CString strRangChuPei;//���򷢲�����--�ó���
	CString strRangMoPei;//���򷢲�����--��ĩ��
	CString strRQZZPLB_S;//�����������ʱ�_ʤ
	CString strRQZZPLB_P;//�����������ʱ�_ƽ
	CString strRQZZPLB_F;//�����������ʱ�_��

	CString strFBPLB_S;//�������ʱ�_ʤ
	CString strFBPLB_P;//�������ʱ�_ƽ
	CString strFBPLB_F;//�������ʱ�_��
	CString strChuPei;//�����򷢲�����--����
	CString strMoPei;//�����򷢲�����--ĩ��
	CString strZZPLB_S;//�������ʱ�_ʤ
	CString strZZPLB_P;//�������ʱ�_ƽ
	CString strZZPLB_F;//�������ʱ�_��

	//--------------����---------------------
	//---����---
	CString strChuZSW;
	CString strChuYaPan;
	CString strChuKSW;
	//---��ʱ��ע��---
	CString strJsZSW;//��ʱ����ˮλ
	CString strJsYaPan;//��ʱ����
	CString strJsKSW;//��ʱ�Ͷ�ˮλ
}SPORT_RECORD_LINE_DATA, *PSPORT_RECORD_LINE_DATA;



//�����̲߳����Ľṹ��
typedef struct _THREAD_DATA
{
	HWND hwnd;//���̶߳Ի���
	bool beExitThread;//�߳���ֹ��־
}THREADDATA, *PTHREADDATA;


// CReptileDlg �Ի���

class CReptileDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CReptileDlg)

public:
	CReptileDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CReptileDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_REPTILE };
#endif
public://-------------wds--------------
	CWinThread *m_pWinThread;
	THREADDATA m_ThreadData;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
