// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "MTApp.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CBCGPMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CBCGPMDIFrameWnd)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_WINDOW_MANAGER, OnWindowManager)
	ON_COMMAND(ID_MDI_MOVE_TO_NEXT_GROUP, OnMdiMoveToNextGroup)
	ON_COMMAND(ID_MDI_MOVE_TO_PREV_GROUP, OnMdiMoveToPrevGroup)
	ON_COMMAND(ID_MDI_NEW_HORZ_TAB_GROUP, OnMdiNewHorzTabGroup)
	ON_COMMAND(ID_MDI_NEW_VERT_GROUP, OnMdiNewVertGroup)
	ON_COMMAND(ID_MDI_CANCEL, OnMdiCancel)
	ON_COMMAND(ID_VIEW_OUTPUT, OnViewOutput)
	ON_UPDATE_COMMAND_UI(ID_VIEW_OUTPUT, OnUpdateViewOutput)
	ON_REGISTERED_MESSAGE(BCGM_ON_RIBBON_CUSTOMIZE, OnRibbonCustomize)
	ON_COMMAND(ID_TOOLS_OPTIONS, OnToolsOptions)
END_MESSAGE_MAP()

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CBCGPMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	CBCGPMDITabParams mdiTabParams;

	mdiTabParams.m_bTabIcons = TRUE;
	mdiTabParams.m_bDocumentMenu = TRUE;
	mdiTabParams.m_style = CBCGPTabWnd::STYLE_3D_SCROLLED;
	mdiTabParams.m_closeButtonMode = CBCGPTabWnd::TAB_CLOSE_BUTTON_HIGHLIGHTED_COMPACT;

	EnableMDITabbedGroups (TRUE, mdiTabParams);

	EnableTearOffMDIChildren();

	if (!CreateRibbonBar ())
	{
		TRACE0("Failed to create ribbon bar\n");
		return -1;      // fail to create
	}


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: add your status bar panes here:
	m_wndStatusBar.AddElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE1, _T("Pane 1"), TRUE), _T("Pane 1"));
	m_wndStatusBar.AddExtendedElement (new CBCGPRibbonStatusBarPane (
		ID_STATUSBAR_PANE2, _T("Pane 2"), TRUE), _T("Pane 2"));

	// Load control bar icons:
	CBCGPToolBarImages imagesWorkspace;
	imagesWorkspace.SetImageSize (CSize (16, 16));
	imagesWorkspace.Load (IDB_WORKSPACE);
	imagesWorkspace.SmoothResize(globalData.GetRibbonImageScale());

	const int nPaneSize = globalUtils.ScaleByDPI(200);


	const int nOutputPaneSize = globalUtils.ScaleByDPI(150);

	if (!m_wndOutput.Create (_T("Output"), this, CSize (nOutputPaneSize, nOutputPaneSize),
		TRUE /* Has gripper */, ID_VIEW_OUTPUT,
		WS_CHILD | WS_VISIBLE | CBRS_BOTTOM))
	{
		TRACE0("Failed to create output bar\n");
		return -1;      // fail to create
	}
	m_wndOutput.SetIcon (imagesWorkspace.ExtractIcon (2), FALSE);

	m_wndOutput.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	EnableAutoHideBars(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndOutput);

	// Enable windows manager:
	EnableWindowsDialog (ID_WINDOW_MANAGER, IDS_WINDOWS_MANAGER, TRUE);

	// Enable windows navigator (activated by Ctrl+Tab/Ctrl+Shift+Tab):
	EnableWindowsNavigator();
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CBCGPMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CBCGPMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CBCGPMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// CMainFrame message handlers




void CMainFrame::OnWindowManager() 
{
	ShowWindowsDialog ();
}

