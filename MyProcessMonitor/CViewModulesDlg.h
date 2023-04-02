#pragma once
#include "afxdialogex.h"


// CViewModulesDlg dialog

class CViewModulesDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewModulesDlg)

public:
	CViewModulesDlg(DWORD dwProcessID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CViewModulesDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VIEWMODULES };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
		DWORD m_dwTheProcId;
public:
	CListCtrl m_lcModules;
	virtual BOOL OnInitDialog();
	void MyInit();
};
