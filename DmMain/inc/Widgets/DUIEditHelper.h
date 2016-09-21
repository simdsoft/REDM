//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: http://www.cppblog.com/wlwlxj/archive/2012/07/01/180982.html
// File Des: http://blog.csdn.net/skilledprogrammer/article/details/6223650
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-27	1.0			
//--------------------------------------------------------
#pragma once
#include "DUIScrollBase.h"

#include <Richedit.h>// 和TextServ.h的顺序不能变
#include <TextServ.h>
#include <Imm.h>
#pragma comment(lib,"imm32.lib")

namespace DM
{
	/// <summary>
	///			RichEdit辅助加载类---不能频繁加载/卸载riched20.dll太多次，有几率触发消息异常，引发崩溃
	/// </summary>
	class DM_EXPORT DUIEditHelper:public DMSingletonT<DUIEditHelper>
	{
	public:
		DUIEditHelper();
		~DUIEditHelper();
	public:
		HRESULT CreateTextServices(IUnknown *punkOuter, ITextHost *pITextHost, IUnknown **ppUnk);

	public:
		HINSTANCE			   m_rich20;
		PCreateTextServices    m_funCreateTextServices; 
	};

	/// <summary>
	///			辅助拖动
	/// </summary>
	class DUIRicheditDropTarget : public IDropTarget
	{
	public:
		DUIRicheditDropTarget(ITextServices *pTxtSvr);
		~DUIRicheditDropTarget();
		void Init();
		void UnInit();

	public:
		//IUnkown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);

		//IDropTarget
		virtual HRESULT STDMETHODCALLTYPE DragEnter(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE DragLeave(void);
		virtual HRESULT STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState, POINTL pt,__RPC__inout DWORD *pdwEffect);
	protected:
		ITextServices    *m_pServ;            
		LONG              m_nRef;
	};
	class DUIRichEdit;
	/// <summary>
	///			辅助光标
	/// </summary>
	class DM_EXPORT DUICaret:public DUIWindow,public IDMTimeline
	{
	public:
		DUICaret();
	public:
		void SetAniCount(int iAniCount);
		void Show();
		void Hide();

		void SetPos(CPoint point);
		void SetSize(CSize size);

		bool IsBlinking();
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		void DM_OnPaint(IDMCanvas* pCanvas);
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void OnDestroy();

	public:
		DMCode OnTimeline();

	public:
		DWORD                                        m_dwPreFrameTime;           ///< 上一帧显示的时间，单位ms
		DWORD                                        m_dwDelta;
		DMColor                                      m_clr;
		DMSmartPtrT<IDMCanvas>						 m_pCanvas;
		CSize                                        m_Size;
		CPoint                                       m_pt;
		bool                                         m_bBlinking;				///< 光标正在显示,false表示光标正在消失

		//-----------------------------------------------
		int											 m_iAniCount;				///< 辅助
		int											 m_iAniDelta;				///< 辅助
		int                                          m_iCurAlpha;
	};

	/// <summary>
	///			细节可参看:http://support.microsoft.com/kb/2701617
	/// </summary>
	class DM_EXPORT DUITextHost: public ITextHost
	{
	public:
		DUITextHost(void);
		virtual~DUITextHost(void);
		void Init();
		void UnInit();

		// 调用CreateTextServices，传入ITextHost指针，返回IUnknown指针,查询到ITextServices和ITextDocument接口,这两个接口完成绝大多数功能
		bool Init(DUIRichEdit* pRichEdit);
		ITextServices* GetTextService();
		POINT GetCaretPos();
	public:
		//---------------------------------------------------
		// IUnknown interface
		//---------------------------------------------------
		virtual HRESULT _stdcall QueryInterface(REFIID riid, void **ppvObject);
		virtual ULONG _stdcall AddRef(void);
		virtual ULONG _stdcall Release(void);

		//---------------------------------------------------
		// ITextHost interface
		//---------------------------------------------------
		virtual HDC TxGetDC();															// Get the DC for the host						
		virtual INT TxReleaseDC(HDC hdc);												// Release the DC gotten from the host
		virtual BOOL TxShowScrollBar(INT fnBar, BOOL fShow);							// Show the scroll bar					
		virtual BOOL TxEnableScrollBar (INT fuSBFlags, INT fuArrowflags);
		virtual BOOL TxSetScrollRange(INT fnBar,LONG nMinPos,INT nMaxPos,BOOL fRedraw);
		virtual BOOL TxSetScrollPos (INT fnBar, INT nPos, BOOL fRedraw);
		virtual void TxInvalidateRect(LPCRECT lpRect, BOOL fMode);
		virtual void TxViewChange(BOOL fUpdate);
		virtual BOOL TxCreateCaret(HBITMAP hbmp, INT xWidth, INT yHeight);
		virtual BOOL TxShowCaret(BOOL fShow);
		virtual BOOL TxSetCaretPos(INT x, INT y);
		virtual BOOL TxSetTimer(UINT idTimer, UINT uTimeout);
		virtual void TxKillTimer(UINT idTimer);
		virtual void TxScrollWindowEx (INT dx,INT dy,LPCRECT lprcScroll,LPCRECT lprcClip,HRGN hrgnUpdate,LPRECT lprcUpdate,UINT fuScroll);
		virtual void TxSetCapture(BOOL fCapture);
		virtual void TxSetFocus();
		virtual void TxSetCursor(HCURSOR hcur, BOOL fText);
		virtual BOOL TxScreenToClient(LPPOINT lppt);
		virtual BOOL TxClientToScreen(LPPOINT lppt);
		virtual HRESULT TxActivate(LONG * plOldState);
		virtual HRESULT TxDeactivate(LONG lNewState);
		virtual HRESULT TxGetClientRect(LPRECT lpRect);
		virtual HRESULT TxGetViewInset(LPRECT lpRect);               // 逻辑坐标,标识文本所在区域离整个区域四边的边距   
		virtual HRESULT TxGetCharFormat(const CHARFORMATW **ppCF );
		virtual HRESULT TxGetParaFormat(const PARAFORMAT **ppPF);
		virtual COLORREF TxGetSysColor(int nIndex);
		virtual HRESULT TxGetBackStyle(TXTBACKSTYLE *pstyle);
		virtual HRESULT TxGetMaxLength(DWORD *plength);
		virtual HRESULT TxGetScrollBars(DWORD *pdwScrollBar);
		virtual HRESULT TxGetPasswordChar(TCHAR *pch);
		virtual HRESULT TxGetAcceleratorPos(LONG *pcp);
		virtual HRESULT TxGetExtent(LPSIZEL lpExtent);
		virtual HRESULT OnTxCharFormatChange(const CHARFORMATW * pcf);
		virtual HRESULT OnTxParaFormatChange(const PARAFORMAT * ppf);
		virtual HRESULT TxGetPropertyBits(DWORD dwMask, DWORD *pdwBits);
		virtual HRESULT TxNotify(DWORD iNotify, void *pv);
		virtual HIMC TxImmGetContext();
		virtual void TxImmReleaseContext(HIMC himc);
		virtual HRESULT TxGetSelectionBarWidth (LONG *plSelBarWidth);
	public:
		DUIRichEdit         *m_pRichEdit;		///  宿主窗口
		ITextServices		*m_pServ;        
		bool				m_bUiActive;
		ULONG				m_uRefNum;        
		POINT				m_ptCaret;
	};

}//namespace DM