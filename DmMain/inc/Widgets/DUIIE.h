//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIIE.h 
// File Des:
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-5	1.0			
//--------------------------------------------------------
#pragma once
#include "IDUIWeb.h"
#include "DUIActiveX.h"
#include <ExDisp.h>
#include <ExDispid.h>

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIIE"/>的xml属性定义
	/// </summary>
	class DUIIEAttr
	{
	public:
		static wchar_t* STRING_url;                                    ///< 指定IE的默认加载URL,示例:url="www.baidu.com"
		static wchar_t* bool_bshowscroll;                              ///< 是否显示或隐藏IE滚动条
		static wchar_t* ACCEL_refreshkey;                              ///< 刷新快捷捷,示例:accel="ctrl+f5"
	};
	DMAttrValueInit(DUIIEAttr,STRING_url)DMAttrValueInit(DUIIEAttr,bool_bshowscroll)DMAttrValueInit(DUIIEAttr,ACCEL_refreshkey)
}

namespace DM
{
	class DUIIE;
	class DM_EXPORT DMIEEvtDispatch : public IDispatch
	{
	public:
		DMIEEvtDispatch(IDMWebEvent* pEventHandler);

		void SetWebBrowser(DUIIE  *pWebBrowser);

		void SetEvtHandler(IDMWebEvent* pEventHandler);
		void SetDUIWnd(DUIWND hWnd);

		//IUnkown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);

