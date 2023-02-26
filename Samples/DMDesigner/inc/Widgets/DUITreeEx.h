//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUITreeEx.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-26	1.0			
//-------------------------------------------------------
#pragma once
#include "DUIScrollBase.h"
#include "DUIItemPanel.h"
#include "DMTreeT.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITreeEx"/>��xml���Զ���
	/// </summary>
	class DUITreeAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* SKIN_toggleskin;								    ///< չ��������С����ͼ��,����״̬(��������+չ������),ʾ��:toggleskin="treetoggle"
		static const char* SKIN_checkskin;                                     ///< ѡ�С���ѡ�У���ѡ��ͼ��,����������checkbox״̬��9��״̬(��������+ѡ������+��ѡ������),ʾ��:checkskin="treecheck"
		static const char* SKIN_eyeskin;                                       ///< 9��״̬(��������+ѡ������+��ѡ������),ʾ��:eyeskin="treecheck"
		static const char* SKIN_lockskin;                                      ///< 9��״̬(��������+ѡ������+��ѡ������),ʾ��:lockskin="treecheck"
		
		static const char* SKIN_itembgskin;                                    ///< �����ڵı���ͼ��,����/ͣ��/ѡ��,ʾ��:itembgskin="itembgskin"
		static const char* COLOR_clritembg;                                    ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhoverbg;                               ///< item���ͣ������ɫ,ʾ��:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                 ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"
		static const char* INT_childoffset;                                    ///< ��������ڸ����ƫ��,�ɱ��������,ʾ��:childoffset="20"
		static const char* INT_itemheight;									    ///< ÿ��߶�,�ɱ��������,ʾ��:itemheight="20"
		static const char* INT_itemwidth;									    ///< ÿ����,�ɱ��������,ʾ��:itemwidth="20"
		static const char* bool_brightclicksel;                                ///< �Ƿ�֧���һ�ѡ��,ʾ��:brightclicksel="1"
		static const char* bool_bcheckbox;                                     ///< �Ƿ�����checkbox,ʾ��:bcheckbox="1"

		static const char* NODE_treeitem;										///< treectrl��xml������
		static const char* ITEM_bcollapsed;									///< ���Ƿ�����,ʾ��:bcollapsed="1"
		static const char* ITEM_height;                                        ///< ��ĸ߶�(��ѡ),ʾ��:height="10"
		static const char* ITEM_width;                                         ///< ��Ŀ��(��ѡ),ʾ��:weidth="10"
		static const char* ITEM_childoffset;                                   ///< ��Ŀ��(��ѡ),ʾ��:childoffset="10"
		static const char* ITEM_data;											///< �������,ʾ��:data="1"
	};
	DMAttrValueInit(DUITreeAttr,SKIN_toggleskin)DMAttrValueInit(DUITreeAttr,SKIN_checkskin)DMAttrValueInit(DUITreeAttr,SKIN_eyeskin)DMAttrValueInit(DUITreeAttr,SKIN_lockskin)
		DMAttrValueInit(DUITreeAttr,SKIN_itembgskin)DMAttrValueInit(DUITreeAttr,COLOR_clritembg)DMAttrValueInit(DUITreeAttr,COLOR_clritemhoverbg)
		DMAttrValueInit(DUITreeAttr,COLOR_clritemselbg)DMAttrValueInit(DUITreeAttr,INT_childoffset)DMAttrValueInit(DUITreeAttr,INT_itemheight)DMAttrValueInit(DUITreeAttr,INT_itemwidth)
		DMAttrValueInit(DUITreeAttr,bool_brightclicksel)DMAttrValueInit(DUITreeAttr,bool_bcheckbox)DMAttrValueInit(DUITreeAttr,NODE_treeitem)DMAttrValueInit(DUITreeAttr,ITEM_bcollapsed)
		DMAttrValueInit(DUITreeAttr,ITEM_height)DMAttrValueInit(DUITreeAttr,ITEM_width)DMAttrValueInit(DUITreeAttr,ITEM_childoffset)DMAttrValueInit(DUITreeAttr,ITEM_data)
}

namespace DM
{
	enum 
	{
		DMTVEX_COLLAPSE = 1,///< ����
		DMTVEX_EXPAND,	   ///< չ��
		DMTVEX_TOGGLE,	   ///< �Զ���ԭ����չ��������
	};

	enum ///< ��������
	{
		DMTVEXMask_Toggle         = 0x00000001UL, 
		DMTVEXMask_CheckBox       = 0x00000002UL, 
	};

