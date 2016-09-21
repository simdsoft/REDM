// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	IProp.h
// File mark:   
// File summary:Prop的相关属性对外接口
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-7-29
// ----------------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		基本属性接口，用于扩展
	/// </summary>
	class DUIPropList;
	class IProp : public DMBase
	{
	public:
		enum ClickArea{ClickExpandBox,ClickName,ClickValue,ClickDrag,ClickDescription};
		virtual DMCode SetOwner(DUIPropList *pOwner) = 0;								///< 设置属性及子属性列表的拥有者
		virtual DMCode SetParent(IProp* pParent) = 0;									///< 设置父属性
		virtual DMCode Expand(bool bExpand = true) = 0;									///< 展开或收缩
		virtual DMCode Reposition(int& y) = 0;											///< 重新布局
		virtual DMCode Invalidate() = 0;												///< 刷新
		virtual DMCode AddSubItem(IProp* pProp) = 0;									///< 增加子属性项
		virtual DMCode RemoveSubItem(IProp* &pProp,bool bDelete = true) = 0;            ///< 删除子属性项
		virtual DMCode RemoveAllSubItem(bool bDelete = true) = 0 ;						///< 删除所有的子项
		virtual DMCode PreDelete() = 0;													///< 在Delete之前预处理
	
		virtual bool IsSubItem(IProp* pSubProp) = 0;									///< pSubProp是否为它的子项					
		virtual bool IsParentExpanded() = 0;											///< 父窗口是否为展开状态
		virtual bool IsExpanded() = 0;													///< 是否展开
		virtual bool IsInPlaceEdit() = 0;												///< 是否处于value控件显示状态
		virtual bool IsGroup() = 0;														///< 是否为group,group只有name，没有value
		virtual IProp* HitTest(CPoint pt, IProp::ClickArea* pnArea = NULL) = 0;			///< pt点击的地方
		virtual int GetHierarchyLevel() = 0;                                            ///< 获得自己处于第几层（不是子项的为0层）
		virtual int GetHeiWithSub() = 0;                                                ///< 获得自己和所有子项的高度
		virtual DUIPropList* GetOwner() = 0;
		virtual IProp* GetParent() = 0;
		virtual LPARAM GetData() = 0;
		virtual void SetData(LPARAM lpData) = 0;
		virtual IProp* FindSubItemByData(LPARAM lpData) = 0;
		virtual void UpdateSubXml(DMXmlNode &XmlNode) = 0;
		virtual CStringW GetName() = 0;
		virtual CStringW GetValue() = 0;
	
		// 绘制
		virtual DMCode OnRender(IDMCanvas* pCanvas) = 0;
		virtual DMCode OnDrawExpandBox(IDMCanvas* pCanvas,CRect rcExpand) = 0;
		virtual DMCode OnDrawName(IDMCanvas* pCanvas, CRect rcName) = 0;
		virtual DMCode OnDrawValue(IDMCanvas* pCanvas, CRect rcValue) = 0;
		virtual DMCode OnDrawDesc(IDMCanvas* pCanvas, CRect rcDesc) = 0;
		

		// 点击
		virtual DMCode OnClickValue(UINT uiMsg, CPoint point) = 0;
		virtual DMCode OnClickName(UINT uiMsg, CPoint point) = 0;
		virtual DMCode OnClickDrag(UINT uiMsg, CPoint point) = 0;

		// 属性的InPlace窗口创建和显示隐藏
		virtual DMCode OnCreateInPlaceCtrl() = 0;
		virtual DMCode OnInPlaceCtrlShow(bool bShow = true) = 0;
	};
	typedef IProp* IPropPtr;

}//namespace DM