#pragma once
#include "DUIScrollBase.h"
#include "DMTreeT.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUITreeCtrl"/>的xml属性定义
	/// </summary>
	class DUITreeCtrlAttr:public DUIScrollBaseAttr
	{
	public:
		static wchar_t* SKIN_toggleskin;								    ///< 展开、收起小三角图案,示例:toggleskin="treetoggle"
		static wchar_t* SKIN_checkskin;                                     ///< 选中、不选中图案,用于启用了checkbox状态，示例:checkskin="treecheck"
		static wchar_t* SKIN_iconskin;                                      ///< ICON连图，示例：iconSkin="menuicons"
		static wchar_t* SKIN_itembgskin;                                    ///< 项所在的背景图案,示例:itembgskin="itembgskin"
		static wchar_t* SKIN_itemhoverskin;                                 ///< 停留项的背景图案,示例:itemhoverskin="itembgskin"
		static wchar_t* SKIN_itemselskin;                                   ///< 选中项的背景图案,示例:itemselskin="itemselskin"
		
		static wchar_t* INT_childoffset;                                    ///< 子项相对于父项的偏移,示例:childoffset="20"
		
		static wchar_t* INT_itemheight;									    ///< 每项高度,示例:itemheight="20"
		static wchar_t* INT_itemmargin;                                     ///< 项的左右空白,示例:itemmargin="5"
		
		static wchar_t* COLOR_clritembg;                                    ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemhoverbg;                               ///< item项的停留背景色,示例:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                 ///< item项的选中背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemtext;                                  ///< item项的文本色,示例:clritemtext="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemhovertext;                             ///< item项的停留文本色,示例:clritemhovertext="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemseltext;                               ///< item项的选中文本色,示例:clritemseltext="pbgra(ff,ff,ff,ff)"

		static wchar_t* bool_bcheckbox;                                     ///< 是否启用checkbox,示例:bcheckbox="1"
		static wchar_t* bool_brightclicksel;                                ///< 是否支持右击选中,示例:brightclicksel="1"
		static wchar_t* bool_bhover;                                        ///< 是否支持hover,默认支持，面板中处理hover可能会出现透明，示例:bhover="1"

		static wchar_t* NODE_item;                                          ///< treectrl的xml项结点名
		static wchar_t* ITEM_text;                                          ///< 项的文本,示例:text="2-1"
		static wchar_t* ITEM_bcollapsed;                                    ///< 项是否收缩,示例:bcollapsed="1"
		static wchar_t* ITEM_icon;                                          ///< 项的icon索引,-1表示不使用icon,示例:icon="0"
		static wchar_t* ITEM_selicon;                                       ///< 项的选中icon索引,示例:selicon="0"
		static wchar_t* ITEM_data;                                          ///< 项的数据,示例:data="1"

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
#define TVE_COLLAPSE            0x0001			///< 收缩
#define TVE_EXPAND              0x0002          ///< 展开
#define TVE_TOGGLE              0x0003			///< 自动，原来是展开就收缩
	
	enum ///< 绘制属性
	{
		DMTVIMask_Toggle         = 0x00000001UL, 
		DMTVIMask_CheckBox       = 0x00000002UL, 
		DMTVIMask_Icon           = 0x00000004UL
	};

	enum 
	{
		DMTVICheckBox_UnChecked,			///< 未选中
		DMTVICheckBox_Checked,				///< 选中
		DMTVICheckBox_PartChecked			///< 部分选中
	};

	enum ///< 鼠标是否停留在check或小三角的矩形区域
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
			bCollapsed			 = false;   // 默认展开
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
		bool            bCollapsed;      // 折叠状态
		bool			bVisible;
		bool			bHasChildren;
		int             nLevel;
		int             nCheckBoxValue;
		int				nContentWidth;
		DWORD			dwToggleState;
		DWORD			dwCheckBoxState;
	} TVITEM, *LPTVITEM;


	/// <summary>
	///		 DUITreeCtrl的内置实现，属性：<see cref="DMAttr::DUITreeCtrlAttr"/>
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
		// Function Des: 对外接口
		//---------------------------------------------------
		bool RemoveAllItems();																			///< 移除所有结点
		bool RemoveItem(HDMTREEITEM hItem);

		///---------------------------------
		/// 插入函数
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, int nImage,
			int nSelectedImage, HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);
		HDMTREEITEM InsertItem(LPCWSTR lpszItem, int nImage,
			int nSelectedImage, LPARAM lParam,
			HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=TRUE);            
		HDMTREEITEM InsertItem(LPTVITEM pItemObj,HDMTREEITEM hParent,HDMTREEITEM hInsertAfter,bool bEnsureVisible);
		HDMTREEITEM InsertItem(DMXmlNode &XmlItem,HDMTREEITEM hParent=DMTVI_ROOT, HDMTREEITEM hInsertAfter=DMTVI_LAST,bool bEnsureVisible=FALSE);

		///---------------------------------
		/// 获取系列函数，可参考CTreeCtrl的成员函数列表
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

	public:// 绘制！！！！
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
		// Function Des: DUI的消息分发系列函数
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
		// Function Des: 可重载函数
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt);							    ///< 仅在项上时设置当前的光标
		virtual void LoadBranch(HDMTREEITEM hParent,DMXmlNode &XmlItem);
		virtual void LoadItemAttribute(DMXmlNode &XmlItem, LPTVITEM pItem);            

		virtual int GetScrollLineSize(bool bVert);
		virtual void ItemLayout();														///< 计算树形控件一项的绘制区				
		virtual int  CalaItemContentWidth(LPTVITEM pItem);
		virtual int  CalcItemWidth(const LPTVITEM pItem);
		virtual int  GetMaxItemWidth();
		virtual int  GetMaxItemWidth(HDMTREEITEM hItem);

		virtual void OnNodeFree(LPTVITEM & pItemData);
	public:// 辅助

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

		bool IsAncestor(HDMTREEITEM hItem1,HDMTREEITEM hItem2);         // hItem2是否为hItem1的子项
		void SetChildrenVisible(HDMTREEITEM hItem,bool bVert);
		void SetChildrenState(HDMTREEITEM hItem, int nCheckValue);
		bool CheckChildrenState(HDMTREEITEM hItem, bool bCheck);    	// 子孙结点状态一致返回true, 否则返回false
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
		DMCode OnAttrCustomEx(LPCWSTR lpszAttribute, LPCWSTR lpszValue, bool bLoadXml);

	public:
		HDMTREEITEM					m_hSelItem;				///< 当前选中项
		HDMTREEITEM					m_hHoverItem;           ///< 当前停留项
		HDMTREEITEM					m_hCaptureItem;         ///< 获得capture项

		int							m_nVisibleItems;		///< 可见项数
		int							m_nMaxItemWidth;        ///< 项的最大宽度

		CRect						m_rcToggle;				///< 小三角图案
		CRect						m_rcCheckBox;			///< checkbox图案
		CRect						m_rcIcon;               ///< 小Icon图案

		int							m_iChildOffset;         ///< 子项相对父项偏移
		int							m_iItemHei;             ///< 项高度    
		int							m_iItemMargin;			///< 项的左右空白
		int							m_iTextOffset;          ///< 文本的相对偏移

		bool						m_bCheckBox;			///< 是否启用checkbox
		bool						m_bRightClickSel;       ///< 是否支持右击选中
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

		UINT						m_uItemMask;             ///< Item的绘制属性
		
		int							m_nItemHoverBtn;
		int							m_nItemPushDownBtn;
	};

}//namespace DM