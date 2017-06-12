#pragma once

#include "Scintilla.h"
#include "SciLexer.h"



// CLexerCtrl
#define IDC_SCINTILLA_CTRL 0X10FFFF00


struct FontStyle
{
	ang::string strFontName;
	ang::graphics::color nForegroundColor;
	ang::graphics::color nBackgroundColor;
	int nFontSize;
	bool bBold;
	bool bItalic;
	bool bStrikeout;
	bool bUnderline;

	inline bool operator == (const FontStyle& other)const {
		return strFontName == other.strFontName
			&& nForegroundColor.code == other.nForegroundColor.code
			&& nBackgroundColor.code == other.nBackgroundColor.code
			&& nFontSize == other.nFontSize
			&& bBold == other.bBold
			&& bItalic == other.bItalic
			&& bStrikeout == other.bStrikeout
			&& bUnderline == other.bUnderline;
	}

	inline bool operator != (const FontStyle& other)const {
		return strFontName != other.strFontName
			|| nForegroundColor.code != other.nForegroundColor.code
			|| nBackgroundColor.code != other.nBackgroundColor.code
			|| nFontSize != other.nFontSize
			|| bBold != other.bBold
			|| bItalic != other.bItalic
			|| bStrikeout != other.bStrikeout
			|| bUnderline != other.bUnderline;
	}
};

ANG_REGISTER_RUNTIME_TYPENAME(FontStyle);

class CLexerCtrl : public CWnd
{
	DECLARE_DYNAMIC(CLexerCtrl)

public:
	CLexerCtrl();
	virtual ~CLexerCtrl();

protected:
	BOOL m_bLinenumbers;
	BOOL m_bSelection;
	BOOL m_bFolding;
	int  m_nSearchflags;

	ang::collections::vector<ang::graphics::color> m_colorInfo;
	ang::collections::map<uint, FontStyle > m_mapFontStyle;

public:
	void SetColorInfo(ang::static_array<ang::graphics::color> color) {
		m_colorInfo = new ang::collections::vector_buffer<ang::graphics::color>(color.size(), color.get());
		index i = 0;
		ang::foreach(m_colorInfo, [&](ang::graphics::color& color) {
			SendMessage(SCI_STYLESETFORE, i++, color.code);
		});

		UpdateUI();
	}

	ang::static_array<ang::graphics::color> GetColorInfo()const { return m_colorInfo; }

public:
	BOOL IsDisplayingLinenumbers() { return m_bLinenumbers; };
	BOOL IsDisplayingSelection() { return m_bSelection; };
	BOOL IsDisplayingFolding() { return m_bFolding; };

	void SetText(ang::cstr_t szText);
	void SetText(ang::cwstr_t szText);
	void SetText(ang::cmstr_t szText);
	CString GetText()const;

	int GetLinenumberChars();
	int GetLinenumberWidth();
	void EnableDisplayingLinenumbers(BOOL enable = TRUE);	
	void EnableDisplayingSelection(BOOL enable = TRUE);
	void EnableDisplayingFolding(BOOL enable = TRUE);

	LONG GetCurrentLine();
	LONG GetCurrentColumn();
	LONG GetCurrentPosition();
	DWORD GetCurrentStyle();
	DWORD GetCurrentFoldinglevel();

	void Cut();
	void Copy();
	void Paste();
	void Clear();
	void Undo();
	void Redo();
	BOOL CanUndo();
	BOOL CanRedo();
	BOOL CanPaste();
	void SelectAll();

	void SetFontName(int nStyle, ang::cstr_t szFontname);
	void SetFontHeight(int nStyle, int nHeight);
	void SetForeground(int nStyle, COLORREF crForeground);
	void SetBackground(int nStyle, COLORREF crBackground);
	void SetBold(int nStyle, BOOL bFlag);
	void SetItalic(int nStyle, BOOL bFlag);
	void SetUnderline(int nStyle, BOOL bFlag);

	void SetOverstrike(BOOL bOverstrike);
	BOOL GetOverstrike();

	CString GetFontName(int nStyle);
	int GetFontHeight(int nStyle);
	COLORREF GetForeground(int nStyle);
	COLORREF GetBackground(int nStyle);
	BOOL GetBold(int nStyle);
	BOOL GetItalic(int nStyle);
	BOOL GetUnderline(int nStyle);
	

	void Init();
	void UpdateUI();
	void DoDefaultFolding(int nMargin, LPARAM lPosition);
	void Refresh();
	void AddBookmark(LPARAM lLine);
	void DeleteBookmark(LPARAM lLine);
	BOOL HasBookmark(LPARAM lLine);
	void FindNextBookmark();
	void FindPreviousBookmark();
	void GotoLine(LPARAM lLine);
	void GotoPosition(LPARAM lPos);
	BOOL SearchForward(ang::cwstr_t szText);
	BOOL SearchBackward(ang::cwstr_t szText);
	void ReplaceSearchedText(ang::cwstr_t szText);
	void SetLexer(int nLexer);
	long GetSelectionStart();
	long GetSelectionEnd();
	CString GetSelectedText();
	int ReplaceAll(ang::cwstr_t szFind, ang::cwstr_t szReplace, BOOL bSelection = TRUE);

	BOOL LoadFile(ang::cwstr_t szFileName);
	BOOL LoadFileDoc(CArchive& ar);
	BOOL SaveFileDoc(CArchive& ar);



protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = IDC_SCINTILLA_CTRL);
	virtual BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID = IDC_SCINTILLA_CTRL);

	
};
