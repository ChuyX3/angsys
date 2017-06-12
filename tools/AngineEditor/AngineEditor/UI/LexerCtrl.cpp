// UI\LexerCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "AngineEditor.h"
#include "UI\LexerCtrl.h"

using namespace ang;

static cwstr_t szExtensions[] =
{
	TEXT("py"),                                                                // SCLEX_PYTHON
	TEXT("c|cc|cpp|cxx|cs|h|hh|hpp|hxx|sma|dlg|rc|rc2|idl|odl|mak|js|java|hlsl|glsl|fx"),   // SCLEX_CPP
	TEXT("htm|html|shtml|htt|cfm|tpl|hta"),         // SCLEX_HTML
	TEXT("xml|gcl|xsl|svg|xul|xsd|dtd|xslt|axl"),   // SCLEX_XML
	TEXT("pl|pm|cgi|pod"),                          // SCLEX_PERL
	TEXT("sql|spec|body|sps|spb|sf|sp"),            // SCLEX_SQL
	TEXT("vb|bas|frm|cls|ctl|pag|dsr|dob"),         // SCLEX_VB
	TEXT("properties"),                             // SCLEX_PROPERTIES
	TEXT("err"),                                    // SCLEX_ERRORLIST
	TEXT("iface|mak"),                              // SCLEX_MAKEFILE
	TEXT("bat|cmd|nt"),                             // SCLEX_BATCH
	TEXT("xcode"),                                  // SCLEX_XCODE
	TEXT("tex|sty"),                                // SCLEX_LATEX
	TEXT("lua"),                                    // SCLEX_LUA
	TEXT("diff"),                                   // SCLEX_DIFF
	TEXT("conf"),                                   // SCLEX_CONF
	TEXT("pas|inc|pp"),                             // SCLEX_PASCAL
	TEXT("ave"),                                    // SCLEX_AVE
	TEXT("ada|ads|adb"),                            // SCLEX_ADA
	TEXT("lsp|lisp|scm|smd|ss"),                    // SCLEX_LISP
	TEXT("rb"),                                     // SCLEX_RUBY
	TEXT("e"),                                      // SCLEX_EIFFEL
	TEXT("e"),                                      // SCLEX_EIFFELKW
	TEXT("tcl"),                                    // SCLEX_TCL
	TEXT("tab|spf"),                                // SCLEX_NNCRONTAB
	TEXT("ant"),                                    // SCLEX_BULLANT
	TEXT("vbs|dsm"),                                // SCLEX_VBSCRIPT
	TEXT("asp|aspx"),                               // SCLEX_ASP
	TEXT("php|php3|php4"),                          // SCLEX_PHP
	TEXT("bc|cln"),                                 // SCLEX_BAAN
	TEXT("m"),                                      // SCLEX_MATLAB
	TEXT("sol"),                                    // SCLEX_SCRIPTOL
	TEXT("asm"),                                    // SCLEX_ASM
	TEXT("cpp"),                                    // SCLEX_CPPNOCASE
	TEXT("f|for|f90|f95"),                          // SCLEX_FORTRAN
	TEXT("f77"),                                    // SCLEX_F77
	TEXT("css"),                                    // SCLEX_CSS
	TEXT("pov"),                                    // SCLEX_POV
	TEXT("lt|lot"),                                 // SCLEX_LOUT
	TEXT("src|em"),                                 // SCLEX_ESCRIPT
	0,
};
static int nFormats[] = {
	SCLEX_PYTHON,
	SCLEX_CPP,
	SCLEX_HTML,
	SCLEX_XML,
	SCLEX_PERL,
	SCLEX_SQL,
	SCLEX_VB,
	SCLEX_PROPERTIES,
	SCLEX_ERRORLIST,
	SCLEX_MAKEFILE,
	SCLEX_BATCH,
	SCLEX_XCODE,
	SCLEX_LATEX,
	SCLEX_LUA,
	SCLEX_DIFF,
	SCLEX_CONF,
	SCLEX_PASCAL,
	SCLEX_AVE,
	SCLEX_ADA,
	SCLEX_LISP,
	SCLEX_RUBY,
	SCLEX_EIFFEL,
	SCLEX_EIFFELKW,
	SCLEX_TCL,
	SCLEX_NNCRONTAB,
	SCLEX_BULLANT,
	SCLEX_VBSCRIPT,
	29,//SCLEX_ASP,
	30,//SCLEX_PHP,
	SCLEX_BAAN,
	SCLEX_MATLAB,
	SCLEX_SCRIPTOL,
	SCLEX_ASM,
	SCLEX_CPPNOCASE,
	SCLEX_FORTRAN,
	SCLEX_F77,
	SCLEX_CSS,
	SCLEX_POV,
	SCLEX_LOUT,
	SCLEX_ESCRIPT,
	0,
};


