/*! Time-stamp: <@(#)SyntaxColor.cpp   06/11/2004 - 10:16:33   William Hennebois >
 *********************************************************************
 *  \file   : SyntaxColor.cpp
 *
 *  Project : just for fun 
 *
 *  Package : Syntax Color 
 *
 *  Company : just for fun
 *
 *  Author  : William Hennebois             Date: 06/11/2004
 *
 *  \brief  Implementation of methods for class 
 *
 *********************************************************************
 * Version History:
 *
 * V 0.10  06/11/2004  WH : First Revision
 *
 *********************************************************************
 */



#include "stdafx.h"
#include "SyntaxColor.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define _DEBUG_STYLE


static char * sDoxygenKeywords =
								"\\a,\\addindex,\\addtogroup,\\anchor,\\arg,\\attention,\\author,\\b,\\brief,\\bug,\\c,\\callgraph,\\category,\\class,\\code,\\copydoc,"
								"\\date,\\def,\\defgroup,\\deprecated,\\dir,\\dontinclude,\\dot,\\dotfile,\\e,\\else,\\elseif,\\em,\\endcode,\\enddot,\\endhtmlonly,"
								"\\endif,\\endlatexonly,\\endlink,\\endmanonly,\\endverbatim,\\endxmlonly,\\enum,\\example,\\exception,\\f$,\\f[,\\f],\\file,\\fn,\\hideinitializer,"
								"\\htmlinclude,\\htmlonly,\\if,\\ifnot,\\image,\\include,\\includelineno,\\ingroup,\\internal,\\invariant,\\interface,\\latexonly,\\li,\\line,\\link,"
								"\\mainpage,\\manonly\\n,\\name,\\namespace,\\nosubgrouping,\\note,\\overload,\\p,\\package,\\page,\\par,\\paragraph,\\param,\\post,\\pre,\\private,"
								"\\privatesection,\\property,\\protected,\\protectedsection,\\protocol,\\public,\\publicsection,\\ref,\\relates,\\relatesalso,\\remarks,\\return,"
								"\\retval,\\sa,\\section,\\showinitializer,\\since,\\skip,\\skipline,\\struct,\\subsection,\\subsubsection,\\test,\\throw,\\todo,\\typedef,\\union,"
								"\\until,\\var,\\verbatim,\\verbinclude,\\version,\\warning,\\weakgroup,\\xmlonly,\\xrefitem";

static char * sKeywords = "if,else,main,struct,__assume,enum,"
								"__multiple_inheritance,switch,auto,__except,__single_inheritance,"
								"template,__based,explicit,__virtual_inheritance,this,bool,extern,"
								"mutable,thread,break,false,naked,throw,case,__fastcall,namespace,"
								"true,catch,__finally,new,try,__cdecl,float,noreturn,__try,char,for,"
								"operator,typedef,class,friend,private,typeid,const,goto,protected,"
								"typename,const_cast,__asm,public,union,continue,inline,register,"
								"unsigned,__declspec,__inline,reinterpret_cast,using,declaration,"
								"directive,default,int,return,uuid,delete,__int8,short,"
								"__uuidof,dllexport,__int16,signed,virtual,dllimport,__int32,sizeof,"
								"void,do,__int64,static,volatile,double,__leave,static_cast,wmain,"
								"dynamic_cast,long,__stdcall,while";
static char * sDirectives = 
								"#define,#elif,#else,#endif,#error,#ifdef,"
								"#ifndef,#import,#include,#line,#pragma,#undef";
static char* sPragmas = 
								"alloc_text,comment,init_seg1,optimize,auto_inline,"
								"component,inline_depth,pack,bss_seg,data_seg,"
								"inline_recursion,pointers_to_members1,check_stack,"
								"function,intrinsic,setlocale,code_seg,hdrstop,message,"
								"vtordisp1,const_seg,include_alias,once,warning"; 


static char* sMySpecialWords = 	"SyntaxColor";



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


// -------------------------------------------------------------------------------------
/*! Constructor
 *
 *
 * \return   void 
 */
