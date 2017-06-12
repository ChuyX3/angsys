/*! Time-stamp: <@(#)SyntaxColor.h   11/11/2004 - 10:14:11   William Hennebois>
 *********************************************************************
 *  \file   : SyntaxColor.h
 *
 *  Project :  just for fun 
 *
 *  Package : Syntax Color + Spelling
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois            Date: 11/11/2004
 *
 *  \brief  Declaration of class 
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  11/11/2004  WH : First Revision
 *
 *********************************************************************
 */

#if !defined(AFX_SYNTAXCOLOR_H__55937F2D_8417_4DB2_9A64_91BC449C5447__INCLUDED_)
#define AFX_SYNTAXCOLOR_H__55937F2D_8417_4DB2_9A64_91BC449C5447__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TOM.h"
#include "Richole.h"
#include <afxtempl.h>
#include <atlbase.h>





//! Highlight have 2 slots, the Normal ( language part) and Comments part
enum KeyWordSlot{KeyWordNormal,KeyWordComments,KeyWordSlotMax};

//! Style fo each highlight , one color font , size by style
enum HighLightStyle {nostyle,normal,syntax,directive,pragma,scomments,mcomments,sstring,dstring,keywordComments,mySpecialWords,styleMax};

//! A Tom style reference the state of parsing, for each characters 
enum SyntaxState
{
	SyntaxStateNormal,		// Normal
	SyntaxStateDblQuote,	//Double Quotes Parsing
	SyntaxStateSingQuote,	//Single Quotes Parsing
	SyntaxStateMuliComment,	//Multi Comment Parsing
	SyntaxStateSingComment,	//Single  Comment Parsing
	SyntaxStateMax

};


//! We code the TOM style to keep parsing information

union TomStyle
{
	long	byLong;
	struct  
	{
		//! highlight style 
		unsigned		ColorStyle  : 8;
		//! Syntax state
		unsigned		SyntaxState : 8;

	}byStruct;
};

//! font attributes 
enum KeyAttrb {noneAttrb,italic=1,bold=2};

class CKeyStyle
{


	//! colors
	COLORREF			m_dwcolor;
	CString				m_sFontName;
	enum KeyAttrb		 m_dwAttrb;
	float				 m_dwSize;

	public:

		CKeyStyle(COLORREF color=0,KeyAttrb fontAttrb=noneAttrb,LPCSTR pFontName="Courier New",float size=10)
		{
			m_dwAttrb	= fontAttrb;
			m_sFontName = pFontName;
			m_dwcolor =  color;
			m_dwSize  = size;
		}

		CKeyStyle( CKeyStyle & src)
		{
			m_dwcolor = src.m_dwcolor;
			m_sFontName = src.m_sFontName;
			m_dwAttrb = src.m_dwAttrb;
			m_dwSize = src.m_dwSize;
		}

		void Apply(ITextFont *pFont)
		{
			pFont->SetForeColor(m_dwcolor);
			pFont->SetName(CComBSTR(m_sFontName));
			pFont->SetItalic(m_dwAttrb & italic ?tomTrue:tomFalse);
			pFont->SetWeight(m_dwAttrb & bold ?FW_BOLD:FW_NORMAL);

			pFont->SetSize(m_dwSize);




		}



};


/*!
 \brief  This class provides the interface for CKeySyn


USAGE
	KeyWords database

*/
class CKeySyn
{
public:

	CString			m_sKey;	// KeyWord
	HighLightStyle	m_dwstyle; // the highlight style

	CKeySyn(LPCSTR pKeyWord=0,HighLightStyle style=normal)
	{
		m_sKey	  = pKeyWord;
		m_dwstyle = style;

	}

	CKeySyn(CKeySyn & src)
	{
		m_sKey = src.m_sKey;
		m_dwstyle = src.m_dwstyle;


	}


};


/*!
 \brief  This class provides the interface for CNoChangeEdit

USAGE
	avoid troubles with notification and modification

\todo finish to implement  this class 

*/
class CNoChangeEdit
{
	CRichEditCtrl *m_pCtrl;
	long			m_mask;
	int				m_bmodify;

	public:

		CNoChangeEdit(CRichEditCtrl *pCtrl)
		{
			m_pCtrl = pCtrl;
			m_mask = m_pCtrl->GetEventMask();
			m_pCtrl->SetEventMask(m_mask & ~(ENM_CHANGE) );
			m_bmodify = m_pCtrl->GetModify();


		}