		//IDispatch
		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo){return E_NOTIMPL;}
		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo){return E_NOTIMPL;}
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId){return E_NOTIMPL;}
		STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr);

	protected:
		ULONG					m_nRef;
		DUIIE					*m_pWebBrowser;
		IDMWebEvent*			m_pEvtHandler;
		DUIWND					m_hWnd;
	};

	class DM_EXPORT DMIEDocHostUIHandler : public IDocHostUIHandler
	{
	public:
		DMIEDocHostUIHandler();

		void SetWebBrowser(DUIIE *pWebBrowser);

		void setScrollBar(bool bHasScrollBar){	m_bHasScrollBar_ = bHasScrollBar;}

		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef( void){return ++m_refCount;};

		virtual ULONG STDMETHODCALLTYPE Release( void){return --m_refCount;};

		virtual HRESULT STDMETHODCALLTYPE ShowContextMenu( 
			/* [in] */ DWORD dwID,
			/* [in] */ POINT *ppt,
			/* [in] */ IUnknown *pcmdtReserved,
			/* [in] */ IDispatch *pdispReserved){return E_NOTIMPL;};

		virtual HRESULT STDMETHODCALLTYPE GetHostInfo( 
			/* [out][in] */ DOCHOSTUIINFO *pInfo);

		virtual HRESULT STDMETHODCALLTYPE ShowUI( 
			/* [in] */ DWORD dwID,
			/* [in] */ IOleInPlaceActiveObject *pActiveObject,
			/* [in] */ IOleCommandTarget *pCommandTarget,
			/* [in] */ IOleInPlaceFrame *pFrame,
			/* [in] */ IOleInPlaceUIWindow *pDoc){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE HideUI( void){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE UpdateUI( void){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE EnableModeless( 
			/* [in] */ BOOL fEnable){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE OnDocWindowActivate( 
			/* [in] */ BOOL fActivate){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE OnFrameWindowActivate( 
			/* [in] */ BOOL fActivate){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE ResizeBorder( 
			/* [in] */ LPCRECT prcBorder,
			/* [in] */ IOleInPlaceUIWindow *pUIWindow,
			/* [in] */ BOOL fRameWindow){return S_OK;}

		virtual HRESULT STDMETHODCALLTYPE TranslateAccelerator( 
			/* [in] */ LPMSG lpMsg,
			/* [in] */ const GUID *pguidCmdGroup,
			/* [in] */ DWORD nCmdID){return S_FALSE;}

		virtual HRESULT STDMETHODCALLTYPE GetOptionKeyPath( 
			/* [out] */ 
			__out  LPOLESTR *pchKey,
			/* [in] */ DWORD dw){return S_FALSE;}

		virtual HRESULT STDMETHODCALLTYPE GetDropTarget( 
			/* [in] */ IDropTarget *pDropTarget,
			/* [out] */ IDropTarget **ppDropTarget){return S_FALSE;}

		virtual HRESULT STDMETHODCALLTYPE GetExternal( 
			/* [out] */ IDispatch **ppDispatch);

		virtual HRESULT STDMETHODCALLTYPE TranslateUrl( 
			/* [in] */ DWORD dwTranslate,
			/* [in] */ 
			__in __nullterminated  OLECHAR *pchURLIn,
			/* [out] */ 
			__out  OLECHAR **ppchURLOut);

		virtual HRESULT STDMETHODCALLTYPE FilterDataObject( 
			/* [in] */ IDataObject *pDO,
			/* [out] */ IDataObject **ppDORet);

	protected:
		ULONG		m_refCount;
		DUIIE		*m_pWebBrowser;
		bool		m_bHasScrollBar_;
	};


	class DM_EXPORT DMIEExternal : public IDispatch
	{
	public:
		DMIEExternal();

		void SetWebBrowser(DUIIE *pWebBrowser);

		void SetEvtHandler(IDMWebEvent* pEventHandler);
		void SetDUIWnd(DUIWND hWnd);

		virtual HRESULT STDMETHODCALLTYPE QueryInterface( 
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef( void){return ++m_refCount;};

		virtual ULONG STDMETHODCALLTYPE Release( void){return --m_refCount;};

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount( 
			/* [out] */ __RPC__out UINT *pctinfo){return E_NOTIMPL;}

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo( 
			/* [in] */ UINT iTInfo,
			/* [in] */ LCID lcid,
			/* [out] */ __RPC__deref_out_opt ITypeInfo **ppTInfo){return E_NOTIMPL;}

		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames( 
			/* [in] */ __RPC__in REFIID riid,
			/* [size_is][in] */ __RPC__in_ecount_full(cNames) LPOLESTR *rgszNames,
			/* [range][in] */ UINT cNames,
			/* [in] */ LCID lcid,
			/* [size_is][out] */ __RPC__out_ecount_full(cNames) DISPID *rgDispId);

		virtual /* [local] */ HRESULT STDMETHODCALLTYPE Invoke( 
			/* [in] */ DISPID dispIdMember,
			/* [in] */ REFIID riid,
			/* [in] */ LCID lcid,
			/* [in] */ WORD wFlags,
			/* [out][in] */ DISPPARAMS *pDispParams,
			/* [out] */ VARIANT *pVarResult,
			/* [out] */ EXCEPINFO *pExcepInfo,
			/* [out] */ UINT *puArgErr);

	protected:
		ULONG					m_refCount;
		DUIIE					*m_pWebBrowser;
		IDMWebEvent*			m_pEvtHandler;
		DUIWND					m_hWnd;
	};

	class DM_EXPORT DMIEOleClientSite : public IOleClientSite
	{
	public:
		DMIEOleClientSite();

		void SetWebBrowser(DUIIE* pWebBrowser);

		// *** IUnknown ***
		STDMETHOD(QueryInterface)(
			/* [in] */ REFIID riid,
			/* [iid_is][out] */ VOID **ppvObject);

		virtual ULONG STDMETHODCALLTYPE AddRef( void){return ++m_refCount;};

		virtual ULONG STDMETHODCALLTYPE Release( void){return --m_refCount;};

		// *** IOleClientSite ***        
		STDMETHOD(SaveObject)(){return E_NOTIMPL;}
		STDMETHOD(GetMoniker)(
			/* [in] */ DWORD dwAssign,
			/* [in] */ DWORD dwWhichMoniker,
			/* [out] */ IMoniker **ppmk){return E_NOTIMPL;}

		STDMETHOD(GetContainer)(
			/* [out] */ IOleContainer **ppContainer){return E_NOTIMPL;}

		STDMETHOD(ShowObject)(){return E_NOTIMPL;}
		STDMETHOD(OnShowWindow)(
			/* [in] */ BOOL fShow){return E_NOTIMPL;}

		STDMETHOD(RequestNewObjectLayout)(){return E_NOTIMPL;}

	protected:
		ULONG					m_refCount;
		DUIIE *					m_pWebBrowser;
	};

	/// <summary>
	///		 DUIIE的内置实现
	/// </summary>
	class DM_EXPORT DUIIE:public DUIActiveX
						 ,public IDMMessageFilter
						 ,public IDUIWeb // 做为纯虚抽象接口，必须放在最后					
	{
		DMDECLARE_CLASS_NAME(DUIIE, DUINAME_IE,DMREG_Window)
	public:
		DUIIE();
		~DUIIE();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_CREATE(OnCreate)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			MSG_WM_DESTROY(OnDestroy) 
		DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		int OnCreate(LPVOID);
		void OnShowWindow(BOOL bShow, UINT nStatus);
		void OnDestroy();
		
		
	public:
		DMCode DV_OnAxActivate(IUnknown *pUnknwn);
		BOOL PreTranslateMessage(MSG* pMsg);
		BOOL IsIEVisible();

	public:
		/// @brief 设置事件接收者
		/// @param[in]		pEventHandler		事件接收者指针
		/// @return HRESULT，S_OK
		HRESULT SetEvtHandler(IDMWebEvent* pEventHandler);
		HRESULT RegisterEvtHandler(BOOL inAdvise);

	public:
		/// @brief 打开指定页面
		/// @param[in]		pszURL		需要打开的url字符串
		/// @return HRESULT，失败为S_FALSE
		HRESULT OpenUrl(LPCWSTR pszURL);

		/// @brief 打开指定页面
		/// @param[in]		pszURL		接收当前url的字符串缓冲区
		/// @param[in]		nMaxLen		字符串缓冲区的最大长度
		/// @return HRESULT，失败为S_FALSE
		HRESULT GetUrl(LPWSTR pszUrl, int nMaxLen);

		/// @brief 停止
		/// @return HRESULT，失败为S_FALSE
		HRESULT Stop();

		/// @brief 关闭浏览器
		/// @return HRESULT，失败为S_FALSE
		HRESULT Quit();

		/// @brief 刷新
		/// @return HRESULT，失败为S_FALSE
		HRESULT Refresh();

		/// @brief 指定刷新级别刷新
		/// @param[in]		nLevel		刷新常量， 简化了参数类型， REFRESH_NORMAL 0x0 / REFRESH_IFEXPIRED 0x01 / REFRESH_COMPLETELY 0x03
		/// @return HRESULT，失败为S_FALSE
		HRESULT Refresh2(UINT32 nLevel);

		/// @brief 后退
		/// @return HRESULT，失败为S_FALSE
		HRESULT GoBack();

		/// @brief 前进
		/// @return HRESULT，失败为S_FALSE
		HRESULT GoForward();

		/// @brief 在当前浏览器控制器下执行脚本
		/// @return HRESULT，失败为S_FALSE
		HRESULT ExecuteScript(LPCWSTR pszScript);

		/// @brief 执行指定函数名字的脚本
		/// @param[in]		 strFun			指定要脚本执行的函数名称
		/// @param[in]		 vecParams		给定要脚本执行的函数的参数列表
		/// @param[out]		 strResult		返回脚本函数执行的结果
		/// @param[in]		 nMaxLen		返回脚本函数执行的结果缓冲区的最大长度
		/// @return HRESULT，失败为E_FAIL
		HRESULT ExecuteScriptFuntion( 
			LPCWSTR strFun, 
			const DM::CArray<LPCWSTR>& vecParams, 
			LPWSTR strResult = NULL,
			int nMaxLen = -1);

		/// @brief 禁止脚本警告
		/// @param[in]		 bDisable		是否禁止
		/// @return HRESULT，失败为E_FAIL
		HRESULT DisableScriptWarning(bool bDisable);

		/// @brief 是否显示IE的滚动条
		/// @param[in]		 bShow			是否显示
		/// @return
		void SetScrollBarShow(bool bShow);

		/// @brief 转发IE的设置属性
		/// @return
		DMCode IESetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml);
		
	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::STRING_url,OnAttributeUrl)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::bool_bshowscroll,OnAttributeShowScroll)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::ACCEL_refreshkey, OnAttributeRefreshKey)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeUrl(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeShowScroll(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeRefreshKey(LPCWSTR pszValue, bool bLoadXml);

	public:
		CStringW				    m_strUrl;
		DWORD						m_dwCookie;
		DMIEEvtDispatch				m_eventDispatch;
		DMComQIPtr<IWebBrowser2>	m_pIE;

		DMIEOleClientSite			m_clientSite;
		DMIEDocHostUIHandler		m_docHostUIHandler;
		DMIEExternal				m_external;
		bool						m_bShowScroll;
		DWORD						m_refreshkey;
	};


}//namespace DM