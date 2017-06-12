
#include	"stdafx.h"
#include	<winreg.h>
#include	"Registry.h"
#include	<stdlib.h>
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CRegistry::CRegistry(LPCSTR path)
{
	VERIFY(SetPath(path));
}

BOOL	CRegistry::SetPath(LPCSTR path)
{
	if(!memicmp(path,"HKEY_",5))	
	{
		m_path=	path;
	}
	else
	{
		m_path+= "/";
		m_path+= path;
	}
	return TRUE;
}


HKEY	CKey::GetHKEY(void)
{
	for(CKey	*r=this;r->m_child;r=r->m_child);
	return(r->m_key);
}



CKey::CKey(LPCSTR path,BOOL *created,CKey *parent)
{
	char lpath[256];
	int l;
	char* f;
	unsigned long exist;
	m_child=0;
	f=strchr(path,'/');
	if(!f)
		f=strchr(path,'\\');
	l=f?f-path:strlen(path);
	strncpy(lpath,path,l);
	lpath[l]=0;
	if(!stricmp(lpath,"HKEY_CLASSES_ROOT"))
		m_key=HKEY_CLASSES_ROOT;
	else if(!stricmp(lpath,"HKEY_CURRENT_USER"))
		m_key=HKEY_CURRENT_USER;
	else if(!stricmp(lpath,"HKEY_LOCAL_MACHINE"))
		m_key=HKEY_LOCAL_MACHINE;
	else if(!stricmp(lpath,"HKEY_USERS"))
		m_key=HKEY_USERS;
	else
	{
		// by prosper : je referme la clef après sa création pour être propre.
		LONG r = RegCreateKeyEx( parent->m_key, lpath, 0, "", REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &m_key, &exist );
//		RegCloseKey( parent->m_key );
	}
	if(f)
	{
		m_child=new CKey(f+1,created,this);
	}
	else
	{
		if(created)
			*created=exist==REG_CREATED_NEW_KEY;
	}
}

CKey::~CKey()
{
	if(m_child)		delete m_child;
	if(m_key == HKEY_CURRENT_USER) return;
	if(m_key == HKEY_CLASSES_ROOT) return;
	if(m_key == HKEY_LOCAL_MACHINE) return;
	if(m_key == HKEY_USERS) return;
	
	RegCloseKey(m_key);
}

int		CRegistry::ReadKeyInt(LPCSTR name,int *r,int default_value)
{
	BOOL			created;
	unsigned long	type,size;
	size=sizeof(*r);

	CKey *pkey=new CKey(m_path,&created);
	ASSERT(pkey);

	HKEY	hkey=pkey->GetHKEY();
	ASSERT(hkey);
	if(RegQueryValueEx(hkey,name,0,&type,(unsigned char*)r,&size)!=ERROR_SUCCESS || type!=REG_DWORD)
		*r=default_value;
	delete pkey;
	return(*r);
}

LPCSTR CRegistry::ReadKeyString(LPCSTR name,LPSTR r,LPCSTR default_value)
{
	BOOL			created;
	unsigned long	type,size;
	size=256;
	CKey *pkey=new CKey(m_path,&created);
	ASSERT(pkey);

	HKEY	hkey=pkey->GetHKEY();
	if(RegQueryValueEx(hkey,name,0,&type,(unsigned char*)r,&size)!=ERROR_SUCCESS || type!=REG_SZ)
		strcpy(r,default_value);
	delete pkey;
	return(r);
}


BOOL	CRegistry::WriteKeyInt(LPCSTR name,int data)
{
	CKey *pkey=new CKey(m_path);
	ASSERT(pkey);

	HKEY	hkey=pkey->GetHKEY();
	ASSERT(hkey);

	int	r=RegSetValueEx(hkey,name,0,REG_DWORD,(const unsigned char*)&data,4);
	delete pkey;
	return r ==  ERROR_SUCCESS;
}

BOOL	CRegistry::WriteKeyString(LPCSTR name,LPCSTR data)
{
	CKey *pkey=new CKey(m_path);
	ASSERT(pkey);
	HKEY	hkey=pkey->GetHKEY();
	ASSERT(hkey);
	int r=RegSetValueEx(hkey,name,0,REG_SZ,(const unsigned char*)data,strlen(data)+1);
	delete pkey;
	return r ==  ERROR_SUCCESS
;
}

int		CRegistry::ReadKey(LPCSTR name,int default_value)
{
	int		r;
	ReadKeyInt(name,&r,default_value);
	return(r);
}

LPCSTR CRegistry::ReadKey(LPCSTR name,LPCSTR default_value)
{
	static	char	buffer[256];
	ReadKeyString(name,buffer,default_value);
	return(buffer);
}


