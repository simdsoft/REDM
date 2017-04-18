#include "DmMainAfx.h"
#include "DMActiveXBase.h "

namespace DM
{
	DMAxContainer::DMAxContainer(): m_lRefCnt(1)
	{
	}

	DMAxContainer::~DMAxContainer()
	{
	}

	BOOL DMAxContainer::CreateControl(LPCRECT pRect, REFGUID clsid, DWORD dwClsCtx /*=CLSCTX_INPROC_SERVER*/ ,bool bSupportMultThread/*=false*/)
	{
		DMASSERT(m_pAxHostDelegate);
		HRESULT hr = E_FAIL;
		DMComPtr<IUnknown> pControl;
		hr = CoCreateInstance(clsid, NULL,dwClsCtx , __uuidof(IUnknown), reinterpret_cast<void**>(&pControl));
		if (SUCCEEDED(hr))
		{
			Init(pControl,pRect,bSupportMultThread);
		}
		return SUCCEEDED(hr);
	}

	///////////////////////////////////////////////////////////////////////////////
	// IUnknown
	HRESULT DMAxContainer::QueryInterface(REFIID iid, void** object)
	{
		HRESULT hr = S_OK;
		*object = NULL;
		if (iid == IID_IUnknown) {
			// Avoid ambiguous resolution of IUnknown
			*object = static_cast<IUnknown*>(static_cast<MinimumIDispatchImpl*>(this));
		} else if (iid == IID_IDispatch) {
			*object = static_cast<MinimumIDispatchImpl*>(this);
		} else if (iid == IID_IOleContainer) {
			*object = static_cast<IOleContainer*>(this);
		} else if (iid == IID_IParseDisplayName) {
			*object = static_cast<IParseDisplayName*>(this);
		} else if (iid == IID_IBindHost) {
			*object = static_cast<IBindHost*>(this);
		} else if (iid == IID_IServiceProvider) {
			*object = static_cast<IServiceProvider*>(this);
		} else {
			hr=    DMActiveXSite::QueryInterface2(iid,object);
		}
		if ( SUCCEEDED(hr) ) {
			static_cast<IUnknown*>(*object)->AddRef();
		}
		return hr;
	}

	ULONG STDMETHODCALLTYPE DMAxContainer::AddRef()
	{
		return ::InterlockedIncrement(&m_lRefCnt);
	}

	ULONG STDMETHODCALLTYPE DMAxContainer::Release()
	{
		// We don't destroy object when it reaches 0
		return ::InterlockedDecrement(&m_lRefCnt);
	}

	// IBindHost
	HRESULT DMAxContainer::CreateMoniker(LPOLESTR szName, IBindCtx* /*pBC*/,
		IMoniker** ppmk, DWORD /*dwReserved*/)
	{
		HRESULT hr;
		typedef HRESULT (WINAPI* pfnCreateURLMonikerEx)(IMoniker *pMkCtx,LPCWSTR szURL,IMoniker **ppmk,DWORD dwFlags);
		HMODULE urlmon = ::GetModuleHandleA("urlmon.dll");
		if (!urlmon)
		{
			urlmon = ::LoadLibraryA("urlmon.dll");
		}
		pfnCreateURLMonikerEx OrigCreateURLMonikerEx = (pfnCreateURLMonikerEx)::GetProcAddress(urlmon,"CreateURLMonikerEx");
		hr = OrigCreateURLMonikerEx(NULL, szName, ppmk, URL_MK_UNIFORM);
		return hr;
	}

	HRESULT DMAxContainer::MonikerBindToStorage(IMoniker *pMk, IBindCtx *pBC,
		IBindStatusCallback *pBSC, REFIID riid, void **ppvObj)
	{
		HRESULT hr = E_FAIL;
		DMComPtr<IBindCtx> pBCCtx = pBC;
		if (pBCCtx == NULL)
		{
			// creates it if not present
			hr = CreateBindCtx(0, &pBCCtx);
		}

		if (pBCCtx != NULL)
		{
			if (pBSC != NULL)
			{
				typedef HRESULT (WINAPI* pfnRegisterBindStatusCallback)( IBindCtx *pbc,IBindStatusCallback *pbsc,IBindStatusCallback **ppbscPrevious,DWORD dwReserved);
				HMODULE urlmon = ::GetModuleHandleA("urlmon.dll");
				if (!urlmon)
					urlmon = ::LoadLibraryA("urlmon.dll");
				pfnRegisterBindStatusCallback OrigRegisterBindStatusCallback = (pfnRegisterBindStatusCallback)::GetProcAddress(urlmon,"RegisterBindStatusCallback");
				hr = OrigRegisterBindStatusCallback(pBCCtx, pBSC, NULL, 0);
			}
			hr = pMk->BindToStorage(pBCCtx, NULL, riid, ppvObj);
		}
		return hr;
	}

	HRESULT DMAxContainer::MonikerBindToObject(IMoniker* /*pMk*/, IBindCtx* /*pBC*/,
		IBindStatusCallback* /*pBSC*/, REFIID /*riid*/, void** /*ppvObj*/)
	{
		DMTRACENOTIMPL(_T("ActiveXContainerImpl::MonikerBindToObject\n"));
	}

	HRESULT DMAxContainer::QueryService(REFGUID guidService, REFIID riid, void **ppvObject)
	{
		HRESULT hr = E_FAIL;
		if (guidService == IID_IBindHost ||
			guidService == GUID_IWebBrowserApp ||
			guidService == GUID_IXcpControlHost)
		{
			hr = QueryInterface(riid, ppvObject);
		}
		return hr;
	}

	// DMAxContainerImpl----------------------------------
	DMAxContainerImpl::DMAxContainerImpl(DUIWindow *pOwner):m_pOwner(pOwner)
	{
		SetAxHost(this);
	}

	HWND DMAxContainerImpl::GetAxHostWindow() const
	{
		return m_pOwner->GetContainer()->OnGetHWnd();
	}

	void DMAxContainerImpl::OnAxActivate(IUnknown *pCtrl)
	{
		m_pOwner->DV_OnAxActivate(pCtrl);
	}

	void DMAxContainerImpl::OnAxInvalidate(LPCRECT pRect,BOOL bErase)
	{
		CRect rc = pRect;
		m_pOwner->DM_InvalidateRect(rc);// Ë¢ÐÂ´¦
	}

	void DMAxContainerImpl::OnAxSetCapture(BOOL fCapture)
	{
		if (fCapture)
		{
			m_pOwner->DM_SetCapture();
		}
		else
		{
			m_pOwner->DM_ReleaseCapture();
		}
	}

	HRESULT DMAxContainerImpl::OnAxGetDC(LPCRECT pRect, DWORD grfFlags, HDC *phDC)
	{
		// NOTE(elaurentin): Control wants an offscreen DC (assuming pRect == NULL or pRect == m_rcPos).
		// I haven't seen a control doing this. Few container would support OFFSCREEN DC.
		// The only reason I'm supporting this is to let "possible" control to repaint without telling the container.

		return S_FALSE;
	}

	HRESULT DMAxContainerImpl::OnAxReleaseDC(HDC hdc)
	{
		return S_FALSE;
	}

}//namespace DM