BOOL CMainFrame::OnShowMDITabContextMenu (CPoint point, DWORD dwAllowedItems, BOOL bDrop)
{
	CMenu menu;
	VERIFY(menu.LoadMenu (bDrop ? IDR_POPUP_DROP_MDITABS : IDR_POPUP_MDITABS));

	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL);

	if ((dwAllowedItems & BCGP_MDI_CREATE_HORZ_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_HORZ_TAB_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CREATE_VERT_GROUP) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_NEW_VERT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_NEXT) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_NEXT_GROUP, MF_BYCOMMAND);
	}

	if ((dwAllowedItems & BCGP_MDI_CAN_MOVE_PREV) == 0)
	{
		pPopup->DeleteMenu (ID_MDI_MOVE_TO_PREV_GROUP, MF_BYCOMMAND);
	}

	CBCGPPopupMenu* pPopupMenu = new CBCGPPopupMenu;
	pPopupMenu->SetAutoDestroy (FALSE);
	pPopupMenu->Create (this, point.x, point.y, pPopup->GetSafeHmenu ());

	return TRUE;
}

void CMainFrame::OnMdiMoveToNextGroup() 
{
	MDITabMoveToNextGroup ();
}

void CMainFrame::OnMdiMoveToPrevGroup() 
{
	MDITabMoveToNextGroup (FALSE);
}

void CMainFrame::OnMdiNewHorzTabGroup() 
{
	MDITabNewGroup (FALSE);
}

void CMainFrame::OnMdiNewVertGroup() 
{
	MDITabNewGroup ();
}

void CMainFrame::OnMdiCancel() 
{
	// TODO: Add your command handler code here
	
}

CBCGPMDIChildWnd* CMainFrame::CreateDocumentWindow (LPCTSTR lpcszDocName, CObject* /*pObj*/)
{
	if (lpcszDocName != NULL && lpcszDocName [0] != '\0')
	{
		CDocument* pDoc = AfxGetApp()->OpenDocumentFile (lpcszDocName);

		if (pDoc != NULL)
		{
			POSITION pos = pDoc->GetFirstViewPosition();

			if (pos != NULL)
			{
				CView* pView = pDoc->GetNextView (pos);
				if (pView == NULL)
				{
					return NULL;
				}

				return DYNAMIC_DOWNCAST (CBCGPMDIChildWnd, pView->GetParent ());
			}   
		}
	}

	return NULL;
}

void CMainFrame::OnClose() 
{
	SaveMDIState (theApp.GetRegSectionPath ());
	CBCGPMDIFrameWnd::OnClose();
}