	enum ///< ����Ƿ�ͣ����С���ǵľ�������
	{
		DMTVEXBtn_None,					               
		DMTVEXBtn_Toggle, 
		DMTVEXBtn_CheckBox,
		DMTVEXBtn_Eye,
		DMTVEXBtn_Lock,
	};

	enum 
	{
		DMTVEXCheckBox_UnChecked,			///< δѡ��
		DMTVEXCheckBox_Checked,				///< ѡ��
		DMTVEXCheckBox_PartChecked			///< ����ѡ��
	};

	enum 
	{
		DMTVEXLock_UnLocked,				///< δ����
		DMTVEXLock_Locked,					///< ����
		DMTVEXLock_PartLocked				///< ��������
	};
 
	enum 
	{ 
		DMTVEXEye_Eyed,						///< �ɼ�
		DMTVEXEye_UnEye,					///< ���ɼ�
		DMTVEXEye_PartUnEyed				///< ���ֲ��ɼ�
	};

	/// <summary>
	///		
	/// </summary>
	typedef struct stTVITMEX
	{
		stTVITMEX(DMXmlNode XmlNode, IDMItemPanelOwner* pItemOwner)
		{
			pPanel.Attach(new DUIItemPanel(XmlNode,pItemOwner));
			hItem = NULL;
			iHeight = iChildOffset = nLevel = NULL;
			lParam = NULL;
			bVisible = true;
			iWidth = 20; 
			iCheckValue = DMTVEXCheckBox_UnChecked;
			iLockValue = DMTVEXLock_UnLocked;
			iEyeValue = DMTVEXEye_Eyed;
			bCollapsed = true;
			bHasChildren = false;
			dwToggleState = DUIWNDSTATE_Normal;
			dwCheckBoxState	= DUIWNDSTATE_Normal;
			dwLockState = DUIWNDSTATE_Normal;
			dwEyeState = DUIWNDSTATE_Normal;
		}
		virtual~stTVITMEX()
		{
			pPanel.Release();
		}
		DMSmartPtrT<DUIItemPanel>       pPanel;			 ///< ���
		HDMTREEITEM						hItem;			 ///< �����еı�ʶ  
		int								nLevel;			 ///< �����Ĳ��	
		int								iHeight;		 ///< �߶�
		int								iWidth;			 ///< ���
		int                             iChildOffset;    ///< ��������ڸ����ƫ��,����toggleskinʱ,�ڼ���ʱ���Զ�����toggleskin��checkskin�Ŀ�ȣ�����Ҳ�ᱻȫ�ֵ�ichildoffsetӰ�쵽,��������һ�����Լ����ichildoffset=0��
		int								iCheckValue;	 ///< checkbox��ǰ״̬������,ѡ��,��ѡ�У�
		int                             iLockValue;      ///< lock�ĵ�ǰ״̬(����������������)
		int                             iEyeValue;       ///< eye�ĵ�ǰ״̬(���ɼ�,��ɼ�,�ɼ�)
		DWORD							dwCheckBoxState; ///< checkbox״̬(normal,hover,sel)
		DWORD							dwToggleState;	 ///< �۵�С���ǵ�״̬(normal,hover,sel)
		DWORD                           dwLockState;     ///< lock�ĵ�״̬(normal,hover,sel)
		DWORD                           dwEyeState;      ///< eye�ĵ�״̬(normal,hover,sel)
		
		bool							bCollapsed;      ///< �۵�״̬
		bool							bVisible;		 ///< �Ƿ�ɼ�
		bool							bHasChildren;    ///< �Ƿ��������
		LPARAM							lParam;			 ///< data
	}TVITEMEX,*LPTVITEMEX;


	/// <summary>
	///		 DUITreeEx������ʵ�֣����ԣ�<see cref="DMAttr::DUITreeAttr"/>
	/// </summary>
	class DUITreeEx 
		: public DUIScrollBase
		, public IDMItemPanelOwner
		, public DMTreeT<LPTVITEMEX>
	{
		DMDECLARE_CLASS_NAME(DUITreeEx, "treeex",DMREG_Window)
	public:
		DUITreeEx();
		~DUITreeEx();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		bool RemoveAllItems();																			///< �Ƴ����н��
		bool RemoveItem(HDMTREEITEM hItem);
		bool SetItemRect(HDMTREEITEM hItem, CRect rcItem);												///< ����ĳ�������

		///---------------------------------
		/// ���뺯��
		HDMTREEITEM InsertItem(LPTVITEMEX pData,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,BOOL bEnsureVisible);
		HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,BOOL bEnsureVisible=FALSE);

