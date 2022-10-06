// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIRectTracker.h
// File mark:   
// File summary:��Ƥ���״β�����ɺ�,�ڲ������ó��˾��Բ���,Ҳ����posֻ�е�һ������Ч��,����ȫ�������ֶ����ã�
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2019-6-20
// ----------------------------------------------------------------
#pragma once

namespace DMAttr
{
	class DUIRectTrackerAttr: public DUIWindowAttr
	{
	public:
		static const char* SKIN_moveskin;                                                 ///< moveСͼ�꣬ʾ��:moveskin="bg"
		static const char* COLOR_clrdot;				                                   ///< dot������ɫ��ʾ��:clrdot="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrdotgray;				                               ///< dot�ҵ���ɫ��ʾ��:clrdotgray="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrboxsolid;                                             ///< box��PS_SOLID������ɫ,box��ʹ��PS_SOLID����һ�Σ���ʹ��PS_DASH����һ��,ʾ��:clrboxsolid="pbgra(ff,00,ff,ff)"
		static const char* COLOR_clrboxdash;                                              ///< box��PS_DASH������ɫ,box��ʹ��PS_SOLID����һ�Σ���ʹ��PS_DASH����һ��,ʾ��:clrboxdash="pbgra(ff,00,ff,ff)"
		static const char* INT_dotwidth;												   ///< dot�Ŀ��,Ĭ��Ϊ6,ʾ��:dotwidth="6"
		static const char* bool_bmain;													   ///< Ϊtrueʱֻ�����½�����dot�ǿ��϶�,����dotȫΪ��,ʾ��:bmain="0"
		static const char* bool_ballgray;												   ///< Ϊtrueʱ,dotȫΪ��,ʾ��:ballgray="0"
	};
	DMAttrValueInit(DUIRectTrackerAttr,SKIN_moveskin)DMAttrValueInit(DUIRectTrackerAttr,COLOR_clrdot)DMAttrValueInit(DUIRectTrackerAttr,COLOR_clrdotgray)
	DMAttrValueInit(DUIRectTrackerAttr, COLOR_clrboxsolid)DMAttrValueInit(DUIRectTrackerAttr, COLOR_clrboxdash)
	DMAttrValueInit(DUIRectTrackerAttr,INT_dotwidth)DMAttrValueInit(DUIRectTrackerAttr,bool_bmain)DMAttrValueInit(DUIRectTrackerAttr,bool_ballgray)
}


namespace DM
{
	/// <summary>
	///		����������
	/// </summary>
	enum DMDSDOT
	{
		DMDSDOT_MOVE,
		DMDSDOT_LEFT,
		DMDSDOT_LEFTOP,
		DMDSDOT_TOP,
		DMDSDOT_RIGHTOP,
		DMDSDOT_RIGHT,
		DMDSDOT_RIGHTBTM,
		DMDSDOT_BTM,
		DMDSDOT_LEFTBTM,
		DMDSDOT_BOX,
	};


	class DMDragMeta;
	typedef DMDelegate<void(DMDragMeta&, int)> DMDragMetaEventHandler;
	/// <summary>
	///		���������,�Լ��¼�����,�û���������ʵ���Զ�����ƺ��¼�
	/// </summary>
	class DMDragMeta
	{
	public:
		DMDragMeta(int idx=-1,HCURSOR hCursor = ::LoadCursor(NULL,IDC_ARROW));

	public:
		virtual void OnRender(IDMCanvas* pCanvas);									///< ����dot����
		virtual void OnRenderCursor();												///< ����dot����ʾ�Ĺ��

	public:// ����
		DMCode AutoDrawRoundRect(IDMCanvas*pCanvas,DMColor TextClr,int iStyle,int iWidth,LPCRECT lpRect, const POINT &pt);

	public:
		int									   m_Index;								 ///< 0Ϊ��������dot�ľ����������ؼ�����,1Ϊ����dot��˳ʱ��8���㣬9Ϊ���Ͻǵ�movСͼ��,��ӦDMDSDOT
		CRect								   m_Rect;								 ///< �˵�ľ�����,0Ϊ���տ�ѡ��box����Ĭ��1-9�㶼��box��Χ����ռ��box��
		HCURSOR								   m_hCursor;							 ///< ͣ���ڴ�dotʱ��ʾ�Ĺ��
		DMColor                                m_crDot[2];                          ///< dot�������ҵ�ʱ�����ɫ
		DMColor                                m_crBox[2];                           ///< box�����ɫ(box���Ȼ���PS_SOLID, �ٻ���PS_DASH)
		DMSmartPtrT<IDMSkin>				   m_pMoveSkin;							 ///< ����movСͼ��
		bool								   m_bDragDot;							 ///< �Ƿ�Ϊ�϶���(��������0Ϊ�����϶���)
		bool								   m_bEnable;							 ///< �˵��Ƿ����
		DMDragMetaEventHandler				   m_DragMetaAction;					 ///< ����ʱ�ĵ����¼�
		
	};

