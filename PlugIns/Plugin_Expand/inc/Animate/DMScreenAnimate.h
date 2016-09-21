#pragma once
#include "DMAnimateWndBase.h"

namespace DM
{
	enum{DMSCREEN_4=0,DMSCREEN_MELT=1,};
	class DMScreenAnimate:public IDMAnimate,public DMAnimateWndBase
	{
		DMDECLARE_CLASS_NAME(DMScreenAnimate,L"DMScreenAnimate",DMREG_Animate);
	public:
		DMScreenAnimate();
	public:
		DMCode AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp, LPARAM lp);
		DMCode AnimateBegin(WPARAM wp, LPARAM lp);		
		DMCode AnitmateGet(WPARAM wp=0, LPARAM lp=0);  
		DMCode AnimateEnd(WPARAM wp, LPARAM lp);	

	public:// 动画类型
		void Scatter4(int count, int nSleeptime);
		void MeltLeft(int count,int nSleeptime);
		void MeltTop(int count,int nSleeptime);

	public:
		DECLARE_MESSAGE_MAP()										// 仿MFC消息映射宏

	public:
		void OnPaint(HDC hdc);
		void OnTimer(UINT_PTR idEvent);

	public:
		DMAnimateWndBase*                   m_pTempWnd;
		IDMAnimateOwner*				    m_pOwner;
		DMSmartPtrT<IDMCanvas>              m_pStartCanvas;
		byte                                m_byAlpha;
	};


}//namespace DM