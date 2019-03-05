#include "DmMainAfx.h"
#include "DMBundle.h"
#include "DMBundleImpl.h"


namespace DM
{
	DMBundle::DMBundle()
	{
		m_pBundleImpl.Attach(new DMBundleImpl);
	}

	DMBundle::DMBundle(const DMBundle& src)
	{
		m_pBundleImpl.reset(src.m_pBundleImpl);
	}

	DMBundle& DMBundle::operator=(const DMBundle& src)
	{
		m_pBundleImpl.reset(src.m_pBundleImpl);
		return *this;
	}

	DMBundle::~DMBundle()
	{
		m_pBundleImpl.Release();
	}

	DMBundle DMBundle::Clone()
	{
		DMBundle cloneBundle;
		if (cloneBundle.m_pBundleImpl) 
		{
			cloneBundle.m_pBundleImpl->operator =(*m_pBundleImpl);
		}
		return cloneBundle;
	}

	void DMBundle::Clear()
	{
		m_pBundleImpl->Clear();
	}

	int DMBundle::Size()
	{
		return m_pBundleImpl->Size();
	}

	bool DMBundle::Remove(LPCWSTR key)
	{
		return m_pBundleImpl->Remove(key);
	}

	bool DMBundle::HasKey(LPCWSTR key)
	{
		return m_pBundleImpl->HasKey(key);
	}

	bool DMBundle::PutBool(LPCWSTR key, bool value)
	{
		return m_pBundleImpl->PutBool(key, value);
	}

	bool DMBundle::GetBool(LPCWSTR key, bool defaultValue/*=false*/) const
	{
		return m_pBundleImpl->GetBool(key, defaultValue);
	}

	bool DMBundle::PutByte(LPCWSTR key, byte value)
	{
		return m_pBundleImpl->PutByte(key, value);
	}

	byte DMBundle::GetByte(LPCWSTR key, byte defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetByte(key, defaultValue);
	}

	bool DMBundle::PutChar(LPCWSTR key, char value)
	{
		return m_pBundleImpl->PutChar(key, value);
	}

	char DMBundle::GetChar(LPCWSTR key, char defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetChar(key, defaultValue);
	}

	bool DMBundle::PutDword(LPCWSTR key, DWORD value)
	{
		return m_pBundleImpl->PutDword(key, value);
	}

	DWORD DMBundle::GetDword(LPCWSTR key, DWORD defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetDword(key, defaultValue);
	}

	bool DMBundle::PutDouble(LPCWSTR key, double value)
	{
		return m_pBundleImpl->PutDouble(key, value);
	}

	double DMBundle::GetDouble(LPCWSTR key, double defaultValue/*=0.0f*/) const
	{
		return m_pBundleImpl->GetDouble(key, defaultValue);
	}

	bool DMBundle::PutFloat(LPCWSTR key, float value)
	{
		return m_pBundleImpl->PutFloat(key, value);
	}

	float DMBundle::GetFloat(LPCWSTR key, float defaultValue/*=0.0f*/) const
	{
		return m_pBundleImpl->GetFloat(key, defaultValue);
	}

	bool DMBundle::PutInt(LPCWSTR key, int value)
	{
		return m_pBundleImpl->PutInt(key, value);
	}

	int DMBundle::GetInt(LPCWSTR key, int defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetInt(key, defaultValue);
	}

	bool DMBundle::PutInt64(LPCWSTR key, INT64 value)
	{
		return m_pBundleImpl->PutInt64(key, value);
	}

	INT64 DMBundle::GetInt64(LPCWSTR key, INT64 defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetInt64(key, defaultValue);
	}

	bool DMBundle::PutShort(LPCWSTR key, short value)
	{
		return m_pBundleImpl->PutShort(key, value);
	}

	short DMBundle::GetShort(LPCWSTR key, short defaultValue/*=0*/) const
	{
		return m_pBundleImpl->GetShort(key, defaultValue);
	}

	bool DMBundle::PutString(LPCWSTR key, LPCSTR value)
	{
		return m_pBundleImpl->PutString(key, value);
	}

	LPCSTR DMBundle::GetString(LPCWSTR key, LPCSTR defaultValue/*=""*/) const
	{
		return m_pBundleImpl->GetString(key, defaultValue);
	}

	bool DMBundle::PutStringW(LPCWSTR key, LPCWSTR value)
	{
		return m_pBundleImpl->PutStringW(key, value);
	}

	LPCWSTR DMBundle::GetStringW(LPCWSTR key, LPCWSTR defaultValue/*=L""*/) const
	{
		return m_pBundleImpl->GetStringW(key, defaultValue);
	}

	bool DMBundle::PutSize(LPCWSTR key, CSize value)
	{
		return m_pBundleImpl->PutSize(key, value);
	}

	CSize DMBundle::GetSize(LPCWSTR key, CSize defaultValue/*=CSize()*/) const
	{
		return m_pBundleImpl->GetSize(key, defaultValue);
	}

	bool DMBundle::PutRect(LPCWSTR key, CRect value)
	{
		return m_pBundleImpl->PutRect(key, value);
	}

	CRect DMBundle::GetRect(LPCWSTR key, CRect defaultValue/*=CRect()*/) const
	{
		return m_pBundleImpl->GetRect(key, defaultValue);
	}

	bool DMBundle::PutVoidPtr(LPCWSTR key, void* value)
	{
		return m_pBundleImpl->PutVoidPtr(key, value);
	}

	void* DMBundle::GetVoidPtr(LPCWSTR key, void* defaultValue/*=NULL*/) const
	{
		return m_pBundleImpl->GetVoidPtr(key, defaultValue);
	}

	bool DMBundle::PutIUnknownPtr(LPCWSTR key, IUnknown* value)
	{
		return m_pBundleImpl->PutIUnknownPtr(key, value);
	}

	IUnknown* DMBundle::GetIUnknownPtr(LPCWSTR key, IUnknown* defaultValue/*=NULL*/) const
	{
		return m_pBundleImpl->GetIUnknownPtr(key, defaultValue);
	}

	bool DMBundle::PutXmlNode(LPCWSTR key, DMXmlNode value)
	{
		return m_pBundleImpl->PutXmlNode(key, value);
	}

	DMXmlNode DMBundle::GetXmlNode(LPCWSTR key) const
	{
		return m_pBundleImpl->GetXmlNode(key);
	}

}//namespace DM