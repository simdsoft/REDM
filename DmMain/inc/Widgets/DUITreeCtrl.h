#pragma once
#include "Widgets/DUIScrollBase.h"
#include "Common/Template/DMTreeT.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITreeCtrl"/>��xml���Զ���
	/// </summary>
	class DUITreeCtrlAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* SKIN_toggleskin;								    ///< չ��������С����ͼ��,ʾ��:toggleskin="treetoggle"
		static const char* SKIN_checkskin;                                     ///< ѡ�С���ѡ��ͼ��,����������checkbox״̬��ʾ��:checkskin="treecheck"
		static const char* SKIN_iconskin;                                      ///< ICON��ͼ��ʾ����iconSkin="menuicons"
		static const char* SKIN_itembgskin;                                    ///< �����ڵı���ͼ��,ʾ��:itembgskin="itembgskin"
		static const char* SKIN_itemhoverskin;                                 ///< ͣ����ı���ͼ��,ʾ��:itemhoverskin="itembgskin"
		static const char* SKIN_itemselskin;                                   ///< ѡ����ı���ͼ��,ʾ��:itemselskin="itemselskin"
		
		static const char* INT_childoffset;                                    ///< ��������ڸ����ƫ��,ʾ��:childoffset="20"
		
		static const char* INT_itemheight;									    ///< ÿ��߶�,ʾ��:itemheight="20"
		static const char* INT_itemmargin;                                     ///< ������ҿհ�,ʾ��:itemmargin="5"
		
		static const char* COLOR_clritembg;                                    ///< item��ı���ɫ,ʾ��:clritembg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhoverbg;                               ///< item���ͣ������ɫ,ʾ��:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemselbg;                                 ///< item���ѡ�б���ɫ,ʾ��:clritemselbg="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemtext;                                  ///< item����ı�ɫ,ʾ��:clritemtext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemhovertext;                             ///< item���ͣ���ı�ɫ,ʾ��:clritemhovertext="pbgra(ff,ff,ff,ff)"
		static const char* COLOR_clritemseltext;                               ///< item���ѡ���ı�ɫ,ʾ��:clritemseltext="pbgra(ff,ff,ff,ff)"

		static const char* bool_bcheckbox;                                     ///< �Ƿ�����checkbox,ʾ��:bcheckbox="1"
		static const char* bool_brightclicksel;                                ///< �Ƿ�֧���һ�ѡ��,ʾ��:brightclicksel="1"
		static const char* bool_bhover;                                        ///< �Ƿ�֧��hover,Ĭ��֧�֣�����д���hover���ܻ����͸����ʾ��:bhover="1"

		static const char* NODE_item;                                          ///< treectrl��xml������
		static const char* ITEM_text;                                          ///< ����ı�,ʾ��:text="2-1"
		static const char* ITEM_bcollapsed;                                    ///< ���Ƿ�����,ʾ��:bcollapsed="1"
		static const char* ITEM_icon;                                          ///< ���icon����,-1��ʾ��ʹ��icon,ʾ��:icon="0"
		static const char* ITEM_selicon;                                       ///< ���ѡ��icon����,ʾ��:selicon="0"
		static const char* ITEM_data;                                          ///< �������,ʾ��:data="1"

	};
	DMAttrValueInit(DUITreeCtrlAttr,SKIN_toggleskin)DMAttrValueInit(DUITreeCtrlAttr,SKIN_checkskin)DMAttrValueInit(DUITreeCtrlAttr,SKIN_iconskin)
	DMAttrValueInit(DUITreeCtrlAttr,SKIN_itembgskin)DMAttrValueInit(DUITreeCtrlAttr,SKIN_itemhoverskin)DMAttrValueInit(DUITreeCtrlAttr,SKIN_itemselskin)
	DMAttrValueInit(DUITreeCtrlAttr,INT_childoffset)
	DMAttrValueInit(DUITreeCtrlAttr,INT_itemheight)DMAttrValueInit(DUITreeCtrlAttr,INT_itemmargin)
	DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritembg)DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritemselbg)
	DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritemtext)DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritemhovertext)DMAttrValueInit(DUITreeCtrlAttr,COLOR_clritemseltext)
	DMAttrValueInit(DUITreeCtrlAttr,bool_bcheckbox)DMAttrValueInit(DUITreeCtrlAttr,bool_brightclicksel)DMAttrValueInit(DUITreeCtrlAttr,bool_bhover)
	DMAttrValueInit(DUITreeCtrlAttr,NODE_item)
	DMAttrValueInit(DUITreeCtrlAttr,ITEM_text)DMAttrValueInit(DUITreeCtrlAttr,ITEM_bcollapsed)DMAttrValueInit(DUITreeCtrlAttr,ITEM_icon)
	DMAttrValueInit(DUITreeCtrlAttr,ITEM_selicon)DMAttrValueInit(DUITreeCtrlAttr,ITEM_data)
}

