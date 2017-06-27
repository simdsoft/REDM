#include "DmMainAfx.h"
#include "DMDynLib.h"

namespace DM
{
	DMDynLib::DMDynLib(const CStringW &strPluginPath)
		:m_hModule(NULL)
	{
		m_strPluginPath = strPluginPath;
	}

	DMDynLib::~DMDynLib(void)
	{
	}

	bool DMDynLib::Load()
	{
		bool bRet = false;
		do 
		{
			CStringW szPath = m_strPluginPath;
			if (szPath.Right(4)!=L".dll")
			{// LoadLibraryExW在szPath为相对路径时,不会自动加.dll后缀
				szPath += L".dll";
			}
			
			m_hModule = ::LoadLibraryExW(szPath, NULL, LOAD_WITH_ALTERED_SEARCH_PATH);
			if (NULL == m_hModule)
			{
				m_hModule = ::LoadLibraryW(szPath);
				if (NULL == m_hModule)
				{
					DMASSERT_EXPR(NULL != m_hModule,DynlibError());
					break;
				}
			}

			bRet = true;
		} while (FALSE);
		return bRet;
	}

	void DMDynLib::Unload()
	{
		if (m_hModule)
		{
			::FreeLibrary(m_hModule);
			m_hModule = NULL;
		}
	}

	const CStringW& DMDynLib::GetName()const
	{
		return m_strPluginPath;
	}

	void* DMDynLib::GetSymbol(const char* pProcName)const
	{
		return (void*)GetProcAddress(m_hModule, pProcName);
	}

	CStringW DMDynLib::DynlibError(void)
	{
		LPVOID lpMsgBuf; 
		FormatMessageW( 
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM | 
			FORMAT_MESSAGE_IGNORE_INSERTS, 
			NULL, 
			GetLastError(), 
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), 
			(LPTSTR) &lpMsgBuf, 
			0, 
			NULL 
			); 
		CStringW strRet = (wchar_t*)lpMsgBuf;
		// Free the buffer.
		LocalFree(lpMsgBuf);
		return strRet;
	}

}//namespace DM

