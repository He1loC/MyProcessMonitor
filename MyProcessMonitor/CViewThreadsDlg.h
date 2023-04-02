#pragma once
#include "afxdialogex.h"


// CViewThreadsDlg dialog

class CViewThreadsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewThreadsDlg)

public:
	CViewThreadsDlg(DWORD dwProcessID,CWnd* pParent = nullptr);   // standard constructor
	virtual ~CViewThreadsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VIEWTHREADS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
		DWORD m_dwTheProcId;
public:
	CListCtrl m_lcThreads;
	virtual BOOL OnInitDialog();
	void MyInit();
};
