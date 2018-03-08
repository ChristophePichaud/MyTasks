// MTAppDoc.cpp : implementation of the CMTAppDoc class
//

#include "stdafx.h"
#include "MTApp.h"

#include "MTAppDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMTAppDoc

IMPLEMENT_DYNCREATE(CMTAppDoc, CDocument)

BEGIN_MESSAGE_MAP(CMTAppDoc, CDocument)
END_MESSAGE_MAP()


// CMTAppDoc construction/destruction

CMTAppDoc::CMTAppDoc()
{
	// TODO: add one-time construction code here

}

CMTAppDoc::~CMTAppDoc()
{
}

BOOL CMTAppDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}




// CMTAppDoc serialization

void CMTAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CMTAppDoc diagnostics

#ifdef _DEBUG
void CMTAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CMTAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CMTAppDoc commands