int GetFormatFromExtension(cwstr_t szExtension)
{
	int i = 0;
	ang::wstring extension = szExtensions[i];
	while (szExtensions[i] != NULL)
	{
		auto ext = extension->split('|');
		for(index j = 0, c = ext->size(); j < c; ++j)
			if(szExtension == ext[j])
				return nFormats[j];
		i++;
		extension = szExtensions[i];
	}
	return SCLEX_NULL;
}

// CLexerCtrl

IMPLEMENT_DYNAMIC(CLexerCtrl, CWnd)

CLexerCtrl::CLexerCtrl()
{
	m_bLinenumbers = FALSE;
	m_bSelection = TRUE;
	m_bFolding = FALSE;
	m_nSearchflags = 0;

	m_colorInfo = {
		RGB(0,0,0),
		RGB(0,0,0),
		RGB(0,130,0),
		RGB(0,0,0),
		RGB(0,0,0),
		RGB(0,100,255),
		RGB(180,0,0),
		RGB(0,0,0),
		RGB(0,0,0),
		RGB(200,0,200),
		RGB(120,120,120),
		RGB(0,0,0),
		RGB(200,200,200),
	};


	/*SendMessage(SCI_STYLESETFORE, 0, RGB(0, 0, 0));
	SendMessage(SCI_STYLESETFORE, 2, RGB(0, 130, 0));
	SendMessage(SCI_STYLESETFORE, 5, RGB(0, 100, 255));
	SendMessage(SCI_STYLESETFORE, 6, RGB(180, 0, 0));
	SendMessage(SCI_STYLESETFORE, 9, RGB(0, 0, 255));
	SendMessage(SCI_STYLESETFORE, 10, RGB(120, 120, 120));
	SendMessage(SCI_STYLESETFORE, 11, RGB(0, 0, 0));*/
}

CLexerCtrl::~CLexerCtrl()
{
}


BEGIN_MESSAGE_MAP(CLexerCtrl, CWnd)
END_MESSAGE_MAP()


// CLexerCtrl message handlers

BOOL CLexerCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	return CreateEx(0, dwStyle, rect, pParentWnd, nID);
}


BOOL CLexerCtrl::CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	if (!CWnd::CreateEx(dwExStyle, L"Scintilla", NULL, dwStyle, rect, pParentWnd, nID, NULL))
	{
		return FALSE;
	}
	Init();
	return TRUE;
}

void CLexerCtrl::SetText(cstr_t szText) 
{
	LRESULT lResult = 0;
	mstring text = szText;
	if (szText != NULL)
		lResult = SendMessage(SCI_SETTEXT, 0, (LPARAM)text->buffer_ptr());
	GotoPosition(0);
	SetFocus();
}

void CLexerCtrl::SetText(cwstr_t szText)
{
	LRESULT lResult = 0;
	mstring text = szText;
	if (szText != NULL)
		lResult = SendMessage(SCI_SETTEXT, 0, (LPARAM)text->buffer_ptr());
	GotoPosition(0);
	SetFocus();
}

void CLexerCtrl::SetText(cmstr_t szText)
{
	LRESULT lResult = 0;
	if (szText != NULL)
		lResult = SendMessage(SCI_SETTEXT, 0, (LPARAM)szText.cstr());
	GotoPosition(0);
	SetFocus();
}

CString CLexerCtrl::GetText()const
{
	mstring text;
	wstring unicode;
	LRESULT lLen = SendMessage(SCI_GETLENGTH, 0, 0) + 1;
	if (lLen > 0)
	{
		text = new strings::mstring_buffer(lLen);
		auto ptr = text->map_buffer(0, lLen);
		text->unmap_buffer(ptr, SendMessage(SCI_GETTEXT, lLen, (LPARAM)ptr));
	}
	unicode = text;
	return unicode->cstr();
}

