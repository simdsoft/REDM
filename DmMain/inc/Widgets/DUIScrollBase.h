//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: 
// File Summary:�ؼ����㹫ʽ��nPage/(nMax-nMin+1)=���鳤��/���������ܳ��ȣ����ǻ���ɻ����Ĳۣ�
// Cur Version: 1.0
// Author:http://hgy413.com/1894.html
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-10	1.0			
//--------------------------------------------------------
#pragma once

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIScrollBase"/>��xml���Զ���
	/// </summary>
	class DUIScrollBaseAttr:public DUIWindowAttr
	{
	public:
		static const char* SKIN_sbskin;								    ///< ������ͼƬ,ʾ��:sbskin="ScrollBar"
		static const char* INT_sbwidth;									///< �������Ŀ�ȣ�Ĭ��Ϊ-1(������ͼƬ��/9),ʾ��:sbwidth="16"
		static const char* INT_arrowwidth;                                 ///< ���£����ң���ͷ�ĸ߶�(���),ʾ��:arrowwidth="10"
		static const char* INT_minithumblen;								///< ��С���ָ߶�, Ĭ��18, ʾ��:minithumblen="18"
		static const char* OPTION_sbenable;							    ///< �Ƿ����ù�����,��Ӧ��DMSB_NULL��DMSB_HORZ��DMSB_VERT,ʾ��:sbenable="1"
		static const char* SIZE_range;                                     ///< ������Χ,ʾ��:range="1,100"
		static const char* POINT_curpos;                                   ///< ��ǰ����λ��,xָ��ˮƽscroll��nPos,yָ����ֱscroll��nPos,ʾ��:curpos="10,20"
		static const char* bool_bnowheelscroll;                            ///< ���ù��ֹ�����ʾ��:bnowheelscroll="1"
		static const char* bool_bpagesplit;                                ///< Ĭ��Ϊtrue,��ʱ����SB_PAGEUP+SB_PAGEDOWN(���»�������),Ϊfalse��ʾ(��������=�ϻ���+������+�»���),ʾ��:bpagesplit="1"
	};
	DMAttrValueInit(DUIScrollBaseAttr,SKIN_sbskin)DMAttrValueInit(DUIScrollBaseAttr,INT_sbwidth)DMAttrValueInit(DUIScrollBaseAttr,INT_arrowwidth)DMAttrValueInit(DUIScrollBaseAttr,INT_minithumblen)
	DMAttrValueInit(DUIScrollBaseAttr,OPTION_sbenable)DMAttrValueInit(DUIScrollBaseAttr,SIZE_range)
	DMAttrValueInit(DUIScrollBaseAttr,POINT_curpos)DMAttrValueInit(DUIScrollBaseAttr,bool_bnowheelscroll)DMAttrValueInit(DUIScrollBaseAttr,bool_bpagesplit)
}


