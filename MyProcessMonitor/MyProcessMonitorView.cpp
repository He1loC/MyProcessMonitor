
// MyProcessMonitorView.cpp : implementation of the CMyProcessMonitorView class
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "MyProcessMonitor.h"
#endif

#include "MyProcessMonitorDoc.h"
#include "MyProcessMonitorView.h"

#include <tlhelp32.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include "CViewThreadsDlg.h"
#include "CViewModulesDlg.h"
#include "CViewWindowsDlg.h"


// CMyProcessMonitorView

IMPLEMENT_DYNCREATE(CMyProcessMonitorView, CListView)

BEGIN_MESSAGE_MAP(CMyProcessMonitorView, CListView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CListView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CListView::OnFilePrintPreview)
	ON_NOTIFY_REFLECT(NM_RCLICK, &CMyProcessMonitorView::OnNMRClick)
	ON_COMMAND(ID_RIGHT_REFRESH, &CMyProcessMonitorView::OnRightRefresh)
	ON_COMMAND(ID_RIGHT_VIEWTHREADS, &CMyProcessMonitorView::OnRightViewthreads)
	ON_COMMAND(ID_RIGHT_VIEWMODULES, &CMyProcessMonitorView::OnRightViewmodules)
	ON_COMMAND(ID_RIGHT_VIEWWINDOWS, &CMyProcessMonitorView::OnRightViewwindows)
END_MESSAGE_MAP()

// CMyProcessMonitorView construction/destruction

CMyProcessMonitorView::CMyProcessMonitorView() noexcept
{
	// TODO: add construction code here

}

CMyProcessMonitorView::~CMyProcessMonitorView()
{
}

BOOL CMyProcessMonitorView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs
	cs.style |= LVS_REPORT;
	BOOL bRet = CListView::PreCreateWindow(cs);
	return bRet;
}


void CMyProcessMonitorView::OnDraw(CDC* /*pDC*/)
{
	CMyProcessMonitorDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}





void CMyProcessMonitorView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().
	// this code only works for a report-mode list view
	//ASSERT(GetStyle() & LVS_REPORT);

	CListCtrl& listCtrl = GetListCtrl();
	listCtrl.SetExtendedStyle(listCtrl.GetExtendedStyle() | LVS_EX_FULLROWSELECT);

	// Insert a column. This override is the most convenient.
	listCtrl.InsertColumn(0, _T("Name"));
	listCtrl.InsertColumn(1, _T("PID"));

	// Set reasonable widths for our columns
	listCtrl.SetColumnWidth(0, 200);
	listCtrl.SetColumnWidth(1, 100);

	MyInit();
}


// CMyProcessMonitorView printing

BOOL CMyProcessMonitorView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMyProcessMonitorView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMyProcessMonitorView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMyProcessMonitorView diagnostics

#ifdef _DEBUG
void CMyProcessMonitorView::AssertValid() const
{
	CListView::AssertValid();
}

void CMyProcessMonitorView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CMyProcessMonitorDoc* CMyProcessMonitorView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyProcessMonitorDoc)));
	return (CMyProcessMonitorDoc*)m_pDocument;
}
#endif //_DEBUG


// CMyProcessMonitorView message handlers


