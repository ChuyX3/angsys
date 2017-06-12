/*! Time-stamp: <@(#)SyntaxColorSpellChecker.h   10/11/2004 - 09:44:57   William Hennebois>
 *********************************************************************
 *  \file   : SyntaxColorSpellChecker.h
 *
 *  Project :  just for fun 
 *
 *  Package : Syntax Color + Spelling
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois            Date: 10/11/2004
 *
 *  \brief  Declaration of class CSyntaxColorSpellChecker
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  10/11/2004  WH : First Revision
 *
 *********************************************************************
 */

#if !defined(AFX_SYNTAXCOLORSPELLCHECKER_H__691371D6_7C92_4620_8883_2EC8DDF2B4B2__INCLUDED_)
#define AFX_SYNTAXCOLORSPELLCHECKER_H__691371D6_7C92_4620_8883_2EC8DDF2B4B2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "SyntaxColor.h"
#include "SpellingChecker.h"


#define	UNDERLINECOLOR		5			// this color is red
#define TOMWAVE				8			// this char is wave ( not documented yet)
#define SPELL_UNDERLINE		(TOMWAVE + (UNDERLINECOLOR<<4))

class CSyntaxColorSpellChecker  : public CSyntaxColor
{
	//! instance of the speller
	CSpellingChecker m_cSpeller;		



public:
	CSyntaxColorSpellChecker();
	virtual ~CSyntaxColorSpellChecker();

	
	virtual bool	Initialize(CRichEditCtrl *pCtrl);
	virtual bool	Terminate();
	virtual bool	OnColorKeyWord(int istart ,int iend ,LPCSTR pword, HighLightStyle style);

	bool			SuggestWords(  CStringArray & str,CHARRANGE  & range,int max=10);
	bool			AddToDictionnary(LPCSTR pText);
	bool			AddToDictionnary(CHARRANGE & cr);

	bool			IgnoreAlways(LPCSTR pText);
	bool			IgnoreAlways(CHARRANGE & cr);

	int				GetSpellSelFromPos(int x,int y,CHARRANGE & cr);
	CString			GetWord(CHARRANGE  cr);
	bool			ReplaceWord(CHARRANGE & cr,LPCSTR pWird);




};

#endif // !defined(AFX_SYNTAXCOLORSPELLCHECKER_H__691371D6_7C92_4620_8883_2EC8DDF2B4B2__INCLUDED_)
