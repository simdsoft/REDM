//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIHeaderCtrl.h 
// File Des: 内置列表头实现
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-13	1.0			
//--------------------------------------------------------
#pragma once
#include "DMDragWnd.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIHeaderCtrl"/>的xml属性定义
	/// </summary>
	class DUIHeaderCtrlAttr:public DUIWindowAttr
	{
	public:
		static wchar_t* SKIN_itemskin;                                             ///< item状态图,示例:itemskin="itemskin"
		static wchar_t* SKIN_sortskin;                                             ///< 默认的sort状态图(小三角),示例:sortskin="sortskin"
		static wchar_t* COLOR_clritembg;										   ///< item项的背景色,示例:clritembg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemhoverbg;									   ///< item项的停留背景色,示例:clritemhoverbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* COLOR_clritemselbg;                                        ///< item项的按下背景色,示例:clritemselbg="pbgra(ff,ff,ff,ff)"
		static wchar_t* RECT_rcitemmargin;										   ///< item项的四边框,意思先缩小四边,再绘skin或颜色,示例:rcitemmargin="0,0,0,0"
		static wchar_t* INT_itemdelta;                                             ///< 每个Item自己左右的差值距离,示例:itemdelta="2"
		static wchar_t* bool_bfixwidth;											   ///< 固定宽度，默认是可变宽度的,示例:bfixwidth="1"
		static wchar_t* bool_bswap;                                                ///< 允许交换列,允许拖动交换列,示例:bswap="1"
		static wchar_t* bool_bsort;                                                ///< 允许排序sort,配合list发送排序消息,示例:bsort="1"

		/// item项相关
		static wchar_t* NODE_item;                                                 ///< 项XML结点
		static wchar_t* ITEM_width;                                                ///< 项的宽度,示例:width="100"
		static wchar_t* ITEM_skin;                                                 ///< 项的状态连图,如不存在，则使用默认的item状态图,示例,skin="skin"
		static wchar_t* ITEM_text;                                                 ///< 项的文本,示例:text="可拖动"
		static wchar_t* ITEM_data;                                                 ///< 项的附加数据,示例:data="100"
		static wchar_t* ITEM_stflag;                                               ///< 项的排序(结合list),分为正常0，从小到大1,从大到小2，示例:stflag="0"
	};
	DMAttrValueInit(DUIHeaderCtrlAttr,SKIN_itemskin)DMAttrValueInit(DUIHeaderCtrlAttr,SKIN_sortskin)
	DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritembg)DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritemhoverbg)DMAttrValueInit(DUIHeaderCtrlAttr,COLOR_clritemselbg)
	DMAttrValueInit(DUIHeaderCtrlAttr,RECT_rcitemmargin)DMAttrValueInit(DUIHeaderCtrlAttr,INT_itemdelta)
	DMAttrValueInit(DUIHeaderCtrlAttr,bool_bfixwidth)DMAttrValueInit(DUIHeaderCtrlAttr,bool_bswap)DMAttrValueInit(DUIHeaderCtrlAttr,bool_bsort)
	DMAttrValueInit(DUIHeaderCtrlAttr,NODE_item)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_width)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_skin)
	DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_text)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_data)DMAttrValueInit(DUIHeaderCtrlAttr,ITEM_stflag)
}

namespace DM
{
#define DMHDI_WIDTH               0x0001
#define DMHDI_TEXT                0x0002
#define DMHDI_SORTFLAG            0x0004
#define DMHDI_LPARAM              0x0008
#define DMHDI_ORDER               0x0010
	/// <summary>
	///		 记录当前项排序状态
	/// </summary>
	typedef enum _DMHDSORTFLAG
	{
		DMT_NULL = 0,
		DMT_UP,
		DMT_DOWN,
	}DMHDSORTFLAG;
	/// <summary>
	///		 仿HDITEM,允许每个item使用一个状态连图,ulState记录了状态
	/// </summary>
	typedef struct _DMHDITEM
	{
		_DMHDITEM()
		{
			lpszText = NULL;
			pSkin    = NULL;
			ulState  = DUIWNDSTATE_Normal;
		}
		~_DMHDITEM()
		{
			if (lpszText)
			{
				free(lpszText);
			}
		}
		UINT					  mask; 
		int						  cxy; 
		IDMSkinPtr			      pSkin;	
		LPWSTR					  lpszText; 
		int						  cchTextMax; 
		int						  fmt; 
		LPARAM					  lParam; 
		UINT                      ulState;   
	    DMHDSORTFLAG			  stFlag;
		int						  iOrder;
		
	} DMHDITEM, *LPDMHDITEM;