CSyntaxColor::CSyntaxColor()
{
	CreateDefaultKeyList();
	CreateDefaultStyle();
	CreateCommentsAndStringsTagsForCpp(),


	// Words separator
	m_tTblSeparator.SetSize(256);
	memset(&m_tTblSeparator[0],false,256);
	memset(&m_tTblSeparator['a'],true,('z' -'a')+1 );
	memset(&m_tTblSeparator ['A'],true,('Z' -'A')+1 );
	m_tTblSeparator ['_'] = true;
	m_tTblSeparator ['#'] = true;
	m_tTblSeparator ['\\'] = true; // for Doxygen

}


// -------------------------------------------------------------------------------------
/*! Destructor
 *
 *
 * \return   
 */
CSyntaxColor::~CSyntaxColor()
{

}



// -------------------------------------------------------------------------------------
/*! Create Table for structure detection comments and strings CPP/C
 *
 *
 * \return void   
 */
void CSyntaxColor::CreateCommentsAndStringsTagsForCpp()
{

	// Table of functions for Comments and quotes

	m_tTblSyntaxFn[SyntaxStateDblQuote]     = OnSyntaxDblQuote;
	m_tTblSyntaxFn[SyntaxStateSingQuote]    = OnSyntaxSingQuote;
	m_tTblSyntaxFn[SyntaxStateMuliComment]  = OnSyntaxMultiCommentCpp;
	m_tTblSyntaxFn[SyntaxStateSingComment]  = OnSyntaxSingCommentCpp;
	m_tTblSyntaxFn[SyntaxStateNormal]		= OnSyntaxNormal;

	// Table of functions for Comments and quotes for multi comments analyze only.
	// The analyze is useful to detect a change in the multi line comments, the multi-line comments could affect all the document 
	// same as m_tTblSyntaxFn but we remove the change of style
	
	m_tTblAnalyseFn[SyntaxStateDblQuote]     = OnSyntaxDblQuote_Analyse;
	m_tTblAnalyseFn[SyntaxStateSingQuote]    = OnSyntaxSingQuote_Analyse;
	m_tTblAnalyseFn[SyntaxStateMuliComment]  = OnSyntaxMultiComment_AnalyseCpp;
	m_tTblAnalyseFn[SyntaxStateSingComment]  = OnSyntaxSingComment_AnalyseCpp;
	m_tTblAnalyseFn[SyntaxStateNormal]		 = OnSyntaxNormal_Analyse;

	// Syntax Table of start for optimization

	m_tTblStructurTags.SetSize(256);
	memset(&m_tTblStructurTags[0],SyntaxStateNormal,256);

	m_tTblStructurTags[ '"']   = SyntaxStateDblQuote;   // 0
	m_tTblStructurTags[ '\'']  = SyntaxStateSingQuote;
	m_tTblStructurTags[ '*']   = SyntaxStateMuliComment; 
	m_tTblStructurTags[ '/']   = SyntaxStateSingComment; 


}









// -------------------------------------------------------------------------------------
/*! Initialize the list of keyword in the two slots 
 *
 *
 * \return void   
 */
void CSyntaxColor::CreateDefaultKeyList()
{
	AddKeyList(sKeywords,syntax,KeyWordNormal);
	AddKeyList(sDirectives,directive,KeyWordNormal);
	AddKeyList(sPragmas,pragma,KeyWordNormal);
	AddKeyList(sDoxygenKeywords,keywordComments,KeyWordComments);
	AddKeyList(sMySpecialWords,mySpecialWords,KeyWordComments);

}




// -------------------------------------------------------------------------------------
/*! Create the style for each highlight
 *
 *
 * \return void   
 */
