// CSLPeilv.cpp : 实现文件
//

#include "stdafx.h"
#include "ChinaSL.h"
#include "CSLPeilv.h"
#include "afxdialogex.h"


#include "luaCfg.h"

#include "vector"
using namespace std;



//BSPHP全局变量，程序各地方都使用同一个网络验证，全程序唯一。
extern CAppen g_BSPHP;
//BSP登录用户名密码，在登录时获取，供程序其他地方使用，全程序唯一。
extern CString g_BspUser;
extern CString g_BspPwd;

//赛事名转换(ReptileDlg.cpp)
//CString ChangeSaiShiString(CString strIn);

//界面dpi调整比率
extern float g_dpi_rate;

typedef struct _MY_SHOW_DATA
{
	CString id;
	CString strZuDui;//主队
	CString strSaiShi;//赛事
	CString strRiQi;//日期
	CString strKeDui;//客队

	CString strSPF;//胜平负（赛果）
	CString strRangFen;//让球胜平负-让分
	CString strRangQiuSPF;//让球胜平负
	CString strBiFen;//全场比分
	CString strZongJQS;//总进球数
	CString strQBCSPF;//半全场胜平负

	CString strChuPei;//非让球发布赔率--初赔
	CString strFBPLB_SPF;//发布赔率表_胜 平 负
	CString strMoPei;//非让球发布赔率--末赔
	CString strZZPLB_SPF;//最终赔率表_胜 平 负

	CString strRangChuPei;//让球发布赔率--让初赔
	CString strRQFBPLB_SPF;//让球发布赔率表_胜 平 负
	CString strRangMoPei;//让球发布赔率--让末赔
	CString strRQZZPLB_SPF;//让球最终赔率表_胜 平 负

	//--------------亚盘---------------------
	//---初盘---
	CString strChuZSW;
	CString strChuYaPan;
	CString strChuKSW;
	//---即时受注盘---
	CString strJsZSW;//即时主队水位
	CString strJsYaPan;//即时亚盘
	CString strJsKSW;//即时客队水位
}MY_SHOW_DATA;

//用于显示到虚拟列表的数据容器
vector<MY_SHOW_DATA> g_ShowData;


// CCSLPeilv 对话框

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


// CCSLPeilv 消息处理程序


