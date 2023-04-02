// CViewWindowsDlg.cpp : implementation file
//

#include "pch.h"
#include "MyProcessMonitor.h"
#include "afxdialogex.h"
#include "CViewWindowsDlg.h"


// CViewWindowsDlg dialog

IMPLEMENT_DYNAMIC(CViewWindowsDlg, CDialogEx)

CViewWindowsDlg::CViewWindowsDlg(DWORD dwProcessID, CWnd* pParent /*=nullptr*/)
	: m_dwTheProcId(dwProcessID)
	, CDialogEx(IDD_DIALOG_VIEWWINDOWS, pParent)
{

}

CViewWindowsDlg::~CViewWindowsDlg()
{
}

void CViewWindowsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_WINDOWS, m_lcWindows);
}


BEGIN_MESSAGE_MAP(CViewWindowsDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewWindowsDlg message handlers


BOOL CViewWindowsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lcWindows.SetExtendedStyle(m_lcWindows.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// Insert a column. This override is the most convenient.
	m_lcWindows.InsertColumn(0, _T("Window Handle"));
	m_lcWindows.InsertColumn(1, _T("Caption"));
	m_lcWindows.InsertColumn(2, _T("Class"));

	// Set reasonable widths for our columns
	m_lcWindows.SetColumnWidth(0, 200);
	m_lcWindows.SetColumnWidth(1, 200);
	m_lcWindows.SetColumnWidth(2, 200);

	MyInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CALLBACK EnumChildWindowsProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	

	
	return TRUE;
}

BOOL CALLBACK EnumWindowsProc(
	_In_ HWND   hwnd,
	_In_ LPARAM lParam
)
{
	DWORD dwPid = 0;
	::GetWindowThreadProcessId(hwnd, &dwPid); // 获得找到窗口所属的进程

	if (dwPid == lParam) // 判断是否是目标进程的窗口
	{
		
		//获取窗口标题
		TCHAR strWindowText[MAX_PATH] = { 0 };
		::GetWindowText(hwnd, strWindowText, MAX_PATH);

		//获取窗口类名
		TCHAR strWindowClassName[MAX_PATH] = { 0 };
		::GetClassName(hwnd, strWindowClassName, MAX_PATH);


		

		EnumChildWindows(hwnd, &EnumChildWindowsProc, lParam);    // 继续查找子窗口
	}
	return TRUE;
}


void CViewWindowsDlg::MyInit()
{
	// TODO: Add your implementation code here.
	// 获取窗口句柄
	auto hDesktop = ::GetDesktopWindow();
	TraverseAllSubWindowRecur(hDesktop);
}

void CViewWindowsDlg::TraverseAllSubWindowRecur(HWND hWnd)
{
	JudgeAndInsertWindow(hWnd);
	auto hChildWnd = ::GetWindow(hWnd, GW_CHILD);
	if (hChildWnd == NULL)
	{
		return;
	}

	do
	{
		TraverseAllSubWindowRecur(hChildWnd);
		hChildWnd = ::GetWindow(hChildWnd, GW_HWNDNEXT);
	} while (hChildWnd != NULL);
}

void CViewWindowsDlg::JudgeAndInsertWindow(HWND hwnd)
{
	DWORD dwPid = 0;
	::GetWindowThreadProcessId(hwnd, &dwPid); // 获得找到窗口所属的进程
	if (dwPid == m_dwTheProcId) // 判断是否是目标进程的窗口
	{
		CString strHandle;
		strHandle.Format(_T("%p"), hwnd);
		//获取窗口标题
		TCHAR strWindowText[MAX_PATH] = { 0 };
		::GetWindowText(hwnd, strWindowText, MAX_PATH);

		//获取窗口类名
		TCHAR strWindowClassName[MAX_PATH] = { 0 };
		::GetClassName(hwnd, strWindowClassName, MAX_PATH);

		auto nIdx = m_lcWindows.GetItemCount();
		m_lcWindows.InsertItem(nIdx, strHandle);
		int j = 1;
		m_lcWindows.SetItemText(nIdx, j++, strWindowText);
		m_lcWindows.SetItemText(nIdx, j++, strWindowClassName);
	}
}
