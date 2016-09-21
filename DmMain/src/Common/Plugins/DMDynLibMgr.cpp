#include "DmMainAfx.h"
#include "DMDynLibMgr.h"

namespace DM
{
	DMDynLibMgr::DMDynLibMgr(void)
	{
	}

	DMDynLibMgr::~DMDynLibMgr(void)
	{
		size_t nCount = m_LibArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			m_LibArray[i]->Unload();
			DM_DELETE(m_LibArray[i]);
		}

		m_LibArray.RemoveAll();
	}

	DMDynLib* DMDynLibMgr::Load(const CStringW& strPluginPath)
	{
		DMDynLib *pLib = NULL;
		do 
		{
			pLib = FindObj(strPluginPath);
			if (NULL != pLib)
			{
				CStringW szName = pLib->GetName();
				szName += L"此dll被尝试加载了两次";
				DMASSERT_EXPR((0),szName);
				break;
			}

			pLib = new DMDynLib(strPluginPath);
			if (!pLib->Load())
			{
				DM_DELETE(pLib);
				break;
			}
			m_LibArray.Add(pLib);
		} while (false);
		return pLib;
	}

	void DMDynLibMgr::Unload(DMDynLib* plib)
	{
		size_t nCount = m_LibArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			if (0 == m_LibArray[i]->GetName().CompareNoCase(plib->GetName()))
			{
				m_LibArray.RemoveAt(i);
				break;
			}
		}
		plib->Unload();
		DM_DELETE(plib);
	}

	DMDynLib* DMDynLibMgr::FindObj(const CStringW& strPluginPath)
	{
		size_t nCount = m_LibArray.GetCount();
		for (size_t i=0; i<nCount; i++)
		{
			if (0 == strPluginPath.CompareNoCase(m_LibArray[i]->GetName()))
			{
				return m_LibArray[i];
			}
		}

		return NULL;
	}

}//namespace DM