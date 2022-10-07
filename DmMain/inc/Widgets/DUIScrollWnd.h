//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des: DUIScrollWndֻ�������һ�����Բ����ӿؼ�DUIScrollFL,DUIScrollFL�Ĵ�С���ǰ������������Ӵ�������
// File Summary:
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2019-6-19	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DMWidgetDef.h"
#include "Widgets/DUIScrollBase.h"

namespace DM
{
	/// <summary>
	///		 DUIScrollWnd�ĸ���,���ھ��Բ���,�������������Ӵ�������,���Ӵ�������仯ʱ������ͬ���仯
	/// </summary>
	class DM_EXPORT DUIScrollFL : public DUIWindow
	{
	public:
		DMDECLARE_CLASS_NAME(DUIScrollFL, DUINAME_ScrollFL, DMREG_Window);

	public:
		CRect GetMeasureRect(bool bUseFLCoordinate = false);				   ///< bUseFLCoordinateΪtrueʱ,��ʾ��ScrollFL�����Ͻ�Ϊ����ԭ��,������������Ϊ����ԭ��

	public:
		virtual DMCode DV_GetChildMeasureLayout(LPRECT lpRect);
		virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		DMCode OnChildLayoutFinishedEvent(DMEventArgs *pEvt);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnDestroy();

	public:
		CRect                                          m_rcMeasure;             ///< ��ScrollFL�����Ͻ�Ϊ����ԭ��,���������ӿؼ�����С����(������ScrollFL�ǿͻ���)
	};


	/// <summary>
	///		 DUIScrollWnd������ʵ��
	/// </summary>
	class DM_EXPORT DUIScrollWnd :public DUIScrollBase
	{
	public:
		DMDECLARE_CLASS_NAME(DUIScrollWnd, DUINAME_ScrollWnd, DMREG_Window);
	public:
		DUIScrollWnd();

	public:
		//---------------------------------------------------
		// Function Des: ����ӿ�
		//---------------------------------------------------
		virtual void UpdateScrollRange();									    ///< ���¹�����Χ������ӿؼ��Ĵ�С�ı���,��Ҫ�������ô�������

		//---------------------------------------------------
		// Function Des: DV methods
		//---------------------------------------------------
		virtual DMCode DV_CreateChildWnds(const DMXmlNode &XmlNode) override;
		virtual void OnRangeCurPosChanged(CPoint ptOld, CPoint ptNew);

	public:
		DM_BEGIN_MSG_MAP()
			MSG_WM_SIZE(OnSize)
		DM_END_MSG_MAP()

	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void OnSize(UINT nType, CSize size);

	public:
		DUIScrollFL*                                          m_pFLChild;
	};



}//namespace DM