void CSyntaxColor::CreateDefaultStyle()
{
	m_tStyle.SetSize(styleMax);
	m_tStyle[normal]    = CKeyStyle(RGB(0,0,0));		// black for normal
	m_tStyle[syntax]    = CKeyStyle(RGB(0,0,255));		// blue for syntax
	m_tStyle[directive] = CKeyStyle(RGB(192,192,192));	// gray for directives 
	m_tStyle[pragma]	= CKeyStyle(RGB(0,0,255));		// blue for pragma
	m_tStyle[mcomments]	= CKeyStyle(RGB(0,200,0));	// green  multi comment green
	m_tStyle[scomments]	= CKeyStyle(RGB(0,200,0));	// green  single comment green
	m_tStyle[sstring]	= CKeyStyle(RGB(0,0,255));		// blue string
	m_tStyle[dstring]	= CKeyStyle(RGB(0,0,255));		// blue string
	m_tStyle[keywordComments]= CKeyStyle(RGB(255,0,0),bold);	// red keyword comments and bold 
	m_tStyle[mySpecialWords]= CKeyStyle(RGB(238,54,199),bold,"Palace Script MT",40);	// Pink



}




// -------------------------------------------------------------------------------------
/*! add a Key List
 *
 * \param pKeys    list of keys 
 * \param style    based style 
 * \param Keyslot  slot ( normal, comments)
 *
 * \return void   
 */
void CSyntaxColor::AddKeyList(char * pKeys, HighLightStyle style,KeyWordSlot Keyslot)
{


	char *key = pKeys;
	while(key != NULL && *key)
	{
		char *strnext = strchr(key,',');
		if(strnext)
		{
			CString skey(key,strnext -key);
			ASSERT(strlen(skey)>=2); // look FindKeyword
			m_tListKeyword[Keyslot] .Add(CKeySyn(skey,style));
			key = strnext+1;
		}
		else
		{
			CString skey(key);
			if(skey != "")
			{
		
				ASSERT(strlen(skey)>=2); // look FindKeyword
				m_tListKeyword[Keyslot] .Add(CKeySyn(skey,style));
			}
		

			key = NULL;

		}
	}
}



// -------------------------------------------------------------------------------------
/*! Init the document
 *
 * \param *pCtrl   Ptr on a rich edit control
 *
 * \return bool   
 */
bool CSyntaxColor::Initialize(CRichEditCtrl *pCtrl)
{
	m_pCtrl = pCtrl;

	IRichEditOle*pOle =  m_pCtrl->GetIRichEditOle();
	if(pOle)
	{
		m_pTomDoc = pOle;
		pOle->Release();
	}

	return true;
}

// -------------------------------------------------------------------------------------
/*! Terminate 
 *
 *
 * \return bool   
 */
bool CSyntaxColor::Terminate()
{
	return true;
}



// -------------------------------------------------------------------------------------
/*! Sets a style to a range
 *
 * \param start   character start 
 * \param end     character end
 * \param style   highlight style
 *
 * \return BOOL   
 */
BOOL CSyntaxColor::SetStyle(int start,int end,HighLightStyle style)
{

 	m_pRange->SetRange(start,end);

	CComPtr<ITextFont> pFont;
	m_pRange->GetFont(&pFont);
	TomStyle newStyle;
//	pFont->GetStyle(&newStyle.byLong);
	newStyle.byLong = 0;
	newStyle.byStruct.ColorStyle = style;
	newStyle.byStruct.SyntaxState = m_SyntaxState;

	m_tStyle[style].Apply(pFont);
	pFont->SetUnderline(0); // Raz Underline 
	pFont->SetStyle(newStyle.byLong);
	return 1;
}


// -------------------------------------------------------------------------------------
/*! Sets a style from a range
 *
 * \param start   
 * \param end   
 *
 * \return KeyStyle   
 */
TomStyle CSyntaxColor::GetStyle(int start,int end)
{
	m_pRange->SetRange(start,end);
	CComPtr<ITextFont> pFont;
	int a = m_pRange->GetFont(&pFont);
	TomStyle	curstyle;
	pFont->GetStyle(&curstyle.byLong);
	return curstyle;
}


// -------------------------------------------------------------------------------------
/*! Close the current highlight style
 *
 * \param end   position to ends
 *
 * \return void   
 */
