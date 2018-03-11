//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDUIWeb.h 
// File Des:提供Web对外接口
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-4-7	1.0			   
//--------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		 Dispatch事件处理,函数说明请参考https://msdn.microsoft.com/en-us/library/aa752084.aspx
	/// </summary>
	class IDMWebEvent
	{
	public:
		virtual HRESULT BeforeNavigate2(DUIWND hWnd, DMIN IDispatch *pDisp, DMIN wchar_t *pUrl,DMIN int Flags,DMIN wchar_t *pTargetFrameName,DMIN VARIANT *pPostData,DMIN wchar_t *pHeaders,DMINOUT VARIANT_BOOL *bCancel){return S_OK;}
		virtual HRESULT ClientToHostWindow(DUIWND hWnd, DMIN long *pCx, DMINOUT long *pCy){return S_OK;}
		virtual HRESULT CommandStateChange(DUIWND hWnd, DMIN LONG command, DMIN VARIANT_BOOL enable){return S_OK;}
		virtual HRESULT DocumentComplete(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl){return S_OK;}
		virtual HRESULT DownloadBegin(DUIWND hWnd){return S_OK;}
		virtual HRESULT DownloadComplete(DUIWND hWnd){return S_OK;}
		virtual HRESULT FileDownload(DUIWND hWnd, DMIN VARIANT_BOOL ActiveDocument,DMINOUT VARIANT_BOOL *Cancel){return S_OK;}
		virtual HRESULT NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl){return S_OK;}
		virtual HRESULT NavigateError(DUIWND hWnd, DMIN IDispatch *pDisp,DMIN wchar_t *pUrl,DMIN wchar_t *pTargetFrameName,DMIN int statusCode,DMINOUT VARIANT_BOOL *bCancel){return S_OK;}
		virtual HRESULT NewWindow2(DUIWND hWnd, DMINOUT IDispatch **pDisp,DMINOUT VARIANT_BOOL *bCancel){return S_OK;}
		virtual HRESULT NewWindow3(DUIWND hWnd, DMINOUT IDispatch **pDisp,DMINOUT VARIANT_BOOL *bCancel,DMIN DWORD dwFlags,DMIN wchar_t *pUrlContext,DMIN wchar_t *pUrl){return S_OK;}
		virtual HRESULT OnFullScreen(DUIWND hWnd, DMIN VARIANT_BOOL bFullScreen){return S_OK;}
		virtual HRESULT OnMenuBar(DUIWND hWnd, DMIN VARIANT_BOOL bMenuBar){return S_OK;}
		virtual HRESULT OnQuit(DUIWND hWnd){return S_OK;}
		virtual HRESULT OnStatusBar(DUIWND hWnd, DMIN VARIANT_BOOL bStatusBar){return S_OK;}
		virtual HRESULT OnTheaterMode(DUIWND hWnd, DMIN VARIANT_BOOL bTheaterMode){return S_OK;}
		virtual HRESULT OnToolBar(DUIWND hWnd, DMIN VARIANT_BOOL bToolBar){return S_OK;}
		virtual HRESULT OnVisible(DUIWND hWnd, DMIN VARIANT_BOOL bVisible){return S_OK;}
		virtual HRESULT PrintTemplateInstantiation(DUIWND hWnd,DMIN IDispatch *pDisp){return S_OK;}
		virtual HRESULT PrintTemplateTeardown(DUIWND hWnd,DMIN IDispatch *pDisp){return S_OK;}
		virtual HRESULT PrivacyImpactedStateChange(DUIWND hWnd, DMIN VARIANT_BOOL bPrivacyImpacted){return S_OK;}
		virtual HRESULT ProgressChange(DUIWND hWnd, DMIN LONG Progress,DMIN LONG ProgressMax){return S_OK;}
		virtual HRESULT PropertyChange(DUIWND hWnd, DMIN wchar_t *pProperty){return S_OK;}
		virtual HRESULT SetPhishingFilterStatus(DUIWND hWnd, DMIN LONG phishingFilterStatus){return S_OK;}
		virtual HRESULT SetSecureLockIcon(DUIWND hWnd, DMIN int SecureLockIcon){return S_OK;}
		virtual HRESULT StatusTextChange(DUIWND hWnd,DMIN wchar_t *pText){return S_OK;}
		virtual HRESULT TitleChange(DUIWND hWnd, DMIN wchar_t *pText){return S_OK;}
		virtual HRESULT WindowClosing(DUIWND hWnd, DMIN VARIANT_BOOL IsChildWindow,DMINOUT VARIANT_BOOL *bCancel){return S_OK;}
		virtual HRESULT WindowSetHeight(DUIWND hWnd, DMIN LONG height){return S_OK;}
		virtual HRESULT WindowSetLeft(DUIWND hWnd, DMIN LONG left){return S_OK;}
		virtual HRESULT WindowSetResizable(DUIWND hWnd, DMIN VARIANT_BOOL bResizable){return S_OK;}
		virtual HRESULT WindowSetTop(DUIWND hWnd, DMIN LONG top){return S_OK;}
		virtual HRESULT WindowSetWidth(DUIWND hWnd, DMIN LONG width){return S_OK;}
		virtual HRESULT WindowStateChanged(DUIWND hWnd, DMIN DWORD dwFlags,DMIN DWORD dwValidFlagsMask){return S_OK;}
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(DUIWND hWnd, REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId){return S_OK;}
		virtual HRESULT STDMETHODCALLTYPE Invoke(DUIWND hWnd, DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr){return S_OK;}

		// 部分接口为webkit特有
		virtual HRESULT OnPreKeyEvent(DUIWND hWnd, MSG* pMsg){return E_NOTIMPL;}
	};

	/// <summary>
	///		 Web对外接口
	/// </summary>
	class IDUIWeb
	{
	public:
		enum{DMWEBTYPE_IE,DMWEBTYPE_WEBKIT,DMWEBTYPE_OSRWEBKIT,};
		/// @brief 返回当前的类型(ie/webkit/离屏webkit)
		/// @return 默认为IE
		virtual int GetWebType(){return DMWEBTYPE_IE;};

		/// @brief 设置事件接收者
		/// @param[in]		pEventHandler		事件接收者指针
		/// @return HRESULT，S_OK
		virtual HRESULT SetEvtHandler(IDMWebEvent* pEventHandler){return E_NOTIMPL;};

		/// @brief 打开指定页面
		/// @param[in]		pszURL		需要打开的url字符串
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT OpenUrl(LPCWSTR pszURL,int iFlags = 0,LPCWSTR pszTargetFrameName = NULL,LPCWSTR pszHeaders = NULL, LPCSTR pszPostData = NULL,int iPostDataLen = 0){return E_NOTIMPL;};

		/// @brief 打开指定页面
		/// @param[in]		pszURL		接收当前url的字符串缓冲区
		/// @param[in]		nMaxLen		字符串缓冲区的最大长度
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GetUrl(LPWSTR pszUrl, int nMaxLen){return E_NOTIMPL;};

		/// @brief 给浏览器窗口设焦点
		virtual HRESULT SetWebFocus(){return E_NOTIMPL;};

		/// @brief 获得OLE窗口句柄
		/// @return HWND，失败为NULL
		virtual HWND GetOleWindow(){return NULL;};

		/// @brief 主动设置Web的区域
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT UpdateWebRect(LPRECT lpRect){return E_NOTIMPL;}

		/// @brief BUSY状态
		/// @return ture or false
		virtual bool IsBusy(){return false;};

		/// @brief 停止
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Stop(){return E_NOTIMPL;};

		/// @brief 关闭浏览器
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Quit(){return E_NOTIMPL;};

		/// @brief 刷新
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Refresh(){return E_NOTIMPL;};

		/// @brief 指定刷新级别刷新
		/// @param[in]		nLevel		刷新常量， 简化了参数类型， REFRESH_NORMAL 0x0 / REFRESH_IFEXPIRED 0x01 / REFRESH_COMPLETELY 0x03
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Refresh2(UINT32 nLevel){return E_NOTIMPL;};

		/// @brief 后退
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GoBack(){return E_NOTIMPL;};

		/// @brief 前进
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GoForward(){return E_NOTIMPL;};

		/// @brief 在当前浏览器控制器下执行脚本
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT ExecuteScript(LPCWSTR pszScript){return E_NOTIMPL;};

		/// @brief 执行指定函数名字的脚本
		/// @param[in]		 strFun			指定要脚本执行的函数名称
		/// @param[in]		 vecParams		给定要脚本执行的函数的参数列表
		/// @param[out]		 strResult		返回脚本函数执行的结果
		/// @param[in]		 nMaxLen		返回脚本函数执行的结果缓冲区的最大长度
		/// @return HRESULT，失败为E_FAIL
		virtual HRESULT ExecuteScriptFuntion(LPCWSTR pszFun, const DM::CArray<LPCWSTR>& vecParams, LPWSTR strResult = NULL,int nMaxLen = -1){return E_NOTIMPL;};

		/// @brief 禁止脚本警告
		/// @param[in]		 bDisable		是否禁止
		/// @return HRESULT，失败为E_FAIL
		virtual HRESULT DisableScriptWarning(bool bDisable){return E_NOTIMPL;};

		/// @brief 是否显示Web的滚动条
		/// @param[in]		 bShow			是否显示
		/// @return
		virtual HRESULT SetScrollBarShow(bool bShow){return E_NOTIMPL;};

		/// @brief 是否显示Web的右键菜单
		/// @param[in]		 bShow			是否显示
		/// @return
		virtual HRESULT SetContextMenuShow(bool bShow){return E_NOTIMPL;};

		/// @brief 转发Web的SetAttribute
		/// @param[in]		 bShow			是否显示
		/// @return
		virtual HRESULT WebSetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml){return E_NOTIMPL;};


		//---------------------------------------------------------
		// 向后添加新接口，以兼容旧版

		/// @brief 是否可导航后退
		/// @return ture or false
		virtual bool CanGoBack(){return false;}

		/// @brief 是否可导航前进
		/// @return ture or false
		virtual bool CanGoForward(){return false;}
	};

}