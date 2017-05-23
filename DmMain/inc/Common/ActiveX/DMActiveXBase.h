//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:http://www.codeproject.com/Articles/33516/ATL-Windowless-ActiveX-Media-Container
// http://blog.csdn.net/cparent/article/details/9160657
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2014-11-17	1.0			
//--------------------------------------------------------
#pragma once
#include "DMActiveXHelper.h"

namespace DM
{
	// 多线程使用
	class GITControl
	{
	public:
		GITControl(IUnknown *pUnknwn = NULL)
		{
			m_dwGitCookie		   = 0;
			m_pUnknown			   = NULL;
			m_git_dwViewObjectType = 0;
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			SetPtr(spGit,pUnknwn);
		}
		bool SetPtr(DMComPtr<IGlobalInterfaceTable>spGit, IUnknown *pUnknwn)
		{
			DMAutoLock autolock(&m_Lock);
			bool bRet = false;
			do 
			{
				if (!spGit)
				{
					break;
				}
				if (m_dwGitCookie)
				{
					spGit->RevokeInterfaceFromGlobal(m_dwGitCookie);
					m_dwGitCookie = 0;
				}
				if (S_OK != spGit->RegisterInterfaceInGlobal(pUnknwn, __uuidof(IUnknown), &m_dwGitCookie))
				{
					break;
				}
				m_pUnknown = pUnknwn;
				bRet = true;
			} while (false);
			return bRet;
		}
		void ClearPtr()
		{
			DMAutoLock autolock(&m_Lock);
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			if (spGit)
			{
				spGit->RevokeInterfaceFromGlobal(m_dwGitCookie);
				m_dwGitCookie = 0;
				m_pUnknown = NULL;
			}
		}
		~GITControl()
		{
			ClearPtr();
		}
		DMComPtr<IUnknown> spControl()
		{
			DMAutoLock autolock(&m_Lock);
			DMComPtr<IUnknown> ptr;
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			if (spGit && S_OK != spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IUnknown), reinterpret_cast<void**>(&ptr)))
			{
				SetPtr(spGit,m_pUnknown);
				spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IUnknown), reinterpret_cast<void**>(&ptr));
			}
			return ptr;
		}
		DMComQIPtr<IOleObject> spOleObject()
		{
			DMAutoLock autolock(&m_Lock);
			DMComQIPtr<IOleObject> ptr;
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			if (spGit && S_OK != spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleObject), reinterpret_cast<void**>(&ptr)))
			{
				SetPtr(spGit,m_pUnknown);
				spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleObject), reinterpret_cast<void**>(&ptr));
			}
			return ptr;
		}
		DMComQIPtr<IOleInPlaceObject> spInPlaceObject()
		{
			DMAutoLock autolock(&m_Lock);
			DMComQIPtr<IOleInPlaceObject> ptr;
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			if (spGit && S_OK != spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleInPlaceObject), reinterpret_cast<void**>(&ptr)))
			{
				SetPtr(spGit,m_pUnknown);
				spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleInPlaceObject), reinterpret_cast<void**>(&ptr));
			}
			return ptr;
		}
		DMComPtr<IViewObjectEx> spViewObject()
		{
			DMAutoLock autolock(&m_Lock);
			DMComPtr<IViewObjectEx> ptr;
			HRESULT hr = S_FALSE;
			do 
			{ 
				DMComPtr<IGlobalInterfaceTable> spGit = GIT();
				if (NULL == spGit)
				{
					break;
				}
				hr = spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IViewObjectEx), reinterpret_cast<void**>(&ptr));
				if (FAILED(hr))
				{
					hr = spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IViewObject2), reinterpret_cast<void**>(&ptr));
					if (SUCCEEDED(hr))
					{
						m_git_dwViewObjectType = 3;
					}
				}
				else
				{
					m_git_dwViewObjectType = 7;
				}
				if (FAILED(hr))
				{
					hr = spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IViewObject), reinterpret_cast<void**>(&ptr));
					if (SUCCEEDED(hr))
					{
						m_git_dwViewObjectType = 1;
					}
				}
			} while (false);
			return ptr;
		}
		DMComQIPtr<IOleInPlaceObjectWindowless> spOleObjectWindowless()
		{
			DMAutoLock autolock(&m_Lock);
			DMComQIPtr<IOleInPlaceObjectWindowless> ptr;
			DMComPtr<IGlobalInterfaceTable> spGit = GIT();
			if (spGit && S_OK != spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleInPlaceObjectWindowless), (void**)&ptr))
			{
				SetPtr(spGit,m_pUnknown);
				spGit->GetInterfaceFromGlobal(m_dwGitCookie, __uuidof(IOleInPlaceObjectWindowless), (void**)&ptr);
			}
			return ptr;
		}

	public:
		DMComPtr<IGlobalInterfaceTable> GIT()
		{
			DMComPtr<IGlobalInterfaceTable> spGit;
			spGit.CoCreateInstance(CLSID_StdGlobalInterfaceTable);
			return spGit;
		}

	public:
		DWORD								   m_git_dwViewObjectType;
		DMComPtr<IUnknown>					   m_pUnknown;
		DWORD								   m_dwGitCookie;					///< 记录GIT
		DMLock                                 m_Lock;
	};

