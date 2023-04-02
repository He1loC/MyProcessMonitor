// CViewModulesDlg.cpp : implementation file
//

#include "pch.h"
#include "MyProcessMonitor.h"
#include "afxdialogex.h"
#include "CViewModulesDlg.h"

#include <tlhelp32.h> 
// CViewModulesDlg dialog

IMPLEMENT_DYNAMIC(CViewModulesDlg, CDialogEx)

CViewModulesDlg::CViewModulesDlg(DWORD dwProcessID, CWnd* pParent /*=nullptr*/)
	: m_dwTheProcId(dwProcessID)
	, CDialogEx(IDD_DIALOG_VIEWMODULES, pParent)
{

}

CViewModulesDlg::~CViewModulesDlg()
{
}

void CViewModulesDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODULES, m_lcModules);
}


BEGIN_MESSAGE_MAP(CViewModulesDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewModulesDlg message handlers


BOOL CViewModulesDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lcModules.SetExtendedStyle(m_lcModules.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// Insert a column. This override is the most convenient.
	m_lcModules.InsertColumn(0, _T("Name"));
	m_lcModules.InsertColumn(1, _T("Base Address"));
	m_lcModules.InsertColumn(2, _T("Size"));

	// Set reasonable widths for our columns
	m_lcModules.SetColumnWidth(0, 200);
	m_lcModules.SetColumnWidth(1, 200);
	m_lcModules.SetColumnWidth(2, 100);

	MyInit();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CViewModulesDlg::MyInit()
{
	// TODO: Add your implementation code here.

	HANDLE hModuleSnap = INVALID_HANDLE_VALUE;
	MODULEENTRY32 me32={};

	//  Take a snapshot of all modules in the specified process. 
	hModuleSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, m_dwTheProcId);
	if (hModuleSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(TEXT("CreateToolhelp32Snapshot (of modules)"));
		return;
	}

	//  Set the size of the structure before using it. 
	me32.dwSize = sizeof(me32);

	//  Retrieve information about the first module, 
	//  and exit if unsuccessful 
	if (!Module32First(hModuleSnap, &me32))
	{
		AfxMessageBox(TEXT("Module32First"));  // Show cause of failure 
		CloseHandle(hModuleSnap);     // Must clean up the snapshot object! 
		return;
	}

	//  Now walk the module list of the process, 
	//  and display information about each module 
	int i = 0;
	CString strModuleName;
	CString strModuleBaseAddress;
	CString strModuleSize;
	do
	{
		strModuleName.Format(TEXT("%s"), me32.szModule);
		strModuleBaseAddress.Format(TEXT("0x%p"), (QWORD)me32.modBaseAddr);
		strModuleSize.Format(TEXT("%d"), me32.modBaseSize);

		m_lcModules.InsertItem(i, strModuleName);

		int j = 1;
		m_lcModules.SetItemText(i, j++, strModuleBaseAddress);
		m_lcModules.SetItemText(i, j++, strModuleSize);

		i++;
	} while (Module32Next(hModuleSnap, &me32));

	//  Do not forget to clean up the snapshot object. 
	CloseHandle(hModuleSnap);
	return;
}