BOOL CMainFrame::CreateRibbonBar ()
{
	m_wndRibbonBar.EnableCustomization();

	if (!m_wndRibbonBar.Create(this))
	{
		return FALSE;
	}

	// Load panel images:
	m_PanelImages.SetImageSize (CSize (16, 16));
	m_PanelImages.Load (IDB_RIBBON_ICONS);
	globalUtils.ScaleByDPI(m_PanelImages);

	// Init main button:
	m_MainButton.SetImage (IDB_RIBBON_MAIN);
	m_MainButton.SetToolTipText (_T("File"));
	m_MainButton.SetText (_T("\nf"));

	m_wndRibbonBar.SetMainButton (&m_MainButton, CSize (45, 45));

	CBCGPRibbonMainPanel* pMainPanel = m_wndRibbonBar.AddMainCategory (
		_T("File"), IDB_RIBBON_FILESMALL, IDB_RIBBON_FILELARGE);

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_NEW, _T("&New"), 0, 0));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_OPEN, _T("&Open..."), 1, 1));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE, _T("&Save"), 2, 2));

	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_SAVE_AS, _T("Save &As..."), 3, 3));

	CBCGPRibbonButton* pBtnPrint = new CBCGPRibbonButton (ID_FILE_PRINT, _T("&Print"), 4, 4);

	pBtnPrint->AddSubItem (new CBCGPRibbonLabel (_T("Preview and print the document")));
	pBtnPrint->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT, _T("&Print"), 4, 4, TRUE));
	pBtnPrint->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_DIRECT, _T("&Quick Print"), 7, 7, TRUE));
	pBtnPrint->AddSubItem (new CBCGPRibbonButton (ID_FILE_PRINT_PREVIEW, _T("Print Pre&view"), 6, 6, TRUE));

	pBtnPrint->SetKeys (_T("p"), _T("w"));

	pMainPanel->Add (pBtnPrint);

	pMainPanel->Add (new CBCGPRibbonSeparator (TRUE));
	pMainPanel->Add (new CBCGPRibbonButton (ID_FILE_CLOSE, _T("&Close"), 5, 5));

	pMainPanel->AddRecentFilesList (_T("Recent Documents"), 300, TRUE /* Show Pins */);

	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_TOOLS_OPTIONS, _T("Opt&ions"), 9));
	pMainPanel->AddToBottom (new CBCGPRibbonMainPanelButton (ID_APP_EXIT, _T("E&xit"), 8));

	// Add "Home" category with "Clipboard" panel:
	CBCGPRibbonCategory* pCategory = m_wndRibbonBar.AddCategory (
		_T("&Home"),
		IDB_RIBBON_HOMESMALL,
		IDB_RIBBON_HOMELARGE);

	// Create "Clipboard" panel:
	CBCGPRibbonPanel* pPanelClipboard = pCategory->AddPanel (
		_T("Clipboard"), 
		m_PanelImages.ExtractIcon (1));

	CBCGPRibbonButton* pBtnPaste = new CBCGPRibbonButton (ID_EDIT_PASTE, _T("Paste\nv"), 0, 0);
	pBtnPaste->SetMenu (IDR_PASTE_MENU, TRUE);
	pPanelClipboard->Add (pBtnPaste);

	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_CUT, _T("Cut\nx"), 1));
	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_COPY, _T("Copy\nc"), 2));
	pPanelClipboard->Add (new CBCGPRibbonButton (ID_EDIT_CLEAR, _T("Erase\nr"), 3));

	pPanelClipboard->SetKeys (_T("zc"));

	// Create "Window" panel:
	CBCGPRibbonPanel* pPanelWindow = pCategory->AddPanel (
		_T("Window"), 
		m_PanelImages.ExtractIcon (2));

	pPanelWindow->Add (new CBCGPRibbonCheckBox (ID_VIEW_OUTPUT, _T("Output\no")));

	pPanelWindow->Add (new CBCGPRibbonCheckBox (ID_VIEW_STATUS_BAR, _T("Status Bar\ns")));

	pPanelWindow->Add (new CBCGPRibbonSeparator);

	CBCGPRibbonButton* pBtnWindows = new CBCGPRibbonButton (0, _T("Windows\nw"), -1, 1);
	pBtnWindows->SetMenu (IDR_WINDOWS_MENU, TRUE);
	pPanelWindow->Add (pBtnWindows);

	// Add the launch button at the bottom of the panel:
	pPanelWindow->EnableLaunchButton (ID_WINDOW_MANAGER, -1, _T("l"));

	pPanelWindow->SetKeys (_T("zw"));

	// Create "Editing" panel:
	CBCGPRibbonPanel* pPanelEditing = pCategory->AddPanel(_T("Editing"),
		m_PanelImages.ExtractIcon (4));

	pPanelEditing->Add(new CBCGPRibbonButton(ID_EDIT_SELECT_ALL, _T("Select All\na"), 5));
	pPanelEditing->Add(new CBCGPRibbonButton(ID_EDIT_FIND, _T("Find\nfd"), 6));
	pPanelEditing->Add(new CBCGPRibbonButton(ID_EDIT_REPLACE, _T("Replace\nr"), 7));
	// Create "<TODO>" panel:
	CBCGPRibbonPanel* pPanelTODO = pCategory->AddPanel (
		_T("<TODO>"));
	pPanelTODO->Add (new CBCGPRibbonLabel (_T("TODO: add elements here")));

	pPanelTODO->SetKeys (_T("zt"));

	// Add some hidden (non-ribbon) elements:
	CBCGPRibbonUndoButton* pUndo = new CBCGPRibbonUndoButton (ID_EDIT_UNDO, _T("Undo"), 4);

	pUndo->AddUndoAction (_T("Undo Item 1"));
	pUndo->AddUndoAction (_T("Undo Item 2"));
	pUndo->AddUndoAction (_T("Undo Item 3"));
	pUndo->AddUndoAction (_T("Undo Item 4"));
	pUndo->AddUndoAction (_T("Undo Item 5"));

	pCategory->AddHidden (pUndo);

	// Add "<TODO>" category with "Clipboard" panel:
	CBCGPRibbonCategory* pTODOCategory = m_wndRibbonBar.AddCategory (
		_T("&TODO"), 
		0 /* TODO: ID-category small images */, 
		0 /* TODO: ID-category large images */);

	// Add quick access toolbar commands:
	CBCGPRibbonQATDefaultState qatState;

	qatState.AddCommand (ID_FILE_NEW, FALSE);
	qatState.AddCommand (ID_FILE_OPEN, FALSE);
	qatState.AddCommand (ID_FILE_SAVE);
	qatState.AddCommand (ID_FILE_PRINT_DIRECT);
	qatState.AddCommand (ID_FILE_PRINT_PREVIEW, FALSE);
	qatState.AddCommand (ID_EDIT_UNDO);

	m_wndRibbonBar.SetQuickAccessDefaultState (qatState);

	// Add "About" button to the right of tabs:
	m_wndRibbonBar.AddToTabs (new CBCGPRibbonButton (ID_APP_ABOUT, _T("\na"), m_PanelImages.ExtractIcon (0)));
	return TRUE;
}