	~CNoChangeEdit()
	{
			m_pCtrl->SetModify(m_bmodify);
			m_pCtrl->SetEventMask(m_mask);
	}



};







/*!
 \brief  This class provides the interface for CSyntaxColor

USAGE
	Main class for colorizing



*/
class CSyntaxColor  
{

	enum								StyleState{start,checkend,forcestart};
	typedef void (CSyntaxColor:: *tChangeState)(StyleState state);

protected:


	void								CreateDefaultKeyList();
	void								CreateDefaultStyle();
	void								CreateCommentsAndStringsTagsForCpp();

	int									ColorizeRange(int iOffset,int end,bool fromScratch=true);
	int									AnalyseMultiCommentRange(int start,int end,bool fromScratch);
	void								GetCharsVisible(CHARRANGE & range);
	BOOL								SetStyle(int start,int end,HighLightStyle style);
	void								CloseStyle( int end);
	void								OpenStyle(int start, HighLightStyle style);
	void								AddCharForKeyWord(WCHAR thechar);
	CKeySyn * 							FindKeyWord(CArray<CKeySyn,CKeySyn &> & m_tListKeyword,LPCSTR word);
	TomStyle							GetStyle( int start,int end);
	KeyWordSlot							GetSlot(HighLightStyle syle);
	
	void								OnSyntaxDblQuote(StyleState state);
	void								OnSyntaxSingQuote(StyleState state);
	void								OnSyntaxSingCommentCpp(StyleState state);
	void								OnSyntaxMultiCommentCpp(StyleState state);
	void								OnSyntaxNormal(StyleState state);


	void								OnSyntaxDblQuote_Analyse(StyleState state);
	void								OnSyntaxSingQuote_Analyse(StyleState state);
	void								OnSyntaxSingComment_AnalyseCpp(StyleState state);
	void								OnSyntaxMultiComment_AnalyseCpp(StyleState state);
	void								OnSyntaxNormal_Analyse(StyleState state);







public:
										CSyntaxColor();
	virtual								~CSyntaxColor();
	
	bool								Initialize(CRichEditCtrl *pCtrl);
	bool								Terminate();
	int									ColorizeAll();
	int									ColorizePage(bool fromScratch = false);
	int									ColorizeLine(int index,bool fromScratch=false);
	int									AnalyseMultiCommentLine(int index,bool fromScratch=false);
	void								AddKeyList(char * pKeys, HighLightStyle style,KeyWordSlot Keyslot = KeyWordNormal);
	virtual  bool						OnColorKeyWord(int istart ,int iend ,LPCSTR pword, HighLightStyle style);
	void								DumpLine(int line);



protected:


	//! TOM object 
	CComQIPtr<ITextDocument>			m_pTomDoc;

	//! rich edit control used
	CRichEditCtrl						*m_pCtrl;
	//! start of the style
	int									m_iStyleCurStart;
	//! Current highlight style
	int									m_eCurHStyle;
	//! Current start of the keyword
	int									m_iCurStartKeyWord;
	//! true if a start of keyword is started
	int									m_bWordStart;
	//! state of the syntax detection
	SyntaxState							m_SyntaxState;
	//! current characters
	long								m_theChar[2];
	//! current offset character in the text document 
	int									m_iOffset;
	//! current state of the syntax detection
	StyleState							m_StyleState;
	//! count of the multi line comments 
	int									m_MultineCommentsUsed;
	//! current range
	ITextRange							*m_pRange;

private:
	//! List of Highlight Keyword
	CArray<CKeySyn,CKeySyn &>			   m_tListKeyword[KeyWordSlotMax];
	// Table of first char of the structure color ( comments, string, other)
	CArray<SyntaxState,SyntaxState & >		m_tTblStructurTags;
	//! Table for fast keyword separator
	CArray<unsigned char,unsigned char & >  m_tTblSeparator;
	//! Table of functions for comments and strings colorizing
	tChangeState							m_tTblSyntaxFn[SyntaxStateMax];
	//! Table of functions for comments and strings analysis
	tChangeState							m_tTblAnalyseFn[SyntaxStateMax];
	//! table of attributes for Keywords style
	CArray<CKeyStyle,CKeyStyle & >		     m_tStyle;
	






};

#endif // !defined(AFX_SYNTAXCOLOR_H__55937F2D_8417_4DB2_9A64_91BC449C5447__INCLUDED_)