BOOL CLexerCtrl::LoadFile(cwstr_t szFileName)
{
	// try to get extension and figure out what lexer to use
	int nIndex = szFileName.find_rev(".", szFileName.size() - 1) +2;
	wstring strExtension;
	szFileName.sub_string(strExtension, nIndex + 1, szFileName.size() - nIndex);
	SendMessage(SCI_SETLEXER, GetFormatFromExtension(strExtension), 0);

	// ty to open file in sharing mode
	CFile file;
	LPARAM len = 0L;
	mstring strCode;
	// ok success - try to get length of file
	if (file.Open(szFileName.data(), CFile::modeRead | CFile::shareDenyWrite))
	{
		len = file.GetLength();
		if (len > 0)
		{
			strCode = new strings::mstring_buffer(len);
			auto ptr = strCode->map_buffer(0, len);
			strCode->unmap_buffer(ptr, file.Read(ptr, len));
		}
		file.Close();
		//    set text to control
		SetText(strCode);
		//    tell scintilla that we have an unmodified document
		SendMessage(SCI_SETSAVEPOINT, 0, 0);
		GotoPosition(0);

		return TRUE;
	}
	// file open error - return
	return FALSE;
}

BOOL CLexerCtrl::LoadFileDoc(CArchive& ar)
{
	// try to get extension and figure out what lexer to use
	cwstr_t strFile(ar.m_strFileName.GetBuffer(), ar.m_strFileName.GetLength());

	index idx = strFile.find_rev(".", strFile.size() - 1);
	wstring strExtension;
	strFile.sub_string(strExtension, idx + 1, strFile.size() - idx);
	SendMessage(SCI_SETLEXER, GetFormatFromExtension(strExtension), 0);

	// ty to open file in sharing mode
	CFile * file = ar.GetFile();
	LPARAM len = 0L;
	LPARAM nTotal;
	mstring strCode;
	// ok success - try to get length of file
	if (file)
	{
		len = file->GetLength();
		if (len > 0)
		{
			strCode = new strings::mstring_buffer(len);
			auto ptr = strCode->map_buffer(0, len);
			strCode->unmap_buffer(ptr, file->Read(ptr, len));
		}
		file->Close();
		//    set text to control
		SetText(strCode);
		//    tell scintilla that we have an unmodified document
		SendMessage(SCI_SETSAVEPOINT, 0, 0);
		GotoPosition(0);

		return TRUE;
	}
	// file open error - return
	return FALSE;
}

BOOL CLexerCtrl::SaveFileDoc(CArchive& ar)
{
	CFile * file = ar.GetFile();
	if (!file)
		return FALSE;

	ar << GetText();
	
	return TRUE;
}

int CLexerCtrl::GetLinenumberChars()
{
	// try to get number of lines in control
	LRESULT lLines = SendMessage(SCI_GETLINECOUNT, 0, 0);

	int nChars = 1;
	while (lLines > 0)
	{
		lLines = lLines / 10;
		nChars++;
	}
	return nChars; // allow an extra char for safety
}


int CLexerCtrl::GetLinenumberWidth()
{
	// get number of chars needed to display highest linenumber
	int nChars = GetLinenumberChars() + 1;
	// get width of character '9' in pixels
	LRESULT lWidth = SendMessage(SCI_TEXTWIDTH, STYLE_LINENUMBER, (LPARAM)_T("9"));
	return nChars * lWidth;
}


void CLexerCtrl::EnableDisplayingLinenumbers( BOOL bFlag)
{
	// if nothing changes just return
	if (IsDisplayingLinenumbers() == bFlag)
		return;
	// if display is turned off we set margin 0 to 0
	if (!bFlag)
	{
		SendMessage(SCI_SETMARGINWIDTHN, 0, 0);
	}
	// if display is turned o we set margin 0 to the calculated width
	else
	{
		int nWidth = GetLinenumberWidth() + 4;
		SendMessage(SCI_SETMARGINWIDTHN, 0, nWidth);
	}
	m_bLinenumbers = bFlag;
}

void CLexerCtrl::Cut()
{
	SendMessage(SCI_CUT, 0, 0);
}

void CLexerCtrl::Copy()
{
	SendMessage(SCI_COPY, 0, 0);
}

void CLexerCtrl::Paste()
{
	SendMessage(SCI_PASTE, 0, 0);
}

void CLexerCtrl::Clear()
{
	SendMessage(SCI_CLEAR, 0, 0);
}

