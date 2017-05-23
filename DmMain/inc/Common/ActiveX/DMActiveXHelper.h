//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:http://www.codeproject.com/Articles/33516/ATL-Windowless-ActiveX-Media-Container
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-17	1.0			
//--------------------------------------------------------
#pragma once
#include "flash10t.tlh"
#include <MsHtmHst.h>
#include <oleauto.h>
#include <OleCtl.h>
#include <unknwn.h>

// {0002DF05-0000-0000-C000-000000000046}
static const GUID GUID_IWebBrowserApp = { 0x0002DF05, 0x0000, 0x0000, { 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } };
// {1B36028E-B491-4BB2-8584-8A9E0A677D6E}
static const GUID GUID_IXcpControlHost = { 0x1B36028E, 0xB491, 0x4BB2, { 0x85, 0x84, 0x8A, 0x9E, 0x0A, 0x67, 0x7D, 0x6E } };
#define DMTRACENOTIMPL(funcname) 	/*LOG_DEBUG(funcname); */return E_NOTIMPL

#ifdef _DISABLE_NO_VTABLE
#define DM_NO_VTABLE
#else
#define DM_NO_VTABLE __declspec(novtable)
#endif

namespace DM
{
	class DM_EXPORT DMBStr
	{
	public:
		DMBStr():bstr_(NULL) {}
		explicit DMBStr(const wchar_t* non_bstr);
		~DMBStr();
		void Reset(BSTR bstr=NULL);
		BSTR Release();
		BSTR Allocate(const wchar_t* str);
		BSTR AllocateBytes(size_t bytes);
		void SetByteLen(size_t bytes);
		void Swap(DMBStr& bstr2);
		BSTR* Receive();
		size_t Length() const;
		size_t ByteLength() const;
		operator BSTR() const{return bstr_;}

	protected:
		BSTR bstr_;

	private:
		bool operator==(const DMBStr& bstr2) const;
		bool operator!=(const DMBStr& bstr2) const;
		DM_DISALLOW_COPY_AND_ASSIGN(DMBStr);
	};

	// ---------------------COMÖÇÄÜÖ¸Õë--------------------------------
#pragma warning (push)
#pragma warning (disable: 4127)  
#pragma warning (disable: 4571) 

#pragma pack(push,8)
	inline IUnknown* DMComPtrAssign(IUnknown** pp, IUnknown* lp)
	{
		if (pp == NULL)
		{
			return NULL; 
		}
		if (lp != NULL)
		{
			lp->AddRef();
		}
		if (*pp)
		{
			(*pp)->Release();
		}
		*pp = lp;
		return lp;
	}

	inline IUnknown* DMComQIPtrAssign(IUnknown** pp, IUnknown* lp, REFIID riid)
	{
		if (pp == NULL)
		{
			return NULL;
		}

		IUnknown* pTemp = *pp;
		*pp = NULL;
		if (lp != NULL)
		{
			lp->QueryInterface(riid, (void**)pp);
		}
		if (pTemp)
		{
			pTemp->Release();
		}
		return *pp;
	}

	template <class T>
	class _DMNoAddRefReleaseOnCComPtr : public T
	{
	private:
		STDMETHOD_(ULONG, AddRef)() = 0;
		STDMETHOD_(ULONG, Release)()= 0;
	};

	template <class T>
	class DMComPtrBase
	{
	protected:
		DMComPtrBase() throw()
		{
			p = NULL;
		}
		DMComPtrBase(int nNull) throw()
		{
			DMASSERT(nNull == 0);
			(void)nNull;
			p = NULL;
		}
		DMComPtrBase(T* lp) throw()
		{
			p = lp;
			if (p != NULL)
			{
				p->AddRef();
			}
		}
	public:
		typedef T _PtrClass;
		~DMComPtrBase() throw()
		{
			if (p)
			{
				p->Release();
			}
		}
		operator T*() const throw()
		{
			return p;
		}
		T& operator*() const
		{
			DMASSERT(p!=NULL);
			return *p;
		}
		//The DMASSERT on operator& usually indicates a bug.  If this is really
		//what is needed, however, take the address of the p member explicitly.
		T** operator&() throw()
		{
			DMASSERT(p==NULL);
			return &p;
		}
		_DMNoAddRefReleaseOnCComPtr<T>* operator->() const throw()
		{
			DMASSERT(p!=NULL);
			return (_DMNoAddRefReleaseOnCComPtr<T>*)p;
		}
		bool operator!() const throw()
		{
			return (p == NULL);
		}
		bool operator<( T* pT) const throw()
		{
			return p < pT;
		}
		bool operator!=( T* pT) const
		{
			return !operator==(pT);
		}
		bool operator==( T* pT) const throw()
		{
			return p == pT;
		}

