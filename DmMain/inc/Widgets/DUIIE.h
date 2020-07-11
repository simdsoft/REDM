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
	///		<see cref="DM::DUIIE"/>��xml���Զ���
	/// </summary>
	class DUIIEAttr
	{
	public:
		static wchar_t* STRING_url;                                    ///< ָ��IE��Ĭ�ϼ���URL,ʾ��:url="www.baidu.com"
		static wchar_t* bool_bshowscroll;                              ///< �Ƿ���ʾ������IE������,Ĭ������,ʾ��:bshowscroll="0"
		static wchar_t* bool_bshowcontext;                             ///< �Ƿ���ʾ�������Ҽ��˵�,Ĭ����ʾ,ʾ��:bshowcontext="0"
		static wchar_t* bool_bdisablescriptwarn;                       ///< �Ƿ��ֹ�ű�����,Ĭ�ϲ���ֹ,ʾ��:bdisablescriptwarn="0"
		static wchar_t* ACCEL_refreshkey;                              ///< ˢ�¿�ݽ�,ʾ��:refreshkey="ctrl+f5"
	};
	DMAttrValueInit(DUIIEAttr,STRING_url)DMAttrValueInit(DUIIEAttr,bool_bshowscroll)DMAttrValueInit(DUIIEAttr,bool_bshowcontext)DMAttrValueInit(DUIIEAttr,bool_bdisablescriptwarn)DMAttrValueInit(DUIIEAttr,ACCEL_refreshkey)
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

		//IUnkown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_nRef;};
		virtual ULONG STDMETHODCALLTYPE Release(void){return --m_nRef;};

		//IDispatch
		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo){return E_NOTIMPL;}
		STDMETHOD(GetTypeInfo)(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo){return E_NOTIMPL;}
		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId){return E_NOTIMPL;}
		STDMETHOD(Invoke)(DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr);

	public:
		ULONG								m_nRef;
		DUIIE								*m_pWebBrowser;
		IDMWebEvent*						m_pEvtHandler;
		DUIWND								m_hWnd;
		bool								m_bCanGoForward;
		bool								m_bCanGoBack;
	};

	/// <summary>
	///		����WebBrowser����Ϊ��ʽ�������
	/// </summary>
	class DM_EXPORT DMIEDocHostUIHandler : public IDocHostUIHandler
	{
	public:
		DMIEDocHostUIHandler();
		void SetWebBrowser(DUIIE *pWebBrowser){m_pWebBrowser=pWebBrowser;};
		void SetScrollBarShow(bool bShow){m_bShowScrollBar = bShow;}
		void SetContextMenuShow(bool bShow) {m_bShowContextMenu = bShow;}

	public:
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_nRef;};
		virtual ULONG STDMETHODCALLTYPE Release(void){return --m_nRef;};

		// IDocHostUIHandler
		STDMETHOD(ShowContextMenu)(DWORD dwID, POINT *ppt, IUnknown *pcmdtReserved, IDispatch *pdispReserved);
		STDMETHOD(GetHostInfo)(DOCHOSTUIINFO *pInfo);
		STDMETHOD(ShowUI)(DWORD dwID,IOleInPlaceActiveObject *pActiveObject,IOleCommandTarget *pCommandTarget,IOleInPlaceFrame *pFrame,IOleInPlaceUIWindow *pDoc){return S_OK;}
		STDMETHOD(HideUI)(void){return S_OK;}
		STDMETHOD(UpdateUI)(void){return S_OK;}
		STDMETHOD(EnableModeless)(BOOL fEnable){return S_OK;}
		STDMETHOD(OnDocWindowActivate)(BOOL fActivate){return S_OK;}
		STDMETHOD(OnFrameWindowActivate)(BOOL fActivate){return S_OK;}
		STDMETHOD(ResizeBorder)(LPCRECT prcBorder,IOleInPlaceUIWindow *pUIWindow,BOOL fRameWindow){return S_OK;}
		STDMETHOD(TranslateAccelerator)(LPMSG lpMsg,const GUID *pguidCmdGroup,DWORD nCmdID){return S_FALSE;}
		STDMETHOD(GetOptionKeyPath)(LPOLESTR *pchKey,DWORD dw){return S_FALSE;}
		STDMETHOD(GetDropTarget)(IDropTarget *pDropTarget,IDropTarget **ppDropTarget){return S_FALSE;}
		STDMETHOD(GetExternal)(IDispatch **ppDispatch);
		STDMETHOD(TranslateUrl)(DWORD dwTranslate,OLECHAR *pchURLIn, __out  OLECHAR **ppchURLOut);
		STDMETHOD(FilterDataObject)(IDataObject *pDO,IDataObject **ppDORet);

	protected:
		ULONG							m_nRef;
		DUIIE							*m_pWebBrowser;
		bool							m_bShowScrollBar;
		bool							m_bShowContextMenu;
	};

	class DM_EXPORT DMIEExternal : public IDispatch
	{
	public:
		DMIEExternal();
		void SetWebBrowser(DUIIE *pWebBrowser);
		void SetEvtHandler(IDMWebEvent* pEventHandler);

	public:
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_nRef;};
		virtual ULONG STDMETHODCALLTYPE Release(void){return --m_nRef;};

		STDMETHOD(GetTypeInfoCount)(UINT *pctinfo){return E_NOTIMPL;}
		STDMETHOD(GetTypeInfo)(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo){return E_NOTIMPL;}
		STDMETHOD(GetIDsOfNames)(REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId);
		STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,EXCEPINFO *pExcepInfo,UINT *puArgErr);

	protected:
		ULONG							m_nRef;
		DUIIE							*m_pWebBrowser;
		IDMWebEvent*					m_pEvtHandler;
		DUIWND							m_hDUIWnd;
	};

	class DM_EXPORT DMIEOleClientSite : public IOleClientSite
	{
	public:
		DMIEOleClientSite();

		void SetWebBrowser(DUIIE* pWebBrowser);

	public:
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_nRef;};
		virtual ULONG STDMETHODCALLTYPE Release(void){return --m_nRef;};
   
		STDMETHOD(SaveObject)(){return E_NOTIMPL;}
		STDMETHOD(GetMoniker)(DWORD dwAssign,DWORD dwWhichMoniker,IMoniker **ppmk){return E_NOTIMPL;}
		STDMETHOD(GetContainer)(IOleContainer **ppContainer){return E_NOTIMPL;}
		STDMETHOD(ShowObject)(){return E_NOTIMPL;}
		STDMETHOD(OnShowWindow)(BOOL fShow){return E_NOTIMPL;}
		STDMETHOD(RequestNewObjectLayout)(){return E_NOTIMPL;}

	protected:
		ULONG					m_nRef;
		DUIIE *					m_pWebBrowser;
	};

	/// <summary>
	///		 DUIIE������ʵ��
	/// </summary>
	class DM_EXPORT DUIIE:public DUIActiveX
						 ,public IDMMessageFilter
						 ,public IDUIWeb // ��Ϊ�������ӿڣ�����������					
	{
		DMDECLARE_CLASS_NAME(DUIIE, DUINAME_IE,DMREG_Window)
	public:
		DUIIE();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_CREATE(OnCreate)
			MSG_WM_DESTROY(OnDestroy) 
			MSG_WM_SHOWWINDOW(OnShowWindow)
		DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		int OnCreate(LPVOID);
		void OnDestroy();
		void OnShowWindow(BOOL bShow, UINT nStatus);
		
		
	public:
		DMCode DV_OnAxActivate(IUnknown *pUnknwn);
		BOOL PreTranslateMessage(MSG* pMsg);

	public:
		/// @brief �����¼�������
		/// @param[in]		pEventHandler		�¼�������ָ��
		/// @return HRESULT��S_OK
		HRESULT SetEvtHandler(IDMWebEvent* pEventHandler);
		HRESULT RegisterEvtHandler(bool inAdvise);

	public:
		/// @brief ��ָ��ҳ��
		/// @param[in]		pszURL		��Ҫ�򿪵�url�ַ���
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT OpenUrl(LPCWSTR pszURL,int iFlags = 0,LPCWSTR pszTargetFrameName = NULL,LPCWSTR pszHeaders = NULL, LPCSTR pszPostData = NULL,int iPostDataLen = 0);

		/// @brief ��õ�ǰҳ��
		/// @param[in]		pszURL		���յ�ǰurl���ַ���������
		/// @param[in]		nMaxLen		�ַ�������������󳤶�, ��ҪΪL'\0'Ԥ���ռ�
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT GetUrl(LPWSTR pszUrl, int nMaxLen);
		CStringW GetUrl();

		/// @brief ������������轹��
		HRESULT SetWebFocus();

		/// @brief ���OLE���ھ��
		/// @return HWND��ʧ��ΪNULL
		HWND GetOleWindow();
		HWND GetIESWindow();
		virtual DMComPtr<IWebBrowser2> Ptr();

		/// @brief ��������Web������
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT UpdateWebRect(LPRECT lpRect);

		/// @brief BUSY״̬
		/// @return ture or false
		bool IsBusy();

		/// @brief ֹͣ
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT Stop();

		/// @brief �ر������
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT Quit();

		/// @brief ˢ��
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT Refresh();

		/// @brief ָ��ˢ�¼���ˢ��
		/// @param[in]		nLevel		ˢ�³����� ���˲������ͣ� REFRESH_NORMAL 0x0 / REFRESH_IFEXPIRED 0x01 / REFRESH_COMPLETELY 0x03
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT Refresh2(UINT32 nLevel);

		// @brief �Ƿ�ɵ�������
		/// @return ture or false
		bool CanGoBack();

		/// @brief ����
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT GoBack();

		/// @brief �Ƿ�ɵ���ǰ��
		/// @return ture or false
		bool CanGoForward();

		/// @brief ǰ��
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT GoForward();

		/// @brief �ڵ�ǰ�������������ִ�нű�
		/// @return HRESULT��ʧ��ΪS_FALSE
		HRESULT ExecuteScript(LPCWSTR pszScript);

		/// @brief ִ��ָ���������ֵĽű�
		/// @param[in]		 strFun			ָ��Ҫ�ű�ִ�еĺ�������
		/// @param[in]		 vecParams		����Ҫ�ű�ִ�еĺ����Ĳ����б�
		/// @param[out]		 strResult		���ؽű�����ִ�еĽ��
		/// @param[in]		 nMaxLen		���ؽű�����ִ�еĽ������������󳤶�, ��ҪΪL'\0'Ԥ���ռ�
		/// @return HRESULT��ʧ��ΪE_FAIL
		HRESULT ExecuteScriptFuntion( 
			LPCWSTR pszFun, 
			const DM::CArray<LPCWSTR>& vecParams, 
			LPWSTR strResult = NULL,
			int nMaxLen = -1);

		/// @brief ��ֹ�ű�����
		/// @param[in]		 bDisable		�Ƿ��ֹ
		/// @return HRESULT��ʧ��ΪE_FAIL
		HRESULT DisableScriptWarning(bool bDisable);

		/// @brief �Ƿ���ʾIE�Ĺ�����
		/// @param[in]		 bShow			�Ƿ���ʾ
		/// @return
		HRESULT SetScrollBarShow(bool bShow);

		/// @brief �Ƿ���ʾIE���Ҽ��˵�
		/// @param[in]		 bShow			�Ƿ���ʾ
		/// @return
		HRESULT SetContextMenuShow(bool bShow);

		/// @brief ת��IE����������
		/// @return
		HRESULT WebSetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::STRING_url,OnAttributeUrl)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::bool_bshowscroll,OnAttributeShowScroll)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::bool_bshowcontext,OnAttributeShowContext)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::bool_bdisablescriptwarn,OnAttributeDisableScriptWarn)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIIEAttr::ACCEL_refreshkey, OnAttributeRefreshKey)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeUrl(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeShowScroll(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeShowContext(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeDisableScriptWarn(LPCWSTR pszValue, bool bLoadXml);
		DMCode OnAttributeRefreshKey(LPCWSTR pszValue, bool bLoadXml);

	public:
		CStringW				    m_strUrl;
		DWORD						m_dwCookie;
		DMIEEvtDispatch				m_EventDispatch;
	

		DMIEOleClientSite			m_ClientSite;
		DMIEDocHostUIHandler		m_DocHostUIHandler;
		DMIEExternal				m_External;
		bool						m_bShowScroll;
		bool                        m_bShowContext;
		bool                        m_bDisableScriptWarn;
		DWORD						m_Refreshkey;
	};


}//namespace DM