#define  m_spGitControl					 m_pGIT->spControl()  
#define  m_spGitOleObject				 m_pGIT->spOleObject()
#define  m_spGitInPlaceObject			 m_pGIT->spInPlaceObject()
#define  m_spGitViewObject				 m_pGIT->spViewObject()
#define  m_spGitOleObjectWindowless		 m_pGIT->spOleObjectWindowless()
#define  m_dwGitViewObjectType			 m_pGIT->m_git_dwViewObjectType

#define  GIT_TEMP \
	DMComPtr<IUnknown> spControl = m_spGitControl; \
	DMComPtr<IViewObjectEx> spViewObject = m_spGitViewObject; \
	DMComQIPtr<IOleObject> spOleObject = m_spGitOleObject; \
	DMComQIPtr<IOleInPlaceObject> spInPlaceObject = m_spGitInPlaceObject; \
	DMComQIPtr<IOleInPlaceObjectWindowless> spOleObjectWindowless = m_spGitOleObjectWindowless;

	// IAxHostDelegate---------------------------------------
	class IAxHostDelegate
	{
	public:
		virtual HWND GetAxHostWindow() const								= 0;
		virtual void OnAxActivate(IUnknown *pCtrl)							= 0;
		virtual void OnAxInvalidate(LPCRECT pRect,BOOL bErase)				= 0;
		virtual void OnAxSetCapture(BOOL fCapture)							= 0;
		virtual HRESULT OnAxGetDC(LPCRECT pRect, DWORD grfFlags, HDC *phDC) = 0;
		virtual HRESULT OnAxReleaseDC(HDC hdc)=0;
	};

	template <class Base> 
	class NoRefIUnknownImpl : public Base
	{
	public:
		~NoRefIUnknownImpl() {Base::FinalRelease();}
		// IUnknown
		virtual ULONG STDMETHODCALLTYPE AddRef() { return 1; }
		virtual ULONG STDMETHODCALLTYPE Release() { return 0; }
	};

	// MinimumIDispatchImpl
	class MinimumIDispatchImpl : public IDispatch 
	{
	public:
		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo){*pctinfo = 0;return S_OK;}
		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo){return E_NOTIMPL;}
		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId){return E_NOTIMPL;}
		STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr){return E_NOTIMPL;}
	};


	template<class T>
	class DM_NO_VTABLE DMActiveXSite:public IOleClientSite,
									public IOleControlSite,
									public IOleInPlaceSiteWindowless,
									public IAdviseSink
	{
		friend T;
	public:
		DMActiveXSite()
		:m_pAxHostDelegate(NULL)
		,m_bSupportMultThread(false)// 默认不支持多线程
		,m_bAllowResize(true)// 默认支持重设大小
		,m_bVisible(true)
		,m_bInplaceActive(false)
		,m_dwMiscStatus(0)
		,m_dwOleObjSink(0)
		,m_dwViewObjectType(0)
		,m_grfFlags(0)
		,m_bCaptured(FALSE)
		,m_bFocused(FALSE)
		{
			memset(&m_rcPos, 0, sizeof(m_rcPos));
		}

		virtual ~DMActiveXSite()
		{
			Clear();
		}

		DMComPtr<IUnknown> ActiveXControl()
		{
			if (m_bSupportMultThread)
			{
				DMComPtr<IUnknown> spControl = m_spGitControl;
				return spControl;
			}
			else
			{
				return m_spControl;
			}
		}

		void SetVisible(bool bShow)
		{
			m_bVisible = bShow;
		}

		void SetAxHost(IAxHostDelegate *pAxHost)
		{
			m_pAxHostDelegate = pAxHost;
		}

		void SetExternalUIHandler(IDocHostUIHandler *pUiHandler)
		{
			m_spDocHostUIHandler = pUiHandler;
		}

		void Init(IUnknown* pControl,LPCRECT pRect = NULL,bool bSupportMultThread=false)
		{
			m_bSupportMultThread = bSupportMultThread;
			if (m_bSupportMultThread)
			{
				m_pGIT->SetPtr(m_pGIT->GIT(),pControl);
			}
			else
			{
				m_spControl		  = pControl;
				m_spOleObject     = pControl;
				m_spInPlaceObject = pControl;
			}
		}

		void Clear()
		{
			if (m_bSupportMultThread)
			{
				GIT_TEMP
				if (spControl != NULL)
				{
					if (spViewObject != NULL )
					{
						spViewObject->SetAdvise(DVASPECT_CONTENT, 0, NULL);
					}
					if (spInPlaceObject != NULL)
					{
						if (m_bInplaceActive)
						{
							spInPlaceObject->InPlaceDeactivate();
							m_bInplaceActive = false;
						}
					}
					if (spOleObject != NULL)
					{
						spOleObject->Unadvise(m_dwOleObjSink);
						spOleObject->SetClientSite(NULL);
						spOleObject->Close(OLECLOSE_NOSAVE);
					}
					m_pGIT->ClearPtr();
				}
			}
			else
			{
				if (m_spControl != NULL)
				{
					if ( m_spViewObject != NULL )
					{
						m_spViewObject->SetAdvise(DVASPECT_CONTENT, 0, NULL);
						m_spViewObject.Release();
					}
					m_spOleObjectWindowless.Release();
					if (m_spInPlaceObject != NULL)
					{
						if (m_bInplaceActive)
						{
							m_spInPlaceObject->InPlaceDeactivate();
							m_bInplaceActive = false;
						}
						m_spInPlaceObject.Release();
					}
					if (m_spOleObject != NULL)
					{
						m_spOleObject->Unadvise(m_dwOleObjSink);
						m_spOleObject->SetClientSite(NULL);
						m_spOleObject->Close(OLECLOSE_NOSAVE);
						m_spOleObject.Release();
					}
					m_spControl = NULL;
				}
			}
		}

		HRESULT InitControl(IStream* pStream = NULL)
		{
			HRESULT hr = E_POINTER;
			do 
			{
				if (m_bSupportMultThread)
				{
					DMComPtr<IUnknown> spControl = m_spGitControl;
					if (0 == spControl)
					{
						hr = E_NOINTERFACE;
						break;
					}

					DMComQIPtr<IPersistStreamInit> spPSI = spControl;
					if (spPSI != NULL)
					{
						if (pStream != NULL)
						{
							hr = spPSI->Load( pStream );
						}
						else
						{
							hr = spPSI->InitNew();
						}
					}
					else if (pStream)
					{
						DMComQIPtr<IPersistStream> spPS(m_spGitOleObject);
						if (spPS)
						{
							hr = spPS->Load(pStream);
						}
					}
				}
				else
				{
					if (0 == m_spControl)
					{
						hr = E_NOINTERFACE;
						break;
					}

					DMComQIPtr<IPersistStreamInit> spPSI = m_spControl;
					if (spPSI != NULL)
					{
						if (pStream != NULL)
						{
							hr = spPSI->Load( pStream );
						}
						else
						{
							hr = spPSI->InitNew();
						}
					}
					else if (pStream)
					{
						DMComQIPtr<IPersistStream> spPS(m_spOleObject);
						if (spPS)
						{
							hr = spPS->Load(pStream);
						}
					}
				}
			} while (false);
			return hr;
		}

		HRESULT ActivateAx(IStream* pStream = NULL)
		{
			HRESULT hr = E_FAIL;
			do 
			{
				if (m_bSupportMultThread)
				{
					DMComQIPtr<IOleObject> spOleObject = m_spGitOleObject; 
					if (0 == spOleObject)
					{
						hr = E_UNEXPECTED;
						break;
					}

					// this must be set even before calling IPersistStreamInit::InitNew
					hr = spOleObject->GetMiscStatus(DVASPECT_CONTENT, &m_dwMiscStatus);

					if (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) 
					{
						hr = spOleObject->SetClientSite(static_cast<IOleClientSite*>(this));
						DMComQIPtr<IOleWindow> ole_window = m_spGitControl;
						HWND hWnd = NULL;
						ole_window->GetWindow(&hWnd);

						if (FAILED(hr)) 
						{
							break;
						}
					}

					hr = InitControl(pStream);
					if (FAILED(hr))
					{
						if (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST)
						{
							spOleObject->SetClientSite(NULL);
						}
						break;
					}
					if (0 == (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST)) 
					{
						hr = spOleObject->SetClientSite(static_cast<IOleClientSite*>(this));
						if (FAILED(hr))
						{
							break;
						}
					}
				}
				else
				{
					if (0 == m_spOleObject)
					{
						hr = E_UNEXPECTED;
						break;
					}

					// this must be set even before calling IPersistStreamInit::InitNew
					hr = m_spOleObject->GetMiscStatus(DVASPECT_CONTENT, &m_dwMiscStatus);

					if (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST) 
					{
						hr = m_spOleObject->SetClientSite(static_cast<IOleClientSite*>(this));
						DMComQIPtr<IOleWindow> ole_window = m_spControl;
						HWND hWnd = NULL;
						ole_window->GetWindow(&hWnd);

						if (FAILED(hr)) 
						{
							break;
						}
					}

					hr = InitControl(pStream);
					if (FAILED(hr))
					{
						if (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST)
						{
							m_spOleObject->SetClientSite(NULL);
						}
						break;
					}
					if (0 == (m_dwMiscStatus & OLEMISC_SETCLIENTSITEFIRST)) 
					{
						hr = m_spOleObject->SetClientSite(static_cast<IOleClientSite*>(this));
						if (FAILED(hr))
						{
							break;
						}
					}
				}
				hr = DoInplaceActivate();
			} while (false);
			return hr;
		}

		bool HitTest(const POINT& pt) const
		{
			bool bRet = false;
			do 
			{
				if (m_bSupportMultThread)
				{
					DMComPtr<IViewObjectEx> spViewObject = m_spGitViewObject;
					if (NULL != spViewObject
						&&7 == m_dwGitViewObjectType) 
					{
						DWORD dwHitResult = HITRESULT_OUTSIDE;
						spViewObject->QueryHitPoint(DVASPECT_CONTENT, &m_rcPos, pt, 0, &dwHitResult);
						bRet = (dwHitResult == HITRESULT_HIT);
						break;
					}
				}
				else
				{
					if (NULL != m_spViewObject
						&&7 == m_dwViewObjectType) 
					{
						DWORD dwHitResult = HITRESULT_OUTSIDE;
						m_spViewObject->QueryHitPoint(DVASPECT_CONTENT, &m_rcPos, pt, 0, &dwHitResult);
						bRet = (dwHitResult == HITRESULT_HIT);
						break;
					}
				}
				bRet = (FALSE != ::PtInRect(&m_rcPos, pt));
			} while (false);
			return bRet;
		}

		bool InsideClippingRegion(const RECT& rcClip) const
		{
			// true when top/let or bottom/right corners intersect into client area
			const POINT* pts = (const POINT*)(&m_rcPos);
			const POINT* ptc = (const POINT*)(&rcClip);
			return (::PtInRect(&rcClip, pts[0])||::PtInRect(&rcClip, pts[1]) 
				||::PtInRect(&m_rcPos,ptc[0])||::PtInRect(&m_rcPos, ptc[1]));
		}

		HRESULT DoVerb(long verb)
		{
			HRESULT hr = E_FAIL;
			do 
			{
				if (0!=(m_dwMiscStatus&(OLEMISC_INVISIBLEATRUNTIME|OLEMISC_NOUIACTIVATE)))
				{
					hr = S_FALSE;
					break;
				}

				if (m_bSupportMultThread)
				{
					DMComQIPtr<IOleObject> spOleObject = m_spGitOleObject;
					if (spOleObject != NULL)
					{
						HWND hWnd = m_pAxHostDelegate->GetAxHostWindow();
						if (FALSE==::IsWindow(hWnd))
						{
							LOG_ERR("[mid]hgy:窗口没初始化时不要初始化activeX,已被坑一次了！！！！！！！！！！！！！！");
							hr = S_FALSE;
							break;
						}
						hr = spOleObject->DoVerb(verb, NULL, static_cast<IOleClientSite*>(this), 0, hWnd, &m_rcPos);
						if (verb == OLEIVERB_INPLACEACTIVATE &&SUCCEEDED(hr))
						{
							m_bInplaceActive = true;
						} 
						else 
						{
							hr = E_UNEXPECTED;
						}
					}
				}
				else
				{
					if (m_spOleObject != NULL)
					{
						HWND hWnd = m_pAxHostDelegate->GetAxHostWindow();
						if (FALSE==::IsWindow(hWnd))
						{
							LOG_ERR("[mid]hgy:窗口没初始化时不要初始化activeX,已被坑一次了！！！！！！！！！！！！！！");
							hr = S_FALSE;
							break;
						}
						hr = m_spOleObject->DoVerb(verb, NULL, static_cast<IOleClientSite*>(this), 0, hWnd, &m_rcPos);
						if (verb == OLEIVERB_INPLACEACTIVATE &&SUCCEEDED(hr))
						{
							m_bInplaceActive = true;
						} 
						else 
						{
							hr = E_UNEXPECTED;
						}
					}
				}
			} while (false);
			return hr;
		}

		LRESULT ProcessPaint(HDC hDC, LPCRECT lpClipRect,bool bIe=false)
		{
			LRESULT lResult = S_FALSE;
			do 
			{
				if (lpClipRect
					&&!InsideClippingRegion(*lpClipRect)) 
				{
					break;
				}

				if (m_bSupportMultThread)
				{
					DMComPtr<IViewObjectEx> spViewObject = m_spGitViewObject; 
					if (m_bVisible 
						&&NULL != spViewObject) 
					{
						if (bIe)
						{
							RECTL rcPos = {0,0,m_rcPos.right-m_rcPos.left,m_rcPos.bottom-m_rcPos.top};
							lResult = spViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC,&rcPos, NULL, NULL, 0);
						}
						else
						{
							RECTL rcPos = {m_rcPos.left,m_rcPos.top,m_rcPos.right,m_rcPos.bottom};
							lResult = spViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC,&rcPos, NULL, NULL, 0);
						}
					}
				}
				else
				{
					if (m_bVisible 
						&&NULL != m_spViewObject) 
					{
						if (bIe)
						{
							RECTL rcPos = {0,0,m_rcPos.right-m_rcPos.left,m_rcPos.bottom-m_rcPos.top};
							lResult = m_spViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC,&rcPos, NULL, NULL, 0);
						}
						else
						{
							RECTL rcPos = {m_rcPos.left,m_rcPos.top,m_rcPos.right,m_rcPos.bottom};
							lResult = m_spViewObject->Draw(DVASPECT_CONTENT, -1, NULL, NULL, NULL, hDC,&rcPos, NULL, NULL, 0);
						}
					}
				}
			} while (false);
			return lResult;
		}

		HRESULT FireAmbientPropertyChange(DISPID dispChanged)
		{
			HRESULT hr = S_OK;
			if (m_bSupportMultThread)
			{
				DMComQIPtr<IOleControl, &__uuidof(IOleControl)> spOleControl(m_spGitControl);
				if (NULL != spOleControl)
				{
					hr = spOleControl->OnAmbientPropertyChange(dispChanged);
				}
			}
			else
			{
				DMComQIPtr<IOleControl, &__uuidof(IOleControl)> spOleControl(m_spControl);
				if (NULL != spOleControl)
				{
					hr = spOleControl->OnAmbientPropertyChange(dispChanged);
				}
			}
		
			return hr;
		}

		LRESULT OnWindowMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
		{
			LRESULT lResult = 0;
			if (m_bSupportMultThread)
			{
				DMComQIPtr<IOleInPlaceObjectWindowless> spOleObjectWindowless = m_spGitOleObjectWindowless;
				if (m_bInplaceActive && spOleObjectWindowless != NULL)
				{
					spOleObjectWindowless->OnWindowMessage(uMsg, wParam, lParam, &lResult);
				}
			}
			else
			{
				if (m_bInplaceActive && m_spOleObjectWindowless != NULL)
				{
					m_spOleObjectWindowless->OnWindowMessage(uMsg, wParam, lParam, &lResult);
				}
			}
		
			return lResult;
		}

		bool ProcessEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT* plResult)
		{
			LRESULT lResult = -1L;
			switch( uMsg )
			{
			case WM_PAINT:
				{
					lResult = ProcessPaint( reinterpret_cast<HDC>(wParam), reinterpret_cast<LPRECT>(lParam));
				}
				break;
			case WM_LBUTTONDOWN:
			case WM_MBUTTONDOWN:
			case WM_RBUTTONDOWN:
			case WM_LBUTTONUP:
			case WM_MBUTTONUP:
			case WM_RBUTTONUP:
			case WM_LBUTTONDBLCLK:
			case WM_MBUTTONDBLCLK:
			case WM_RBUTTONDBLCLK:
			case WM_MOUSEMOVE:
			case WM_KEYUP:
			case WM_KEYDOWN:
			case WM_CAPTURECHANGED:
			case WM_CANCELMODE:
			case WM_SETCURSOR:
			case WM_SETFOCUS:  // internal use
			case WM_KILLFOCUS: // internal use
				{
					BOOL bHandled = TRUE;
					lResult = OnWindowMessage(uMsg, wParam, lParam, bHandled);
				}
				break;
			default: // unsupported!
				break;
			}
			if (plResult)
			{
				*plResult = lResult;
			}
			return (lResult != -1L);
		}

		HRESULT SetRect(const RECT* lpRect)
		{
			HRESULT hr = S_FALSE;
			if (!::EqualRect(&m_rcPos, lpRect))
			{
				memcpy(&m_rcPos, lpRect, sizeof(m_rcPos));
				if (m_bSupportMultThread)
				{
					DMComQIPtr<IOleInPlaceObject> spInPlaceObject = m_spGitInPlaceObject;
					if (spInPlaceObject != NULL) 
					{
						hr = spInPlaceObject->SetObjectRects(lpRect, lpRect);
					}
				}
				else
				{
					if (m_spInPlaceObject != NULL) 
					{
						hr = m_spInPlaceObject->SetObjectRects(lpRect, lpRect);
					}
				}
				hr = SetExtent(m_rcPos.right - m_rcPos.left, m_rcPos.bottom - m_rcPos.top);
			}
			return hr;
		}

		// ----------------------------------------



		STDMETHOD(QueryInterface2)(REFIID iid, void** object)
		{
			HRESULT hr = S_OK;
			*object = NULL;
			if (iid == IID_IOleClientSite) {
				*object = static_cast<IOleClientSite*>(this);
			} else if (iid == IID_IOleControlSite) {
				*object = static_cast<IOleControlSite*>(this);
			} else if (iid == IID_IOleInPlaceSite) {
				*object = static_cast<IOleInPlaceSite*>(this);
			} else if (iid == IID_IOleInPlaceSiteEx) {
				*object = static_cast<IOleInPlaceSiteEx*>(this);
			} else if (iid == IID_IOleInPlaceSiteWindowless) {
				*object = static_cast<IOleInPlaceSiteWindowless*>(this);
			} else if (iid == IID_IAdviseSink ) {
				*object = static_cast<IAdviseSink*>(this);
			} else if( iid == IID_IDocHostUIHandler && m_spDocHostUIHandler){
				*object = m_spDocHostUIHandler;
			} else {
				hr = E_NOINTERFACE;
			}
			return hr;
		}

		STDMETHOD(SaveObject)(void)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::SaveObject\n");
		}

		STDMETHOD(GetMoniker)(DWORD /*dwAssign*/, DWORD dwWhichMoniker, IMoniker** ppmk)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::GetMoniker\n");
		}

		STDMETHOD(GetContainer)(IOleContainer **ppContainer)
		{
			HRESULT hr = E_POINTER;
			if (NULL != ppContainer) 
			{
				hr = static_cast<T*>(this)->QueryInterface(__uuidof(IOleContainer), reinterpret_cast<void**>(ppContainer));
			}
			return hr;
		}

		STDMETHOD(ShowObject)(void)
		{
			HWND hWnd = m_pAxHostDelegate->GetAxHostWindow();
			HRESULT hr = E_FAIL;
			if (::IsWindow(hWnd)&&FALSE==::IsRectEmpty(&m_rcPos))
			{
				m_bVisible = true;
				if (m_bInplaceActive)
				{
					m_pAxHostDelegate->OnAxInvalidate(&m_rcPos,TRUE);
					hr = S_OK;
				}
			}
			return hr;
		}

		STDMETHOD(OnShowWindow)(BOOL fShow)
		{
			m_bVisible = (FALSE!=fShow);
			return S_OK;
		}

		STDMETHOD(RequestNewObjectLayout)(void)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::RequestNewObjectLayout\n");
		}

		// IOleControlSite
		STDMETHOD(OnControlInfoChanged)(void)
		{
			return S_OK;
		}

		STDMETHOD(LockInPlaceActive)(BOOL /*fLock*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::LockInPlaceActive\n");
		}

		STDMETHOD(GetExtendedControl)(IDispatch**)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::GetExtendedControl\n");
		}

		STDMETHOD(TransformCoords)(POINTL* /*pPtlHimetric*/, POINTF* /*pPtfContainer*/, DWORD /*dwFlags*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::TransformCoords\n");
		}

		STDMETHOD(TranslateAccelerator)(MSG* /*pMsg*/, DWORD /*grfModifiers*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::TranslateAccelerator\n");
		}

		STDMETHOD(OnFocus)(BOOL fGotFocus)
		{
			// ignore this, controls should use SetFocus(flag) instead
			return S_OK;
		}

		STDMETHOD(ShowPropertyFrame)(void)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::ShowPropertyFrame\n");
		}

		// IOleInPlaceSiteWindowless----------------------------------------------
		STDMETHOD(CanWindowlessActivate)(void)
		{
			// support Windowless activation
			return S_OK;
		}

		STDMETHOD(GetCapture)(void)
		{
			HRESULT hr = m_bCaptured?S_OK:S_FALSE;
			return hr;
		}

		STDMETHOD(SetCapture)(BOOL fCapture)
		{
			if (fCapture!=m_bCaptured)
			{
				m_bCaptured=fCapture;
				m_pAxHostDelegate->OnAxSetCapture(fCapture);
			}
			return S_OK;
		}

		STDMETHOD(GetFocus)(void)
		{
			return m_bFocused?S_OK: S_FALSE;
		}

		STDMETHOD(SetFocus)(BOOL fFocus)
		{
			if(m_bFocused!=fFocus)
			{
				m_bFocused=fFocus;
				InvalidateRect(NULL,true);
			}

			return S_OK;
		}

		STDMETHOD(GetDC)(LPCRECT pRect, DWORD grfFlags, HDC *phDC)
		{
			return m_pAxHostDelegate->OnAxGetDC(pRect,grfFlags,phDC);
		}

		STDMETHOD(ReleaseDC)(HDC hDC)
		{
			return m_pAxHostDelegate->OnAxReleaseDC(hDC);
		}

		STDMETHOD(InvalidateRect)(LPCRECT pRect, BOOL fErase)
		{
			// Blindly honor the specified region
			// well behaving control should pass a valid intersect rect or NULL
			if (NULL == pRect) 
			{
				pRect = &m_rcPos;
			}
			m_pAxHostDelegate->OnAxInvalidate(pRect,fErase);// 此处刷新
			return S_OK;
		}

		STDMETHOD(InvalidateRgn)(HRGN hRGN, BOOL fErase)
		{
			UNREFERENCED_PARAMETER(hRGN);
			return InvalidateRect(NULL, fErase);
		}

		STDMETHOD(ScrollRect)(INT /*dx*/, INT /*dy*/, LPCRECT /*pRectScroll*/, LPCRECT /*pRectClip*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::ScrollRect\n");
		}

		STDMETHOD(AdjustRect)(LPRECT /*prc*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::AdjustRect\n");
		}

		STDMETHOD(OnDefWindowMessage)(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT *plResult)
		{
			HRESULT hr = S_OK;
			LRESULT lResult;
			// NOTE(elaurentin): typically, windowless control shouldn't need this
			lResult = DefWindowProc(m_pAxHostDelegate->GetAxHostWindow(), msg, wParam, lParam);
			if (NULL != plResult) 
			{
				*plResult = lResult;
			}
			return hr;
		}

		// IOleInPlaceSiteEx
		STDMETHOD(OnInPlaceActivateEx)(BOOL *pfNoRedraw, DWORD dwFlags)
		{
			HRESULT hr = S_FALSE;
			do 
			{
				if (m_bSupportMultThread)
				{
					OleLockRunning(m_spGitOleObject, TRUE, FALSE);
					if (pfNoRedraw)
					{
						*pfNoRedraw = FALSE;
					}
					m_spGitInPlaceObject = m_spGitControl;
					if (0!=(dwFlags & ACTIVATE_WINDOWLESS) != 0) 
					{
						m_spGitOleObjectWindowless = m_spGitControl;
						if (NULL != m_spGitOleObjectWindowless) 
						{
							m_bInplaceActive = true;
							hr = S_OK;
						}
					} 
					else 
					{
						// object is not windowless, ensure we won't dispatch message to this control
						m_bInplaceActive = false;
					}
				}
				else
				{
					OleLockRunning(m_spOleObject, TRUE, FALSE);
					if (pfNoRedraw)
					{
						*pfNoRedraw = FALSE;
					}
					m_spInPlaceObject = m_spControl;
					if (0!=(dwFlags & ACTIVATE_WINDOWLESS) != 0) 
					{
						m_spOleObjectWindowless = m_spControl;
						if (NULL != m_spOleObjectWindowless) 
						{
							m_bInplaceActive = true;
							hr = S_OK;
						}
					} 
					else 
					{
						// object is not windowless, ensure we won't dispatch message to this control
						m_bInplaceActive = false;
					}
				}
				
			} while (false);
			return hr;
		}

		STDMETHOD(OnInPlaceDeactivateEx)(BOOL /*fNoRedraw*/)
		{
			m_bInplaceActive = false;
			return S_OK;
		}

		STDMETHOD(RequestUIActivate)(void)
		{
			return S_OK;
		}

		// IOleInPlaceSite
		STDMETHOD(CanInPlaceActivate)(void)
		{
			return S_OK;
		}

		STDMETHOD(OnInPlaceActivate)(void)
		{
			return S_OK;
		}

		STDMETHOD(OnUIActivate)(void)
		{
			return S_OK;
		}

		STDMETHOD(GetWindowContext)(IOleInPlaceFrame **ppFrame,
									IOleInPlaceUIWindow **ppDoc,
									LPRECT lprcPosRect,
									LPRECT lprcClipRect,
									LPOLEINPLACEFRAMEINFO lpFrameInfo)
		{
			if (ppFrame)*ppFrame = NULL;
			if (ppDoc)*ppDoc = NULL;
			if (lprcPosRect) memcpy(lprcPosRect, &m_rcPos, sizeof(m_rcPos));
			if (lprcClipRect)memcpy(lprcClipRect, &m_rcPos, sizeof(m_rcPos));
			if (lpFrameInfo) 
			{
				lpFrameInfo->fMDIApp		= FALSE;
				lpFrameInfo->hwndFrame		= m_pAxHostDelegate->GetAxHostWindow();
				lpFrameInfo->haccel			= NULL;
				lpFrameInfo->cAccelEntries	= 0;
			}
			return S_OK;
		}

		STDMETHOD(Scroll)(SIZE /*scrollExtant*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::Scroll\n");
		}

		STDMETHOD(OnUIDeactivate)(BOOL /*fUndoable*/)
		{
			return S_OK;
		}

		STDMETHOD(OnInPlaceDeactivate)(void)
		{
			return OnInPlaceDeactivateEx(TRUE);
		}

		STDMETHOD(DiscardUndoState)(void)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::DiscardUndoState");
		}

		STDMETHOD(DeactivateAndUndo)(void)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::DeactivateAndUndo");
		}

		STDMETHOD(OnPosRectChange)(LPCRECT lprcPosRect)
		{
			HRESULT hr = E_UNEXPECTED;
			if (m_bAllowResize)
			{
				hr = S_OK;
				if (lprcPosRect && memcmp(&m_rcPos, lprcPosRect, sizeof(m_rcPos)) != 0 )
				{
					if (m_bSupportMultThread)
					{
						if (m_spGitInPlaceObject)
						{
							memcpy(&m_rcPos, lprcPosRect, sizeof(m_rcPos));
							hr = m_spGitInPlaceObject->SetObjectRects(&m_rcPos, &m_rcPos);
						}
					}
					else
					{
						if (m_spInPlaceObject)
						{
							memcpy(&m_rcPos, lprcPosRect, sizeof(m_rcPos));
							hr = m_spInPlaceObject->SetObjectRects(&m_rcPos, &m_rcPos);
						}
					}
				
					if (SUCCEEDED(hr))
					{
						hr = SetExtent(m_rcPos.right - m_rcPos.left, m_rcPos.bottom - m_rcPos.top);
					}
				}
			}
			return hr;
		}

		// IOleWindow
		STDMETHOD(GetWindow)(HWND *phwnd)
		{
			*phwnd=m_pAxHostDelegate->GetAxHostWindow();
			return S_OK;
		}

		STDMETHOD(ContextSensitiveHelp)(BOOL /*fEnterMode*/)
		{
			DMTRACENOTIMPL(L"DMActiveXSite::ContextSensitiveHelp\n");
		}

		// IAdviseSink
		STDMETHOD_(void, OnDataChange)(FORMATETC* /*pFormatetc*/, STGMEDIUM* /*pStgmed*/)
		{
		}

		STDMETHOD_(void, OnViewChange)(DWORD /*dwAspect*/, LONG /*lindex*/)
		{
		}

		STDMETHOD_(void, OnRename)(IMoniker* /*pmk*/)
		{
		}

		STDMETHOD_(void, OnSave)(void)
		{
		}

		STDMETHOD_(void, OnClose)(void)
		{
		}

	protected:
		HRESULT SetExtent(int width, int height)
		{
			HRESULT hr = E_UNEXPECTED;
			if (m_bSupportMultThread)
			{
				if (NULL != m_spGitOleObject)
				{
					SIZEL pxsize = {width, height};
					SIZEL hmsize = {0};
					DMPixelToHiMetric(&pxsize, &hmsize,NULL);

					hr = m_spGitOleObject->SetExtent(DVASPECT_CONTENT, &hmsize);
					hr = m_spGitOleObject->GetExtent(DVASPECT_CONTENT, &hmsize);

					DMHiMetricToPixel(&hmsize, &pxsize,NULL);
					m_rcPos.right = m_rcPos.left + pxsize.cx;
					m_rcPos.bottom = m_rcPos.top + pxsize.cy;
				}
			}
			else
			{
				if (NULL != m_spOleObject)
				{
					SIZEL pxsize = {width, height};
					SIZEL hmsize = {0};
					DMPixelToHiMetric(&pxsize, &hmsize,NULL);
			
					hr = m_spOleObject->SetExtent(DVASPECT_CONTENT, &hmsize);
					hr = m_spOleObject->GetExtent(DVASPECT_CONTENT, &hmsize);

					DMHiMetricToPixel(&hmsize, &pxsize,NULL);
					m_rcPos.right = m_rcPos.left + pxsize.cx;
					m_rcPos.bottom = m_rcPos.top + pxsize.cy;
				}
			}
			return hr;
		}

		HRESULT DoInplaceActivate()
		{
			HRESULT hr;
			if (m_bSupportMultThread)
			{
				m_pAxHostDelegate->OnAxActivate(m_spGitControl);
				m_dwGitViewObjectType = 0;
				if (m_spGitViewObject)
				{
					// todo.
				}
				DMComQIPtr<IAdviseSink> advise_sink = m_spGitControl;
				m_spGitOleObject->Advise(advise_sink, &m_dwOleObjSink);

				if (m_spGitViewObject)
				{
					hr=m_spGitViewObject->SetAdvise(DVASPECT_CONTENT, 0, this);
				}
			}
			else
			{
				m_pAxHostDelegate->OnAxActivate(m_spControl);
				m_dwViewObjectType = 0;
				hr = m_spOleObject->QueryInterface(__uuidof(IViewObjectEx), reinterpret_cast<void**>(&m_spViewObject));
				if (FAILED(hr))
				{
					hr = m_spOleObject->QueryInterface(__uuidof(IViewObject2), reinterpret_cast<void**>(&m_spViewObject));
					if (SUCCEEDED(hr))
					{
						m_dwViewObjectType = 3;
					}
				}
				else 
				{
					m_dwViewObjectType = 7;
				}

				if (FAILED(hr))
				{
					hr = m_spOleObject->QueryInterface(__uuidof(IViewObject), reinterpret_cast<void**>(&m_spViewObject));
					if (SUCCEEDED(hr))
					{
						m_dwViewObjectType = 1;
					}
				}
				DMComQIPtr<IAdviseSink> advise_sink = m_spControl;
				m_spOleObject->Advise(advise_sink, &m_dwOleObjSink);

				if (m_spViewObject)
				{
					hr=m_spViewObject->SetAdvise(DVASPECT_CONTENT, 0, this);
				}

				//hr = SetRect(lpRect);
			}
			// Do Inplace activate if possible
			hr = DoVerb(OLEIVERB_INPLACEACTIVATE);
			return hr;
		}

	public:
		bool                            m_bSupportMultThread;		///< 是否支持多线程
		bool                            m_bAllowResize;
		bool                            m_bVisible;
		bool                            m_bInplaceActive;
		BOOL                            m_bFocused;
		BOOL                            m_bCaptured;
		DWORD                           m_dwMiscStatus;
		DWORD                           m_dwViewObjectType;
		DWORD                           m_dwOleObjSink;
		DWORD                           m_grfFlags;
		RECT                            m_rcPos;
		DMLazyT<GITControl>             m_pGIT;
		DMComPtr<IUnknown>              m_spControl;
		DMComPtr<IViewObjectEx>         m_spViewObject;
		DMComQIPtr<IOleObject>          m_spOleObject;
		DMComQIPtr<IOleInPlaceObject>   m_spInPlaceObject;
		DMComQIPtr<IOleInPlaceObjectWindowless> m_spOleObjectWindowless;
		DMComPtr<IDocHostUIHandler>     m_spDocHostUIHandler;
		IAxHostDelegate                *m_pAxHostDelegate;
		CStringW                        m_strUrl;
	};

	/**
	* ActiveXContainerImpl
	*/
	class DM_NO_VTABLE DMAxContainer : public IOleContainer,
									  public IBindHost,
									  public IServiceProvider,
									  public MinimumIDispatchImpl,
									  public DMActiveXSite<DMAxContainer>
	{
	public:
		DMAxContainer();
		virtual ~DMAxContainer();

		BOOL CreateControl(LPCRECT pRect, REFGUID guid,DWORD dwClsCtx=CLSCTX_INPROC_SERVER, bool bSupportMultThread = false);
		// IOleContainer
		STDMETHOD(EnumObjects)( DWORD /*grfFlags*/, IEnumUnknown** /*ppenum*/)
		{
			DMTRACENOTIMPL(L"ActiveXContainerImpl::EnumObjects\n");
		}

		STDMETHOD(LockContainer)(BOOL /*fLock*/)
		{
			DMTRACENOTIMPL(L"ActiveXContainerImpl::LockContainer\n");
		}

		// IOleContainer::IParseDisplayName
		STDMETHOD(ParseDisplayName)(IBindCtx* /*pbc*/, LPOLESTR /*pszDisplayName*/,
			ULONG* /*pchEaten*/,IMoniker** /*ppmkOut*/)
		{
			DMTRACENOTIMPL(L"ActiveXContainerImpl::ParseDisplayName\n");
		}

		// IBindHost
		STDMETHOD(CreateMoniker)(LPOLESTR szName, IBindCtx* pBC,IMoniker** ppmk, DWORD dwReserved);
		STDMETHOD(MonikerBindToStorage)(IMoniker *pMk, IBindCtx *pBC,IBindStatusCallback *pBSC, REFIID riid, void **ppvObj);
		STDMETHOD(MonikerBindToObject)(IMoniker* pMk, IBindCtx* pBC,IBindStatusCallback* pBSC, REFIID riid, void** ppvObj);

		// IServiceProvider
		STDMETHOD(QueryService)(REFGUID guidService, REFIID riid, void **ppvObject);
		STDMETHOD(QueryInterface)(REFIID iid, void** object);
		virtual ULONG STDMETHODCALLTYPE AddRef();
		virtual ULONG STDMETHODCALLTYPE Release();

	private:
		LONG            m_lRefCnt;
	};


	// DMAxContainerImpl-------------------------------------------------------------------
	class DMAxContainerImpl : public DMAxContainer,public IAxHostDelegate
	{
	public:
		DMAxContainerImpl(DUIWindow *pOwner);

	public:
		virtual HWND GetAxHostWindow() const;
		virtual void OnAxActivate(IUnknown *pCtrl);
		virtual void OnAxInvalidate(LPCRECT pRect,BOOL bErase);
		virtual void OnAxSetCapture(BOOL fCapture);
		virtual HRESULT OnAxGetDC(LPCRECT pRect, DWORD grfFlags, HDC *phDC);
		virtual HRESULT OnAxReleaseDC(HDC hdc);

	protected:
		DUIWindow				*m_pOwner;
	};


}//namespace DM