namespace DM
{
#define TVE_COLLAPSE            0x0001			///< ����
#define TVE_EXPAND              0x0002          ///< չ��
#define TVE_TOGGLE              0x0003			///< �Զ���ԭ����չ��������
	
	enum ///< ��������
	{
		DMTVIMask_Toggle         = 0x00000001UL, 
		DMTVIMask_CheckBox       = 0x00000002UL, 
		DMTVIMask_Icon           = 0x00000004UL
	};

	enum 
	{
		DMTVICheckBox_UnChecked,			///< δѡ��
		DMTVICheckBox_Checked,				///< ѡ��
		DMTVICheckBox_PartChecked			///< ����ѡ��
	};

	enum ///< ����Ƿ�ͣ����check��С���ǵľ�������
	{
		DMTVIBtn_None,					               
		DMTVIBtn_Toggle, 
		DMTVIBtn_CheckBox,
	};

	typedef struct tagTVITEM 
	{    
		tagTVITEM()
		{
			nImage				 = -1;
			nSelectedImage		 = -1;
			lParam				 = NULL;

			hItem				 = NULL;
			bCollapsed			 = false;   // Ĭ��չ��
			bVisible			 = true;
			bHasChildren		 = false;
			nLevel				 = 0;
			nCheckBoxValue		 = DMTVICheckBox_UnChecked;
			nContentWidth			 = 0;
			dwToggleState		 = DUIWNDSTATE_Normal;
			dwCheckBoxState		 = DUIWNDSTATE_Normal;
		}

		CStringW        strText;
		int             nImage;
		int             nSelectedImage;
		LPARAM          lParam;    

		HDMTREEITEM     hItem;                    
		bool            bCollapsed;      // �۵�״̬
		bool			bVisible;
		bool			bHasChildren;
		int             nLevel;
		int             nCheckBoxValue;
		int				nContentWidth;
		DWORD			dwToggleState;
		DWORD			dwCheckBoxState;
	} TVITEM, *LPTVITEM;