void CSyntaxColor::CloseStyle(int end)
{
	if(end -m_iStyleCurStart >0)
	{

		SetStyle(m_iStyleCurStart,end,(HighLightStyle)m_eCurHStyle);
		m_iStyleCurStart = end;
	}

}



// -------------------------------------------------------------------------------------
/*! Open a new highlight style
 *
 * \param start		position to start
 * \param newstyle		the style to use 
 *
 * \return void   
 */
void CSyntaxColor::OpenStyle(int start, HighLightStyle newstyle)
{
	if(m_eCurHStyle != newstyle) 
	{
		if(start - m_iStyleCurStart) CloseStyle(start);
		m_eCurHStyle = (unsigned)newstyle;
	}

}



// -------------------------------------------------------------------------------------
/*! Check for a keyword 
 *
 * \param CArray<CKeySyn   
 * \param m_tListKeyword   
 * \param word   
 *
 * \return CKeySyn *   
 */
CKeySyn * CSyntaxColor::FindKeyWord(CArray<CKeySyn,CKeySyn &> & tListKeyword,LPCSTR word)
{
	int nb = tListKeyword.GetSize();
	if(nb == 0 ) return 0;

	CKeySyn *pKey = &tListKeyword[0];
	for(int a = 0; a < nb ; a++,pKey++)
	{
		// a key word at least 2 chars 
		if(*((short *)(LPCSTR(pKey->m_sKey)))  == *((short *)word))
		{
			if(pKey->m_sKey == word) return pKey;
		}
		
	}

	return 0;

}


// -------------------------------------------------------------------------------------
/*! Manage keywords
 *
 * \param thechar   the char 
 * \param curpos   
 * \param offset   
 *
 * \return void   
 */
void CSyntaxColor::AddCharForKeyWord(WCHAR thechar)
{

	// check for separators
	if(m_tTblSeparator[(unsigned char)thechar])
	{
		// it's not a separator
		// check a word is already opened 
		if(!m_bWordStart)
		{
			// if not record a new word
			m_bWordStart = true;
			m_iCurStartKeyWord = m_iOffset;
		}
		// if already opened, let's leave curpos continue to record characters 

	}
	else
	{
		// it's a separator
		// let's check if a word has been opened 
		
		if(m_iOffset - m_iCurStartKeyWord && m_bWordStart)
		{
			// yes, get it and check it
			CComBSTR  str;
			m_pRange->SetRange(m_iCurStartKeyWord,m_iOffset);
			m_pRange->GetText(&str);
			CString word(str);
			OnColorKeyWord(m_iCurStartKeyWord,m_iOffset,word,(HighLightStyle)m_eCurHStyle);
			

		}
		// if not, record this pos has a start of word
		m_bWordStart = false;

	}

}



// -------------------------------------------------------------------------------------
/*! returns the slot ie comments or normal, it is not the same set of keywords
 *
 * \param style   
 *
 * \return   
 */
KeyWordSlot		CSyntaxColor::GetSlot(HighLightStyle style)
{

	switch(style)
		{
			default:
				{
					return KeyWordNormal;
					break;
				}
			case scomments:
			case mcomments:
				{
					return KeyWordComments;
				}

		}


}



// -------------------------------------------------------------------------------------
/*! Call back to colonize KeyWords
 *
 * \param istart   
 * \param iend   
 * \param pword   
 * \param style   
 *
 * \return   true if the keyword has been colored
 */
bool	CSyntaxColor::OnColorKeyWord(int istart ,int iend ,LPCSTR pword, HighLightStyle style)
{
		HighLightStyle	eCurStyle = style;
		CKeySyn		 *pKeySyntax = 0;
		KeyWordSlot slot = GetSlot(style);


		// key word not in a string 
		
		if(slot == KeyWordComments ||  m_eCurHStyle == normal )
		{
			pKeySyntax = FindKeyWord(m_tListKeyword[slot],pword);
			if(pKeySyntax)
			{
		
				CloseStyle(istart);
				OpenStyle(istart,pKeySyntax->m_dwstyle);
				OpenStyle(iend,eCurStyle);
				return true;
			}
		}

	return false;
}


