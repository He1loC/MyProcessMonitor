#pragma once
#include "afxdialogex.h"


// CViewWindowsDlg dialog

class CViewWindowsDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CViewWindowsDlg)

public:
	CViewWindowsDlg(DWORD dwProcessID, CWnd* pParent = nullptr);   // standard constructor
	virtual ~CViewWindowsDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_VIEWWINDOWS };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
		DWORD m_dwTheProcId;
public:
	CListCtrl m_lcWindows;
	virtual BOOL OnInitDialog();
	void MyInit();
	void TraverseAllSubWindowRecur(HWND hWnd);
	void JudgeAndInsertWindow(HWND hwnd);

};