		///---------------------------------
		/// ��ȡϵ�к������ɲο�CTreeCtrl�ĳ�Ա�����б�
		HDMTREEITEM GetRootItem();
		HDMTREEITEM GetRootItem(HDMTREEITEM hItem);
		HDMTREEITEM GetNextSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetPrevSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetChildItem(HDMTREEITEM hItem,bool bFirst =true);
		HDMTREEITEM GetParentItem(HDMTREEITEM hItem);
		HDMTREEITEM GetSelectedItem();
		HDMTREEITEM GetNextItem(HDMTREEITEM hItem);
		bool SelectItem(HDMTREEITEM hItem,bool bEnsureVisible=true);
		bool HoverItem(HDMTREEITEM hItem,bool bEnsureVisible=true);
		bool ItemHasChildren(HDMTREEITEM hItem);
		bool SetItemData(HDMTREEITEM hItem, LPARAM lParam);
		LPARAM GetItemData(HDMTREEITEM hItem) const;

		bool GetCheckState(HDMTREEITEM hItem) const;    
		bool SetCheckState(HDMTREEITEM hItem,bool bCheck); 
		bool SetLockState(HDMTREEITEM hItem,bool bLock);
		bool SetEyeState(HDMTREEITEM hItem,bool bUnEye);

		bool Expand(HDMTREEITEM hItem,UINT nCode = DMTVEX_EXPAND);
		bool EnsureVisible(HDMTREEITEM hItem,bool bFirstVisible = false);

	public:// ���ƣ�������
		virtual void DrawItem(IDMCanvas* pCanvas,CRect& rc,HDMTREEITEM hItem);
		virtual void RedrawItem(HDMTREEITEM hItem);

	public: 
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_RBUTTONDOWN(OnRButtonDown);
		MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			DM_MSG_WM_SETFOCUS(DM_OnSetFocus)
			DM_MSG_WM_KILLFOCUS(DM_OnKillFocus)
			MSG_WM_CHAR(OnChar)
			MSG_WM_KEYDOWN(OnKeyDown)
			MSG_WM_MOUSEWHEEL(OnMouseWheel)
			MESSAGE_RANGE_HANDLER_EX(WM_MOUSEFIRST,WM_MOUSELAST,OnMouseEvent)
			MESSAGE_RANGE_HANDLER_EX(WM_KEYFIRST,WM_KEYLAST,OnKeyEvent)
			MESSAGE_RANGE_HANDLER_EX(WM_IME_STARTCOMPOSITION,WM_IME_KEYLAST,OnKeyEvent)
			MESSAGE_HANDLER_EX(WM_IME_CHAR,OnKeyEvent)
			DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnDestroy();
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnRButtonDown(UINT nFlags, CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void DM_OnSetFocus();
		void DM_OnKillFocus();
		void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
		void OnKeyDown(TCHAR nChar, UINT nRepCnt, UINT nFlags);
		BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
		LRESULT OnMouseEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);
		LRESULT OnKeyEvent(UINT uMsg,WPARAM wParam,LPARAM lParam);