		// Release the interface and set to NULL
		void Release() throw()
		{
			T* pTemp = p;
			if (pTemp)
			{
				p = NULL;
				pTemp->Release();
			}
		}
		// Compare two objects for equivalence
		bool IsEqualObject( IUnknown* pOther) throw()
		{
			if (p == NULL && pOther == NULL)
				return true;    // They are both NULL objects

			if (p == NULL || pOther == NULL)
				return false;    // One is NULL the other is not

			DMComPtr<IUnknown> punk1;
			DMComPtr<IUnknown> punk2;
			p->QueryInterface(__uuidof(IUnknown), (void**)&punk1);
			pOther->QueryInterface(__uuidof(IUnknown), (void**)&punk2);
			return punk1 == punk2;
		}
		// Attach to an existing interface (does not AddRef)
		void Attach( T* p2) throw()
		{
			if (p)
				p->Release();
			p = p2;
		}
		// Detach the interface (does not Release)
		T* Detach() throw()
		{
			T* pt = p;
			p = NULL;
			return pt;
		}
		HRESULT CopyTo( T** ppT) throw()
		{
			DMASSERT(ppT != NULL);
			if (ppT == NULL)
				return E_POINTER;
			*ppT = p;
			if (p)
				p->AddRef();
			return S_OK;
		}
		HRESULT CoCreateInstance( REFCLSID rclsid,  LPUNKNOWN pUnkOuter = NULL,  DWORD dwClsContext = CLSCTX_ALL) throw()
		{
			DMASSERT(p == NULL);
			return ::CoCreateInstance(rclsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
		}
		HRESULT CoCreateInstance( LPCOLESTR szProgID,  LPUNKNOWN pUnkOuter = NULL,  DWORD dwClsContext = CLSCTX_ALL) throw()
		{
			CLSID clsid;
			HRESULT hr = CLSIDFromProgID(szProgID, &clsid);
			DMASSERT(p == NULL);
			if (SUCCEEDED(hr))
				hr = ::CoCreateInstance(clsid, pUnkOuter, dwClsContext, __uuidof(T), (void**)&p);
			return hr;
		}
		template <class Q>
		HRESULT QueryInterface( Q** pp) const throw()
		{
			DMASSERT(pp != NULL);
			return p->QueryInterface(__uuidof(Q), (void**)pp);
		}
		T* p;
	};

	template <class T>
	class DMComPtr : public DMComPtrBase<T>
	{
	public:
		DMComPtr() throw()
		{
		}
		DMComPtr(int nNull) throw() :
		DMComPtrBase<T>(nNull)
		{
		}
		DMComPtr(T* lp) throw() :
		DMComPtrBase<T>(lp)