	/// <summary>
	///		 DUITreeCtrl������ʵ�֣����ԣ�<see cref="DMAttr::DUITreeCtrlAttr"/>
	/// </summary>
	class DM_EXPORT DUITreeCtrl
		: public DUIScrollBase
		, public DMTreeT<LPTVITEM>
	{
		DMDECLARE_CLASS_NAME(DUITreeCtrl, DUINAME_TreeCtrl,DMREG_Window)
	public:
		DUITreeCtrl();
		virtual ~DUITreeCtrl();
		void Init();
		void UnInit();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		bool RemoveAllItems();																			///< �Ƴ����н��
		bool RemoveItem(HDMTREEITEM hItem);

		///---------------------------------
		/// ���뺯��
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, int nImage,
			int nSelectedImage, HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, int nImage,
			int nSelectedImage, LPARAM lParam,
			HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);            
		HDMTREEITEM InsertItem(LPTVITEM pItemObj,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,bool bEnsureVisible);
		HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=FALSE);

		///---------------------------------
		/// ��ȡϵ�к������ɲο�CTreeCtrl�ĳ�Ա�����б�
		HDMTREEITEM GetRootItem();
		HDMTREEITEM GetNextSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetPrevSiblingItem(HDMTREEITEM hItem);
		HDMTREEITEM GetChildItem(HDMTREEITEM hItem,bool bFirst =TRUE);
		HDMTREEITEM GetParentItem(HDMTREEITEM hItem);
		HDMTREEITEM GetSelectedItem();
		HDMTREEITEM GetNextItem(HDMTREEITEM hItem);
		bool SelectItem(HDMTREEITEM hItem,bool bEnsureVisible=TRUE);
		CStringW GetItemText(HDMTREEITEM hItem) const;
		bool SetItemText(HDMTREEITEM hItem, LPCWSTR lpszItem);
		bool GetItemImage(HDMTREEITEM hItem, int& nImage, int& nSelectedImage) const;
		LPARAM GetItemData(HDMTREEITEM hItem) const;
		bool SetItemData(HDMTREEITEM hItem, LPARAM lParam);
		bool ItemHasChildren(HDMTREEITEM hItem);

		bool GetCheckState(HDMTREEITEM hItem) const;    
		bool SetCheckState(HDMTREEITEM hItem, bool bCheck);   

		bool Expand(HDMTREEITEM hItem , UINT nCode = TVE_EXPAND );
		bool EnsureVisible(HDMTREEITEM hItem);

	public:// ���ƣ�������
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, HDMTREEITEM hItem);
		virtual void RedrawItem(HDMTREEITEM hItem);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_ERASEBKGND(DM_OnEraseBkgnd)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_DESTROY(OnDestroy)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_RBUTTONDOWN(OnRButtonDown);
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		BOOL DM_OnEraseBkgnd(IDMCanvas* pCanvas);
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnDestroy();
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnRButtonDown(UINT nFlags, CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();

	public:
		//---------------------------------------------------
		// Function Des: �����غ���
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);							    ///< ��������ʱ���õ�ǰ�Ĺ��
		virtual void LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem);
		virtual void LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEM pItem);            

		virtual int GetScrollLineSize(bool bVert);
		virtual void ItemLayout();														///< �������οؼ�һ��Ļ�����				
		virtual int  CalaItemContentWidth(LPTVITEM pItem);
		virtual int  CalcItemWidth(const LPTVITEM pItem);
		virtual int  GetMaxItemWidth();
		virtual int  GetMaxItemWidth(HDMTREEITEM hItem);

		virtual void OnNodeFree(LPTVITEM & pItemData);
	public:// ����

		HDMTREEITEM HitTest(CPoint &pt);    
		void PageUp();
		void PageDown();

		void ItemLButtonDown(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonUp(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemLButtonDbClick(HDMTREEITEM hItem, UINT nFlags,CPoint pt);

		void ItemMouseMove(HDMTREEITEM hItem, UINT nFlags,CPoint pt);
		void ItemMouseLeave(HDMTREEITEM hItem);

		void ModifyToggleState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);
		void ModifyChekcBoxState(HDMTREEITEM hItem, DWORD dwStateAdd, DWORD dwStateRemove);

		bool IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2);         // hItem2�Ƿ�ΪhItem1������
		void SetChildrenVisible(HDMTREEITEM hItem,bool bVert);
		void SetChildrenState(HDMTREEITEM hItem, int nCheckValue);
		bool CheckChildrenState(HDMTREEITEM hItem, bool bCheck);    	// ������״̬һ�·���true, ���򷵻�false
		void CheckState(HDMTREEITEM hItem, bool bCheck, bool bCheckChild = TRUE);    

		int  GetItemShowIndex(HDMTREEITEM hItemObj);
		bool GetItemRect(LPTVITEM pItem,CRect &rcItem);
		int ItemHitTest(HDMTREEITEM hItem,CPoint &pt) const;

	public:
		DM_BEGIN_ATTRIBUTES()       
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_toggleskin,OnAttrCustomEx)  
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_checkskin, OnAttrCustomEx)
			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_iconskin,  OnAttrCustomEx)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_itembgskin, m_pItemBgSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_itemhoverskin, m_pItemHoverSkin, DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::SKIN_itemselskin, m_pItemSelSkin, DM_ECODE_NOXMLLOADREFRESH)
			
			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::INT_childoffset, m_iChildOffset, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::INT_itemheight,  m_iItemHei,     DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::INT_itemmargin, m_iItemMargin,   DM_ECODE_NOXMLLOADREFRESH)

			DM_CUSTOMEX_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::bool_bcheckbox, OnAttrCustomEx)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::bool_brightclicksel, m_bRightClickSel, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::bool_bhover, m_bHover, DM_ECODE_NOXMLLOADREFRESH)
		
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritembg,m_crItemBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritemhoverbg,m_crItemHoverBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritemselbg,m_crItemSelBg,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritemtext,m_crItemText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritemhovertext,m_crItemHoverText,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUITreeCtrlAttr::COLOR_clritemseltext,m_crItemSelText,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttrCustomEx(LPCSTR lpszAttribute, LPCSTR lpszValue, bool bLoadXml);

	public:
		HDMTREEITEM					m_hSelItem;				///< ��ǰѡ����
		HDMTREEITEM					m_hHoverItem;           ///< ��ǰͣ����
		HDMTREEITEM					m_hCaptureItem;         ///< ���capture��

		int							m_nVisibleItems;		///< �ɼ�����
		int							m_nMaxItemWidth;        ///< ��������

		CRect						m_rcToggle;				///< С����ͼ��
		CRect						m_rcCheckBox;			///< checkboxͼ��
		CRect						m_rcIcon;               ///< СIconͼ��

		int							m_iChildOffset;         ///< ������Ը���ƫ��
		int							m_iItemHei;             ///< ��߶�    
		int							m_iItemMargin;			///< ������ҿհ�
		int							m_iTextOffset;          ///< �ı������ƫ��

		bool						m_bCheckBox;			///< �Ƿ�����checkbox
		bool						m_bRightClickSel;       ///< �Ƿ�֧���һ�ѡ��
		bool                        m_bHover;

		IDMSkinPtr					m_pItemBgSkin;
		IDMSkinPtr                  m_pItemHoverSkin;
		IDMSkinPtr					m_pItemSelSkin;
		IDMSkinPtr					m_pIconSkin;
		IDMSkinPtr					m_pToggleSkin;
		IDMSkinPtr					m_pCheckSkin;

		DMColor						m_crItemBg;
		DMColor                     m_crItemHoverBg;
		DMColor						m_crItemSelBg;
		DMColor						m_crItemText;
		DMColor                     m_crItemHoverText;
		DMColor						m_crItemSelText;

		UINT						m_uItemMask;             ///< Item�Ļ�������
		
		int							m_nItemHoverBtn;
		int							m_nItemPushDownBtn;
	};

}//namespace DM