void CLexerCtrl::SelectAll()
{
	SendMessage(SCI_SELECTALL, 0, 0);
}

void CLexerCtrl::Undo()
{
	SendMessage(SCI_UNDO, 0, 0);
}

void CLexerCtrl::Redo()
{
	SendMessage(SCI_REDO, 0, 0);
}

BOOL CLexerCtrl::CanUndo()
{
	return SendMessage(SCI_CANUNDO, 0, 0) != 0;
}

BOOL CLexerCtrl::CanRedo()
{
	return SendMessage(SCI_CANREDO, 0, 0) != 0;
}

BOOL CLexerCtrl::CanPaste()
{
	return SendMessage(SCI_CANPASTE, 0, 0) != 0;
}

LONG CLexerCtrl::GetCurrentLine()
{
	LONG lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_LINEFROMPOSITION, lPos, 0) + 1;
}

LONG CLexerCtrl::GetCurrentColumn()
{
	LONG lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETCOLUMN, lPos, 0) + 1;
}

LONG CLexerCtrl::GetCurrentPosition()
{
	return SendMessage(SCI_GETCURRENTPOS, 0, 0);
}

DWORD CLexerCtrl::GetCurrentStyle()
{
	LONG lPos = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	return SendMessage(SCI_GETSTYLEAT, lPos, 0);
}

DWORD CLexerCtrl::GetCurrentFoldinglevel()
{
	LONG lLine = GetCurrentLine();
	DWORD level = (SendMessage(SCI_GETFOLDLEVEL, lLine, 0)) & SC_FOLDLEVELNUMBERMASK;
	return level - 1024;
}

void CLexerCtrl::SetFontName(int nStyle, cstr_t szFontname)
{
	SendMessageA(GetSafeHwnd(),SCI_STYLESETFONT, nStyle, (LPARAM)szFontname.get());
}

void CLexerCtrl::SetFontHeight(int nStyle, int nHeight)
{
	SendMessage(SCI_STYLESETSIZE, nStyle, (LPARAM)nHeight);
}

void CLexerCtrl::SetForeground(int nStyle, COLORREF crForeground)
{
	SendMessage(SCI_STYLESETFORE, nStyle, (LPARAM)crForeground);
}

void CLexerCtrl::SetBackground(int nStyle, COLORREF crBackground)
{
	SendMessage(SCI_STYLESETBACK, nStyle, (LPARAM)crBackground);
}

void CLexerCtrl::SetBold(int nStyle, BOOL bBold)
{
	SendMessage(SCI_STYLESETBOLD, nStyle, (LPARAM)bBold);
}

void CLexerCtrl::SetItalic(int nStyle, BOOL bItalic)
{
	SendMessage(SCI_STYLESETITALIC, nStyle, (LPARAM)bItalic);
}

void CLexerCtrl::SetUnderline(int nStyle, BOOL bUnderline)
{
	SendMessage(SCI_STYLESETUNDERLINE, nStyle, (LPARAM)bUnderline);
}

BOOL CLexerCtrl::GetOverstrike()
{
	return SendMessage(SCI_GETOVERTYPE, 0, 0);
}

void CLexerCtrl::SetOverstrike(BOOL bOverstrike)
{
	SendMessage(SCI_SETOVERTYPE, bOverstrike, 0);
}


CString CLexerCtrl::GetFontName(int nStyle)
{
	char text[200];
	SendMessage(SCI_STYLEGETFONT, nStyle, (LPARAM)text);

	wstring out = text;
	return out->cstr();
}

int CLexerCtrl::GetFontHeight(int nStyle)
{
	return SendMessage(SCI_STYLEGETSIZE, nStyle);
}

COLORREF CLexerCtrl::GetForeground(int nStyle)
{
	return SendMessage(SCI_STYLEGETFORE, nStyle);
}

COLORREF CLexerCtrl::GetBackground(int nStyle)
{
	return SendMessage(SCI_STYLEGETBACK, nStyle);
}

BOOL CLexerCtrl::GetBold(int nStyle)
{
	return SendMessage(SCI_STYLEGETBOLD, nStyle);
}

BOOL CLexerCtrl::GetItalic(int nStyle)
{
	return SendMessage(SCI_STYLEGETITALIC, nStyle);
}

BOOL CLexerCtrl::GetUnderline(int nStyle)
{
	return SendMessage(SCI_STYLEGETUNDERLINE, nStyle);
}


