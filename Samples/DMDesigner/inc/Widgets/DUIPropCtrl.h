// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	DUIPropCtrl.h 
// File mark:   
/*DUIPropFrame:
1.���������ӿؼ�:
DUIHeaderCtrl+DUISplitLayout(DUIPropList + DUIPropDesc)
2.��¼���²���:
m_iHeadHei:DUIHeaderCtrl�ĸ߶�
m_iDescHei:DUIPropDesc�ĳ�ʼ�߶�
m_iLeftColWid:DUIHeaderCtrl��ߵĿ��
			  3.������Ϣ:
OnHeaderSizeChanging:��DUIHeaderCtrl�ı���ʱ�����뱣֤���ұ�ͬʱ�仯��ͬʱ֪ͨ��DUIPropList
						 4.���֣�
						 DUIHeaderCtrl+DUISplitLayoutʹ�þ��Բ��֣���OnSize�иı�ʱ���²���


						 DUIPropList
						 1.��¼���²���
m_iRowHei:�и�
m_iLeftColWid:��DUIPropFrame��OnHeaderSizeChanging�仯ʱͬ���仯
			  m_pExpandSkin��չ��������ͨ��boxƤ��

			  2.������Ϣ:
					 ��m_iLeftColWid�ı�ʱ�����뱣֤DUIHeaderCtrlͬʱ���ı�*/
// File summary:ģ��MFCPropertyGridCtrl
// Author:		guoyouhuang
// Edition:     1.0
// Create date: 2016-8-2
// ----------------------------------------------------------------
#pragma once 
#include "DUIScrollBase.h"
#include "DUIHeaderCtrl.h"
#include "IProp.h"

namespace DMAttr
{
	class DUIPropListAttr:public DUIScrollBaseAttr
	{
	public:
		static const char* INT_rowhei;													 ///< ��ĸ߶�,Ĭ��Ϊ20,ʾ��:rowhei="20"
		static const char* INT_leftcolwid;												 ///< ���ֵ��Ĭ�Ͽ��,Ĭ��Ϊ30,ʾ��:leftcolwid="30"
		static const char* SKIN_expandskin;                                             ///< չ������ͨ��skin,ʾ��:expandskin="skin"
		static const char* SKIN_itembgskin;                                             ///< �������ͨ�ñ���skin,ʾ��:itembgskin="skin"
	};
	DMAttrValueInit(DUIPropListAttr,INT_rowhei)DMAttrValueInit(DUIPropListAttr,INT_leftcolwid)DMAttrValueInit(DUIPropListAttr,SKIN_expandskin)DMAttrValueInit(DUIPropListAttr,SKIN_itembgskin)

	class DUIPropFrameAttr : public DUIScrollBaseAttr
	{
	public:
		static const char* INT_headhei;												 ///< ͷ���ĸ߶�,Ĭ��Ϊ24,ʾ��:headhei="24"
		static const char* INT_deschei;												 ///< ��������Ĭ�ϸ߶�,Ĭ��Ϊ20,ʾ��:deschei="20"
		static const char* bool_bshowhead;												 ///< �Ƿ���ʾͷ��ʾ��:bshowhead="1"
		static const char* bool_bshowdesc;												 ///< �Ƿ���ʾ������,ʾ��:bshowdesc="1"
		static const char* SKIN_headitemskin;                                           ///< headͷ��itemskin,ʾ��:headitemskin="skin"
	};
	DMAttrValueInit(DUIPropFrameAttr,INT_headhei)DMAttrValueInit(DUIPropFrameAttr,INT_deschei)
	DMAttrValueInit(DUIPropFrameAttr,bool_bshowhead)DMAttrValueInit(DUIPropFrameAttr,bool_bshowdesc)DMAttrValueInit(DUIPropFrameAttr,SKIN_headitemskin)
}

