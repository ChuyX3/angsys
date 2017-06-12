/*! Time-stamp: <@(#)SpellingChecker.h   11/11/2004 - 10:13:18   William Hennebois>
 *********************************************************************
 *  \file   : SpellingChecker.h
 *
 *  Project :  just for fun 
 *
 *  Package : Syntax Color + Spelling
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois            Date: 11/11/2004
 *
 *  \brief  Declaration of class  CSpellingChecker
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  11/11/2004  WH : First Revision
 *
 *********************************************************************
 */

#if !defined(AFX_SPELLINGCHECKER_H__74B86030_E656_4AC0_B3BD_CC4A6A448802__INCLUDED_)
#define AFX_SPELLINGCHECKER_H__74B86030_E656_4AC0_B3BD_CC4A6A448802__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <afxtempl.h>
#include "csapi.h"



typedef WORD  ( *PROCSpellVer)(         WORD FAR *lpwVer, 
                                   WORD FAR *lpwIdEngine,
                                   WORD FAR *lpwSpellType);

typedef WORD  ( *PROCSpellInit)(        SPLID FAR *lpSid,
                                   WSC FAR  *lpWsc);

typedef WORD  ( *PROCSpellOptions)(     SPLID    splid,
                                   long     lSpellOptions);

typedef WORD  ( *PROCSpellCheck)(       SPLID    splid,
                                   SCCC     iScc,
                                   LPSIB    lpSib,
                                   LPSRB    lpSrb);

typedef WORD  ( *PROCSpellTerminate)(   SPLID    splid,
                                   BOOL     fForce);
                                 
typedef WORD  ( *PROCSpellVerifyMdr)(   LPSPATH  lpspathMdr,
								   LID		lidExpected,
                                   LID FAR  *lpLid);
                                 
typedef WORD  ( *PROCSpellOpenMdr)(     SPLID    splid, 
                                   LPSPATH  lpspathMain, 
                                   LPSPATH  lpspathExc,
                                   BOOL     fCreateUdrExc,
								   BOOL		fCache,
								   LID		lidExpected,
                                   LPMDRS   lpMdrs);

typedef WORD  ( *PROCSpellOpenUdr)(     SPLID    splid, 
                                   LPSPATH  lpspathUdr, 
                                   BOOL     fCreateUdr,
                                   WORD     udrpropType,
                                   UDR FAR  *lpUdr,
								   BOOL FAR *lpfReadonly);

typedef WORD  ( *PROCSpellAddUdr)(      SPLID    splid, 
                                   UDR      udr, 
                                   CHAR FAR *lpszAdd);

typedef WORD  ( *PROCSpellAddChangeUdr)(SPLID    splid,
                                   UDR      udr,
                                   CHAR FAR *lpszAdd,
                                   CHAR FAR *lpszChange);

typedef WORD  ( *PROCSpellDelUdr)(      SPLID    splid, 
                                   UDR      udr, 
                                   CHAR FAR *lpszDel);

typedef WORD  ( *PROCSpellClearUdr)(    SPLID    splid, 
                                   UDR      udr);

typedef WORD  ( *PROCSpellGetSizeUdr)(  SPLID    splid, 
                                   UDR      udr, 
                                   WORD FAR  *lpcWords);

typedef WORD  ( *PROCSpellGetListUdr)(  SPLID    splid, 
                                   UDR      udr, 
                                   WORD     iszStart,
                                   LPSRB    lpSrb);

typedef WORD  ( *PROCSpellCloseMdr)(    SPLID    splid, 
                                   LPMDRS   lpMdrs);

typedef WORD  ( *PROCSpellCloseUdr)(    SPLID    splid, 
                                   UDR      udr, 
                                   BOOL     fForce);



class CSpellingChecker  
{

		PROCSpellVer			m_lpfnSpellVer;
		PROCSpellInit			m_lpfnSpellInit;
		PROCSpellCheck			m_lpfnSpellCheck;
		PROCSpellTerminate		m_lpfnSpellTerminate;
		PROCSpellVerifyMdr		m_lpfnSpellVerifyMdr;
		PROCSpellOptions		m_lpfnSpellOptions;
		PROCSpellOpenMdr		m_lpfnSpellOpenMdr;
		PROCSpellOpenUdr		m_lpfnSpellOpenUdr;
		PROCSpellCloseMdr		m_lpfnSpellCloseMdr;
		PROCSpellCloseUdr		m_lpfnSpellCloseUdr;
		PROCSpellAddUdr			m_lpfnSpellAddUdr;
		PROCSpellAddChangeUdr	m_lpfnSpellAddChangeUdr;
		PROCSpellDelUdr			m_lpfnSpellDelUdr;
		PROCSpellClearUdr		m_lpfnSpellClearUdr;
		PROCSpellGetSizeUdr		m_lpfnSpellGetSizeUdr;
		PROCSpellGetListUdr		m_lpfnSpellGetListUdr;

		bool InitEngine();
		bool CheckUserDicFile(LPCSTR pWord);
		bool AddUsrDicFile(LPCSTR pWord);

public:
	CSpellingChecker();
	virtual ~CSpellingChecker();
	bool Initialize(int lang = lidAmerican);
	bool Terminate();
	bool CheckWord(LPCSTR pWord);
	bool SuggestWords(LPCSTR pWord,int max, CStringArray & tList );
	bool IgnoreAlways(LPCSTR pWord);
	bool AddToUserDic(LPCSTR pWord);




protected:
	
    SPLID		m_Handle;
    WSC			m_SpecChars;
	HMODULE		m_hinstSpell;
    MDRS		m_Mdrs;
	UDR			m_udr;
	BOOL		m_fUdrRO;
    int			m_Language;
	bool		m_bEnable;
	LID			m_lid;









};

#endif // !defined(AFX_SPELLINGCHECKER_H__74B86030_E656_4AC0_B3BD_CC4A6A448802__INCLUDED_)