	/// <summary>
	///		 DUIHeaderCtrl的内置实现
	/// </summary>
	class DM_EXPORT DUIHeaderCtrl: public DUIWindow,public DMArrayT<LPDMHDITEM>
	{
		DMDECLARE_CLASS_NAME(DUIHeaderCtrl, DUINAME_HeaderCtrl,DMREG_Window)
	public:
		DUIHeaderCtrl();
		~DUIHeaderCtrl();
	public:
		//---------------------------------------------------
		// Function Des: 接口
		//---------------------------------------------------
		int InsertItem(int nIndex, DMXmlNode&XmlNode);
		bool GetItem(int iItem,DMHDITEM* pItem);
		size_t GetItemCount() ;
		int GetTotalWidth();
		int GetItemWidth(int iItem);
		int SetItemWidth(int iItem,int iWid,bool bFire=true);
		bool DeleteItem(int iItem);
		void DeleteAllItems();


	public:// 绘制！！！！
		virtual void DrawItem(IDMCanvas* pCanvas, CRect& rc, const LPDMHDITEM pItem);
		void RedrawItem(int iItem);

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_MOUSELEAVE(OnMouseLeave)
			MSG_WM_ACTIVATEAPP(OnActivateApp)
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI的消息分发系列函数
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnMouseLeave();
		void OnActivateApp(BOOL bActive, DWORD dwThreadID);
		void OnDestroy();

	public:
		//---------------------------------------------------
		// Function Des: 可重载函数
		//---------------------------------------------------
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
	    DMCode DV_OnSetCursor(const CPoint &pt);             // 设置当前的光标,用于两个item之间变成IDC_SIZEWE光标


	public:
		void PreArrayObjRemove(const LPDMHDITEM &obj);

	public:// 辅助
		DWORD HitTest(CPoint pt);													 ///< 如果位于两个item之间，高位和低位不同，否则相同
		bool IsItemHover(DWORD dwHitTest);
		void CreateDragCanvas(UINT iItem);
	    void DrawDraggingState(DWORD dwDragTo);
		CRect GetItemRect(UINT iItem);

	public:  
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::SKIN_itemskin,OnAttributeItemSkin)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::SKIN_sortskin,m_pSortSkin,DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritembg,m_crItemBg[0],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritemhoverbg,m_crItemBg[1],DM_ECODE_NOXMLLOADREFRESH)
			DM_COLOR_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::COLOR_clritemselbg,m_crItemBg[2],DM_ECODE_NOXMLLOADREFRESH)
			DM_RECT_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::RECT_rcitemmargin,m_rcItemMargin,DM_ECODE_NOXMLLOADREFRESH);
			DM_INT_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::INT_itemdelta,m_nItemDelta, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bfixwidth,m_bFixWidth,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bswap,m_bSwap,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIHeaderCtrlAttr::bool_bsort,m_bSort,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()
	public:
		DMCode OnAttributeItemSkin(LPCWSTR lpszValue, bool bLoadXml);
	public:
		IDMSkin*											m_pItemSkin;			///< 表头项绘制Skin
		IDMSkin*											m_pSortSkin;			///< 排序标志Skin
		DMColor											    m_crItemBg[3];          ///< item背景的三态,在item没有skin时使用
		CRect                                               m_rcItemMargin;			///< item项的四边框,意思先缩小四边,再绘skin或颜色
		bool											    m_bFixWidth;			///< 表项宽度固定开关
		bool												m_bSort;				///< 表头可以点击排序
		bool											    m_bSwap;				///< 允许拖动调整位置开关

		bool												m_bDragging;			///< 正在拖动标志
		DMSmartPtrT<IDMCanvas>								m_pDragCanvas;		    ///< 显示拖动窗口的临时画布
		CPoint												m_ptClick;				///< 当前点击坐标
		DWORD												m_dwHitTest;			///< 鼠标位置
		DWORD												m_dwDragTo;				///< 拖放目标    
		int													m_nAdjItemOldWidth;		///< 保存被拖动项的原始宽度
		int													m_nItemDelta;           ///< 每个Item自己左右的小留下差值,
	};

}//namespace DM