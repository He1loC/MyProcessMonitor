// CViewThreadsDlg.cpp : implementation file
//

#include "pch.h"
#include "MyProcessMonitor.h"
#include "afxdialogex.h"
#include "CViewThreadsDlg.h"

#include <tlhelp32.h>

// CViewThreadsDlg dialog

IMPLEMENT_DYNAMIC(CViewThreadsDlg, CDialogEx)

CViewThreadsDlg::CViewThreadsDlg(DWORD dwProcessID, CWnd* pParent /*=nullptr*/)
	: m_dwTheProcId(dwProcessID)
	,CDialogEx(IDD_DIALOG_VIEWTHREADS, pParent)
{
	
}

CViewThreadsDlg::~CViewThreadsDlg()
{
}

void CViewThreadsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_THREADS, m_lcThreads);
}


BEGIN_MESSAGE_MAP(CViewThreadsDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewThreadsDlg message handlers


BOOL CViewThreadsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  Add extra initialization here
	m_lcThreads.SetExtendedStyle(m_lcThreads.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	// Insert a column. This override is the most convenient.
	m_lcThreads.InsertColumn(0, _T("TID"));
	m_lcThreads.InsertColumn(1, _T("Start Address"));

	// Set reasonable widths for our columns
	m_lcThreads.SetColumnWidth(0, 200);
	m_lcThreads.SetColumnWidth(1, 100);

	MyInit();

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void CViewThreadsDlg::MyInit()
{
	// TODO: Add your implementation code here.
	m_lcThreads.DeleteAllItems();

	HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
	THREADENTRY32 te32{};

	// Take a snapshot of all running threads  
	hThreadSnap = CreateToolhelp32Snapshot(TH32CS_SNAPTHREAD, 0);
	if (hThreadSnap == INVALID_HANDLE_VALUE)
		return;

	// Fill in the size of the structure before using it. 
	te32.dwSize = sizeof(te32);

	// Retrieve information about the first thread,
	// and exit if unsuccessful
	if (!Thread32First(hThreadSnap, &te32))
	{
		AfxMessageBox(TEXT("Thread32First"));  // Show cause of failure
		CloseHandle(hThreadSnap);     // Must clean up the snapshot object!
		return;
	}
	// Now walk the thread list of the system,
	// and display information about each thread
	// associated with the specified process
	int i = 0;
	CString strThreadID;
	CString strThreadStartAddr;
	do
	{
		if (te32.th32OwnerProcessID == m_dwTheProcId)
		{
			strThreadID.Format(_T("%d"), te32.th32ThreadID);
			m_lcThreads.InsertItem(i, strThreadID);
			i++;
		}
	} while (Thread32Next(hThreadSnap, &te32));

	//  Don't forget to clean up the snapshot object.
	CloseHandle(hThreadSnap);
	return;
}