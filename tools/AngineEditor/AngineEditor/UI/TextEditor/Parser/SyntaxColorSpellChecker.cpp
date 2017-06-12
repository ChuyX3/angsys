/*! Time-stamp: <@(#)SyntaxColorSpellChecker.cpp   09/11/2004 - 16:48:50   William Hennebois>
 *********************************************************************
 *  \file   : SyntaxColorSpellChecker.cpp
 *
 *  Project :  just for fun 
 *
 *  Package : Syntax Color + Spelling
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois            Date: 09/11/2004
 *
 *  \brief  Implementation of methods for class CSyntaxColorSpellChecker
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  09/11/2004  WH : First Revision
 *
 *********************************************************************
 */

#include "stdafx.h"
#include "SyntaxColorSpellChecker.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSyntaxColorSpellChecker::CSyntaxColorSpellChecker()
{

}


// -------------------------------------------------------------------------------------
/*! Destuctor
 *
 *
 * \return   
 */
CSyntaxColorSpellChecker::~CSyntaxColorSpellChecker()
{

}



// -------------------------------------------------------------------------------------
/*! Constructor
 *
 * \param *pCtrl   
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::Initialize(CRichEditCtrl *pCtrl)
{
	if(!m_cSpeller.Initialize(lidAmerican)) return false;
	return CSyntaxColor::Initialize(pCtrl);

}

// -------------------------------------------------------------------------------------
/*! Close 
 *
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::Terminate()
{
	if(!m_cSpeller.Terminate()) return false;

	return CSyntaxColor::Terminate();
}



// -------------------------------------------------------------------------------------
/*! Called when a word is parsed
 *
 * \param istart   character start
 * \param iend	   character end
 * \param pword    word to spell
 * \param style    high light style in use
 *
 * \return   
 */
bool	CSyntaxColorSpellChecker::OnColorKeyWord(int istart ,int iend ,LPCSTR pword, HighLightStyle style)
{

	// by default keyword detected has a good spell
	if(CSyntaxColor::OnColorKeyWord(istart,iend,pword,style)) return true;
	
	
	long newunderline = 0;
	// check only for comments and strings
	if(style == scomments || style == mcomments || style == sstring || style == dstring )
	{
		newunderline = m_cSpeller.CheckWord(pword) ? 0: SPELL_UNDERLINE;
	}

	// close current  style to colorize previous run
	CloseStyle(iend);


	m_pRange->SetRange(istart,iend);
	ITextFont *pFont;
	m_pRange->GetFont(&pFont);
	pFont->SetUnderline(newunderline);
	pFont->Release();

	return false;

	
	

	
}



// -------------------------------------------------------------------------------------
/*! gets the suggestion list
 *
 * \param str   the list of words 
 * \param range  the selection 
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::SuggestWords(CStringArray & str,CHARRANGE  & range,int max)
{

	CComBSTR word;
	CComPtr<ITextRange> pRange;
	if(m_pTomDoc->Range(range.cpMin,range.cpMax,&pRange) !=  S_OK ) return false;
	pRange->GetText(&word);
	m_cSpeller.SuggestWords(CString(word),max,str);

	return true;
	
}



// -------------------------------------------------------------------------------------
/*! Gets the selected word
 *
 * \param cr   the selection
 *
 * \return CString   
 */
CString CSyntaxColorSpellChecker::GetWord(CHARRANGE  cr)
{

	CComPtr<ITextRange> pRange;
	if(m_pTomDoc->Range(cr.cpMin,cr.cpMax,&pRange) !=  S_OK ) return CString("");
	CComBSTR bstr;
	pRange->GetText(&bstr);
	CString word(bstr);
//word.TrimLeft(' ');
//word.TrimRight(' ');
	return word;

}


// -------------------------------------------------------------------------------------
/*! Add a word to the Dictionary
 *
 * \param pText   the word
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::AddToDictionnary(LPCSTR pText)
{

	if(!m_cSpeller.AddToUserDic(pText)) return false;
	if(!m_cSpeller.IgnoreAlways(pText)) return  false;
	ColorizeAll();
	return true;

}



// -------------------------------------------------------------------------------------
/*! Add a selection to the AddToDictionnary
 *
 * \param cr   the selection
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::AddToDictionnary(CHARRANGE & cr)
{

	return AddToDictionnary(GetWord(cr));

}




// -------------------------------------------------------------------------------------
/*! Ignore always the word
 *
 * \param pText   the word
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::IgnoreAlways(LPCSTR pText)
{
	if(!m_cSpeller.IgnoreAlways(pText)) return false;
	ColorizeAll();
	return true;


	
}


// -------------------------------------------------------------------------------------
/*! Igore always the selection  
 *
 * \param cr   the selection 
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::IgnoreAlways(CHARRANGE & cr)
{
	return IgnoreAlways(GetWord(cr));
	
}

				




// -------------------------------------------------------------------------------------
/*! Gets the selection from a point
 *
 * \param xScreen   pos x screen coordinates
 * \param yScreen   pos y screen coordinates
 * \param cr		the selection 
 *
 * \return int   
 */
int CSyntaxColorSpellChecker::GetSpellSelFromPos(int xScreen,int yScreen,CHARRANGE & cr)
{
	
	CPoint pos(xScreen,yScreen);
	CComPtr<ITextRange> pRange;
	if(m_pTomDoc->RangeFromPoint(pos.x,pos.y,&pRange) !=  S_OK ) return false;
	long   positions;
	long   positione;

	pRange->Expand(tomWord,NULL);
	// remove spaces 
	CComVariant var(C1_SPACE);
	pRange->MoveEndWhile(&var,tomBackward,NULL);

	pRange->GetStart(&positions);
	pRange->GetEnd(&positione);
	cr.cpMin = positions;
	cr.cpMax = positione;

	// check spelling
	CComPtr<ITextFont> pFont;
	pRange->GetFont(&pFont);

	long	 underline = 0;
	if(pFont->GetUnderline(&underline) != S_OK) return false;
	if(underline != SPELL_UNDERLINE) return false;
	return true;
}





// -------------------------------------------------------------------------------------
/*! replace the selection by a new word
 *
 * \param cr   the selection 
 * \param pWord   the Word 
 *
 * \return bool   
 */
bool CSyntaxColorSpellChecker::ReplaceWord(CHARRANGE & cr,LPCSTR pWord)
{

	CComPtr< ITextRange> pSel;
	if(m_pTomDoc->Range(cr.cpMin,cr.cpMax,&pSel) !=  S_OK ) return false;
	CComBSTR bstr(pWord);
	pSel->SetText(bstr);
	ColorizeAll(); // remove under lines
	return true;
}

