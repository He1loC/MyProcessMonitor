
// MyProcessMonitorView.h : interface of the CMyProcessMonitorView class
//

#pragma once


class CMyProcessMonitorView : public CListView
{
protected: // create from serialization only
	CMyProcessMonitorView() noexcept;
	DECLARE_DYNCREATE(CMyProcessMonitorView)

// Attributes
public:
	CMyProcessMonitorDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMyProcessMonitorView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	void MyInit();
	afx_msg void OnNMRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRightRefresh();
	afx_msg void OnRightViewthreads();
	afx_msg void OnRightViewmodules();
	afx_msg void OnRightViewwindows();
};

#ifndef _DEBUG  // debug version in MyProcessMonitorView.cpp
inline CMyProcessMonitorDoc* CMyProcessMonitorView::GetDocument() const
   { return reinterpret_cast<CMyProcessMonitorDoc*>(m_pDocument); }
#endif

