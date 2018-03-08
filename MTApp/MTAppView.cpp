// MTAppView.cpp : implementation of the CMTAppView class
//

#include "stdafx.h"
#include "MTApp.h"

#include "MTAppDoc.h"
#include "MTAppView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMTAppView

IMPLEMENT_DYNCREATE(CMTAppView, CView)

BEGIN_MESSAGE_MAP(CMTAppView, CView)
	ON_WM_CONTEXTMENU()
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_MESSAGE(WM_PRINTCLIENT, OnPrintClient)
	ON_REGISTERED_MESSAGE(BCGM_CHANGEVISUALMANAGER, OnChangeVisualManager)
END_MESSAGE_MAP()

// CMTAppView construction/destruction

CMTAppView::CMTAppView()
{
	// TODO: add construction code here

}

CMTAppView::~CMTAppView()
{
}

BOOL CMTAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMTAppView drawing

void CMTAppView::OnDraw(CDC* /*pDC*/)
{
	CMTAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


// CMTAppView printing

void CMTAppView::OnFilePrintPreview()
{
	BCGPPrintPreview (this);
}

BOOL CMTAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMTAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMTAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// CMTAppView diagnostics

#ifdef _DEBUG
void CMTAppView::AssertValid() const
{
	CView::AssertValid();
}

void CMTAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMTAppDoc* CMTAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMTAppDoc)));
	return (CMTAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CMTAppView message handlers

LRESULT CMTAppView::OnPrintClient(WPARAM wp, LPARAM lp)
{
	if ((lp & PRF_CLIENT) == PRF_CLIENT)
	{
		CDC* pDC = CDC::FromHandle((HDC) wp);
		ASSERT_VALID(pDC);
		
		OnDraw(pDC);
	}
	
	return 0;
}

LRESULT CMTAppView::OnChangeVisualManager(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	return 0;
}

void CMTAppView::OnContextMenu(CWnd*, CPoint point)
{
	if (CBCGPPopupMenu::GetSafeActivePopupMenu() != NULL)
	{
		return;
	}

	theApp.ShowPopupMenu(IDR_CONTEXT_MENU, point, this);
}