void CLexerCtrl::EnableDisplayingSelection(BOOL bFlag)
{
	m_bSelection = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 1, 16);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 1, 0);
}

void CLexerCtrl::EnableDisplayingFolding(BOOL bFlag)
{
	m_bFolding = bFlag;
	if (bFlag)
		SendMessage(SCI_SETMARGINWIDTHN, 2, 16);
	else
		SendMessage(SCI_SETMARGINWIDTHN, 2, 0);
}

void CLexerCtrl::Init()
{
	// clear all text styles
	SendMessage(SCI_CLEARDOCUMENTSTYLE, 0, 0);
	// set the number of styling bits to 7 - the asp/html views need a lot of styling - default is 5
	// If you leave the default you will see twiggle lines instead of ASP code
	SendMessage(SCI_SETSTYLEBITS, 7, 0);
	// set the display for indetation guides to on - this displays virtical dotted lines from the beginning of 
	// a code block to the end of the block
	SendMessage(SCI_SETINDENTATIONGUIDES, TRUE, 0);
	// set tabwidth to 3
	SendMessage(SCI_SETTABWIDTH, 3, 0);
	// set indention to 3
	SendMessage(SCI_SETINDENT, 3, 0);
	// set the caret blinking time to 400 milliseconds
	SendMessage(SCI_SETCARETPERIOD, 400, 0);
	// source folding section
	// tell the lexer that we want folding information - the lexer supplies "folding levels"
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.html.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.comment"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.at.else"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.flags"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("fold.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("styling.within.preprocessor"), (LPARAM)_T("1"));
	SendMessage(SCI_SETPROPERTY, (WPARAM)_T("asp.default.language"), (LPARAM)_T("1"));
	// Tell scintilla to draw folding lines UNDER the folded line
	SendMessage(SCI_SETFOLDFLAGS, 16, 0);
	// Set margin 2 = folding margin to display folding symbols
	SendMessage(SCI_SETMARGINMASKN, 2, SC_MASK_FOLDERS);
	// allow notifications for folding actions
	SendMessage(SCI_SETMODEVENTMASK, SC_MOD_INSERTTEXT | SC_MOD_DELETETEXT, 0);
	//   SendMessage(SCI_SETMODEVENTMASK, SC_MOD_CHANGEFOLD|SC_MOD_INSERTTEXT|SC_MOD_DELETETEXT, 0);
	// make the folding margin sensitive to folding events = if you click into the margin you get a notification event
	SendMessage(SCI_SETMARGINSENSITIVEN, 2, TRUE);
	// define a set of markers to displa folding symbols
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPEN, SC_MARK_MINUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDER, SC_MARK_PLUS);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERSUB, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERTAIL, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEREND, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDEROPENMID, SC_MARK_EMPTY);
	SendMessage(SCI_MARKERDEFINE, SC_MARKNUM_FOLDERMIDTAIL, SC_MARK_EMPTY);
	// set the forground color for some styles

	if(!m_colorInfo.is_empty())for (index i = 0, c = m_colorInfo->counter(); i < c; ++i)
	{
		SendMessage(SCI_STYLESETFORE, i, m_colorInfo[i].code);
	}

	//SendMessage(SCI_STYLESETFORE, 0, RGB(0, 0, 0));
	//SendMessage(SCI_STYLESETFORE, 2, RGB(0, 130, 0));
	//SendMessage(SCI_STYLESETFORE, 5, RGB(0, 100, 255));
	//SendMessage(SCI_STYLESETFORE, 6, RGB(180, 0, 0));
	//SendMessage(SCI_STYLESETFORE, 9, RGB(0, 0, 255));
	//SendMessage(SCI_STYLESETFORE, 10, RGB(120, 120, 120));
	//SendMessage(SCI_STYLESETFORE, 11, RGB(0, 0, 0));
	// set the backgroundcolor of brace highlights
	SendMessage(SCI_STYLESETBACK, STYLE_BRACELIGHT, RGB(0, 255, 0));
	// set end of line mode to CRLF
	SendMessage(SCI_CONVERTEOLS, 2, 0);
	SendMessage(SCI_SETEOLMODE, 2, 0);
	//   SendMessage(SCI_SETVIEWEOL, TRUE, 0);
	// set markersymbol for marker type 0 - bookmark
	SendMessage(SCI_MARKERDEFINE, 0, SC_MARK_CIRCLE);
	// display all margins
	EnableDisplayingLinenumbers(TRUE);
	EnableDisplayingFolding(TRUE);
	EnableDisplayingSelection(TRUE);

}