void CMyProcessMonitorView::MyInit()
{
	// TODO: Add your implementation code here.
	CListCtrl& listCtrl = GetListCtrl();

	listCtrl.DeleteAllItems();

	// Take a snapshot of all processes in the system.
	auto hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		AfxMessageBox(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return;
	}

	PROCESSENTRY32 pe32{};
	pe32.dwSize = sizeof(pe32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		AfxMessageBox(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return;
	}

	int i = 0;
	CString strProcessID;
	CString strProcessName;
	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		strProcessName.Format(_T("%s"), pe32.szExeFile);
		listCtrl.InsertItem(i, strProcessName);

		strProcessID.Format(_T("%d"), pe32.th32ProcessID);
		listCtrl.SetItemText(i, 1, strProcessID);
		i++;

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return;

}
/*
BOOL GetProcessList()
{
	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	// Take a snapshot of all processes in the system.
	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printError(TEXT("CreateToolhelp32Snapshot (of processes)"));
		return(FALSE);
	}

	// Set the size of the structure before using it.
	pe32.dwSize = sizeof(PROCESSENTRY32);

	// Retrieve information about the first process,
	// and exit if unsuccessful
	if (!Process32First(hProcessSnap, &pe32))
	{
		printError(TEXT("Process32First")); // show cause of failure
		CloseHandle(hProcessSnap);          // clean the snapshot object
		return(FALSE);
	}

	// Now walk the snapshot of processes, and
	// display information about each process in turn
	do
	{
		_tprintf(TEXT("\n\n====================================================="));
		_tprintf(TEXT("\nPROCESS NAME:  %s"), pe32.szExeFile);
		_tprintf(TEXT("\n-------------------------------------------------------"));

		// Retrieve the priority class.
		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			printError(TEXT("OpenProcess"));
		else
		{
			dwPriorityClass = GetPriorityClass(hProcess);
			if (!dwPriorityClass)
				printError(TEXT("GetPriorityClass"));
			CloseHandle(hProcess);
		}

		_tprintf(TEXT("\n  Process ID        = 0x%08X"), pe32.th32ProcessID);
		_tprintf(TEXT("\n  Thread count      = %d"), pe32.cntThreads);
		_tprintf(TEXT("\n  Parent process ID = 0x%08X"), pe32.th32ParentProcessID);
		_tprintf(TEXT("\n  Priority base     = %d"), pe32.pcPriClassBase);
		if (dwPriorityClass)
			_tprintf(TEXT("\n  Priority class    = %d"), dwPriorityClass);

		// List the modules and threads associated with this process
		ListProcessModules(pe32.th32ProcessID);
		ListProcessThreads(pe32.th32ProcessID);

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
	return(TRUE);
}*/


void CMyProcessMonitorView::OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: Add your control notification handler code here
	CListCtrl& listCtrl = GetListCtrl();


	CPoint pt;
	::GetCursorPos(&pt);

	CMenu menu;
	menu.LoadMenu(IDR_MENU_RIGHT);
	auto pSubMn = menu.GetSubMenu(0);

	// µ¯³ö×Ó²Ëµ¥
	pSubMn->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, pt.x,
		pt.y, this);


	*pResult = 0;
}


void CMyProcessMonitorView::OnRightRefresh()
{
	// TODO: Add your command handler code here
	MyInit();
}


void CMyProcessMonitorView::OnRightViewthreads()
{
	// TODO: Add your command handler code here
	CListCtrl& listCtrl = GetListCtrl();
	auto nIdx = listCtrl.GetSelectionMark();
	if (nIdx == -1)
	{
		return;
	}

	auto strProcessId = listCtrl.GetItemText(nIdx, 1);
	auto nProcessId = _tcstoul(strProcessId,NULL,10);

	CViewThreadsDlg dlg{ nProcessId };
	dlg.DoModal();
}


void CMyProcessMonitorView::OnRightViewmodules()
{
	// TODO: Add your command handler code here
	CListCtrl& listCtrl = GetListCtrl();
	auto nIdx = listCtrl.GetSelectionMark();
	if (nIdx == -1)
	{
		return;
	}

	auto strProcessId = listCtrl.GetItemText(nIdx, 1);
	auto nProcessId = _tcstoul(strProcessId, NULL, 10);

	CViewModulesDlg dlg{ nProcessId };
	dlg.DoModal();

}


void CMyProcessMonitorView::OnRightViewwindows()
{
	// TODO: Add your command handler code here
	CListCtrl& listCtrl = GetListCtrl();
	auto nIdx = listCtrl.GetSelectionMark();
	if (nIdx == -1)
	{
		return;
	}

	auto strProcessId = listCtrl.GetItemText(nIdx, 1);
	auto nProcessId = _tcstoul(strProcessId, NULL, 10);

	CViewWindowsDlg dlg{ nProcessId };
	dlg.DoModal();
}
