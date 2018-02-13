// CSLPeilv.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "CSLPeilv.h"
#include "afxdialogex.h"


#include "luaCfg.h"

#include "vector"
using namespace std;



//BSPHPȫ�ֱ�����������ط���ʹ��ͬһ��������֤��ȫ����Ψһ��
extern CAppen g_BSPHP;
//BSP��¼�û������룬�ڵ�¼ʱ��ȡ�������������ط�ʹ�ã�ȫ����Ψһ��
extern CString g_BspUser;
extern CString g_BspPwd;

//������ת��(ReptileDlg.cpp)
//CString ChangeSaiShiString(CString strIn);

//����dpi��������
extern float g_dpi_rate;

typedef struct _MY_SHOW_DATA
{
	CString id;
	CString strZuDui;//����
	CString strSaiShi;//����
	CString strRiQi;//����
	CString strKeDui;//�Ͷ�

	CString strSPF;//ʤƽ����������
	CString strRangFen;//����ʤƽ��-�÷�
	CString strRangQiuSPF;//����ʤƽ��
	CString strBiFen;//ȫ���ȷ�
	CString strZongJQS;//�ܽ�����
	CString strQBCSPF;//��ȫ��ʤƽ��

	CString strChuPei;//�����򷢲�����--����
	CString strFBPLB_SPF;//�������ʱ�_ʤ ƽ ��
	CString strMoPei;//�����򷢲�����--ĩ��
	CString strZZPLB_SPF;//�������ʱ�_ʤ ƽ ��

	CString strRangChuPei;//���򷢲�����--�ó���
	CString strRQFBPLB_SPF;//���򷢲����ʱ�_ʤ ƽ ��
	CString strRangMoPei;//���򷢲�����--��ĩ��
	CString strRQZZPLB_SPF;//�����������ʱ�_ʤ ƽ ��

	//--------------����---------------------
	//---����---
	CString strChuZSW;
	CString strChuYaPan;
	CString strChuKSW;
	//---��ʱ��ע��---
	CString strJsZSW;//��ʱ����ˮλ
	CString strJsYaPan;//��ʱ����
	CString strJsKSW;//��ʱ�Ͷ�ˮλ
}MY_SHOW_DATA;

//������ʾ�������б����������
vector<MY_SHOW_DATA> g_ShowData;


// CCSLPeilv �Ի���

IMPLEMENT_DYNAMIC(CCSLPeilv, CDialogEx)

CCSLPeilv::CCSLPeilv(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_CSL_PEILV, pParent)
{

}

CCSLPeilv::~CCSLPeilv()
{
}

void CCSLPeilv::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_CSL_PEILV, m_listCtrl);
}


BEGIN_MESSAGE_MAP(CCSLPeilv, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_PEILV_QUERY, &CCSLPeilv::OnBnClickedBtnPeilvQuery)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LST_CSL_PEILV, &CCSLPeilv::OnCustomdrawLstCslPeilv)
	ON_WM_CTLCOLOR()
	ON_NOTIFY(NM_RCLICK, IDC_LST_CSL_PEILV, &CCSLPeilv::OnNMRClickLstCslPeilv)
	ON_COMMAND(ID_MENU_OPEN_URL, &CCSLPeilv::OnMenuOpenUrl)
	ON_BN_CLICKED(IDC_BTN_CLEAR, &CCSLPeilv::OnBnClickedBtnClear)
	ON_NOTIFY(LVN_GETDISPINFO, IDC_LST_CSL_PEILV, &CCSLPeilv::OnLvnGetdispinfoLstCslPeilv)
END_MESSAGE_MAP()


// CCSLPeilv ��Ϣ�������


