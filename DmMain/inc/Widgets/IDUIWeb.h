//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDUIIE.h 
// File Des:提供IE对外接口
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
		virtual DMCode BeforeNavigate2(DUIWND hWnd, DMIN IDispatch *pDisp, DMIN wchar_t *pUrl,DMIN int Flags,DMIN wchar_t *pTargetFrameName,DMIN wchar_t *pPostData,DMIN wchar_t *pHeaders,DMINOUT VARIANT_BOOL *bCancel){return DM_ECODE_NOTIMPL;}
		virtual DMCode ClientToHostWindow(DUIWND hWnd, DMIN long *pCx, DMINOUT long *pCy){return DM_ECODE_NOTIMPL;}
		virtual DMCode CommandStateChange(DUIWND hWnd, DMIN LONG command, DMIN VARIANT_BOOL enable){return DM_ECODE_NOTIMPL;}
		virtual DMCode DocumentComplete(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl){return DM_ECODE_NOTIMPL;}
		virtual DMCode DownloadBegin(DUIWND hWnd){return DM_ECODE_NOTIMPL;}
		virtual DMCode DownloadComplete(DUIWND hWnd){return DM_ECODE_NOTIMPL;}
		virtual DMCode FileDownload(DUIWND hWnd, DMIN VARIANT_BOOL ActiveDocument,DMINOUT VARIANT_BOOL *Cancel){return DM_ECODE_NOTIMPL;}
		virtual DMCode NavigateComplete2(DUIWND hWnd,DMIN IDispatch *pDisp,DMIN wchar_t *pUrl){return DM_ECODE_NOTIMPL;}
		virtual DMCode NavigateError(DUIWND hWnd, DMIN IDispatch *pDisp,DMIN wchar_t *pUrl,DMIN wchar_t *pTargetFrameName,DMIN int statusCode,DMINOUT VARIANT_BOOL *bCancel){return DM_ECODE_NOTIMPL;}
		virtual DMCode NewWindow2(DUIWND hWnd, DMINOUT IDispatch **pDisp,DMINOUT VARIANT_BOOL *bCancel){return DM_ECODE_NOTIMPL;}
		virtual DMCode NewWindow3(DUIWND hWnd, DMINOUT IDispatch **pDisp,DMINOUT VARIANT_BOOL *bCancel,DMIN DWORD dwFlags,DMIN wchar_t *pUrlContext,DMIN wchar_t *pUrl){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnFullScreen(DUIWND hWnd, DMIN VARIANT_BOOL bFullScreen){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnMenuBar(DUIWND hWnd, DMIN VARIANT_BOOL bMenuBar){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnQuit(DUIWND hWnd){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnStatusBar(DUIWND hWnd, DMIN VARIANT_BOOL bStatusBar){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnTheaterMode(DUIWND hWnd, DMIN VARIANT_BOOL bTheaterMode){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnToolBar(DUIWND hWnd, DMIN VARIANT_BOOL bToolBar){return DM_ECODE_NOTIMPL;}
		virtual DMCode OnVisible(DUIWND hWnd, DMIN VARIANT_BOOL bVisible){return DM_ECODE_NOTIMPL;}
		virtual DMCode PrintTemplateInstantiation(DUIWND hWnd,DMIN IDispatch *pDisp){return DM_ECODE_NOTIMPL;}
		virtual DMCode PrintTemplateTeardown(DUIWND hWnd,DMIN IDispatch *pDisp){return DM_ECODE_NOTIMPL;}
		virtual DMCode PrivacyImpactedStateChange(DUIWND hWnd, DMIN VARIANT_BOOL bPrivacyImpacted){return DM_ECODE_NOTIMPL;}
		virtual DMCode ProgressChange(DUIWND hWnd, DMIN LONG Progress,DMIN LONG ProgressMax){return DM_ECODE_NOTIMPL;}
		virtual DMCode PropertyChange(DUIWND hWnd, DMIN wchar_t *pProperty){return DM_ECODE_NOTIMPL;}
		virtual DMCode SetPhishingFilterStatus(DUIWND hWnd, DMIN LONG phishingFilterStatus){return DM_ECODE_NOTIMPL;}
		virtual DMCode SetSecureLockIcon(DUIWND hWnd, DMIN int SecureLockIcon){return DM_ECODE_NOTIMPL;}
		virtual DMCode StatusTextChange(DUIWND hWnd,DMIN wchar_t *pText){return DM_ECODE_NOTIMPL;}
		virtual DMCode TitleChange(DUIWND hWnd, DMIN wchar_t *pText){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowClosing(DUIWND hWnd, DMIN VARIANT_BOOL IsChildWindow,DMINOUT VARIANT_BOOL *bCancel){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowSetHeight(DUIWND hWnd, DMIN LONG height){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowSetLeft(DUIWND hWnd, DMIN LONG left){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowSetResizable(DUIWND hWnd, DMIN VARIANT_BOOL bResizable){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowSetTop(DUIWND hWnd, DMIN LONG top){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowSetWidth(DUIWND hWnd, DMIN LONG width){return DM_ECODE_NOTIMPL;}
		virtual DMCode WindowStateChanged(DUIWND hWnd, DMIN DWORD dwFlags,DMIN DWORD dwValidFlagsMask){return DM_ECODE_NOTIMPL;}
		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(DUIWND hWnd, REFIID riid, LPOLESTR* rgszNames,UINT cNames, LCID lcid, DISPID* rgDispId){return E_NOTIMPL;}
		virtual HRESULT STDMETHODCALLTYPE Invoke(DUIWND hWnd, DISPID dispIdMember, REFIID riid, LCID lcid, WORD wFlags,DISPPARAMS* pDispParams, VARIANT* pVarResult, EXCEPINFO* pExcepInfo,UINT* puArgErr){return E_NOTIMPL;}
	};

	/// <summary>
	///		 IE对外接口
	/// </summary>
	class IDUIWeb
	{
	public:
		/// @brief 设置事件接收者
		/// @param[in]		pEventHandler		事件接收者指针
		/// @return HRESULT，S_OK
		virtual HRESULT SetEvtHandler(IDMWebEvent* pEventHandler) = 0;

		/// @brief 打开指定页面
		/// @param[in]		pszURL		需要打开的url字符串
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT OpenUrl(LPCWSTR pszURL) = 0;

		/// @brief 打开指定页面
		/// @param[in]		pszURL		接收当前url的字符串缓冲区
		/// @param[in]		nMaxLen		字符串缓冲区的最大长度
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GetUrl(LPWSTR pszUrl, int nMaxLen) = 0;

		/// @brief 停止
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Stop() = 0;

		/// @brief 关闭浏览器
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Quit() = 0;

		/// @brief 刷新
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Refresh() = 0;

		/// @brief 指定刷新级别刷新
		/// @param[in]		nLevel		刷新常量， 简化了参数类型， REFRESH_NORMAL 0x0 / REFRESH_IFEXPIRED 0x01 / REFRESH_COMPLETELY 0x03
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT Refresh2(UINT32 nLevel) = 0;

		/// @brief 后退
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GoBack() = 0;

		/// @brief 前进
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT GoForward() = 0;

		/// @brief 在当前浏览器控制器下执行脚本
		/// @return HRESULT，失败为S_FALSE
		virtual HRESULT ExecuteScript(LPCWSTR pszScript) = 0;

		/// @brief 执行指定函数名字的脚本
		/// @param[in]		 strFun			指定要脚本执行的函数名称
		/// @param[in]		 vecParams		给定要脚本执行的函数的参数列表
		/// @param[out]		 strResult		返回脚本函数执行的结果
		/// @param[in]		 nMaxLen		返回脚本函数执行的结果缓冲区的最大长度
		/// @return HRESULT，失败为E_FAIL
		virtual HRESULT ExecuteScriptFuntion(LPCWSTR strFun, const DM::CArray<LPCWSTR>& vecParams, LPWSTR strResult = NULL,int nMaxLen = -1) = 0;

		/// @brief 禁止脚本警告
		/// @param[in]		 bDisable		是否禁止
		/// @return HRESULT，失败为E_FAIL
		virtual HRESULT DisableScriptWarning(bool bDisable) = 0;

		/// @brief 是否显示IE的滚动条
		/// @param[in]		 bShow			是否显示
		/// @return
		virtual void SetScrollBarShow(bool bShow) = 0;

		/// @brief 转发DUIIE的SetAttribute
		/// @param[in]		 bShow			是否显示
		/// @return
		virtual DMCode IESetAttribute(LPCWSTR pszAttribute,LPCWSTR pszValue,bool bLoadXml) = 0;					
	};

}