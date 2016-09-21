#pragma once

namespace DM
{
#define TIMER_FADE                     1111
	class DMAnimateWndBase : public DMCWnd
	{
	public:
		DMAnimateWndBase();

	public:
		DMCode BeginFade(bool bEnd,int iSpeed,int iTimerDelta);
		virtual DMCode StartFadeFinish(){return DM_ECODE_NOTIMPL;};
		virtual DMCode EndFadeFinish(){return DM_ECODE_NOTIMPL;};
	public: 
		virtual DMCode Create(LPCRECT lpRect,DWORD dwStyle=WS_POPUP,DWORD dwExStyle=WS_EX_TRANSPARENT|WS_EX_TOOLWINDOW|WS_EX_TOPMOST); 

	public:
		void OnTimer(UINT_PTR idEvent);
	public:// 辅助
		BOOL DMANI_ClientToScreen(HWND hWnd,CRect &rc);
		BOOL UpdateLayerWnd(IDMCanvas*pCanvas,byte alpha,CPoint pt=CPoint(0,0));

	public:
		DECLARE_MESSAGE_MAP()										// 仿MFC消息映射宏

	public:

		// 控制淡入淡出--------------------------------------------
		int                                 m_byFadeAlpha;				///< 渐显渐隐时的alpha
		int                                 m_iFadeSpeed;				///< 渐显渐隐的速度
		bool                                m_bFadeEnd;					///< 为false表示渐显，为true表示渐隐
		bool                                m_bFadeing;					///< 正在渐显渐隐过程中

	};
}//namespace DM