	class DM_EXPORT DUIRectTracker : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIRectTracker,DUINAME_RectTracker,DMREG_Window);
	public:
		DUIRectTracker();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ� methods
		//---------------------------------------------------
		bool SetBoxRect(CRect& rect, bool bFire = true);							///< ����falseʱ��ʾrect���ڲ��ı�			
		void InitDragMeta(CRect rcBox,bool bMain=false,bool bAllGray=false);	    ///< rcBoxָ�ؼ���,	bMainʱֻ�����½�����dot�ǿ��϶�	
		DMDragMeta* HitTestDragMeta(CPoint pt);										
		CRect CalcTrackerRect(CRect rcBox);											///< ��box�������Trackerռ������
		CRect CalcBoxRect(CRect rcTracker);											///< ��Trackerռ�������Ƶ�����ǰ��box��


	public:// ����
		//---------------------------------------------------
		// Function Des: ���� methods
		//---------------------------------------------------
		void LinkDragMetaEvent();
		void OnDragBox(DMDragMeta& meta, int iAction);								///< �϶��ڲ�
		void OnDragLeft(DMDragMeta& meta, int iAction);								///< �϶����
		void OnDragLeftTop(DMDragMeta& meta, int iAction);							///< �϶����Ͻ�
		void OnDragTop(DMDragMeta& meta, int iAction);								///< �϶��ϱ�
		void OnDragRightTop(DMDragMeta& meta, int iAction);							///< �϶����Ͻ�
		void OnDragRight(DMDragMeta& meta, int iAction);							///< �϶��ұ�
		void OnDragRightBottom(DMDragMeta& meta, int iAction);						///< �϶����½�
		void OnDragBottom(DMDragMeta& meta, int iAction);							///< �϶��±�
		void OnDragLeftBottom(DMDragMeta& meta, int iAction);						///< �϶����½�
		void OnDragMove(DMDragMeta& meta, int iAction);								///< �ƶ�
		void InternalHandleDrag(CRect& rect, int* pHori, int* pVert);
		

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnSize(UINT nType, CSize size);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::SKIN_moveskin,OnAttributeMoveSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrdot, OnAttributeDotClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrdotgray, OnAttributeDotGrayClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrboxsolid, OnAttributeBoxSolidClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::COLOR_clrboxdash, OnAttributeBoxDashClr)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::INT_dotwidth, OnAttributeDotWidth)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::bool_bmain, OnAttributeIsMain)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIRectTrackerAttr::bool_ballgray, OnAttributeIsAllGray)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeMoveSkin(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotGrayClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBoxSolidClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeBoxDashClr(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeDotWidth(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeIsMain(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeIsAllGray(LPCSTR lpszValue, bool bLoadXml);

	public:
		DMColor                                 m_crDot[2];                        ///< �������������ҵ�ʱ�����ɫ
		DMColor                                 m_crBox[2];                         ///< box�����ɫ(box���Ȼ���PS_SOLID, �ٻ���PS_DASH)
		DMSmartPtrT<IDMSkin>				    m_pMoveSkin;						///< ����movСͼ��
		int                                     m_dotWidth;                         ///< Dot�Ŀ��
		bool								    m_bMain;							///< Ϊtrueʱֻ�����½�����dot�ǿ��϶�
		bool                                    m_bAllGray;							///< Ϊtrueʱdotȫ��

		//
		CPoint								    m_StartDragPt;
		CPoint									m_TrackDragPt;
		CRect									m_StartDragRc;						///< ��ʼ�϶�ʱԪ�������С

		DMDragMeta*								m_pCurdragMeta;
		DMDragMeta								m_dragMetas[10];
		int										m_dragMetaCount;
		bool                                    m_bDown;							///< ֻ���ڰ���ʱͣ����dot�ϻ�box�ϲ�����m_bDown

		bool                                    m_bFirstInit;
	};

}