		{
		}
		DMComPtr( const DMComPtr<T>& lp) throw() :
		DMComPtrBase<T>(lp.p)
		{
		}
		T* operator=( T* lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<T*>(DMComPtrAssign((IUnknown**)&p, lp));
			}
			return *this;
		}
		template <typename Q>
		T* operator=( const DMComPtr<Q>& lp) throw()
		{
			if( !IsEqualObject(lp) )
			{
				return static_cast<T*>(DMComQIPtrAssign((IUnknown**)&p, lp, __uuidof(T)));
			}
			return *this;
		}
		T* operator=( const DMComPtr<T>& lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<T*>(DMComPtrAssign((IUnknown**)&p, lp));
			}
			return *this;
		}
	};

	//specialization for IDispatch
	template <>
	class DMComPtr<IDispatch> : public DMComPtrBase<IDispatch>
	{
	public:
		DMComPtr() throw()
		{
		}
		DMComPtr(IDispatch* lp) throw() :
		DMComPtrBase<IDispatch>(lp)
		{
		}
		DMComPtr(const DMComPtr<IDispatch>& lp) throw() :
		DMComPtrBase<IDispatch>(lp.p)
		{
		}
		IDispatch* operator=(IDispatch* lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<IDispatch*>(DMComPtrAssign((IUnknown**)&p, lp));
			}
			return *this;
		}
		IDispatch* operator=(const DMComPtr<IDispatch>& lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<IDispatch*>(DMComPtrAssign((IUnknown**)&p, lp.p));
			}
			return *this;
		}

		// IDispatch specific stuff
		HRESULT GetPropertyByName( LPCOLESTR lpsz,  VARIANT* pVar) throw()
		{
			DMASSERT(p);
			DMASSERT(pVar);
			DISPID dwDispID;
			HRESULT hr = GetIDOfName(lpsz, &dwDispID);
			if (SUCCEEDED(hr))
				hr = GetProperty(dwDispID, pVar);
			return hr;
		}
		HRESULT GetProperty( DISPID dwDispID,  VARIANT* pVar) throw()
		{
			return GetProperty(p, dwDispID, pVar);
		}
		HRESULT PutPropertyByName( LPCOLESTR lpsz,  VARIANT* pVar) throw()
		{
			DMASSERT(p);
			DMASSERT(pVar);
			DISPID dwDispID;
			HRESULT hr = GetIDOfName(lpsz, &dwDispID);
			if (SUCCEEDED(hr))
				hr = PutProperty(dwDispID, pVar);
			return hr;
		}
		HRESULT PutProperty( DISPID dwDispID,  VARIANT* pVar) throw()
		{
			return PutProperty(p, dwDispID, pVar);
		}
		HRESULT GetIDOfName( LPCOLESTR lpsz,  DISPID* pdispid) throw()
		{
			return p->GetIDsOfNames(IID_NULL, const_cast<LPOLESTR*>(&lpsz), 1, LOCALE_USER_DEFAULT, pdispid);
		}
		// Invoke a method by DISPID with no parameters
		HRESULT Invoke0( DISPID dispid,  VARIANT* pvarRet = NULL) throw()
		{
			DISPPARAMS dispparams = { NULL, NULL, 0, 0};
			return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
		}
		// Invoke a method by name with no parameters
		HRESULT Invoke0( LPCOLESTR lpszName,  VARIANT* pvarRet = NULL) throw()
		{
			HRESULT hr;
			DISPID dispid;
			hr = GetIDOfName(lpszName, &dispid);
			if (SUCCEEDED(hr))
				hr = Invoke0(dispid, pvarRet);
			return hr;
		}
		// Invoke a method by DISPID with a single parameter
		HRESULT Invoke1( DISPID dispid, VARIANT* pvarParam1,  VARIANT* pvarRet = NULL) throw()
		{
			DISPPARAMS dispparams = { pvarParam1, NULL, 1, 0};
			return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
		}
		// Invoke a method by name with a single parameter
		HRESULT Invoke1( LPCOLESTR lpszName, VARIANT* pvarParam1,  VARIANT* pvarRet = NULL) throw()
		{
			HRESULT hr;
			DISPID dispid;
			hr = GetIDOfName(lpszName, &dispid);
			if (SUCCEEDED(hr))
				hr = Invoke1(dispid, pvarParam1, pvarRet);
			return hr;
		}
		// Invoke a method by DISPID with two parameters
		HRESULT Invoke2( DISPID dispid,  VARIANT* pvarParam1,  VARIANT* pvarParam2,  VARIANT* pvarRet = NULL) throw();
		// Invoke a method by name with two parameters
		HRESULT Invoke2( LPCOLESTR lpszName,  VARIANT* pvarParam1,  VARIANT* pvarParam2,  VARIANT* pvarRet = NULL) throw()
		{
			HRESULT hr;
			DISPID dispid;
			hr = GetIDOfName(lpszName, &dispid);
			if (SUCCEEDED(hr))
				hr = Invoke2(dispid, pvarParam1, pvarParam2, pvarRet);
			return hr;
		}
		// Invoke a method by DISPID with N parameters
		HRESULT InvokeN(DISPID dispid, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
		{
			DISPPARAMS dispparams = { pvarParams, NULL, nParams, 0};
			return p->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD, &dispparams, pvarRet, NULL, NULL);
		}
		// Invoke a method by name with Nparameters
		HRESULT InvokeN(LPCOLESTR lpszName, VARIANT* pvarParams, int nParams, VARIANT* pvarRet = NULL) throw()
		{
			HRESULT hr;
			DISPID dispid;
			hr = GetIDOfName(lpszName, &dispid);
			if (SUCCEEDED(hr))
				hr = InvokeN(dispid, pvarParams, nParams, pvarRet);
			return hr;
		}
		static HRESULT PutProperty( IDispatch* p,  DISPID dwDispID,  VARIANT* pVar) throw()
		{
			DMASSERT(p);
			DMASSERT(pVar != NULL);
			if (pVar == NULL)
				return E_POINTER;

			if(p == NULL)
				return E_INVALIDARG;

			DISPPARAMS dispparams = {NULL, NULL, 1, 1};
			dispparams.rgvarg = pVar;
			DISPID dispidPut = DISPID_PROPERTYPUT;
			dispparams.rgdispidNamedArgs = &dispidPut;

			if (pVar->vt == VT_UNKNOWN || pVar->vt == VT_DISPATCH || 
				(pVar->vt & VT_ARRAY) || (pVar->vt & VT_BYREF))
			{
				HRESULT hr = p->Invoke(dwDispID, IID_NULL,
					LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUTREF,
					&dispparams, NULL, NULL, NULL);
				if (SUCCEEDED(hr))
					return hr;
			}
			return p->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYPUT,
				&dispparams, NULL, NULL, NULL);
		}
		static HRESULT GetProperty( IDispatch* p,  DISPID dwDispID,  VARIANT* pVar) throw()
		{
			DMASSERT(p);
			DMASSERT(pVar != NULL);
			if (pVar == NULL)
				return E_POINTER;

			if(p == NULL)
				return E_INVALIDARG;

			DISPPARAMS dispparamsNoArgs = {NULL, NULL, 0, 0};
			return p->Invoke(dwDispID, IID_NULL,
				LOCALE_USER_DEFAULT, DISPATCH_PROPERTYGET,
				&dispparamsNoArgs, pVar, NULL, NULL);
		}
	};

	template <class T, const IID* piid = &__uuidof(T)>
	class DMComQIPtr : public DMComPtr<T>
	{
	public:
		DMComQIPtr() throw()
		{
		}
		DMComQIPtr( T* lp) throw() :
		DMComPtr<T>(lp)
		{
		}
		DMComQIPtr( const DMComQIPtr<T,piid>& lp) throw() :
		DMComPtr<T>(lp.p)
		{
		}
		DMComQIPtr( IUnknown* lp) throw()
		{
			if (lp != NULL)
				lp->QueryInterface(*piid, (void **)&p);
		}
		T* operator=( T* lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<T*>(DMComPtrAssign((IUnknown**)&p, lp));
			}
			return *this;
		}
		T* operator=( const DMComQIPtr<T,piid>& lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<T*>(DMComPtrAssign((IUnknown**)&p, lp.p));
			}
			return *this;
		}
		T* operator=( IUnknown* lp) throw()
		{
			if(*this!=lp)
			{
				return static_cast<T*>(DMComQIPtrAssign((IUnknown**)&p, lp, *piid));
			}
			return *this;
		}
	};

	//Specialization to make it work
	template<>
	class DMComQIPtr<IUnknown, &IID_IUnknown> : public DMComPtr<IUnknown>
	{
	public:
		DMComQIPtr() throw()
		{
		}
		DMComQIPtr( IUnknown* lp) throw()
		{
			//Actually do a QI to get identity
			if (lp != NULL)
				lp->QueryInterface(__uuidof(IUnknown), (void **)&p);
		}
		DMComQIPtr( const DMComQIPtr<IUnknown,&IID_IUnknown>& lp) throw() :
		DMComPtr<IUnknown>(lp.p)
		{
		}
		IUnknown* operator=( IUnknown* lp) throw()
		{
			if(*this!=lp)
			{
				//Actually do a QI to get identity
				return DMComQIPtrAssign((IUnknown**)&p, lp, __uuidof(IUnknown));
			}
			return *this;
		}

		IUnknown* operator=( const DMComQIPtr<IUnknown,&IID_IUnknown>& lp) throw()
		{
			if(*this!=lp)
			{
				return DMComPtrAssign((IUnknown**)&p, lp.p);
			}
			return *this;
		}
	};

	typedef DMComQIPtr<IDispatch, &__uuidof(IDispatch)> CComDispatchDriver;
#pragma pack(pop)
#pragma warning (pop)    

}//namespace DM