	public:
		//---------------------------------------------------
		// Function Des: �����غ���
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		virtual DMCode DV_OnSetCursor(const CPoint &pt);
		virtual DMCode DV_OnUpdateToolTip(CPoint pt, DMToolTipInfo &tipInfo);
		virtual void LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem);
		virtual void LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEMEX pData);      	
		virtual void OnNodeFree(LPTVITEMEX &pItemData);
		virtual int GetScrollLineSize(bool bVert){return m_iDefItemHei;};


		// ����ʵ��
		DUIWindow* GetOwnerWindow();   
		DMCode OnSetCapture(DUIItemPanel* pPanel);
		DMCode OnReleaseCapture(DUIItemPanel* pPanel);
		DMCode OnGetPanelRect(DUIItemPanel* pPanel,LPRECT lpRect);							///< ��ʹ��*lpRect��ֵ��

	public:// ����
		void SetChildrenVisible(HDMTREEITEM hItem,bool bVisible);
		void SetChildrenState(HDMTREEITEM hItem,int iCheckValue);
		void CheckState(HDMTREEITEM hItem, bool bCheck,bool bCheckChild = true);  
		bool CheckChildrenState(HDMTREEITEM hItem,bool bCheck);    
		bool IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2);         // hItem2�Ƿ�ΪhItem1������
		bool ItemIsValid(HDMTREEITEM hItem);

		void ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyLockState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyEyeState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt,CPoint ptsrc);
		void ItemMouseLeave(HDMTREEITEM hItem);

		// eye+lock
		bool IsParentLocked(HDMTREEITEM hItem);
		bool IsParentUnEyed(HDMTREEITEM hItem);
		void SetChildrenLockState(HDMTREEITEM hItem,bool bLocked);
		void SetChildrenEyeState(HDMTREEITEM hItem,bool bUnEye);


		// λ�����
		HDMTREEITEM HitTest(CPoint &pt);													///< �Զ��޸�pt��λ��Ϊ��Ե�ǰ���ƫ����(�紦��XOffset��һ������,��pt.xΪ��ֵ)
		int ItemHitTest(HDMTREEITEM hItem,CPoint &pt, CPoint &ptsrc);						///<����pt�ѱ�ת����Ϊ��Ե�ǰ���ƫ������ptsrcΪԭʼ��,ֻ���ж�x����

		int GetTotalHeight();
		int GetTotalWidth();
		int GetItemHeightWithAllChild(HDMTREEITEM hItem);									///< ����ĳһ���Լ��������пɼ�������ܸ߶�
		int GetItemWidthWithAllChild(HDMTREEITEM hItem);									///< ����ĳһ���Լ��������пɼ�������ܿ��
		int GetItemXOffset(HDMTREEITEM hItem);                                              ///< ����ĳ�������ƫ�ƣ����ɼ�����Ϊ0��
		int GetItemYOffset(HDMTREEITEM hItem);                                              ///< ����ĳ�������ƫ�ƣ����ɼ�����Ϊ0��
		bool GetItemRect(HDMTREEITEM hItem,CRect &rcItem);									///< ����ĳ������(��rcClient��ͬһ����ϵ��
		bool GetToggleRect(HDMTREEITEM hItem,CRect &rcToggle);                              ///< ����ĳ��toggle����(��rcClient��ͬһ����ϵ��
		bool GetCheckBoxRect(HDMTREEITEM hItem,CRect &rcCheckBox);                          ///< ����ĳ��checkbox����(��rcClient��ͬһ����ϵ��
		bool GetLockRect(HDMTREEITEM hItem,CRect &rcLock);								///< ����ĳ��lock����(��rcClient��ͬһ����ϵ��
		bool GetEyeRect(HDMTREEITEM hItem,CRect &rcEye);								///< ����ĳ��eye����(��rcClient��ͬһ����ϵ��
		void UpdateScrollRangeSize();

	public:
		DM_BEGIN_ATTRIBUTES()  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_toggleskin, m_pToggleSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_checkskin, m_pCheckSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_eyeskin, m_pEyeSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_lockskin, m_pLockSkin, DM_ECODE_NOXMLLOADREFRESH)  
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeAttr::SKIN_itembgskin, m_pItemBgSkin, DM_ECODE_NOXMLLOADREFRESH)  

			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_childoffset, m_iDefChildOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_itemheight,  m_iDefItemHei,     DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeAttr::INT_itemwidth,  m_iDefItemWid,     DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeAttr::bool_brightclicksel, m_bRightClickSel, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeAttr::bool_bcheckbox, m_bCheckBox, DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritembg,		m_crItemBg[0],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeAttr::COLOR_clritemselbg,	m_crItemBg[2],DM_ECODE_NOXMLLOADREFRESH)
			DM_END_ATTRIBUTES()

	public:
		IDMSkinPtr					m_pToggleSkin;			///< С����6��ͼ
		IDMSkinPtr					m_pCheckSkin;			///< checkbox9��ͼ
		IDMSkinPtr		            m_pEyeSkin;
		IDMSkinPtr                  m_pLockSkin;
		IDMSkinPtr					m_pItemBgSkin;			///< ��������ͼ
		DMColor					    m_crItemBg[3];			///< ��������ɫ

		int						    m_iDefItemHei;		    ///< ���Ĭ�ϸ߶�
		int						    m_iDefItemWid;		    ///< ���Ĭ�Ͽ��
		int							m_iDefChildOffset;      ///< ������Ը���Ĭ��ƫ��

		bool						m_bRightClickSel;       ///< �Ƿ�֧���һ�ѡ��
		bool						m_bCheckBox;			///< �Ƿ�����checkbox



		// ����
		HDMTREEITEM					m_hSelItem;				///< ��ǰѡ����
		HDMTREEITEM					m_hHoverItem;           ///< ��ǰͣ����
		HDMTREEITEM					m_hCaptureItem;         ///< ���capture��(������������ٻظ�)

		// 
		int							m_nItemHoverBtn;		///< ��ӦС���Ǻ�checkbox
		int							m_nItemPushDownBtn;

		//
		DMSmartPtrT<DUIItemPanel>	m_pCapturePanel;		///< ��ǰ������setcapture�����,�����ֻ����갴�µ�˲�����ã��������
	};

}//namespace DM