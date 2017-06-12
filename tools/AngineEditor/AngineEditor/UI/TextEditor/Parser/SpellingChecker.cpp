/*! Time-stamp: <@(#)SpellingChecker.cpp   09/11/2004 - 16:42:46   William Hennebois>
 *********************************************************************
 *  \file   : SpellingChecker.cpp
 *
 *  Project :  just for fun 
 *
 *  Package : Syntax Color + Spelling
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois            Date: 09/11/2004
 *
 *  \brief  Implementation of methods for class 
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  09/11/2004  WH : First Revision
 *
 *********************************************************************
 */

#include "stdafx.h"
#include "SpellingChecker.h"
#include "Registry.h"
#include "stdio.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


static CString		m_MdrNameLex;
static CString		m_UdrNameDic;
static CString		m_UsrExeclDic;


// -------------------------------------------------------------------------------------
/*! constructor
 *
 *
 * \return   
 */
CSpellingChecker::CSpellingChecker()
{
	m_hinstSpell  = 0;
	m_UdrNameDic  = "UserDic.dic";
	m_UsrExeclDic = "User Exclusion.Dic";
	m_bEnable = false;

}


// -------------------------------------------------------------------------------------
/*! destructor
 *
 *
 * \return   
 */
CSpellingChecker::~CSpellingChecker()
{
	Terminate();
}



// -------------------------------------------------------------------------------------
/*! Initialize Speller, load library etc..
 *
 * \param lang   LID language
 *
 * \return bool   
 */