BOOL CCSLPeilv::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//��ȡlua����
	ReadLuaColorCfg();

	//m_listCtrl.Init();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	// ��ʼ���б�ؼ�
	m_listCtrl.SetExtendedStyle(
		LVS_EX_AUTOSIZECOLUMNS | //�б�ͷ����Ӧ��С
		LVS_EX_FLATSB |
		LVS_EX_FULLROWSELECT | // ���ж���ѡ�в�����������ֻ��һ�ֶο�ѡ������
		LVS_EX_HEADERDRAGDROP | // ������ק
		LVS_EX_ONECLICKACTIVATE | // �ȵ�������ƹ�ʱ����������ѡ��
		LVS_EX_GRIDLINES | // ������
		LVS_EX_UNDERLINEHOT// �ȵ��»��ߣ������ø��� 
		| LVS_EX_DOUBLEBUFFER //ʹ��˫����,����˸
//		|LVS_EX_CHECKBOXES // �������ʾѡ���(check box)
);
	//�����б�ͷ
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_listCtrl.GetHeaderCtrl();
	pHeader->EnableWindow(0);

	int nFormat = LVCFMT_CENTER;
	//listctr�����
	m_listCtrl.InsertColumn(0, _T("id"), nFormat, 52 * g_dpi_rate);//msdn˵:The leftmost column in a list view control must be left-aligned.
	m_listCtrl.InsertColumn(1, _T("����"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(2, _T("����"), nFormat, 60 * g_dpi_rate);
	m_listCtrl.InsertColumn(3, _T("����"), nFormat, 76 * g_dpi_rate);
	m_listCtrl.InsertColumn(4, _T("�Ͷ�"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(5, _T("����"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(6, _T("�÷�"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(7, _T("����"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(8, _T("ȫ��"), nFormat, 48 * g_dpi_rate);
	m_listCtrl.InsertColumn(9, _T("����"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(10, _T("��ȫ��"), nFormat, 51 * g_dpi_rate);

	m_listCtrl.InsertColumn(11, _T("����"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(12, _T("����ֵ"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(13, _T("ĩ��"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(14, _T("ĩ��ֵ"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(15, _T("�ó���"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(16, _T("�ó���ֵ"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(17, _T("��ĩ��"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(18, _T("��ĩ��ֵ"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(19, _T("����ˮ"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(20, _T("������"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(21, _T("����ˮ"), nFormat, 51 * g_dpi_rate);

	m_listCtrl.InsertColumn(22, _T("��ˮλ"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(23, _T("��ʱ��"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(24, _T("��ˮλ"), nFormat, 51 * g_dpi_rate);

	//��������
	CFont *pfont1 = m_listCtrl.GetFont();
	LOGFONT logfont;//���Ū�����Ա,ȫ�ֱ���,��̬��Ա  
	pfont1->GetLogFont(&logfont);
	logfont.lfHeight = (LONG)(logfont.lfHeight * 1.1);   //��������޸�����ĸ߱���
	logfont.lfWidth = (LONG)(logfont.lfWidth * 1.1);     //��������޸�����Ŀ����
	//logfont.lfWeight *= 1;
	//strcpy_s(logfont.lfFaceName, LF_FACESIZE, "Times New Roman");
	CFont font1;
	font1.CreateFontIndirect(&logfont);
	m_listCtrl.SetFont(&font1);
	font1.Detach();

	//�����и�
	CImageList m_l;
	m_l.Create(1, 24 * 1.1, ILC_COLOR32, 1, 0);
	m_listCtrl.SetImageList(&m_l, LVSIL_SMALL);

	//���ñ�񱳾�ɫ
	m_listCtrl.SetBkColor(RGB(0, 0, 0));
	//m_listCtrl.SetBkColor(RGB(0x6C, 0x6F, 0x85));

	// �����Ҽ������˵�
	VERIFY(m_menu.LoadMenu(IDR_MENU));
	// ���õ��Ի���
	SetMenu(&m_menu);

	SetDlgItemInt(IDC_EDT_REPTILE_DEFAULT_NUM, g_Ĭ������);

	//
	//��ȡ���ݿ������б�
	CCslAdo cslAdo_chuYaPan;
	CString strSQL;
	long iRstCount;
	//�������ݿ� 
	if (cslAdo_chuYaPan.InitADOConnect())
	{
		strSQL = "select distinct chuYaPan from peilv_info";
		try
		{
			cslAdo_chuYaPan.m_pConnection->CursorLocation = adUseClient;//����GetRecordCount()������
			cslAdo_chuYaPan.m_pRecordset = cslAdo_chuYaPan.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_chuYaPan.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info��ѯ����");
		}
		if (cslAdo_chuYaPan.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info������");
		}
		else
		{
			//��ȡ����ת������
			iRstCount = cslAdo_chuYaPan.m_pRecordset->GetRecordCount();
			//g_pSaiShiTemp = new _SaiShi_info[g_iRstCountSaiShi];
			for (long i = 0; i < iRstCount; i++)
			{
				CString strChuYaPan = (TCHAR*)(_bstr_t)cslAdo_chuYaPan.m_pRecordset->GetCollect("chuYaPan");
				//g_pSaiShiTemp[i].strSaiShiShort = (TCHAR*)(_bstr_t)cslAdo_chuYaPan.m_pRecordset->GetCollect("SaiShiShort");

				if (strChuYaPan != "")
				{
					((CComboBox*)GetDlgItem(IDC_COMBO_PEILV_ChuYAPAN))->AddString(strChuYaPan);
				}

				cslAdo_chuYaPan.m_pRecordset->MoveNext();
			}
		}
	}
	cslAdo_chuYaPan.ExitADOConnect();

	//�������ݿ� 
	if (cslAdo_chuYaPan.InitADOConnect())
	{
		strSQL = "select distinct jsYaPan from peilv_info";
		try
		{
			cslAdo_chuYaPan.m_pConnection->CursorLocation = adUseClient;//����GetRecordCount()������
			cslAdo_chuYaPan.m_pRecordset = cslAdo_chuYaPan.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_chuYaPan.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info��ѯ����");
		}
		if (cslAdo_chuYaPan.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info������");
		}
		else
		{
			//��ȡ����ת������
			iRstCount = cslAdo_chuYaPan.m_pRecordset->GetRecordCount();
			//g_pSaiShiTemp = new _SaiShi_info[g_iRstCountSaiShi];
			for (long i = 0; i < iRstCount; i++)
			{
				CString strChuYaPan = (TCHAR*)(_bstr_t)cslAdo_chuYaPan.m_pRecordset->GetCollect("jsYaPan");
				//g_pSaiShiTemp[i].strSaiShiShort = (TCHAR*)(_bstr_t)cslAdo_chuYaPan.m_pRecordset->GetCollect("SaiShiShort");

				if (strChuYaPan != "")
				{
					((CComboBox*)GetDlgItem(IDC_COMBO_PEILV_JSYAPAN))->AddString(strChuYaPan);
				}

				cslAdo_chuYaPan.m_pRecordset->MoveNext();
			}
		}
	}
	cslAdo_chuYaPan.ExitADOConnect();





#if WDS_ADMIN
	//------����ע����------���ڿ�ר���������ڣ�ע������ת������
	// ��ȡע����д��KEY��δ������ֱ����ת�������������У�
	// ��ȡ��������㱾��Ӳ����Ϣ������key�����ȡ�ĶԱȣ���ͬ����������
	// ��ͬ�����������С�
	TCHAR keyInReg[100] = { 0 };
	bool bReg = ReadRegInfo(keyInReg);
	if (!bReg)
	{
		//MessageBox(TEXT("����δע�������������222"));
		//���ù��ܰ�ť
		GetDlgItem(IDC_BTN_PEILV_QUERY)->ShowWindow(0);
	}
	else
	{
		//���ݻ����������ȨKey
		CString machine_code = CalculateMachineCode();//����õ�������
		CString license_key = CalculateLicenseKey(machine_code);

		//�ԱȻ�ȡ��ע�����Ȩ�ͼ������Ȩ
		if (keyInReg == license_key)
		{
			//OnBnClickedBtnReadFile();
			//MessageBox(TEXT("ע��ɹ�"));
		}
		else
		{
			//MessageBox(TEXT("ע���벻��ȷ��������ע��222"));
			GetDlgItem(IDC_BTN_PEILV_QUERY)->ShowWindow(0);
		}
	}
#endif

	//��ʾ1���˻���Ϣ
	CString strUserInfo;
	strUserInfo.Format(_T("��¼�û�: %s\r\n"), g_BspUser);
	strUserInfo += g_BSPHP.AppEn_GetUserInfo("��¼��IP: UserLogInIp\r\n����ʱ��:\r\n UserVipDate");
	SetDlgItemText(IDC_EDT_BSP_GONGGAO, strUserInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

// �Զ���ListCtrl��Ϣӳ��ĺ���,����NM_CUSTOMDRAW��Ϣ
// ����յ��ǳ���Ļ滭��Ϣ����������
void CCSLPeilv::OnCustomdrawLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	//return;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;// �����ú�ǰ������ʧЧ�������else if��������
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF clrNewTextColor, clrNewBkColor;

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
		// ��ȡ�����Item��Index	
		int index = m_listCtrl.GetNextSelectedItem(pos);
		if (index == nItem)// ���Ҫˢ�µ���Ϊ��ǰѡ�����
		{
			clrNewTextColor = RGB(255, 255, 255);// �ı�Ϊ��ɫ			
			clrNewBkColor = RGB(49, 106, 197);// ����blue
		}
		else// ���Ҫˢ�µĲ��ǵ�ǰѡ�����
		{
			//���в�ͬɫ
			if (nItem % 2)
			{
				clrNewTextColor = RGB(0, 255, 0);// �ı���
				clrNewBkColor = RGB(0, 0, 0);//������
			}
			else
			{
				clrNewTextColor = RGB(0, 255, 0);// �ı���		
												 //clrNewBkColor = RGB(0x6C, 0x6F, 0x85);//����
				clrNewBkColor = RGB(0, 0, 0);//������
			}
		}
		// ָ���ı�������ɫ
		//if (0 == pLVCD->iSubItem);
		if (1 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (2 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (3 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (4 == pLVCD->iSubItem)clrNewTextColor = color_�Ͷ�;
		if (5 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (6 == pLVCD->iSubItem)
		{
			CString strItem = m_listCtrl.GetItemText(nItem, 6);
			if (strItem.Find("+") != -1)
			{
				clrNewTextColor = color_�÷�;
			}
		}
		if (7 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (8 == pLVCD->iSubItem)clrNewTextColor = color_ȫ��;
		if (9 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (10 == pLVCD->iSubItem)clrNewTextColor = color_��ȫ��;
		if (11 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (12 == pLVCD->iSubItem)clrNewTextColor = color_����ֵ;
		if (13 == pLVCD->iSubItem)clrNewTextColor = color_ĩ��;
		if (14 == pLVCD->iSubItem)clrNewTextColor = color_ĩ��ֵ;
		if (15 == pLVCD->iSubItem)clrNewTextColor = color_�ó���;
		if (16 == pLVCD->iSubItem)clrNewTextColor = color_�ó���ֵ;
		if (17 == pLVCD->iSubItem)clrNewTextColor = color_��ĩ��;
		if (18 == pLVCD->iSubItem)clrNewTextColor = color_��ĩ��ֵ;
		if (19 == pLVCD->iSubItem)clrNewTextColor = color_����ˮ;
		if (20 == pLVCD->iSubItem)clrNewTextColor = color_����;
		if (21 == pLVCD->iSubItem)clrNewTextColor = color_����ˮ;
		if (22 == pLVCD->iSubItem)clrNewTextColor = color_��ˮλ;
		if (23 == pLVCD->iSubItem)clrNewTextColor = color_������;
		if (24 == pLVCD->iSubItem)clrNewTextColor = color_��ˮλ;


		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
}

BOOL CCSLPeilv::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)// ����ESC
			return TRUE;
		if (pMsg->wParam == VK_RETURN)// ���ػس�
		{
			CWnd* wnd = GetFocus();
			if (wnd != NULL)
			{
				TCHAR str[260];
				CString ClassName = _T("Button");// ��ť��
				GetClassName(wnd->m_hWnd, str, 260);// ȡ�ÿؼ�������
				UINT id = wnd->GetDlgCtrlID();// �õ��ؼ�ID��
				if (ClassName == str)
				{// �����ǰ�����ǰ�ť������WM_COMMAND��Ϣ����ð�ť
					SendMessage(WM_COMMAND, id, (LPARAM)wnd->m_hWnd);
					return TRUE;
				}
			}
			// �����ǰ���㲻������ؼ������������뽹��Ϊ��һ���ؼ�
			CWnd *mwnd = GetNextDlgTabItem(wnd);
			if (mwnd)
			{
				mwnd->SetFocus();// ���ý���Ϊ��һ��TABѭ��Ŀؼ�
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CCSLPeilv::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	//TCHAR str[260];
	//GetClassName(pWnd->m_hWnd, str, 260);// ȡ�ÿؼ�������
	//CString ClassName = _T("Edit");// �༭����
	//							   //UINT id = pWnd->GetDlgCtrlID();// �õ��ؼ�ID��
	//							   //if (ClassName != str)
	//{//���༭���⣬�����ܱ�Ķ���ɫ
	// //����������ɫ
	//	pDC->SetTextColor(RGB(255, 255, 255));
	//	//pDC->SelectObject(&m_Font); //����������ʽ

	//	//���ñ���ɫ
	//	pDC->SetBkMode(TRANSPARENT);
	//	//hbr = CreateSolidBrush(RGB(0, 120, 215));//��ɫ
	//	//hbr = CreateSolidBrush(RGB(0x6C, 0x6F, 0x85));//��ɫ
	//	hbr = CreateSolidBrush(RGB(0x80, 0xB0, 0x90));
	//}
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

//ͨ������txt�ļ���ȡ������ת��********���� ����Ϊ���ݿ��ȡ*********
CString ChangeSaiShiStringByTxt(CString strIn)
{
	CStdioFile file;
	if (!file.Open("������ת����.txt", CFile::modeRead | CFile::shareDenyNone/*���������κη���*/))
	{
		//��ʧ��
		//AddMsgEditCtrlString("�� ������ת����.txt ʧ��");
		return strIn;
	}
	CString strTempLine;
	while (file.ReadString(strTempLine))
	{
		int i = strTempLine.Find(_T("="));
		CString OriginName = strTempLine.Left(i);//�ֽ��ǰ����
		if (OriginName == strIn)
		{
			int j = strTempLine.GetLength();
			for (; i < j; i++)
			{
				if (strTempLine.GetAt(i) != _T('='))
				{
					break;
				}
			}
			strIn = strTempLine.Right(j - i);//�ֽ���󲿷�
		}
	}
	file.Close();

	return strIn;
}

struct _SaiShi_info
{
	CString strSaiShi;
	CString strSaiShiShort;
};
_SaiShi_info *g_pSaiShiTemp = NULL;
long g_iRstCountSaiShi = 0;
//ͨ�����ݿ��ȡ������ת��
CString ChangeSaiShiStringByDataBase(CString strIn)
{
	if (g_pSaiShiTemp != NULL)
	{
		for (long i = 0; i < g_iRstCountSaiShi; i++)
		{
			if (strIn == g_pSaiShiTemp[i].strSaiShi)
			{
				strIn = g_pSaiShiTemp[i].strSaiShiShort;
			}
		}
	}
	return strIn;
}
void CCSLPeilv::ShowRecordset()
{
	DWORD StartMS = GetTickCount();

	CCslAdo cslAdo;
	//�������ݿ� 
	if (!cslAdo.InitADOConnect())
	{
		return;
	}


	//IDC_EDT_PEILV_
	CString strZhuDui, strSaiShi, strKeDui;
	CString strChuPei, strFBPLB_S, strFBPLB_P, strFBPLB_F;
	CString strMoPei, strZZPLB_S, strZZPLB_P, strZZPLB_F;
	CString strRangChuPei, strRQFBPLB_S, strRQFBPLB_P, strRQFBPLB_F;
	CString strRangMoPei, strRQZZPLB_S, strRQZZPLB_P, strRQZZPLB_F;
	CString strChuZSW, strChuYaPan, strChuKSW;
	CString strJsZSW, strJsYaPan, strJsKSW;

	GetDlgItemText(IDC_EDT_PEILV_ZhuDui, strZhuDui);
	GetDlgItemText(IDC_EDT_PEILV_SAISHI, strSaiShi);
	GetDlgItemText(IDC_EDT_PEILV_KeDui, strKeDui);

	GetDlgItemText(IDC_EDT_PEILV_ChuPei, strChuPei);
	GetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_S, strFBPLB_S);
	GetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_P, strFBPLB_P);
	GetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_F, strFBPLB_F);

	GetDlgItemText(IDC_EDT_PEILV_MoPei, strMoPei);
	GetDlgItemText(IDC_EDT_PEILV_ZZPLB_S, strZZPLB_S);
	GetDlgItemText(IDC_EDT_PEILV_ZZPLB_P, strZZPLB_P);
	GetDlgItemText(IDC_EDT_PEILV_ZZPLB_F, strZZPLB_F);

	GetDlgItemText(IDC_EDT_PEILV_RangChuPei, strRangChuPei);
	GetDlgItemText(IDC_EDT_PEILV_RQFBPLB_S, strRQFBPLB_S);
	GetDlgItemText(IDC_EDT_PEILV_RQFBPLB_P, strRQFBPLB_P);
	GetDlgItemText(IDC_EDT_PEILV_RQFBPLB_F, strRQFBPLB_F);

	GetDlgItemText(IDC_EDT_PEILV_RangMoPei, strRangMoPei);
	GetDlgItemText(IDC_EDT_PEILV_RQZZPLB_S, strRQZZPLB_S);
	GetDlgItemText(IDC_EDT_PEILV_RQZZPLB_P, strRQZZPLB_P);
	GetDlgItemText(IDC_EDT_PEILV_RQZZPLB_F, strRQZZPLB_F);

	GetDlgItemText(IDC_EDT_PEILV_ChuZSW, strChuZSW);
	//GetDlgItemText(IDC_EDT_PEILV_ChuYAPAN, strChuYaPan); 
	GetDlgItemText(IDC_COMBO_PEILV_ChuYAPAN, strChuYaPan);
	GetDlgItemText(IDC_EDT_PEILV_ChuKSW, strChuKSW);

	GetDlgItemText(IDC_EDT_PEILV_JSZSW, strJsZSW);
	//GetDlgItemText(IDC_EDT_PEILV_JSYAPAN, strJsYaPan);
	GetDlgItemText(IDC_COMBO_PEILV_JSYAPAN, strJsYaPan);
	GetDlgItemText(IDC_EDT_PEILV_JSKSW, strJsKSW);

	strZhuDui = strZhuDui.Trim();
	strSaiShi = strSaiShi.Trim();
	strKeDui = strKeDui.Trim();
	strChuYaPan = strChuYaPan.Trim();
	strJsYaPan = strJsYaPan.Trim();

	CString strQueryNum;
	UINT iQueryNum = GetDlgItemInt(IDC_EDT_REPTILE_DEFAULT_NUM);
	if (iQueryNum == 0)
	{
		strQueryNum = " ";
	}
	else
	{
		strQueryNum.Format(" top %d ", iQueryNum);
	}
	//MessageBox(strQueryNum);

	//VMProtectBeginUltra("WDS_VMBEGIN OF ShowRecordset");

	CString strSQL;

	//if (strZhuDui == "" &&
	//	strSaiShi == "" &&
	//	strKeDui == "" &&
	//	strChuPei == "" &&
	//	strFBPLB_S == ""&&
	//	strFBPLB_P == ""&&
	//	strFBPLB_F == ""&&
	//	strMoPei == "" &&
	//	strZZPLB_S == ""&&
	//	strZZPLB_P == ""&&
	//	strZZPLB_F == ""&&
	//	strRangChuPei == "" &&
	//	strRQFBPLB_S == "" &&
	//	strRQFBPLB_P == "" &&
	//	strRQFBPLB_F == "" &&
	//	strRangMoPei == "" &&
	//	strRQZZPLB_S == "" &&
	//	strRQZZPLB_P == "" &&
	//	strRQZZPLB_F == "" &&
	//	strChuZSW == ""&&
	//	strChuYaPan == ""&&
	//	strChuKSW == ""&&
	//	strJsZSW == ""&&
	//	strJsYaPan == ""&&
	//	strJsKSW == "")
	//{
	//	strSQL = "select " + strQueryNum + " * from PeiLv_Info order by id desc";
	//}
	//else
	{
		//strSQL = "select * from PeiLv_Info where";
		strSQL = "select " + strQueryNum + " * from PeiLv_Info where";
		if (strZhuDui != "")
		{
			//strSQL += " ZhuDui like '%" + strZhuDui + "%' and ";
			strSQL += " ZhuDui = '" + strZhuDui + "' and ";
		}
		if (strSaiShi != "")
		{
			strSQL += " (SaiShi like '%" + strSaiShi + "%' or SaiShi in (select SaiShi from SaiShi_Info where SaiShiShort like '%" + strSaiShi + "%')) and ";
		}
		if (strKeDui != "")
		{
			//strSQL += " KeDui like '%" + strKeDui + "%' and ";
			strSQL += " KeDui = '" + strKeDui + "' and ";
		}

		if (strChuPei != "")
		{
			strSQL += " ChuPei='" + strChuPei + "' and ";
		}
		if (strFBPLB_S != "")
		{
			strSQL += " FBPLB_S like '%" + strFBPLB_S + "%' and ";
		}
		if (strFBPLB_P != "")
		{
			strSQL += " FBPLB_P like '%" + strFBPLB_P + "%' and ";
		}
		if (strFBPLB_F != "")
		{
			strSQL += " FBPLB_F like '%" + strFBPLB_F + "%' and ";
		}

		if (strMoPei != "")
		{
			strSQL += " MoPei='" + strMoPei + "' and ";
		}
		if (strZZPLB_S != "")
		{
			strSQL += " ZZPLB_S like '%" + strZZPLB_S + "%' and ";
		}
		if (strZZPLB_P != "")
		{
			strSQL += " ZZPLB_P like '%" + strZZPLB_P + "%' and ";
		}
		if (strZZPLB_F != "")
		{
			strSQL += " ZZPLB_F like '%" + strZZPLB_F + "%' and ";
		}

		if (strRangChuPei != "")
		{
			strSQL += " RangChuPei='" + strRangChuPei + "' and ";
		}
		if (strRQFBPLB_S != "")
		{
			strSQL += " RQFBPLB_S like '%" + strRQFBPLB_S + "%' and ";
		}
		if (strRQFBPLB_P != "")
		{
			strSQL += " RQFBPLB_P like '%" + strRQFBPLB_P + "%' and ";
		}
		if (strRQFBPLB_F != "")
		{
			strSQL += " RQFBPLB_F like '%" + strRQFBPLB_F + "%' and ";
		}

		if (strRangMoPei != "")
		{
			strSQL += " RangMoPei='" + strRangMoPei + "' and ";
		}
		if (strRQZZPLB_S != "")
		{
			strSQL += " RQZZPLB_S like '%" + strRQZZPLB_S + "%' and ";
		}
		if (strRQZZPLB_P != "")
		{
			strSQL += " RQZZPLB_P like '%" + strRQZZPLB_P + "%' and ";
		}
		if (strRQZZPLB_F != "")
		{
			strSQL += " RQZZPLB_F like '%" + strRQZZPLB_F + "%' and ";
		}

		if (strChuZSW != "")
		{
			strSQL += " ChuZSW like '%" + strChuZSW + "%' and ";
		}
		if (strChuYaPan != "")
		{
			strSQL += " ChuYaPan = '" + strChuYaPan + "' and ";
		}
		if (strChuKSW != "")
		{
			strSQL += " ChuKSW like '%" + strChuKSW + "%' and ";
		}

		if (strJsZSW != "")
		{
			strSQL += " JsZSW like '%" + strJsZSW + "%' and ";
		}
		if (strJsYaPan != "")
		{
			strSQL += " JsYaPan = '" + strJsYaPan + "' and ";
		}
		if (strJsKSW != "")
		{
			strSQL += " JsKSW like '%" + strJsKSW + "%' and ";
		}
		strSQL = strSQL.Left(strSQL.GetLength() - 5);
		strSQL += " order by id desc";
	}

	//MessageBox(strSQL);
	//return;

	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "��ʼ��ѯ����...");
	try
	{
		//cslAdo.m_pConnection->CursorLocation = adUseClient;//����GetRecordCount()������
		cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cslAdo.dump_com_error(e);
		SetDlgItemText(IDC_STATIC_QUERY_RESULT, "��ѯ����");
		cslAdo.ExitADOConnect();
		return;
	}
	if (cslAdo.m_pRecordset->adoBOF)
	{
		//MessageBox("data is empty");
		SetDlgItemText(IDC_STATIC_QUERY_RESULT, "�޷��ϵ�����");
		cslAdo.ExitADOConnect();
		return;
	}
	cslAdo.m_pRecordset->MoveFirst();


	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "��ʼ��ѯ����ת��...");
	//��ȡ����ת����,������ȫ�ֵĶ�̬����g_pSaiShiTemp��
	CCslAdo cslAdo_SaiShi;
	//�������ݿ� 
	if (cslAdo_SaiShi.InitADOConnect())
	{
		strSQL = "select * from SaiShi_Info";
		try
		{
			cslAdo_SaiShi.m_pConnection->CursorLocation = adUseClient;//����GetRecordCount()������
			cslAdo_SaiShi.m_pRecordset = cslAdo_SaiShi.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_SaiShi.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "SaiShi_Info��ѯ����");
		}
		if (cslAdo_SaiShi.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "SaiShi_Info������");
		}
		else
		{
			//��ȡ����ת������
			g_iRstCountSaiShi = cslAdo_SaiShi.m_pRecordset->GetRecordCount();
			g_pSaiShiTemp = new _SaiShi_info[g_iRstCountSaiShi];
			for (long i = 0; i < g_iRstCountSaiShi; i++)
			{
				g_pSaiShiTemp[i].strSaiShi = (TCHAR*)(_bstr_t)cslAdo_SaiShi.m_pRecordset->GetCollect("SaiShi");
				g_pSaiShiTemp[i].strSaiShiShort = (TCHAR*)(_bstr_t)cslAdo_SaiShi.m_pRecordset->GetCollect("SaiShiShort");

				cslAdo_SaiShi.m_pRecordset->MoveNext();
			}
		}
	}
	cslAdo_SaiShi.ExitADOConnect();


	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "��ʼ��ʾ����...");
	int i = 0;
	//for (; !cslAdo.m_pRecordset->adoEOF; i++)
	//{
	//	// ��ʾÿ����¼���б�ؼ�
	//	int item = m_listCtrl.InsertItem(i, "");
	//	m_listCtrl.SetItemText(i, 0, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("id"));
	//	m_listCtrl.SetItemText(i, 1, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZhuDui"));

	//	strSaiShi = ChangeSaiShiStringByDataBase((TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SaiShi"));
	//	m_listCtrl.SetItemText(i, 2, strSaiShi);

	//	m_listCtrl.SetItemText(i, 3, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RiQi"));
	//	m_listCtrl.SetItemText(i, 4, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("KeDui"));
	//	m_listCtrl.SetItemText(i, 5, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SPF"));
	//	m_listCtrl.SetItemText(i, 6, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangFen"));
	//	m_listCtrl.SetItemText(i, 7, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangQiuSPF"));
	//	m_listCtrl.SetItemText(i, 8, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("BiFen"));
	//	m_listCtrl.SetItemText(i, 9, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZongJQS"));
	//	m_listCtrl.SetItemText(i, 10, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("QBCSPF"));

	//	CString strTemp;

	//	//�����򷢲�����--����
	//	m_listCtrl.SetItemText(i, 11, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuPei"));
	//	//�������ʱ�--����ֵ
	//	CString strFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_S");
	//	CString strFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_P");
	//	CString strFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_F");
	//	strTemp = strFBPLB_S + "  " + strFBPLB_P + "  " + strFBPLB_F;
	//	m_listCtrl.SetItemText(i, 12, strTemp);

	//	//ĩ��
	//	m_listCtrl.SetItemText(i, 13, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("MoPei"));
	//	//�������ʱ�--ĩ��ֵ
	//	CString strZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_S");
	//	CString strZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_P");
	//	CString strZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_F");
	//	strTemp = strZZPLB_S + "  " + strZZPLB_P + "  " + strZZPLB_F;
	//	m_listCtrl.SetItemText(i, 14, strTemp);

	//	//------ȡ��-----������(��������-��������)
	//	//double f1 = atof(strZZPLB_S) - atof(strFBPLB_S);
	//	//double f2 = atof(strZZPLB_P) - atof(strFBPLB_P);
	//	//double f3 = atof(strZZPLB_F) - atof(strFBPLB_F);
	//	//strTemp.Format("%.2f|%.2f|%.2f", f1, f2, f3);
	//	//m_listCtrl.SetItemText(i, 13, strTemp);


	//	//���򷢲�����-�ó���
	//	m_listCtrl.SetItemText(i, 15, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangChuPei"));
	//	//���򷢲����ʱ�-�ó���ֵ
	//	CString strRQFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_S");
	//	CString strRQFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_P");
	//	CString strRQFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_F");
	//	strTemp = strRQFBPLB_S + "  " + strRQFBPLB_P + "  " + strRQFBPLB_F;
	//	m_listCtrl.SetItemText(i, 16, strTemp);

	//	//��ĩ��
	//	m_listCtrl.SetItemText(i, 17, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangMoPei"));
	//	//�����������ʱ�-��ĩ��ֵ
	//	CString strRQZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_S");
	//	CString strRQZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_P");
	//	CString strRQZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_F");
	//	strTemp = strRQZZPLB_S + "  " + strRQZZPLB_P + "  " + strRQZZPLB_F;
	//	m_listCtrl.SetItemText(i, 18, strTemp);

	//	//�����(����ĩ��-����)
	//	//f1 = atof(strRQZZPLB_S) - atof(strRQFBPLB_S);
	//	//f2 = atof(strRQZZPLB_P) - atof(strRQFBPLB_P);
	//	//f3 = atof(strRQZZPLB_F) - atof(strRQFBPLB_F);
	//	//strTemp.Format("%.2f|%.2f|%.2f", f1, f2, f3);
	//	//m_listCtrl.SetItemText(i, 17, strTemp);

	//	//��ˮλ
	//	m_listCtrl.SetItemText(i, 19, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsZSW"));
	//	//����
	//	m_listCtrl.SetItemText(i, 20, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsYaPan"));
	//	//��ˮλ
	//	m_listCtrl.SetItemText(i, 21, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsKSW"));


	//	strTemp = "";

	//	cslAdo.m_pRecordset->MoveNext();
	//}

	//�����������
	int S = 0, P = 0, F = 0;
	//�ȷּ������
	int BiFen1 = 0, BiFen2 = 0;

	MY_SHOW_DATA tempData;
	//���������
	vector<MY_SHOW_DATA>().swap(g_ShowData);
	for (; !cslAdo.m_pRecordset->adoEOF; cslAdo.m_pRecordset->MoveNext(), i++)
	{
		// ��ʾÿ����¼���б�ؼ�
		tempData.id = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("id");
		tempData.strZuDui = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZhuDui");

		strSaiShi = ChangeSaiShiStringByDataBase((TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SaiShi"));
		tempData.strSaiShi = strSaiShi;

		tempData.strRiQi = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RiQi");
		tempData.strKeDui = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("KeDui");

		//��������
		tempData.strSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SPF");
		if (tempData.strSPF == "ʤ")
			S++;
		else if (tempData.strSPF == "ƽ")
			P++;
		else
			F++;

		tempData.strRangFen = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangFen");
		tempData.strRangQiuSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangQiuSPF");

		//����ȫ��
		tempData.strBiFen = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("BiFen");
		if (tempData.strBiFen == "ʤ����")
			tempData.strBiFen;
		else
		{
			int ið�� = tempData.strBiFen.Find(":");
			CString strBiFen1 = tempData.strBiFen.Mid(0, ið��);
			CString strBiFen2 = tempData.strBiFen.Mid(ið�� + 1);
			BiFen1 += atoi(strBiFen1);
			BiFen2 += atoi(strBiFen2);
		}

		tempData.strZongJQS = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZongJQS");
		tempData.strQBCSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("QBCSPF");

		//�����򷢲�����--����
		tempData.strChuPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuPei");
		//�������ʱ�--����ֵ
		CString strFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_S");
		CString strFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_P");
		CString strFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_F");
		tempData.strFBPLB_SPF = strFBPLB_S + "  " + strFBPLB_P + "  " + strFBPLB_F;

		//ĩ��
		tempData.strMoPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("MoPei");
		//�������ʱ�--ĩ��ֵ
		CString strZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_S");
		CString strZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_P");
		CString strZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_F");
		tempData.strZZPLB_SPF = strZZPLB_S + "  " + strZZPLB_P + "  " + strZZPLB_F;

		//���򷢲�����-�ó���
		tempData.strRangChuPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangChuPei");
		//���򷢲����ʱ�-�ó���ֵ
		CString strRQFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_S");
		CString strRQFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_P");
		CString strRQFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_F");
		tempData.strRQFBPLB_SPF = strRQFBPLB_S + "  " + strRQFBPLB_P + "  " + strRQFBPLB_F;

		//��ĩ��
		tempData.strRangMoPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangMoPei");
		//�����������ʱ�-��ĩ��ֵ
		CString strRQZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_S");
		CString strRQZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_P");
		CString strRQZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_F");
		tempData.strRQZZPLB_SPF = strRQZZPLB_S + "  " + strRQZZPLB_P + "  " + strRQZZPLB_F;

		//����ˮλ
		tempData.strChuZSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuZSW");
		//������
		tempData.strChuYaPan = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuYaPan");
		//����ˮλ
		tempData.strChuKSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuKSW");

		//��ˮλ
		tempData.strJsZSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsZSW");
		//����
		tempData.strJsYaPan = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsYaPan");
		//��ˮλ
		tempData.strJsKSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsKSW");

		//��������
		g_ShowData.push_back(tempData);
	}
	CString strSPF;
	strSPF.Format("ʤ:%d\rƽ:%d\r��:%d", S, P, F);
	SetDlgItemText(IDC_STATIC_SPF, strSPF);

	CString strBiFen;
	strBiFen.Format("��:%d\rʧ:%d\r", BiFen1, BiFen2);
	SetDlgItemText(IDC_STATIC_BIFEN, strBiFen);

	//���õ�ǰ��¼��
	m_listCtrl.SetItemCountEx(i, LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
	//Invalidate�����, ����ʧȥ����ʱ, ����ʵʱ��ʾ����
	m_listCtrl.Invalidate();

	DWORD useMS = GetTickCount() - StartMS;//����򿪺�ʱ

	CString strResult;
	strResult.Format("��ѯ���,��%d��,��ʱ%d��", i, useMS / 1000);
	SetDlgItemText(IDC_STATIC_QUERY_RESULT, strResult);


	//CString info;
	//vector<_bstr_t> column_name;
	////�洢���������������ʾ�������
	//for (int i = 0; i< cslAdo.m_pRecordset->Fields->GetCount(); i++)
	//{
	//	column_name.push_back(cslAdo.m_pRecordset->Fields->GetItem(_variant_t((long)i))->Name);
	//	info += (LPCSTR)cslAdo.m_pRecordset->Fields->GetItem(_variant_t((long)i))->Name;
	//	info += "\t";
	//}
	//info += "\n";
	//cslAdo.m_pRecordset->MoveFirst();
	////�Ա���б�������,��ʾ����ÿһ�е�����
	//while (!cslAdo.m_pRecordset->adoEOF)
	//{
	//	vector<_bstr_t>::iterator iter = column_name.begin();
	//	int i = 0;
	//	for (iter,i; iter != column_name.end(); iter++,i++)
	//	{
	//		if (cslAdo.m_pRecordset->GetCollect(*iter).vt != VT_NULL)
	//		{
	//			info += cslAdo.m_pRecordset->GetCollect(*iter);
	//		}
	//		else
	//		{
	//			info += "\n";
	//		}
	//	}
	//	cslAdo.m_pRecordset->MoveNext();
	//}
	//MessageBox(info);

	delete[]g_pSaiShiTemp;
	cslAdo.ExitADOConnect();

	//VMProtectEnd();
}

void CCSLPeilv::OnBnClickedBtnPeilvQuery()
{
	m_listCtrl.DeleteAllItems();
	ShowRecordset();
}

CString g_strID;
//����ListControl�Ҽ�������Ϣ
void CCSLPeilv::OnNMRClickLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������

	if (pNMItemActivate->iItem != -1 && WDS_ADMIN)
	{
		//ȡ����ID��ֵ
		g_strID = m_listCtrl.GetItemText(pNMItemActivate->iItem, 0);

		CMenu* popup = m_menu.GetSubMenu(0);// �˵�
		ASSERT(popup != NULL);
		//ָ�������˵�����ʾλ��
		POINT point;
		GetCursorPos(&point);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}
//��Ӧ�˵���Ϣ
void CCSLPeilv::OnMenuOpenUrl()
{
	CString strURL = "http://info.sporttery.cn/football/pool_result.php?id=" + g_strID;
	ShellExecute(NULL, "open", strURL, NULL, NULL, SW_SHOWNORMAL);
}


void CCSLPeilv::OnBnClickedBtnClear()
{
	SetDlgItemText(IDC_EDT_PEILV_ZhuDui, "");
	SetDlgItemText(IDC_EDT_PEILV_SAISHI, "");
	SetDlgItemText(IDC_EDT_PEILV_KeDui, "");

	SetDlgItemText(IDC_EDT_PEILV_ChuPei, "");
	SetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_S, "");
	SetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_P, "");
	SetDlgItemText(IDC_EDT_PEILV_FRQFBPLB_F, "");

	SetDlgItemText(IDC_EDT_PEILV_MoPei, "");
	SetDlgItemText(IDC_EDT_PEILV_ZZPLB_S, "");
	SetDlgItemText(IDC_EDT_PEILV_ZZPLB_P, "");
	SetDlgItemText(IDC_EDT_PEILV_ZZPLB_F, "");

	SetDlgItemText(IDC_EDT_PEILV_RangChuPei, "");
	SetDlgItemText(IDC_EDT_PEILV_RQFBPLB_S, "");
	SetDlgItemText(IDC_EDT_PEILV_RQFBPLB_P, "");
	SetDlgItemText(IDC_EDT_PEILV_RQFBPLB_F, "");

	SetDlgItemText(IDC_EDT_PEILV_RangMoPei, "");
	SetDlgItemText(IDC_EDT_PEILV_RQZZPLB_S, "");
	SetDlgItemText(IDC_EDT_PEILV_RQZZPLB_P, "");
	SetDlgItemText(IDC_EDT_PEILV_RQZZPLB_F, "");

	SetDlgItemText(IDC_EDT_PEILV_ChuZSW, "");
	SetDlgItemText(IDC_COMBO_PEILV_ChuYAPAN, "");
	SetDlgItemText(IDC_EDT_PEILV_ChuKSW, "");

	SetDlgItemText(IDC_EDT_PEILV_JSZSW, "");
	SetDlgItemText(IDC_COMBO_PEILV_JSYAPAN, "");
	SetDlgItemText(IDC_EDT_PEILV_JSKSW, "");

	GetDlgItem(IDC_EDT_PEILV_ChuPei)->SetFocus();
}


//----------------------list control �û����������ݣ������������ʾ̫��-----------------------------
//1������list control����owner dataΪtrue
//2���������ݽṹ������MY_SHOW_DATA��������˽ṹ�ı���vector<MY_SHOW_DATA> g_ShowData; ʹ��vector�����Ƚṹ�����ѯ���ļ�¼����
//3�����õ�ǰ��¼��num��m_listCtrl.SetItemCountEx(num, LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
//4�������Invalidate, ����ʧȥ����ʱ, ����ʵʱ��ʾ����m_listCtrl.Invalidate();
//5����Ӧ�ؼ���LVN_GETDISPINFO��Ϣ�����º���,ʹ��switch case����ʾ�Լ�֮ǰ��������
void CCSLPeilv::OnLvnGetdispinfoLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	*pResult = 0;
	LV_ITEM* pItem = &(pDispInfo)->item;

	if (pItem->mask & LVIF_TEXT)
	{
		//m_LstCtr.SetRedraw(FALSE);
		CString text;
		switch (pItem->iSubItem)
		{
		case 0:
			text = g_ShowData[pItem->iItem].id;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 1:
			text = g_ShowData[pItem->iItem].strZuDui;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 2:
			text = text = g_ShowData[pItem->iItem].strSaiShi;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 3:
			text = g_ShowData[pItem->iItem].strRiQi;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 4:
			text = g_ShowData[pItem->iItem].strKeDui;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 5:
			text = g_ShowData[pItem->iItem].strSPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 6:
			text = g_ShowData[pItem->iItem].strRangFen;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 7:
			text = g_ShowData[pItem->iItem].strRangQiuSPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 8:
			text = g_ShowData[pItem->iItem].strBiFen;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 9:
			text = g_ShowData[pItem->iItem].strZongJQS;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 10:
			text = g_ShowData[pItem->iItem].strQBCSPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 11:
			text = g_ShowData[pItem->iItem].strChuPei;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 12:
			text = g_ShowData[pItem->iItem].strFBPLB_SPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 13:
			text = g_ShowData[pItem->iItem].strMoPei;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 14:
			text = g_ShowData[pItem->iItem].strZZPLB_SPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 15:
			text = g_ShowData[pItem->iItem].strRangChuPei;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 16:
			text = g_ShowData[pItem->iItem].strRQFBPLB_SPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 17:
			text = g_ShowData[pItem->iItem].strRangMoPei;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 18:
			text = g_ShowData[pItem->iItem].strRQZZPLB_SPF;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 19:
			text = g_ShowData[pItem->iItem].strChuZSW;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 20:
			text = g_ShowData[pItem->iItem].strChuYaPan;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 21:
			text = g_ShowData[pItem->iItem].strChuKSW;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 22:
			text = g_ShowData[pItem->iItem].strJsZSW;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 23:
			text = g_ShowData[pItem->iItem].strJsYaPan;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		case 24:
			text = g_ShowData[pItem->iItem].strJsKSW;
			lstrcpyn(pItem->pszText, text, pItem->cchTextMax);
			break;
		default:
			break;
		}
	}

}