LRESULT CMainFrame::OnRibbonCustomize (WPARAM wp, LPARAM /*lp*/)
{
	ShowOptions (wp == 0 ? 1 : 0);
	return 1;
}

void CMainFrame::OnToolsOptions()
{
	ShowOptions (0);
}

void CMainFrame::ShowOptions (int nPage)
{
	// Create custom categories:
	
	// "Popular" items:
	
	CList<UINT, UINT> lstPopular;
	
	lstPopular.AddTail (ID_FILE_NEW);
	lstPopular.AddTail (ID_FILE_OPEN);
	lstPopular.AddTail (ID_FILE_SAVE);
	lstPopular.AddTail (ID_FILE_PRINT_PREVIEW);
	lstPopular.AddTail (ID_FILE_PRINT_DIRECT);
	lstPopular.AddTail (ID_EDIT_UNDO);
	
	// Hidden commands:
	CList<UINT,UINT> lstHidden;
	m_wndRibbonBar.GetItemIDsList (lstHidden, TRUE);
	
	// All commands:
	CList<UINT,UINT> lstAll;
	m_wndRibbonBar.GetItemIDsList (lstAll);

	// Create "Customize QAT" page:
	CBCGPRibbonCustomizeQATPage pageCustomizeQAT(&m_wndRibbonBar);
	
	pageCustomizeQAT.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeQAT.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeQAT.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create "Customize Ribbon" page:
	CBCGPRibbonCustomizeRibbonPage pageCustomizeRibbon(&m_wndRibbonBar);
	
	pageCustomizeRibbon.AddCustomCategory (_T("Popular Commands"), lstPopular);
	pageCustomizeRibbon.AddCustomCategory (_T("Commands not in the Ribbon"), lstHidden);
	pageCustomizeRibbon.AddCustomCategory (_T("All Commands"), lstAll);
	
	// Create property sheet:
	CBCGPPropertySheet propSheet(_T("Options"), this, nPage);
	propSheet.m_psh.dwFlags |= PSH_NOAPPLYNOW;
	propSheet.EnableLayout(TRUE, NULL, TRUE);
	propSheet.EnableVisualManagerStyle(TRUE, TRUE);
	
	propSheet.SetLook(CBCGPPropertySheet::PropSheetLook_List, -1 /* Default list width */);
	
	propSheet.AddPage (&pageCustomizeRibbon);
	
	propSheet.AddPage (&pageCustomizeQAT);

	// TODO: add your option pages here:
	// COptionsPage1 pageOptions1;
	// propSheet.AddPage (&pageOptions1);
	//
	// COptionsPage1 pageOptions2;
	// propSheet.AddPage (&pageOptions2);
	
	if (propSheet.DoModal() != IDOK)
	{
		return;
	}
}
void CMainFrame::OnViewOutput() 
{
	ShowControlBar (&m_wndOutput,
					!(m_wndOutput.IsVisible ()),
					FALSE, TRUE);
	RecalcLayout ();
}

void CMainFrame::OnUpdateViewOutput(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck (m_wndOutput.IsVisible ());
	pCmdUI->Enable (!GetDockManager ()->IsPrintPreviewValid ());
}