// -------------------------------------------------------------------------------------
/*! Callback for double quote ie "bla bla bla"
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxDblQuote(StyleState state)
{
	if(state == checkend)
	{

		if(m_theChar[0] == '"' )
		{
			CloseStyle(m_iOffset);
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{

		OpenStyle(m_iOffset,dstring);
		m_SyntaxState = SyntaxStateDblQuote;
		m_StyleState  = checkend;
		m_theChar[0] = 0;


	}



}


// -------------------------------------------------------------------------------------
/*! Callback for single quote ie 'bla'
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxSingQuote(StyleState state)

{
	if(state == checkend)
	{

		if(m_theChar[0] == '\'' )
		{
			CloseStyle(m_iOffset);
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{

		OpenStyle(m_iOffset,sstring);
		m_SyntaxState = SyntaxStateSingQuote;
		m_theChar[0] = 0; // Raz because m_theChar[1] will fail the end
		m_StyleState  = checkend;



	}

}


// -------------------------------------------------------------------------------------
/*! Callback for multi during normal state SyntaxStateMuliComment ei  C++ /* bla bla 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxMultiCommentCpp(StyleState state)
{

	if(state == checkend)
	{

		if(m_theChar[1] == '*' && m_theChar[0] == '/' )
		{
			CloseStyle(m_iOffset);
			m_MultineCommentsUsed--;
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{
		if((m_theChar[1] == '/' && m_theChar[0] == '*') || state == forcestart)
		{

			// the syle start a cur-1
			OpenStyle(m_iOffset-1,mcomments);
			m_SyntaxState = SyntaxStateMuliComment;
			m_StyleState  = checkend;

			if(!m_MultineCommentsUsed) m_MultineCommentsUsed++;

		}


	}


}




// -------------------------------------------------------------------------------------
/*! callback for multi during normal state ei  C++ // bla bla
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxSingCommentCpp(StyleState state)
{
	if(state == checkend)
	{

		if( m_theChar[0] == '\r' )
		{
			CloseStyle(m_iOffset);
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{
		if((m_theChar[0] == '/' && m_theChar[1] == '/')|| state == forcestart )
		{
			// the style start at cur-1
			OpenStyle(m_iOffset-1,scomments);
			m_SyntaxState = SyntaxStateSingComment;
			m_StyleState  = checkend;

		}


	}

}


// -------------------------------------------------------------------------------------
/*! callback for multi during normal state ei language C++,c html ...
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxNormal(StyleState state)
{

	if(state == checkend)
	{

		SyntaxState  synstate= m_tTblStructurTags[(unsigned char )m_theChar[0]];

		if(synstate != SyntaxStateNormal)
		{
			(this->*(m_tTblSyntaxFn[synstate]))(start);
		}

	}
	else
	{

		OpenStyle(m_iOffset,normal);
		m_SyntaxState = SyntaxStateNormal;
		m_StyleState  = checkend;


	}

}



// -------------------------------------------------------------------------------------
/*! Analyze for multi comments 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxDblQuote_Analyse(StyleState state)
{
	if(state == checkend)
	{

		if(m_theChar[0] == '"' )
		{
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{

		m_SyntaxState = SyntaxStateDblQuote;
		m_StyleState  = checkend;
		m_theChar[0] = 0;


	}



}


// -------------------------------------------------------------------------------------
/*! Analyze for multi comments 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxSingQuote_Analyse(StyleState state)

{
	if(state == checkend)
	{

		if(m_theChar[0] == '\'' )
		{
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{

		m_SyntaxState = SyntaxStateSingQuote;
		m_theChar[0] = 0; // Raz because m_theChar[1] will fail the end
		m_StyleState  = checkend;



	}

}


// -------------------------------------------------------------------------------------
/*! Analyze for multi comments 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxMultiComment_AnalyseCpp(StyleState state)
{

	if(state == checkend)
	{

		if(m_theChar[1] == '*' && m_theChar[0] == '/' )
		{
			m_MultineCommentsUsed--;
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{
		if((m_theChar[1] == '/' && m_theChar[0] == '*') || state == forcestart)
		{

			// the style start a cur-1
			m_SyntaxState = SyntaxStateMuliComment;
			m_StyleState  = checkend;

			if(!m_MultineCommentsUsed) m_MultineCommentsUsed++;

		}


	}


}







// -------------------------------------------------------------------------------------
/*! Analyze for multi comments 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxSingComment_AnalyseCpp(StyleState state)
{
	if(state == checkend)
	{

		if( m_theChar[0] == '\r' )
		{
			m_SyntaxState = SyntaxStateNormal;
			m_StyleState  = start;
		}

	}
	else
	{
		if((m_theChar[0] == '/' && m_theChar[1] == '/')|| state == forcestart )
		{
			m_SyntaxState = SyntaxStateSingComment;
			m_StyleState  = checkend;

		}


	}

}


// -------------------------------------------------------------------------------------
/*! Analyze for multi comments 
 *
 * \param state   
 *
 * \return   
 */
