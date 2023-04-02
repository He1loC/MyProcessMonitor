
// MyProcessMonitor.h : main header file for the MyProcessMonitor application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CMyProcessMonitorApp:
// See MyProcessMonitor.cpp for the implementation of this class
//

class CMyProcessMonitorApp : public CWinApp
{
public:
	CMyProcessMonitorApp() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CMyProcessMonitorApp theApp;
