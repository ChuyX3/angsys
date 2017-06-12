
// registry.h: classe permettant l'accès à la base de registres
//

#ifndef	_REGISTRY_H
#define	_REGISTRY_H

#define REGSKYMAP "HKEY_CURRENT_USER/Software/MAKETOD2"
#include <afxtempl.h>

class CKey
{
	public:
				CKey(LPCSTR path,BOOL *created=0,CKey *parent=0);
				~CKey();
		HKEY	GetHKEY(void);
		HKEY GetLast()
		{
			if(m_child==0) return m_key;
			return m_child->GetLast();
		}

	private:
		CKey	*m_child;
		HKEY	m_key;
};


class	CRegistry
{
	CString	m_path;

public:
	CRegistry(LPCSTR path=REGSKYMAP);
	int		ReadKeyInt(LPCSTR name,int *r,int default_value);
	LPCSTR 	ReadKeyString(LPCSTR name,LPSTR r,LPCSTR default_value);
	BOOL	WriteKeyInt(LPCSTR name,int data);
	BOOL	WriteKeyString(LPCSTR name,LPCSTR data);
	int		ReadKey(LPCSTR name,int default_value);
	LPCSTR	ReadKey(LPCSTR name,LPCSTR default_value);
	BOOL	WriteKey(LPCSTR name,int value);
	BOOL	WriteKey(LPCSTR name,LPCSTR value);
	BOOL	DeleteKey(LPCSTR name);
	BOOL	SetPath(LPCSTR path);
	BOOL	TestKey(LPCSTR name);
	float   ReadKey(LPCSTR name,float default_value);
	BOOL    WriteKey(LPCSTR name,float default_value);
	BOOL	EnumKeys(LPCSTR name,CArray<CString,CString> & m);
	BOOL	EnumValue(LPCSTR name,CArray<CString,CString> & m);
	
};

class PlgResourceHandle
{
public :
	PlgResourceHandle( AFX_EXTENSION_MODULE &dll )
	{
		m_OldInst = AfxGetResourceHandle() ;
		AfxSetResourceHandle( dll.hResource ) ;
	}
	~PlgResourceHandle() 
	{
		AfxSetResourceHandle( m_OldInst ) ;
	}
private :
	HINSTANCE	m_OldInst ;
} ;
#endif