void	CSyntaxColor::OnSyntaxNormal_Analyse(StyleState state)
{

	if(state == checkend)
	{

		SyntaxState  synstate= m_tTblStructurTags[(unsigned char )m_theChar[0]];

		if(synstate != SyntaxStateNormal)
		{
			(this->*(m_tTblAnalyseFn[synstate]))(start);
		}

	}
	else
	{
		m_SyntaxState = SyntaxStateNormal;
		m_StyleState  = checkend;


	}

}






// -------------------------------------------------------------------------------------
/*! Main function to colorize a range 
 *
 * \param iOffset   
 * \param end   
 * \param fromScratch   
 *
 * \return void   
 */
int CSyntaxColor::ColorizeRange(int start,int end,bool fromScratch)
{
	if( end == -1) //send entire text of rich edit box
	{
		end = m_pCtrl->GetTextLength();

	}
	if(end - start <= 0) return 0;
	
	



	// Get a TOM interface & freeze to avoid screen feedback
	long count;
	m_pTomDoc->Freeze(&count);
	// suspend the undo during operations 
	m_pTomDoc->Undo( tomSuspend,NULL);


	m_pTomDoc->Range(start,end,&m_pRange);



	//Setup some vars

	m_iOffset			= start;	// cur offset
	m_iStyleCurStart	= start;	// init start of style
	m_iCurStartKeyWord  = start;	// init start for keyword
	m_eCurHStyle		= normal;	// init cur style
	m_bWordStart		= false;	// init word detection state

	m_theChar[0]		= 0;		// init cur char 
	m_theChar[1]		= 0;		// and back char
	
	// init state of the structure detection
	m_SyntaxState = SyntaxStateNormal;
	m_StyleState  = CSyntaxColor::start;



	// Continue the style ? for multi line comments 

	if(!fromScratch)
	{
		// with style we start ?//always the char before if 0 we start normal 
		long backpos;
		// find \r in the prevoius line 
		m_pRange->SetRange(m_iOffset,m_iOffset+1);
		m_pRange->FindTextStart(CComBSTR("\r"),tomBackward,0,NULL);
		// gets the pos of the /r
		m_pRange->GetStart(&backpos);
		// if we are in the multi-line style we contine, other we start from normal style
		SyntaxState previous  = (SyntaxState)GetStyle(backpos,backpos+1).byStruct.SyntaxState;
		if(previous == SyntaxStateMuliComment)
		{
			m_SyntaxState = SyntaxStateMuliComment;
		}


	}

	// Will be use for full colorize detection
	m_MultineCommentsUsed = m_SyntaxState == SyntaxStateMuliComment ? 1 : 0;


	// parse the selection

	(this->*(m_tTblSyntaxFn[m_SyntaxState]))(forcestart);

	for(int  r = start ; r < end ;r++)
	{
		// shift sequence
		m_theChar[1] = m_theChar[0];

		m_pRange->SetRange(r,r+1);
		m_pRange->GetChar(&m_theChar[0]);

		(this->*(m_tTblSyntaxFn[m_SyntaxState]))(m_StyleState); // manage comments & strings styles 
		AddCharForKeyWord((WCHAR)m_theChar[0]); // manage keywords
		m_iOffset++;

	}
	AddCharForKeyWord(0); // close keyword
	CloseStyle(m_iOffset);// close styles
	m_pRange->Release();
	m_pTomDoc->Unfreeze(&count);
	m_pTomDoc->Undo( tomResume,NULL);
	return m_MultineCommentsUsed;

}