BOOL CCSLPeilv::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//读取lua配置
	ReadLuaColorCfg();

	//m_listCtrl.Init();

	// TODO:  在此添加额外的初始化
	// 初始化列表控件
	m_listCtrl.SetExtendedStyle(
		LVS_EX_AUTOSIZECOLUMNS | //列表头自适应大小
		LVS_EX_FLATSB |
		LVS_EX_FULLROWSELECT | // 整行都可选中并高亮，否则只第一字段可选并高亮
		LVS_EX_HEADERDRAGDROP | // 列名拖拽
		LVS_EX_ONECLICKACTIVATE | // 热点高亮，移过时高亮，但不选中
		LVS_EX_GRIDLINES | // 网格线
		LVS_EX_UNDERLINEHOT// 热点下划线，需设置高亮 
		| LVS_EX_DOUBLEBUFFER //使用双缓存,降闪烁
//		|LVS_EX_CHECKBOXES // 在左侧显示选择框(check box)
);
	//禁用列表头
	CHeaderCtrl* pHeader = (CHeaderCtrl*)m_listCtrl.GetHeaderCtrl();
	pHeader->EnableWindow(0);

	int nFormat = LVCFMT_CENTER;
	//listctr添加列
	m_listCtrl.InsertColumn(0, _T("id"), nFormat, 52 * g_dpi_rate);//msdn说:The leftmost column in a list view control must be left-aligned.
	m_listCtrl.InsertColumn(1, _T("主队"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(2, _T("赛事"), nFormat, 60 * g_dpi_rate);
	m_listCtrl.InsertColumn(3, _T("日期"), nFormat, 76 * g_dpi_rate);
	m_listCtrl.InsertColumn(4, _T("客队"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(5, _T("赛果"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(6, _T("让分"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(7, _T("让球"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(8, _T("全场"), nFormat, 48 * g_dpi_rate);
	m_listCtrl.InsertColumn(9, _T("进球"), nFormat, 39 * g_dpi_rate);
	m_listCtrl.InsertColumn(10, _T("半全场"), nFormat, 51 * g_dpi_rate);

	m_listCtrl.InsertColumn(11, _T("初赔"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(12, _T("初赔值"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(13, _T("末赔"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(14, _T("末赔值"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(15, _T("让初赔"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(16, _T("让初赔值"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(17, _T("让末赔"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(18, _T("让末赔值"), nFormat, 110 * g_dpi_rate);

	m_listCtrl.InsertColumn(19, _T("初主水"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(20, _T("初亚盘"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(21, _T("初客水"), nFormat, 51 * g_dpi_rate);

	m_listCtrl.InsertColumn(22, _T("主水位"), nFormat, 51 * g_dpi_rate);
	m_listCtrl.InsertColumn(23, _T("即时盘"), nFormat, 110 * g_dpi_rate);
	m_listCtrl.InsertColumn(24, _T("客水位"), nFormat, 51 * g_dpi_rate);

	//设置字体
	CFont *pfont1 = m_listCtrl.GetFont();
	LOGFONT logfont;//最好弄成类成员,全局变量,静态成员  
	pfont1->GetLogFont(&logfont);
	logfont.lfHeight = (LONG)(logfont.lfHeight * 1.1);   //这里可以修改字体的高比例
	logfont.lfWidth = (LONG)(logfont.lfWidth * 1.1);     //这里可以修改字体的宽比例
	//logfont.lfWeight *= 1;
	//strcpy_s(logfont.lfFaceName, LF_FACESIZE, "Times New Roman");
	CFont font1;
	font1.CreateFontIndirect(&logfont);
	m_listCtrl.SetFont(&font1);
	font1.Detach();

	//设置行高
	CImageList m_l;
	m_l.Create(1, 24 * 1.1, ILC_COLOR32, 1, 0);
	m_listCtrl.SetImageList(&m_l, LVSIL_SMALL);

	//设置表格背景色
	m_listCtrl.SetBkColor(RGB(0, 0, 0));
	//m_listCtrl.SetBkColor(RGB(0x6C, 0x6F, 0x85));

	// 加载右键弹出菜单
	VERIFY(m_menu.LoadMenu(IDR_MENU));
	// 设置到对话框
	SetMenu(&m_menu);

	SetDlgItemInt(IDC_EDT_REPTILE_DEFAULT_NUM, g_默认条数);

	//
	//获取数据库亚盘列表
	CCslAdo cslAdo_chuYaPan;
	CString strSQL;
	long iRstCount;
	//连接数据库 
	if (cslAdo_chuYaPan.InitADOConnect())
	{
		strSQL = "select distinct chuYaPan from peilv_info";
		try
		{
			cslAdo_chuYaPan.m_pConnection->CursorLocation = adUseClient;//这样GetRecordCount()才能用
			cslAdo_chuYaPan.m_pRecordset = cslAdo_chuYaPan.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_chuYaPan.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info查询出错");
		}
		if (cslAdo_chuYaPan.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info无数据");
		}
		else
		{
			//获取赛事转换数据
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

	//连接数据库 
	if (cslAdo_chuYaPan.InitADOConnect())
	{
		strSQL = "select distinct jsYaPan from peilv_info";
		try
		{
			cslAdo_chuYaPan.m_pConnection->CursorLocation = adUseClient;//这样GetRecordCount()才能用
			cslAdo_chuYaPan.m_pRecordset = cslAdo_chuYaPan.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_chuYaPan.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info查询出错");
		}
		if (cslAdo_chuYaPan.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "peilv_info无数据");
		}
		else
		{
			//获取赛事转换数据
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
	//------程序注册检测------后期可专门做个窗口，注册后才跳转主界面
	// 读取注册表中存的KEY。未读到，直接跳转到程序限制运行，
	// 读取到，则计算本机硬件信息，生成key，与读取的对比，不同就限制运行
	// 相同程序正常运行。
	TCHAR keyInReg[100] = { 0 };
	bool bReg = ReadRegInfo(keyInReg);
	if (!bReg)
	{
		//MessageBox(TEXT("程序还未注册过，功能受限222"));
		//禁用功能按钮
		GetDlgItem(IDC_BTN_PEILV_QUERY)->ShowWindow(0);
	}
	else
	{
		//根据机器码计算授权Key
		CString machine_code = CalculateMachineCode();//计算得到机器码
		CString license_key = CalculateLicenseKey(machine_code);

		//对比获取的注册表授权和计算的授权
		if (keyInReg == license_key)
		{
			//OnBnClickedBtnReadFile();
			//MessageBox(TEXT("注册成功"));
		}
		else
		{
			//MessageBox(TEXT("注册码不正确，请重新注册222"));
			GetDlgItem(IDC_BTN_PEILV_QUERY)->ShowWindow(0);
		}
	}
#endif

	//显示1次账户信息
	CString strUserInfo;
	strUserInfo.Format(_T("登录用户: %s\r\n"), g_BspUser);
	strUserInfo += g_BSPHP.AppEn_GetUserInfo("登录的IP: UserLogInIp\r\n到期时间:\r\n UserVipDate");
	SetDlgItemText(IDC_EDT_BSP_GONGGAO, strUserInfo);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

// 自定义ListCtrl消息映射的函数,处理NM_CUSTOMDRAW消息
// 会接收到非常多的绘画消息，谨慎处理
void CCSLPeilv::OnCustomdrawLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;
	//return;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYITEMDRAW;
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
		*pResult = CDRF_NOTIFYSUBITEMDRAW;// 被设置后，前面设置失效，下面的else if才起作用
	else if ((CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage)
	{
		COLORREF clrNewTextColor, clrNewBkColor;

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		POSITION pos = m_listCtrl.GetFirstSelectedItemPosition();
		// 获取点击的Item的Index	
		int index = m_listCtrl.GetNextSelectedItem(pos);
		if (index == nItem)// 如果要刷新的项为当前选择的项
		{
			clrNewTextColor = RGB(255, 255, 255);// 文本为白色			
			clrNewBkColor = RGB(49, 106, 197);// 背景blue
		}
		else// 如果要刷新的不是当前选择的项
		{
			//隔行不同色
			if (nItem % 2)
			{
				clrNewTextColor = RGB(0, 255, 0);// 文本绿
				clrNewBkColor = RGB(0, 0, 0);//背景黑
			}
			else
			{
				clrNewTextColor = RGB(0, 255, 0);// 文本绿		
												 //clrNewBkColor = RGB(0x6C, 0x6F, 0x85);//背景
				clrNewBkColor = RGB(0, 0, 0);//背景黑
			}
		}
		// 指定文本绘制颜色
		//if (0 == pLVCD->iSubItem);
		if (1 == pLVCD->iSubItem)clrNewTextColor = color_主队;
		if (2 == pLVCD->iSubItem)clrNewTextColor = color_赛事;
		if (3 == pLVCD->iSubItem)clrNewTextColor = color_日期;
		if (4 == pLVCD->iSubItem)clrNewTextColor = color_客队;
		if (5 == pLVCD->iSubItem)clrNewTextColor = color_赛果;
		if (6 == pLVCD->iSubItem)
		{
			CString strItem = m_listCtrl.GetItemText(nItem, 6);
			if (strItem.Find("+") != -1)
			{
				clrNewTextColor = color_让分;
			}
		}
		if (7 == pLVCD->iSubItem)clrNewTextColor = color_让球;
		if (8 == pLVCD->iSubItem)clrNewTextColor = color_全场;
		if (9 == pLVCD->iSubItem)clrNewTextColor = color_进球;
		if (10 == pLVCD->iSubItem)clrNewTextColor = color_半全场;
		if (11 == pLVCD->iSubItem)clrNewTextColor = color_初赔;
		if (12 == pLVCD->iSubItem)clrNewTextColor = color_初赔值;
		if (13 == pLVCD->iSubItem)clrNewTextColor = color_末赔;
		if (14 == pLVCD->iSubItem)clrNewTextColor = color_末赔值;
		if (15 == pLVCD->iSubItem)clrNewTextColor = color_让初赔;
		if (16 == pLVCD->iSubItem)clrNewTextColor = color_让初赔值;
		if (17 == pLVCD->iSubItem)clrNewTextColor = color_让末赔;
		if (18 == pLVCD->iSubItem)clrNewTextColor = color_让末赔值;
		if (19 == pLVCD->iSubItem)clrNewTextColor = color_初主水;
		if (20 == pLVCD->iSubItem)clrNewTextColor = color_初盘;
		if (21 == pLVCD->iSubItem)clrNewTextColor = color_初客水;
		if (22 == pLVCD->iSubItem)clrNewTextColor = color_主水位;
		if (23 == pLVCD->iSubItem)clrNewTextColor = color_终亚盘;
		if (24 == pLVCD->iSubItem)clrNewTextColor = color_客水位;


		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;
		*pResult = CDRF_DODEFAULT;
	}
}

BOOL CCSLPeilv::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)// 拦截ESC
			return TRUE;
		if (pMsg->wParam == VK_RETURN)// 拦截回车
		{
			CWnd* wnd = GetFocus();
			if (wnd != NULL)
			{
				TCHAR str[260];
				CString ClassName = _T("Button");// 按钮类
				GetClassName(wnd->m_hWnd, str, 260);// 取得控件类型名
				UINT id = wnd->GetDlgCtrlID();// 得到控件ID号
				if (ClassName == str)
				{// 如果当前焦点是按钮，则发送WM_COMMAND消息点击该按钮
					SendMessage(WM_COMMAND, id, (LPARAM)wnd->m_hWnd);
					return TRUE;
				}
			}
			// 如果当前焦点不是上面控件，则设置输入焦点为下一个控件
			CWnd *mwnd = GetNextDlgTabItem(wnd);
			if (mwnd)
			{
				mwnd->SetFocus();// 设置焦点为下一个TAB循序的控件
				return TRUE;
			}
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}


HBRUSH CCSLPeilv::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//TCHAR str[260];
	//GetClassName(pWnd->m_hWnd, str, 260);// 取得控件类型名
	//CString ClassName = _T("Edit");// 编辑框类
	//							   //UINT id = pWnd->GetDlgCtrlID();// 得到控件ID号
	//							   //if (ClassName != str)
	//{//除编辑框外，背景能变的都变色
	// //设置字体颜色
	//	pDC->SetTextColor(RGB(255, 255, 255));
	//	//pDC->SelectObject(&m_Font); //设置字体样式

	//	//设置背景色
	//	pDC->SetBkMode(TRANSPARENT);
	//	//hbr = CreateSolidBrush(RGB(0, 120, 215));//蓝色
	//	//hbr = CreateSolidBrush(RGB(0x6C, 0x6F, 0x85));//灰色
	//	hbr = CreateSolidBrush(RGB(0x80, 0xB0, 0x90));
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

//通过本地txt文件获取赛事名转换********废弃 ，换为数据库获取*********
CString ChangeSaiShiStringByTxt(CString strIn)
{
	CStdioFile file;
	if (!file.Open("赛事名转换表.txt", CFile::modeRead | CFile::shareDenyNone/*允许其他任何访问*/))
	{
		//打开失败
		//AddMsgEditCtrlString("打开 赛事名转换表.txt 失败");
		return strIn;
	}
	CString strTempLine;
	while (file.ReadString(strTempLine))
	{
		int i = strTempLine.Find(_T("="));
		CString OriginName = strTempLine.Left(i);//分解出前部分
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
			strIn = strTempLine.Right(j - i);//分解出后部分
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
//通过数据库获取赛事名转换
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
	//连接数据库 
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

	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "开始查询数据...");
	try
	{
		//cslAdo.m_pConnection->CursorLocation = adUseClient;//这样GetRecordCount()才能用
		cslAdo.m_pRecordset = cslAdo.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
	}
	catch (_com_error e)
	{
		cslAdo.dump_com_error(e);
		SetDlgItemText(IDC_STATIC_QUERY_RESULT, "查询出错");
		cslAdo.ExitADOConnect();
		return;
	}
	if (cslAdo.m_pRecordset->adoBOF)
	{
		//MessageBox("data is empty");
		SetDlgItemText(IDC_STATIC_QUERY_RESULT, "无符合的数据");
		cslAdo.ExitADOConnect();
		return;
	}
	cslAdo.m_pRecordset->MoveFirst();


	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "开始查询赛事转换...");
	//获取赛事转换表,并存入全局的动态数组g_pSaiShiTemp中
	CCslAdo cslAdo_SaiShi;
	//连接数据库 
	if (cslAdo_SaiShi.InitADOConnect())
	{
		strSQL = "select * from SaiShi_Info";
		try
		{
			cslAdo_SaiShi.m_pConnection->CursorLocation = adUseClient;//这样GetRecordCount()才能用
			cslAdo_SaiShi.m_pRecordset = cslAdo_SaiShi.m_pConnection->Execute((_bstr_t)strSQL, NULL, adCmdText);
		}
		catch (_com_error e)
		{
			cslAdo_SaiShi.dump_com_error(e);
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "SaiShi_Info查询出错");
		}
		if (cslAdo_SaiShi.m_pRecordset->adoBOF)
		{
			SetDlgItemText(IDC_STATIC_QUERY_RESULT, "SaiShi_Info无数据");
		}
		else
		{
			//获取赛事转换数据
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


	SetDlgItemText(IDC_STATIC_QUERY_RESULT, "开始显示数据...");
	int i = 0;
	//for (; !cslAdo.m_pRecordset->adoEOF; i++)
	//{
	//	// 显示每条记录到列表控件
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

	//	//非让球发布赔率--初赔
	//	m_listCtrl.SetItemText(i, 11, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuPei"));
	//	//发布赔率表--初赔值
	//	CString strFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_S");
	//	CString strFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_P");
	//	CString strFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_F");
	//	strTemp = strFBPLB_S + "  " + strFBPLB_P + "  " + strFBPLB_F;
	//	m_listCtrl.SetItemText(i, 12, strTemp);

	//	//末赔
	//	m_listCtrl.SetItemText(i, 13, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("MoPei"));
	//	//最终赔率表--末赔值
	//	CString strZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_S");
	//	CString strZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_P");
	//	CString strZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_F");
	//	strTemp = strZZPLB_S + "  " + strZZPLB_P + "  " + strZZPLB_F;
	//	m_listCtrl.SetItemText(i, 14, strTemp);

	//	//------取消-----发布差(最终赔率-发布赔率)
	//	//double f1 = atof(strZZPLB_S) - atof(strFBPLB_S);
	//	//double f2 = atof(strZZPLB_P) - atof(strFBPLB_P);
	//	//double f3 = atof(strZZPLB_F) - atof(strFBPLB_F);
	//	//strTemp.Format("%.2f|%.2f|%.2f", f1, f2, f3);
	//	//m_listCtrl.SetItemText(i, 13, strTemp);


	//	//让球发布赔率-让初赔
	//	m_listCtrl.SetItemText(i, 15, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangChuPei"));
	//	//让球发布赔率表-让初赔值
	//	CString strRQFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_S");
	//	CString strRQFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_P");
	//	CString strRQFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_F");
	//	strTemp = strRQFBPLB_S + "  " + strRQFBPLB_P + "  " + strRQFBPLB_F;
	//	m_listCtrl.SetItemText(i, 16, strTemp);

	//	//让末赔
	//	m_listCtrl.SetItemText(i, 17, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangMoPei"));
	//	//让球最终赔率表-让末赔值
	//	CString strRQZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_S");
	//	CString strRQZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_P");
	//	CString strRQZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_F");
	//	strTemp = strRQZZPLB_S + "  " + strRQZZPLB_P + "  " + strRQZZPLB_F;
	//	m_listCtrl.SetItemText(i, 18, strTemp);

	//	//让球差(让球末赔-初赔)
	//	//f1 = atof(strRQZZPLB_S) - atof(strRQFBPLB_S);
	//	//f2 = atof(strRQZZPLB_P) - atof(strRQFBPLB_P);
	//	//f3 = atof(strRQZZPLB_F) - atof(strRQFBPLB_F);
	//	//strTemp.Format("%.2f|%.2f|%.2f", f1, f2, f3);
	//	//m_listCtrl.SetItemText(i, 17, strTemp);

	//	//主水位
	//	m_listCtrl.SetItemText(i, 19, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsZSW"));
	//	//亚盘
	//	m_listCtrl.SetItemText(i, 20, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsYaPan"));
	//	//客水位
	//	m_listCtrl.SetItemText(i, 21, (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsKSW"));


	//	strTemp = "";

	//	cslAdo.m_pRecordset->MoveNext();
	//}

	//赛果计算变量
	int S = 0, P = 0, F = 0;
	//比分计算变量
	int BiFen1 = 0, BiFen2 = 0;

	MY_SHOW_DATA tempData;
	//先清空向量
	vector<MY_SHOW_DATA>().swap(g_ShowData);
	for (; !cslAdo.m_pRecordset->adoEOF; cslAdo.m_pRecordset->MoveNext(), i++)
	{
		// 显示每条记录到列表控件
		tempData.id = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("id");
		tempData.strZuDui = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZhuDui");

		strSaiShi = ChangeSaiShiStringByDataBase((TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SaiShi"));
		tempData.strSaiShi = strSaiShi;

		tempData.strRiQi = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RiQi");
		tempData.strKeDui = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("KeDui");

		//赛果计算
		tempData.strSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("SPF");
		if (tempData.strSPF == "胜")
			S++;
		else if (tempData.strSPF == "平")
			P++;
		else
			F++;

		tempData.strRangFen = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangFen");
		tempData.strRangQiuSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangQiuSPF");

		//计算全场
		tempData.strBiFen = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("BiFen");
		if (tempData.strBiFen == "胜其他")
			tempData.strBiFen;
		else
		{
			int i冒号 = tempData.strBiFen.Find(":");
			CString strBiFen1 = tempData.strBiFen.Mid(0, i冒号);
			CString strBiFen2 = tempData.strBiFen.Mid(i冒号 + 1);
			BiFen1 += atoi(strBiFen1);
			BiFen2 += atoi(strBiFen2);
		}

		tempData.strZongJQS = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZongJQS");
		tempData.strQBCSPF = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("QBCSPF");

		//非让球发布赔率--初赔
		tempData.strChuPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuPei");
		//发布赔率表--初赔值
		CString strFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_S");
		CString strFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_P");
		CString strFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("FBPLB_F");
		tempData.strFBPLB_SPF = strFBPLB_S + "  " + strFBPLB_P + "  " + strFBPLB_F;

		//末赔
		tempData.strMoPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("MoPei");
		//最终赔率表--末赔值
		CString strZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_S");
		CString strZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_P");
		CString strZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ZZPLB_F");
		tempData.strZZPLB_SPF = strZZPLB_S + "  " + strZZPLB_P + "  " + strZZPLB_F;

		//让球发布赔率-让初赔
		tempData.strRangChuPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangChuPei");
		//让球发布赔率表-让初赔值
		CString strRQFBPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_S");
		CString strRQFBPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_P");
		CString strRQFBPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQFBPLB_F");
		tempData.strRQFBPLB_SPF = strRQFBPLB_S + "  " + strRQFBPLB_P + "  " + strRQFBPLB_F;

		//让末赔
		tempData.strRangMoPei = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RangMoPei");
		//让球最终赔率表-让末赔值
		CString strRQZZPLB_S = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_S");
		CString strRQZZPLB_P = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_P");
		CString strRQZZPLB_F = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("RQZZPLB_F");
		tempData.strRQZZPLB_SPF = strRQZZPLB_S + "  " + strRQZZPLB_P + "  " + strRQZZPLB_F;

		//初主水位
		tempData.strChuZSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuZSW");
		//初亚盘
		tempData.strChuYaPan = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuYaPan");
		//初客水位
		tempData.strChuKSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("ChuKSW");

		//主水位
		tempData.strJsZSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsZSW");
		//亚盘
		tempData.strJsYaPan = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsYaPan");
		//客水位
		tempData.strJsKSW = (TCHAR*)(_bstr_t)cslAdo.m_pRecordset->GetCollect("JsKSW");

		//存入容器
		g_ShowData.push_back(tempData);
	}
	CString strSPF;
	strSPF.Format("胜:%d\r平:%d\r负:%d", S, P, F);
	SetDlgItemText(IDC_STATIC_SPF, strSPF);

	CString strBiFen;
	strBiFen.Format("进:%d\r失:%d\r", BiFen1, BiFen2);
	SetDlgItemText(IDC_STATIC_BIFEN, strBiFen);

	//设置当前记录数
	m_listCtrl.SetItemCountEx(i, LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
	//Invalidate必须加, 窗口失去焦点时, 不会实时显示数据
	m_listCtrl.Invalidate();

	DWORD useMS = GetTickCount() - StartMS;//计算打开耗时

	CString strResult;
	strResult.Format("查询完毕,共%d条,耗时%d秒", i, useMS / 1000);
	SetDlgItemText(IDC_STATIC_QUERY_RESULT, strResult);


	//CString info;
	//vector<_bstr_t> column_name;
	////存储表的所有列名，显示表的列名
	//for (int i = 0; i< cslAdo.m_pRecordset->Fields->GetCount(); i++)
	//{
	//	column_name.push_back(cslAdo.m_pRecordset->Fields->GetItem(_variant_t((long)i))->Name);
	//	info += (LPCSTR)cslAdo.m_pRecordset->Fields->GetItem(_variant_t((long)i))->Name;
	//	info += "\t";
	//}
	//info += "\n";
	//cslAdo.m_pRecordset->MoveFirst();
	////对表进行遍历访问,显示表中每一行的内容
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
//处理ListControl右键单击消息
void CCSLPeilv::OnNMRClickLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	if (pNMItemActivate->iItem != -1 && WDS_ADMIN)
	{
		//取得列ID的值
		g_strID = m_listCtrl.GetItemText(pNMItemActivate->iItem, 0);

		CMenu* popup = m_menu.GetSubMenu(0);// 菜单
		ASSERT(popup != NULL);
		//指定弹出菜单的显示位置
		POINT point;
		GetCursorPos(&point);
		popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	}

	*pResult = 0;
}
//响应菜单消息
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


//----------------------list control 用户管理项数据，避免大数据显示太慢-----------------------------
//1、设置list control属性owner data为true
//2、定义数据结构，比如MY_SHOW_DATA，并定义此结构的变量vector<MY_SHOW_DATA> g_ShowData; 使用vector向量等结构保存查询出的记录数据
//3、设置当前记录数num，m_listCtrl.SetItemCountEx(num, LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
//4、必须加Invalidate, 窗口失去焦点时, 不会实时显示数据m_listCtrl.Invalidate();
//5、响应控件的LVN_GETDISPINFO消息，如下函数,使用switch case来显示自己之前保存数据
void CCSLPeilv::OnLvnGetdispinfoLstCslPeilv(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVDISPINFO *pDispInfo = reinterpret_cast<NMLVDISPINFO*>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
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

