//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
//
// File Name: DMHWnd.h
// File Des: ��������,����CWnd���Create
// File Summary:  ��Ա�������Ƿ���ClassName_Data���У�XML�������Ƿ���ClassName_XmlInfo����
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-11	1.0
//      k000		2017-09-07	1.0			Add OnPostCreate method called by
//																		DM_CreateWindowEx after window is created.
//-------------------------------------------------------
#pragma once
#include "DMContainerImpl.h"
#include "DMHWndHelper.h"
#include "DMMsgLoop.h"
namespace DM
{
	// The NativeWindow shadow style
	enum {
		// ���κ���ӰЧ��
		NWSDS_NULL = 0,

		// CS_DROPDOWNЧ��, �����ڶ��������ڵ�top-level����, ����menu��tooltip
		// top-level����΢����: ��Child����WS_CHILD���, ��Parent, ��Owner, ����Parent��DesktopWindow�Ĵ���
		NWSDS_DROPSHADOW = 1,

		// dwm���ı���ӰЧ��, ������ͨDialog��������
		NWSDS_DWMSHADOW = 2,
	};

	/// <summary>
	///		��Ϊ����Ҫ�������࣬����ʵ�������ڶ�Ӧ��ֱ�ӻ��Ӽ̳��ڴ���,���ԣ�<see cref="DMAttr::DMHWndAttr"/>
	/// </summary>
	/// <remarks>
	///		�ڲ�ʵ���˼�����Դ���������ڡ���Ϣ�ַ��������Ϊ
	/// <remarks>
	class DM_EXPORT DMHWnd:public DMCWnd          // ���ڻ�����Ϣ����
						  ,public DMContainerImpl // ��Ϣ�ַ�����DUI����
						  ,public DUIWindow		  // ����������ΪDUI����
						  ,public IDMAnimateOwner // �������ڵ�ӵ����
	{
		DMDECLARE_CLASS_NAME(DMHWnd,L"hwnd",DMREG_Window);
	public:
		DMHWnd();
	public:
		//---------------------------------------------------
		// Function Des: �������ڽӿں���
		//---------------------------------------------------
		HWND DM_CreateWindow(LPCWSTR lpszXmlId,int x=0, int y=0, int nWidth=0, int nHeight=0, HWND hWndParent=NULL, int shadowStyle=NWSDS_NULL);///< ��������

		/// -------------------------------------------------
		/// @brief			��������
		/// @param[in]		lpszXmlId       ʹ�õ�XML�ļ���dmindex.xml�еı�ʶ
		/// @param[in]		lpWindowName	������
		/// @param[in]		dwStyle			��ʽ
		/// @param[in]		dwExStyle		��չ��ʽ
		/// @param[in]		x				x����
		/// @param[in]		y				y����
		/// @param[in]		nWidth			���
		/// @param[in]		nHeight			�߶�
		/// @param[in]		hWndParent		������
		/// @param[in]		lpParam			Long pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message
		/// @param[in]		shadowStyle     ������Ӱ���, ���: NativeWindowShadowStyle
		/// @return			HWND
		HWND DM_CreateWindowEx(LPCWSTR lpszXmlId, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, int shadowStyle=NWSDS_NULL);


		/// -------------------------------------------------
		/// @brief			��������
		/// @param[in]		pXmlBuf         ʹ�õ�XML�ļ���buf,�ⲿ��ת����utf8��ʽ
		/// @param[in]		bufLen          ʹ�õ�XML�ļ���buf����
		/// @param[in]		lpWindowName	������
		/// @param[in]		dwStyle			��ʽ
		/// @param[in]		dwExStyle		��չ��ʽ
		/// @param[in]		x				x����
		/// @param[in]		y				y����
		/// @param[in]		nWidth			���
		/// @param[in]		nHeight			�߶�
		/// @param[in]		hWndParent		������
		/// @param[in]		lpParam			Long pointer to a value to be passed to the window through the CREATESTRUCT structure passed in the lParam parameter the WM_CREATE message
		/// @param[in]		shadowStyle     ������Ӱ���, ���: NativeWindowShadowStyle
		/// @return			HWND
		HWND DM_CreateWindowEx(void *pXmlBuf, size_t bufLen, LPCWSTR lpWindowName,DWORD dwStyle, DWORD dwExStyle, int x, int y, int nWidth, int nHeight, HWND hWndParent, PVOID lpParam, int shadowStyle=NWSDS_NULL);


		/// -------------------------------------------------
		/// @brief		���´��ڵ�HDC��
		/// @param[in]  hdc              Ҫ���µ�hdc
		/// @param[in]  rcInvalid        Ҫ���µ�����
		virtual void UpdateHWnd(HDC hdc,const CRect &rcInvalid,bool bUpdate = false);
		void RedrawAll();

		//---------------------------------------------------
		// Function Des: XML
		//---------------------------------------------------
		DMCode LoadDMData(LPCWSTR lpszXmlId);									///< ����XML
		DMCode LoadDMData(void *pXmlBuf, size_t bufLen);
		DMCode InitFromDMData();												///< ͨ��XML���ݳ�ʼ��

	public:

		//---------------------------------------------------
		// Function Des: ��Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnPaint(HDC hdc);
		void OnPrint(HDC hdc, UINT uFlags);
		void OnSize(UINT nType, CSize size);									///< ���軭����С
		void OnSizing(UINT nSide, LPRECT lpRect);
		void OnGetMinMaxInfo(LPMINMAXINFO lpMMI);								///< ���������С����
		void OnDestroy();
		void OnActivate(UINT nState, BOOL bMinimized, HWND wndOther);
		void OnSetFocus(HWND wndOld);
		void OnKillFocus(HWND wndFocus);
		void OnTimer(UINT_PTR idEvent);											///< �ַ�FastTimer
		virtual void OnDUITimer(char id);										///< �����������Լ������˶�ʱ�������ش���

		void OnMove(CPoint pt);
		void OnMouseMove(UINT nFlags, CPoint point);
		void OnMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		BOOL OnNcActivate(BOOL bActive);
		BOOL OnEraseBkgnd(HDC hdc);
		BOOL OnSetCursor(HWND hWnd, UINT nHitTest, UINT message);
		UINT OnNcHitTest(CPoint point);
		int OnCreate(LPCREATESTRUCT lpCreateStruct);
		LRESULT OnNcCreate(LPCREATESTRUCT lpCreateStruct);						///< ���OnNcCreate����false�����ڽ����ᱻ������
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);
		LRESULT OnMouseEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);			///< ���ڴ����������Ϣ
		LRESULT OnKeyEvent(UINT uMsg, WPARAM wParam, LPARAM lParam);			///< ������Ϣ���ݣ��罹��
		LRESULT OnHostMsg(UINT uMsg, WPARAM wParam, LPARAM lParam);


		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DM_AnimateWindow(DWORD dwTime,DWORD dwFlags);					    ///< ����Ч��
		DUIWindow* GetAnimateOwnerWnd();
		DMCode AnimateBegin_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};
		DMCode AnimateMid_Callback(IDMAnimate*pAni, WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};
		DMCode AnimateEnd_Callback(IDMAnimate*pAni,WPARAM wp, LPARAM lp){return DM_ECODE_NOTIMPL;};

		//---------------------------------------------------
		// Function Des: ��������
		//---------------------------------------------------
		DMCode OnSetCaptureWnd(DUIWND DUIWnd,DUIWNDPtr pOldDUIWnd=NULL);
		DMCode OnReleaseCaptureWnd();
		DMCode OnFireEvent(DMEventArgs &Evt);
		DMCode OnGetDraw(IDMDraw** ppObj);
		DMCode OnGetCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas**ppCanvas);
		DMCode OnReleaseCanvas(LPCRECT lpRect,DWORD dcFlags,IDMCanvas*pCanvas);
		HWND   OnGetHWnd();
		DMCode OnGetContainerRect(LPRECT lpRect);
		DMCode OnIsTranslucent();
		DMCode OnUpdateWindow();
		DMCode OnForceUpdateWindow();
		DMCode OnUpdateRect(LPCRECT lpRect,DUIWND hDUIWnd);
		DMCode OnClientToScreen(LPRECT lpRect);
		DMCode OnIsSizChanging();
		DMCode OnRegisterTimeline(IDMTimeline *pHandler);
		DMCode OnUnregisterTimeline(IDMTimeline *pHandler);
		CStringW OnGetTransId();

		//---------------------------------------------------
		// Function Des: spy++����
		//---------------------------------------------------
		LRESULT OnSpy(UINT uMsg, WPARAM wParam, LPARAM lParam);

	public:// ����
		DMCode DM_UpdateLayeredWindow(IDMCanvas* pCanvas,BYTE byAlpha, LPRECT lpRect);
		DMCode DM_UpdateShowCanvas(LPRECT lpRect);

		BOOL DM_EnableShadowEffect(BOOL bEnabled = TRUE, int margin = 1);

  protected:
		virtual void OnAfterCreated();							///< ���ڴ����ɹ������
		virtual void OnAfterClosed();							///< �����˳���,�ڲ��ж�����˴���������Ϣѭ��,�Զ�����WM_QUIT��Ϣ,�û������ش���

	public:
		DECLARE_MSG_MAP()
		DECLARE_EVENT_MAP()

		DM_BEGIN_ATTRIBUTES()
			DM_CHAIN_ATTRIBUTTE(m_pHWndXmlInfo)
		DM_END_ATTRIBUTES()

	public:
		// ������ --------------------------------------------
		DMSmartPtrT<IDMCanvas>                  m_pShowCanvas;
		DMSmartPtrT<IDMDraw>                    m_pDraw;

		// ��͸�������ɰ� ------------------------------------
		DMDummyWnd								m_dummyWnd;

		// �������� ------------------------------------------
		DMHWnd_Data								m_HWndData;
		DMSmartPtrT<DMHWnd_XmlInfo>			    m_pHWndXmlInfo;

		DMSmartPtrT<IDMToolTip>                 m_pToolTip;

		bool                                    m_bSizeChanging;///< size�ı䵽ˢ���ڼ�,��Ϊ��size�ı��ڣ���ʱ������ǿ�Ƹ��»�������Ϊ������ʱ��0��

		DMSmartPtrT<DMMsgLoop>                  m_pCurMsgLoop;  ///< ��ǰ��Ϣѭ������,���ų���ʹ�ö��̵߳�

	};
}//namespace DM