namespace DM 
{
	class DUIPropList;
	class DUIPropDesc;
	class DUIPropFrame : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPropFrame,"propframe",DMREG_Window);
	public:
		DUIPropFrame();
		IPropPtr AddInitAttrProperty(DMXmlInitAttrPtr pInitAttr,IPropPtr pParent = NULL, CStringA strValue = "");
		IPropPtr FindItemByData(LPARAM lpData);
	public: 
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
		void OnSize(UINT nType,CSize size);
		void OnDestroy();

	public:
		DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		DMCode AdjustLayout();
		DMCode OnHeaderSizeChanging(DMEventArgs *pEvt);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIPropFrameAttr::INT_headhei, m_iHeadHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIPropFrameAttr::INT_deschei, m_iDescHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIPropFrameAttr::bool_bshowhead,m_bShowHead,DM_ECODE_NOXMLLOADREFRESH);
			DM_bool_ATTRIBUTE(DMAttr::DUIPropFrameAttr::bool_bshowdesc,m_bShowDesc,DM_ECODE_NOXMLLOADREFRESH);
			DM_WSTRING_ATTRIBUTE(DMAttr::DUIPropFrameAttr::SKIN_headitemskin,m_strHeadItemSkin,DM_ECODE_NOXMLLOADREFRESH)
		DM_END_ATTRIBUTES()

	public:
		int                                    m_iHeadHei;              ///< ͷ��
		int                                    m_iDescHei;              ///< �������߶�
		bool                                   m_bShowHead;             ///< �Ƿ���ʾͷ
		bool                                   m_bShowDesc;             ///< �Ƿ���ʾdesc
		CStringW                               m_strHeadItemSkin;		///< ���ڴ���headʱʹ��

		//
		DUIHeaderCtrl*                         m_pHeadCtrl;
		DUISplitLayout*                        m_pSplit;
		DUIPropList*                           m_pPropList;
		DUIPropDesc*                           m_pPropDesc;
		bool                                   m_bFirst;
	};

	class DUIPropList : public DUIScrollBase, public DMArrayT<IDMRegPtr>, public IDMAccelHandler
	{
		DMDECLARE_CLASS_NAME(DUIPropList,"proplist",DMREG_Window);
	public:
		DUIPropList();
		~DUIPropList();

		DMCode AdjustLayout();
		DMCode RegisterProp(IDMReg &RegObj,bool bReplace);
		DMCode CreateRegObj(void** ppObj, LPCSTR lpszClassName);
		int AddProperty(IPropPtr pProp, bool bRedraw = true);
		bool DelProperty(IPropPtr pProp, bool bRedraw = true);
		bool RemoveAllProp(bool bDelete = true,bool bRedraw = true);
		void UpdateXml(DMXmlNode &XmlNode);
		IPropPtr FindItemByData(LPARAM lpData);
		IPropPtr SetCurSel(IPropPtr pProp, bool bRedraw = true);
		IPropPtr GetCurSel(); 
		IPropPtr HitTest(CPoint pt,IProp::ClickArea* pnArea = NULL,bool bPropsOnly = false);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
			MSG_WM_DESTROY(OnDestroy)
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_LBUTTONDOWN(OnLButtonDown)
			MSG_WM_LBUTTONDBLCLK(OnLButtonDbClick)
			MSG_WM_MOUSEMOVE(OnMouseMove)
			MSG_WM_LBUTTONUP(OnLButtonUp)
			DM_MSG_WM_ENABLE(DM_OnEnable)
		DM_END_MSG_MAP()
		void OnSize(UINT nType,CSize size);
		void OnDestroy();
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnLButtonDown(UINT nFlags,CPoint pt);
		void OnLButtonDbClick(UINT nFlags,CPoint pt);
		void OnMouseMove(UINT nFlags,CPoint pt);
		void OnLButtonUp(UINT nFlags,CPoint pt);
		void DM_OnEnable(BOOL bEnable,UINT nStatus);

	public:
		virtual DMCode DV_CreateChildWnds(DMXmlNode &XmlNode);
		virtual DMCode DV_OnSetCursor(const CPoint &pt); 
		virtual bool EqualArrayObj(const IDMRegPtr &objsrc, const IDMRegPtr &objdest);
		virtual void PreArrayObjRemove(const IDMRegPtr &obj);
		void UpdateScrollRangeSize();
		void OnRangeSizeChanged(CSize szOld,CSize szNew);
		void OnRangeCurPosChanged(CPoint ptOld,CPoint ptNew);

		bool OnAccelPressed(const DUIAccel& Accel);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_INT_ATTRIBUTE(DMAttr::DUIPropListAttr::INT_rowhei, m_iRowHei, DM_ECODE_NOXMLLOADREFRESH)
			DM_INT_ATTRIBUTE(DMAttr::DUIPropListAttr::INT_leftcolwid, m_iLeftColWid,DM_ECODE_NOXMLLOADREFRESH)
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIPropListAttr::SKIN_expandskin,m_pExpandSkin,DM_ECODE_NOXMLLOADREFRESH);
			DM_SKINPTR_ATTRIBUTE(DMAttr::DUIPropListAttr::SKIN_itembgskin,m_pItemBgSkin,DM_ECODE_NOXMLLOADREFRESH);
		DM_END_ATTRIBUTES()

	public:
		int                                    m_iRowHei;               ///< �и�
		int                                    m_iLeftColWid;           ///< ���name�Ŀ��
		IDMSkinPtr                             m_pExpandSkin;			///< չ��������boxƤ��
		IDMSkinPtr                             m_pItemBgSkin;			///< name��value��ͨ�ñ���Ƥ��

		DM::CList<IPropPtr>					   m_lstProps;				///< �����б�
		IPropPtr							   m_pSel;					///< ��ǰѡ�е������б�

		CPoint                                 m_ptDrag;
		bool                                   m_bDragging;
		DUIPropFrame*                          m_pFrame;
		DUIPropDesc*                           m_pDesc;

		DWORD								   m_accel;					///< ɾ����
	};

	class DUIPropDesc : public DUIWindow
	{
		DMDECLARE_CLASS_NAME(DUIPropDesc,"propdesc",DMREG_Window);
	public:
		DUIPropDesc();
	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
		DM_END_MSG_MAP()
		void DM_OnPaint(IDMCanvas* pCanvas);
	public:
		DUIPropList*                           m_pPropList;
	};

	
}//namespace DM