bool CSpellingChecker::Initialize(int lang)
{


	m_Language = lang; 
	
	CString regKey;
	regKey.Format("HKEY_LOCAL_MACHINE/SOFTWARE/Microsoft/Shared Tools/Proofing Tools/Spelling/%d/Normal",m_Language);

	CRegistry reg(regKey);

	CString engine =  reg.ReadKey("Engine","");
	m_MdrNameLex   =  reg.ReadKey("Dictionary","");


	
	m_hinstSpell = LoadLibrary(engine);
	if(!m_hinstSpell) return false;



	if ((m_lpfnSpellVer = (PROCSpellVer)GetProcAddress (m_hinstSpell, "SpellVer")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellInit = (PROCSpellInit)GetProcAddress (m_hinstSpell, "SpellInit")) == NULL)
		goto LoadError;
	
	if ((m_lpfnSpellOptions = (PROCSpellOptions)GetProcAddress (m_hinstSpell, "SpellOptions")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellCheck = (PROCSpellCheck)GetProcAddress (m_hinstSpell, "SpellCheck")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellTerminate = (PROCSpellTerminate)GetProcAddress (m_hinstSpell, "SpellTerminate")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellVerifyMdr = (PROCSpellVerifyMdr)GetProcAddress (m_hinstSpell, "SpellVerifyMdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellOpenMdr = (PROCSpellOpenMdr)GetProcAddress (m_hinstSpell, "SpellOpenMdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellOpenUdr = (PROCSpellOpenUdr)GetProcAddress (m_hinstSpell, "SpellOpenUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellAddUdr = (PROCSpellAddUdr)GetProcAddress (m_hinstSpell, "SpellAddUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellAddChangeUdr = (PROCSpellAddChangeUdr)GetProcAddress (m_hinstSpell, "SpellAddChangeUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellDelUdr = (PROCSpellDelUdr)GetProcAddress (m_hinstSpell, "SpellDelUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellClearUdr = (PROCSpellClearUdr)GetProcAddress (m_hinstSpell, "SpellClearUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellGetSizeUdr = (PROCSpellGetSizeUdr)GetProcAddress (m_hinstSpell, "SpellGetSizeUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellGetListUdr = (PROCSpellGetListUdr)GetProcAddress (m_hinstSpell, "SpellGetListUdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellCloseMdr = (PROCSpellCloseMdr)GetProcAddress (m_hinstSpell, "SpellCloseMdr")) == NULL)
		goto LoadError;
	if ((m_lpfnSpellCloseUdr = (PROCSpellCloseUdr)GetProcAddress (m_hinstSpell, "SpellCloseUdr")) == NULL)
		goto LoadError;
	if(!InitEngine()) return false;		

	m_bEnable = true;
	return true ;

LoadError:
	FreeLibrary (m_hinstSpell);
	return false;
}


// -------------------------------------------------------------------------------------
/*! Terminate
 *
 *
 * \return bool   
 */
bool CSpellingChecker::Terminate()
{
	if(m_Handle)
	{
		m_lpfnSpellCloseUdr(m_Handle, m_udr,FALSE);
		m_lpfnSpellCloseMdr(m_Handle, &m_Mdrs);
		m_lpfnSpellTerminate(m_Handle, FALSE);
	}
	m_Handle = 0;
	if(m_hinstSpell) FreeLibrary (m_hinstSpell);
	m_hinstSpell = 0;
	m_bEnable = false;
	return true;
}



// -------------------------------------------------------------------------------------
/*! 
 *
 *
 * \return bool   
 */
bool CSpellingChecker::InitEngine()
{
	int err;
    m_SpecChars.bIgnore = 0;
    m_SpecChars.bHyphenHard = 45;
    m_SpecChars.bHyphenSoft = 31;
    m_SpecChars.bHyphenNonBreaking = 30;
    m_SpecChars.bEmDash = 151;
    m_SpecChars.bEnDash = 150;
    m_SpecChars.bEllipsis = 133;
    m_SpecChars.rgLineBreak[0] = 11;
    m_SpecChars.rgLineBreak[1] = 10;
    m_SpecChars.rgParaBreak[0] = 13;
    m_SpecChars.rgParaBreak[1] = 10;


	WORD Ver;
    WORD Engineid;
    WORD SpellType;

	// We should verify this
	if(m_lpfnSpellVer(&Ver,&Engineid,&SpellType) != 0) return false;

    if(m_lpfnSpellInit(&m_Handle, &m_SpecChars) != 0) return false;
	if (m_lpfnSpellVerifyMdr((char *)LPCSTR(m_MdrNameLex), m_Language, &m_lid))	return false;
    if((err = m_lpfnSpellOpenMdr(m_Handle, (char *)LPCSTR(m_MdrNameLex),"MyDic.dic", TRUE, TRUE, m_Language, &m_Mdrs)) != 0) return false;
	if(m_lpfnSpellOptions(m_Handle, soSuggestFromUserDict | soUseAllOpenUdr | soSuggestFromUserDict | soIgnoreAllCaps | soIgnoreMixedDigits) != 0)		return false;
	
    if((err = m_lpfnSpellOpenUdr(m_Handle, (char *)LPCSTR(m_UdrNameDic), TRUE,IgnoreAlwaysProp,&m_udr,NULL)) != 0)	return false;
  
   return true;
}



// -------------------------------------------------------------------------------------
/*! Check the word
 *
 * \param pWord   the word to check
 *
 * \return bool   
 */
bool CSpellingChecker::CheckWord(LPCSTR pWord)
{
	if(!m_bEnable) return true;

   	BYTE rgbRating[32];
	char rgsz[256];
	SIB InputBuffer = { strlen(pWord) , 1, 1, fssStartsSentence, (LPSTR)pWord, &m_Mdrs.mdr, &m_udr };
	SRB ResultBuffer = { 0, 0, 0, 0, 0, sizeof(rgsz) , (CHAR FAR *)&rgsz, (BYTE FAR *)&rgbRating, sizeof(rgbRating)};
    if(m_lpfnSpellCheck(m_Handle, sccVerifyWord, &InputBuffer, &ResultBuffer) != 0) return false;
	int err = ResultBuffer.scrs;
	if(err != scrsNoErrors) return false;
	return true;
}


// -------------------------------------------------------------------------------------
/*! Ask for list of words 
 *
 * \param pWord  the word 
 * \param max    max entries in the list
 * \param tList   the list 
 *
 * \return bool   
 */
bool CSpellingChecker::SuggestWords(LPCSTR pWord,int max, CStringArray & tList )
{
	if(!m_bEnable) return false;

   	BYTE rgbRating[32];
	char rgsz[256];

	SIB InputBuffer = { strlen(pWord) , 1, 1, fssStartsSentence, (LPSTR)pWord, &m_Mdrs.mdr, &m_udr };
	SRB ResultBuffer = { 0, 0, 0, 0, 0, sizeof(rgsz) , (CHAR FAR *)&rgsz, (BYTE FAR *)&rgbRating, sizeof(rgbRating)};


	tList.SetSize(0);

	int wCommand = sccSuggest;
	do
  	{
		if (m_lpfnSpellCheck(m_Handle, wCommand,  &InputBuffer, &ResultBuffer) != 0)
		{
			return false;				
		}
		for (int i = 0, cWordsInserted = 0; cWordsInserted < ResultBuffer.csz ; cWordsInserted++)
		{
			if(max == 0) goto gotostop;
			max--;

			tList.Add((char *) &ResultBuffer.lrgsz[i]);
			while(ResultBuffer.lrgsz[i++]);
		}
		wCommand = sccSuggestMore;
	}
	while (ResultBuffer.scrs != scrsNoMoreSuggestions);
gotostop:;

	return true;
}




// -------------------------------------------------------------------------------------
/*! Ignore this word
 *
 * \param pWord   the word to ignore
 *
 * \return bool   
 */
bool CSpellingChecker::IgnoreAlways(LPCSTR pWord)
{
	if(!m_bEnable) return false;

  	if (m_lpfnSpellAddUdr(m_Handle, udrIgnoreAlways, (char *)pWord) != 0)
	{
		return false;
	}
	return true;

}



// -------------------------------------------------------------------------------------
/*! Add to dic
 *
 * \param pWord   
 *
 * \return bool   
 */
bool CSpellingChecker::AddToUserDic(LPCSTR pWord)
{
	if(!m_bEnable) return false;


	if(!CheckUserDicFile(pWord))
	{
		AddUsrDicFile(pWord);
	}
	return IgnoreAlways(pWord);
}


// -------------------------------------------------------------------------------------
/*! Internal
 *
 * \param pWord   the word to add
 *
 * \return bool   
 */
bool CSpellingChecker::CheckUserDicFile(LPCSTR pWord)
{

	FILE *pf = fopen(LPCSTR(m_UdrNameDic),"r");
	if(pf == 0) return false;
	char sword[100];;
	while(fscanf(pf,"%s",sword) != EOF)
	{
		if(strcmp(pWord,sword) == 0)
		{
			fclose(pf);
			return true;
		}
	}
	
	fclose(pf);
	return false;
}


// -------------------------------------------------------------------------------------
/*! 
 *
 * \param pWord   
 *
 * \return bool   
 */
bool CSpellingChecker::AddUsrDicFile(LPCSTR pWord)
{

	FILE *pf = fopen(LPCSTR(m_UdrNameDic),"a");
	if(pf == 0) return false;
	fwrite(pWord,1,strlen(pWord),pf);
	fwrite("\r\n",1,2,pf);
	fclose(pf);


	return true;

}