float CRegistry::ReadKey(LPCSTR name,float default_value)
{
	CString text;
	text.Format("%f",default_value);
	CString readval;
	readval = ReadKey(name,text);
	return  (float)atof((LPCTSTR)readval);
}


BOOL 	CRegistry::WriteKey(LPCSTR name,float value)
{
//	ASSERT(value >= 0.0F);
//	ASSERT(value <= 1.0F);
	CString text;
	text.Format("%f",value);
	return WriteKey(name,text);
	
}



BOOL 	CRegistry::WriteKey(LPCSTR name,int value)
{
	return WriteKeyInt(name,value);
}

BOOL	CRegistry::WriteKey(LPCSTR name,LPCSTR value)
{
	return WriteKeyString(name,value);
}


BOOL	CRegistry::DeleteKey(LPCSTR name)
{
	// Is the key to delete exists ?
	CString spath = m_path;
	if( spath.Left(1) != "/" )
		spath += "/";
	spath += name;
	CKey *pcurrentkey=new CKey(spath);
	if(pcurrentkey != NULL)
	{
		HKEY	hcurrentkey=pcurrentkey->GetHKEY();
		if( hcurrentkey )
		{
			char buf_name[_MAX_PATH], buf_class[_MAX_PATH];
			DWORD size_name, size_class;
			FILETIME ft;
			while( TRUE )
			{
				size_name = sizeof(buf_name);
				size_class = sizeof(buf_class);
				LONG r = RegEnumKeyEx( hcurrentkey, 0, buf_name, &size_name, NULL, buf_class, &size_class, &ft );
				if( r != ERROR_SUCCESS )
					break;

				CRegistry reg( spath );
				reg.DeleteKey( buf_name );
			}
		}

		delete pcurrentkey;

		// Delete master key.
		CKey *pkey=new CKey(m_path);
		ASSERT(pkey);
		HKEY	hkey=pkey->GetHKEY();
		if( hkey )
		{
			int err = RegDeleteKey( hkey, name );
			delete pkey;
			return err ==  ERROR_SUCCESS;
		}
	}
	return FALSE;
}



BOOL	CRegistry::TestKey(LPCSTR name)
{
	CKey *pkey=new CKey(m_path);
	ASSERT(pkey);
	HKEY	hkey=pkey->GetHKEY();
	ASSERT(hkey);
	HKEY   h;
	int err = RegOpenKey(hkey,name,&h);
	delete pkey;
	if(err != ERROR_SUCCESS) return FALSE;
	return TRUE;
}




BOOL	CRegistry::EnumKeys(LPCSTR name,CArray<CString,CString> & m)

{
	// Is the key to delete exists ?
	CString spath = m_path;
	if( spath.Left(1) != "/" )
		spath += "/";
	spath += name;
	CKey *pcurrentkey=new CKey(spath);
	if(pcurrentkey != NULL)
	{
		HKEY	hcurrentkey=pcurrentkey->GetHKEY();
		int index = 0;
		if( hcurrentkey )
		{
			char buf_name[_MAX_PATH], buf_class[_MAX_PATH];
			DWORD size_name=_MAX_PATH, size_class;
			FILETIME ft;
			while( TRUE )
			{
				size_name = sizeof(buf_name);
				size_class = sizeof(buf_class);
				LONG r = RegEnumKeyEx( hcurrentkey, index++, buf_name, &size_name, NULL, buf_class, &size_class, &ft );
				if( r != S_OK )
				{
					break;
			
				}
				m.Add(CString(buf_name));
			}
		}

		delete pcurrentkey;

			}
	return FALSE;
}




BOOL	CRegistry::EnumValue(LPCSTR name,CArray<CString,CString> & m)

{
	// Is the key to delete exists ?
	CString spath = m_path;
	if( spath.Left(1) != "/" )
		spath += "/";
	spath += name;
	CKey *pcurrentkey=new CKey(spath);
	if(pcurrentkey != NULL)
	{
		HKEY	hcurrentkey=pcurrentkey->GetHKEY();
		int index = 0;
		if( hcurrentkey )
		{
			TCHAR  buf_name[_MAX_PATH]; 
			TCHAR  buf_class[_MAX_PATH];
			DWORD  size_name=_MAX_PATH;
			DWORD  size_class =_MAX_PATH;
			while( TRUE )
			{
				size_name = sizeof(buf_name);
				size_class = sizeof(buf_class);
				DWORD		type;

				LONG r = RegEnumValue( hcurrentkey, index++, buf_name, &size_name, NULL, &type, (LPBYTE)buf_class, &size_class );
				if( r != S_OK )
				{
					break;
			
				}
				m.Add(CString(buf_name));
			}
		}

		delete pcurrentkey;

			}
	return FALSE;
}