// -------------------------------------------------------------------------------------
/*! Perform a Analysis of a range, it is the same than ColorizeRange but don't change the style
 *
 * \param iOffset   
 * \param end   
 * \param fromScratch   
 *
 * \return void   
 */
int CSyntaxColor::AnalyseMultiCommentRange(int start,int end,bool fromScratch)
{
	if( end == -1) //send entire text of rich edit box
	{
		end = m_pCtrl->GetTextLength();
	}
	if(end - start <= 0) return 0;
	long count;
	m_pTomDoc->Freeze(&count);
	m_pTomDoc->Undo( tomSuspend,NULL);
	m_pTomDoc->Range(start,end,&m_pRange);

	//setup some vars
	m_iOffset			= start;
	m_iStyleCurStart	= start;
	m_iCurStartKeyWord  = start;
	m_eCurHStyle		= normal;
	m_bWordStart		= false;

	m_theChar[0]		= 0;
	m_theChar[1]		= 0;

	
	m_SyntaxState = SyntaxStateNormal;
	m_StyleState = CSyntaxColor::start;


	// continue the style ? for multi line comments 

	if(!fromScratch)
	{// with style we start ?//always the char before if 0 we start normal 
		long backpos;
		// find \r
		m_pRange->SetRange(m_iOffset,m_iOffset+1);
		m_pRange->FindTextStart(CComBSTR("\r"),tomBackward,0,NULL);
		m_pRange->GetStart(&backpos);
		SyntaxState previous  = (SyntaxState)GetStyle(backpos,backpos+1).byStruct.SyntaxState;
		if(previous == SyntaxStateMuliComment)
		{
			m_SyntaxState = SyntaxStateMuliComment;
		}


	}


	// will be use for full colorize
	m_MultineCommentsUsed = m_SyntaxState == SyntaxStateMuliComment ? 1 : 0;


	m_MultineCommentsUsed = m_SyntaxState == SyntaxStateMuliComment ? 1 : 0;
	(this->*(m_tTblAnalyseFn[m_SyntaxState]))(forcestart);

	for(int  r = start ; r < end ;r++)
	{

		// shift sequence
		m_theChar[1] = m_theChar[0];
		m_pRange->SetRange(r,r+1);
		m_pRange->GetChar(&m_theChar[0]);
		(this->*(m_tTblAnalyseFn[m_SyntaxState]))(m_StyleState);
		m_iOffset++;

	}
	m_pRange->Release();
	m_pTomDoc->Undo( tomResume,NULL);
	m_pTomDoc->Unfreeze(&count);
	return m_MultineCommentsUsed;

}





// -------------------------------------------------------------------------------------
/*! Returns  a range of characters displayed on the screen ( the page)
 *
 * \param range   
 *
 * \return void   
 */
void CSyntaxColor::GetCharsVisible(CHARRANGE & range)
{

	FORMATRANGE fr;

	// Get the page width and height from the printer.


	CDC dc;
	dc.CreateCompatibleDC(m_pCtrl->GetDC());

	CRect rcPage;
	m_pCtrl->GetClientRect(rcPage);

	CRect page;
    page.left = 0;
    page.top = 0;
    page.right = ::MulDiv(rcPage.Width(),1440, dc.GetDeviceCaps(LOGPIXELSX));
    page.bottom = ::MulDiv(rcPage.Height(),1440, dc.GetDeviceCaps(LOGPIXELSY));

	// Format the text and render it to the printer.
	fr.hdc = dc;
	fr.hdcTarget = dc;

	fr.rc = page;
	fr.rcPage = page;
	int  first = m_pCtrl->GetFirstVisibleLine();

	fr.chrg.cpMin = m_pCtrl->LineIndex(first);
	fr.chrg.cpMax = -1;
	long nb = m_pCtrl->FormatRange(&fr, FALSE);
	m_pCtrl->FormatRange(NULL, FALSE);
	range.cpMin = fr.chrg.cpMin;
	range.cpMax = nb+1;

}