void CLexerCtrl::UpdateUI()
{
	// do brace matching
	LPARAM lStart = SendMessage(SCI_GETCURRENTPOS, 0, 0);
	LPARAM lEnd = SendMessage(SCI_BRACEMATCH, lStart - 1, 0);
	// if there is a matching brace highlight it
	if (lEnd >= 0)
		SendMessage(SCI_BRACEHIGHLIGHT, lStart - 1, lEnd);
	// if there is NO matching brace erase old highlight
	else
		SendMessage(SCI_BRACEHIGHLIGHT, -1, -1);
}

void CLexerCtrl::DoDefaultFolding(int nMargin, LPARAM lPosition)
{
	// simply toggle fold
	if (nMargin == 2)
	{
		LPARAM lLine = SendMessage(SCI_LINEFROMPOSITION, lPosition, 0);
		SendMessage(SCI_TOGGLEFOLD, lLine, 0);
	}
}

void CLexerCtrl::Refresh()
{
	SendMessage(SCI_COLOURISE, 0, -1);
}

void CLexerCtrl::AddBookmark(LPARAM lLine)
{
	SendMessage(SCI_MARKERADD, lLine - 1, 0);
}

void CLexerCtrl::DeleteBookmark(LPARAM lLine)
{
	if (HasBookmark(lLine))
	{
		SendMessage(SCI_MARKERDELETE, lLine - 1, 0);
	}
}

BOOL CLexerCtrl::HasBookmark(LPARAM lLine)
{
	LRESULT n = SendMessage(SCI_MARKERGET, lLine - 1, 0);
	// check mask for markerbit 0
	if (n & 0x1)
		return TRUE;
	return FALSE;
}

void CLexerCtrl::FindNextBookmark()
{
	LONG lLine = SendMessage(SCI_MARKERNEXT, GetCurrentLine(), 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine, 0);
}

void CLexerCtrl::FindPreviousBookmark()
{
	LRESULT lLine = SendMessage(SCI_MARKERPREVIOUS, GetCurrentLine() - 2, 1);
	if (lLine >= 0)
		SendMessage(SCI_GOTOLINE, lLine, 0);
}

void CLexerCtrl::GotoPosition(LPARAM lPos)
{
	SendMessage(SCI_GOTOPOS, lPos, 0);
}

void CLexerCtrl::GotoLine(LPARAM lLine)
{
	SendMessage(SCI_GOTOLINE, lLine - 1, 0);
}

BOOL CLexerCtrl::SearchForward(cwstr_t szText)
{
	if (szText == NULL)
		return FALSE;
	mstring text = szText;

	LPARAM lPos = GetCurrentPosition();
	TextToFind tf;
	tf.lpstrText = (char const*)text->cstr().get();
	tf.chrg.cpMin = lPos + 1;
	tf.chrg.cpMax = SendMessage(SCI_GETLENGTH, 0, 0);
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
	if (lPos >= 0)
	{
		SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (long)&tf);
		return TRUE;
	}
	return FALSE;
}