namespace DM
{
	/// <summary>
	///		 DUIScrollBase������ʵ��,������Ϊ������չscrollbar,���ԣ�<see cref="DMAttr::DUIScrollBaseAttr"/>
	/// </summary>
	class DM_EXPORT DUIScrollBase:public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIScrollBase,DUINAME_ScrollBase,DMREG_Window);
	public:
		DUIScrollBase();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		bool GetScrollInfo(bool bVert, LPSCROLLINFO lpsi);						///< ��ù�������Ϣ
		bool SetScrollInfo(bool bVert,LPSCROLLINFO lpsi, bool bRedraw);			///< ǿ�Ƹı��������Ϣ
		bool ShowScrollBar(int wBar, bool bShow);								///< �Ƿ���ʾ������
		bool EnableScrollBar(int wBar,bool bEnable);							///< �Ƿ���ù�����					
		bool SetScrollPos(bool bVert, int nNewPos,bool bRedraw);                ///< ���ù������µĹ���λ��
		bool SetScrollRange(bool bVert,int nMinPos,int nMaxPos,bool bRedraw);	///< ���ù������Ĺ�����Χ
		bool GetScrollRange(bool bVert,LPINT lpMinPos,LPINT lpMaxPos);			///< ��ù������Ĺ�����Χ
		bool IsScrollBarEnable(bool bVert);	                                    ///< �������Ƿ����							
		void SetScrollInfo(SCROLLINFO si,bool bVert);							///< ���������ù�������Ϣ
		int GetScrollPos(bool bVert);                                           ///< ��ȡ��������ǰλ��

		// ����scroll������
		virtual void UpdateScrollBar();											///< ����size��viewsize�Զ����㣬�Ƿ�����ˮƽ����ֱ������
		void SetRangeSize(CSize szRange);										///< ���ù�����Χ
		void SetRangeCurPos(CPoint pt);											///< ���õ�ǰ����λ��

		// �ؼ�������������
		virtual bool OnScroll(bool bVert,UINT uCode,int nPos);					///< ����,������������������ʵ�ֵ��������������������
		virtual int GetScrollLineSize(bool bVert){return 10;}					///< ������¼�ͷ��������

		virtual void OnRangeSizeChanged(CSize szOld,CSize szNew){}              ///< ��������Χ�ı�ʱ����
		virtual void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);           ///< ������λ�øı�ʱ����
		
	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_CREATE(OnCreate)
			MSG_WM_SIZE(OnSize)
			DM_MSG_WM_NCPAINT(DM_OnNcPaint)
			MSG_WM_NCLBUTTONDOWN(OnNcLButtonDown)
			MSG_WM_NCLBUTTONUP(OnNcLButtonUp)
			MSG_WM_NCMOUSEMOVE(OnNcMouseMove)
			MSG_WM_NCMOUSELEAVE(OnNcMouseLeave)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
			MSG_WM_NCCALCSIZE(OnNcCalcSize)
			MSG_WM_SHOWWINDOW(OnShowWindow)
			DM_MSG_WM_TIMER(DM_OnTimer)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		int OnCreate(LPVOID);
		void OnSize(UINT nType,CSize size);
		void DM_OnNcPaint(IDMCanvas *pCanvas);
		void OnNcLButtonDown(UINT nFlags, CPoint point);
		void OnNcLButtonUp(UINT nFlags,CPoint pt);
		void OnNcMouseMove(UINT nFlags, CPoint point) ;
		void OnNcMouseLeave();
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		LRESULT OnNcCalcSize(BOOL bCalcValidRects, LPARAM lParam);// �˴�����ͻ�����С
		void OnShowWindow(BOOL bShow, UINT nStatus); 
		void DM_OnTimer(char cTimerID);

	public:
		//---------------------------------------------------
		// Function Des: ����
		//---------------------------------------------------
		DMCode DV_GetClientRect(LPRECT lpRect);										///< ��ʱ���������ͻ���Ҫ�ų�������
		DMCode DV_OnNcHitTest(CPoint pt);											///< ģ��ǿͻ���
		DMCode DV_UpdateSkin(WPARAM wp, LPARAM lp);								    ///< ����
		
	public:
		bool HasScrollBar(bool bVert);												///< �Ƿ���ڹ�������ˮƽ/��ֱ��
		CRect GetScrollBarRect(bool bVert);											///< ��ù�������������ˮƽ/��ֱ��
		CRect GetSbPartRect(bool bVert,UINT uSBCode);								///< ��ȡ������ָ��ĳ���ֵ�����
		CRect GetSbRailwayRect(bool bVert);											///< ��ȡ����������ͷ�������
		CRect GetSbSlideRectByPos(bool bVert,int nPos);								///< ����nPos��ù����������ڵ�λ��
		SBSTATEINFO HitTest(CPoint pt);												///< ��ȡ״̬����Ϣ���
		int GetSbSlideLength(bool bVert);											///< ��ȡ��������ĳ���
		void ScrollUpdateWindow();													///< ���������µ�������ʾ

	public:
		void DrawScrollBar(IDMCanvas *pCanvas,LPCRECT lpRectDraw,int iSbCode,int iState,bool bVert, BYTE alpha=0xff);
		void DrawMoreScrollBar(IDMCanvas *pCanvas,int iSbCode,int iState,bool bVert, BYTE alpha=0xff);
		int CalcState(bool bVert,int sbCode, SBSTATEINFO& sbCmpInfo);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::bool_bnowheelscroll,m_bnowheelscroll,DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::bool_bpagesplit,m_bpagesplit,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::SKIN_sbskin, OnAttributesbSkin)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_sbwidth,OnAttributesbWidth)
			DM_INT_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_arrowwidth,m_isbAllowSize,DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::INT_minithumblen,m_iMiniThumbLen,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::OPTION_sbenable,OnAttributesbEnable)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::SIZE_range,OnAttributeRange)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIScrollBaseAttr::POINT_curpos,OnAttributePos)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributesbSkin(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributesbWidth(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributesbEnable(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributeRange(LPCSTR lpszValue, bool bLoadXml);
		DMCode OnAttributePos(LPCSTR lpszValue, bool bLoadXml);

	public:
		bool                        m_bnowheelscroll;   ///< ��ֹͨ�����ֹ���
		bool                        m_bpagesplit;       ///< trueʱ����SB_PAGEUP+SB_PAGEDOWN(���»�������)
		DMSmartPtrT<IDMSkin>        m_psbSkin;			///< ������ͼ��
		CRect						m_rcsbClient;       ///< ��ǰ�ͻ�����(����й����������ȥ������)
		int							m_isbWid;			///< �������Ŀ��(Ĭ��ˮƽ����ֱ�����������ͬ,��ʹ��һ��������
		int							m_isbVisible;		///< ��������ʾ��Ϣ,��Ӧ��DMSB_NULL��DMSB_HORZ��DMSB_VERT
		int							m_isbEnable;	    ///< �������ɲ�����Ϣ,��Ӧ��DMSB_NULL��DMSB_HORZ��DMSB_VERT
		int                         m_isbAllowSize;     ///< ���������¼�ͷ�߶�(������)
		int                         m_iMiniThumbLen;    ///< ��С���ָ߶�

		SBSTATEINFO                 m_sbInfo;           ///< ��¼״̬����Ϣ��ϣ���DMScrollBarSkinImpl���Ӧ

		SCROLLINFO					m_siVer;			///< �ؼ��ṹ,�����ͨ�������������ƹ�������״̬����ֱ��
		SCROLLINFO                  m_siHoz;			///< �ؼ��ṹ,�����ͨ�������������ƹ�������״̬��ˮƽ��

		int							m_isbDragPos;		///< ��ӦSCROLLINFO.nPos(ˮƽ����ֱ)
		CPoint						m_sbDragPt;			///< �϶���λ�ã�x��ʾˮƽ��y��ʾ��ֱ)
		bool						m_bsbDrag;          ///< �϶�״̬����갴��ʱ�ڻ����ϣ�

		DWORD						m_dwUpdateTime;		///< ��¼����UpdateSWindow��ʱ��
		DWORD						m_dwUpdateInterval;
		
		CSize                       m_szRange;          ///< �������Ŀ��
		CPoint                      m_ptCurPos;         ///< xָ��ˮƽscroll��nPos,yָ����ֱscroll��nPos
		bool                        m_bUseRange;        ///< ���edit�����Զ������Ƿ���ʾ������������Ҫm_szRange + m_ptCurPos
	};

}//namespace DM