// -------------------------------------------------------------------------------------
/*! Not really used
 *
 * \param fromScratch   
 *
 * \return void   
 */
int CSyntaxColor::ColorizePage(bool fromScratch)
{
	CNoChangeEdit save(m_pCtrl);
	CHARRANGE r;
	GetCharsVisible(r);
	int ret = ColorizeRange(r.cpMin,r.cpMax,fromScratch);

	return ret;

}




// -------------------------------------------------------------------------------------
/*!
 *
 * \param index   
 * \param fromScratch   
 *
 * \return void   
 */
int  CSyntaxColor::ColorizeLine(int index,bool fromScratch)
{
	CNoChangeEdit save(m_pCtrl);
	// Compute size line included CRLF
	int start = m_pCtrl->LineIndex(index);
	int end   = m_pCtrl->LineIndex(index+1);
	int ret = ColorizeRange(start,end,fromScratch);
	return ret;
}



// -------------------------------------------------------------------------------------
/*! 
 *
 * \param index   
 * \param fromScratch   
 *
 * \return void   
 */
int CSyntaxColor::AnalyseMultiCommentLine(int index,bool fromScratch)
{
	CNoChangeEdit save(m_pCtrl);
	// Compute size line included CRLF
	int start = m_pCtrl->LineIndex(index);
	int end   = m_pCtrl->LineIndex(index+1);
	int ret = AnalyseMultiCommentRange(start,end,fromScratch);
	return ret;
}





// -------------------------------------------------------------------------------------
/*! <Detailed description of the method>
 *
 *
 * \return   
 */
int	 CSyntaxColor::ColorizeAll()
{
	CNoChangeEdit save(m_pCtrl);
	return  ColorizeRange(0,-1,true); 
}







// -------------------------------------------------------------------------------------
/*! For debug
 *
 * \param line   
 *
 * \return void   
 */
void CSyntaxColor::DumpLine(int line)
{

	// Compute size line included CRLF
	int start = m_pCtrl->LineIndex(line);
	int end   = m_pCtrl->LineIndex(line+1);
	if(end < 0) end = m_pCtrl->GetTextLength();



	long count;
	m_pTomDoc->Freeze(&count);
	m_pTomDoc->Range(start,end,&m_pRange);

	//! contents of the line 
	BSTR	m_pBlkText;


	m_pRange->GetText(&m_pBlkText);

	WCHAR *lpszBuf = m_pBlkText;


	TRACE("----------- %d (%d,%d)----------------\n",line,start,end);

	TRACE("\nOffs : ");
	for( int a = start; a < end ; a++)
	{
		TRACE("%02d,",a-start);
	}


	TRACE("\nBytes: ");
	for(  a = start; a < end ; a++)
	{
		TRACE("%02x,",lpszBuf[a-start]);
	}


	TRACE("\nRun  : ");
	for(  a = start; a < end ; a++)
	{
		TRACE("%X%X,",GetStyle(a,a+1).byStruct.ColorStyle,GetStyle(a,a+1).byStruct.SyntaxState,GetStyle(a,a+1));
	}

	TRACE("\nStr  : ");
	for( a = start; a < end ; a++)
	{
		TRACE("%c  ",lpszBuf[a-start]);
	}

	TRACE("\nEnd %d \n",line);

	if(m_pBlkText) ::SysFreeString(m_pBlkText);
	m_pRange->Release();
	
	m_pTomDoc->Unfreeze(&count);

}