BOOL CLexerCtrl::SearchBackward(cwstr_t szText)
{
	if (szText == NULL)
		return FALSE;
	mstring text = szText;

	long lPos = GetCurrentPosition();
	long lMinSel = GetSelectionStart();
	TextToFind tf;
	tf.lpstrText = (char const*)text->cstr().get();
	if (lMinSel >= 0)
		tf.chrg.cpMin = lMinSel - 1;
	else
		tf.chrg.cpMin = lPos - 1;
	tf.chrg.cpMax = 0;
	lPos = SendMessage(SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
	if (lPos >= 0)
	{
		SetFocus();
		GotoPosition(lPos);
		SendMessage(SCI_SETSEL, tf.chrgText.cpMin, tf.chrgText.cpMax);
		SendMessage(SCI_FINDTEXT, m_nSearchflags, (LPARAM)&tf);
		return TRUE;
	}
	return FALSE;
}

void CLexerCtrl::ReplaceSearchedText(cwstr_t szText)
{
	if (szText == NULL)
		return;
	mstring text = szText;
	SendMessage(SCI_TARGETFROMSELECTION, 0, 0);
	if (m_nSearchflags & SCFIND_REGEXP)
		SendMessage(SCI_REPLACETARGETRE, text->size(), (LPARAM)text->cstr().get());
	else
		SendMessage(SCI_REPLACETARGET, text->size(), (LPARAM)text->cstr().get());
}
/////////////////////////////////////
// @mfunc Set lexer format - see scintilla.h
// @rvalue void | not used
//
void CLexerCtrl::SetLexer(
	int nLexer) //@parm lexer code e.g. 
{
	SendMessage(SCI_SETLEXER, nLexer, 0);
}
/////////////////////////////////////
// @mfunc Get start of selection (anchor) as character position
// @rvalue long | character position of selection start (anchor) - -1 on error
//
LONG CLexerCtrl::GetSelectionStart()
{
	return SendMessage(SCI_GETSELECTIONSTART, 0, 0);
}
/////////////////////////////////////
// @mfunc Get end of selection as character position
// @rvalue long | character position of selection end - -1 on error
//
LONG CLexerCtrl::GetSelectionEnd()
{
	return SendMessage(SCI_GETSELECTIONEND, 0, 0);
}
/////////////////////////////////////
// @mfunc Get selected string
// @rvalue CString | string with currentliy selected text
//
CString CLexerCtrl::GetSelectedText()
{
	long lLen = (GetSelectionEnd() - GetSelectionStart()) + 1;
	if (lLen > 0)
	{
		TCHAR *p = new TCHAR[lLen + 1];
		if (p != NULL)
		{
			*p = '\0';
			SendMessage(SCI_GETSELTEXT, 0, (long)p);
			CString strReturn = p;
			delete[] p;
			return strReturn;
		}
	}
	return _T("");
}

int CLexerCtrl::ReplaceAll(cwstr_t szFind, cwstr_t szReplace, BOOL bUseSelection)
{
	int nCount = 0;
	// different branches for replace in selection or total file

	mstring strFind = szFind, strReplace = szReplace;
	if (bUseSelection)
	{
		//    get starting selection range
		long lLen = 0;
		long lStart = GetSelectionStart();
		long lEnd = GetSelectionEnd();
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart);
		SendMessage(SCI_SETTARGETEND, lEnd);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strFind->size(), (LPARAM)strFind->cstr().get());
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strReplace->size(), (LPARAM)strReplace->cstr().get());
			else
				lLen = SendMessage(SCI_REPLACETARGET, strReplace->size(), (LPARAM)strReplace->cstr().get());
			//       the end of the selection was changed - recalc the end
			lEnd = GetSelectionEnd();
			//       move start of target behind last change and end of target to new end of selection
			SendMessage(SCI_SETTARGETSTART, lPos + lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strFind->size(), (LPARAM)strFind->cstr().get());
			nCount++;
		}
	}
	else
	{
		//    start with first and last char in buffer
		long lLen = 0;
		long lStart = 0;
		long lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);
		//    set target to selection
		SendMessage(SCI_SETTARGETSTART, lStart, 0);
		SendMessage(SCI_SETTARGETEND, lEnd, 0);
		//    try to find text in target for the first time
		long lPos = SendMessage(SCI_SEARCHINTARGET, strFind->size(), (LPARAM)strFind->cstr().get());
		//    loop over selection until end of selection reached - moving the target start each time
		while (lPos < lEnd && lPos >= 0)
		{
			if (m_nSearchflags & SCFIND_REGEXP) // check for regular expression flag
				lLen = SendMessage(SCI_REPLACETARGETRE, strReplace->size(), (LPARAM)strReplace->cstr().get());
			else
				lLen = SendMessage(SCI_REPLACETARGET, strReplace->size(), (LPARAM)strReplace->cstr().get());
			//       the end of the selection was changed - recalc the end
			lEnd = SendMessage(SCI_GETTEXTLENGTH, 0, 0);
			//       move start of target behind last change and end of target to new end of buffer
			SendMessage(SCI_SETTARGETSTART, lPos + lLen);
			SendMessage(SCI_SETTARGETEND, lEnd);
			//       find again - if nothing found loop exits
			lPos = SendMessage(SCI_SEARCHINTARGET, strFind->size(), (LPARAM)strFind->cstr().get());
			nCount++;
		}
	}
	return nCount;
}
