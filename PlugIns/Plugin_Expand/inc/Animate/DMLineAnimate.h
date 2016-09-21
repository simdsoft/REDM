#pragma once

namespace DM
{
	class DMLineAnimate:public IDMAnimate
	{
		DMDECLARE_CLASS_NAME(DMLineAnimate,L"DMLineAnimate",DMREG_Animate);
	public:
		DMLineAnimate();
	public:
		DMCode AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp, LPARAM lp);
		DMCode AnimateBegin(WPARAM wp, LPARAM lp);		
		DMCode AnitmateGet(WPARAM wp=0, LPARAM lp=0);  
		DMCode AnimateEnd(WPARAM wp, LPARAM lp);	
	public:
		IDMAnimateOwner*				    m_pOwner; 
	};

	//-----------------------------------
	enum DMLineAniType {	DMLineAniInit, DMLineAniDraw, DMLineAniErase, DMLineAniTerm};
	struct DMLineAniData
	{
		BOOL	bOpen;
		RECT	rcScreen;
		RECT	rcWnd;
		int		iParameter;
		int		iAfterimages;
		int		iTotalSteps;
		int		iStep;
		POINT	ptRelRightTop;
		POINT	ptCenter;
		HWND	hWnd;
		HDC		hDC;
		enum	DMLineAniType animType;
		BYTE	bBuffer[4000];
	};

	class DMLineAniEffect  
	{
	public:
		enum EffectType {
			Random=-1,
			Spin=0,
			Vortex,
			ScatterGather,
			Spike,
			Fireworks
		};

		typedef BOOL (*EFFUNC)( DMLineAniData * );

		DMLineAniEffect();
		virtual ~DMLineAniEffect();

		void Setup( int iSteps, int iAfterimages, int iDelay = 10);
		void Effect( EffectType effect );
		void Open( const CRect &rcWnd );
		void Close( const CRect &rcWnd );

	private:
		void Defaults();
		void ChooseFunc();
		void DMLineAniate( DMLineAniData &animData );

	private:
		RECT	m_rcScreen;
		int		m_iParameter;
		int		m_iAfterimages;
		int		m_iTotalSteps;
		int		m_iDelay;
		EffectType	m_Effect;
		EFFUNC	m_Func;

		RECT	m_rcWnd;
		POINT	m_ptRelRightTop;
		POINT	m_ptCenter;

		HWND	m_hWnd;
		DMAutoDC m_hDC;
		HPEN	m_hPen;
	};

}//namespace DM