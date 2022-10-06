//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMScrollBarSkinImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-28	1.0			
//-------------------------------------------------------
#pragma once
#include "Modules/Skin/DMImgListSkinImpl.h"

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DMImgListSkinImpl"/>��xml���Զ���
	/// </summary>
	class DMScrollBarSkinImplAttr:public DMImgListSkinImplAttr
	{
	public:
		static const char* RECT_margin;                            ///< scrollΪ9�������,ָ�����Ƶ��ı߿�,margin="1,1,1,1"

	};
	DMAttrValueInit(DMScrollBarSkinImplAttr,RECT_margin);
}

namespace DM
{
	/// <summary>
	///		scroll��ͼ����ʵ�֣����ԣ�<see cref="DMAttr::DMScrollBarSkinImplAttr"/>
	/// </summary>
	class DMScrollBarSkinImpl:public DMImgListSkinImpl
	{
		DMDECLARE_CLASS_NAME(DMScrollBarSkinImpl,"scrollbar",DMREG_Skin);
	public:
		DMCode Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha=0xFF); 

	public:
		//--------------------------------------------------
		// �ؼ�:�ϼ�ͷ���¼�ͷ����ֱ����������ֱ���ۡ����ͷ���Ҽ�ͷ��ˮƽ��������ˮƽ���ۡ�С�߽�
		// ״̬:������ͣ������������á���Active
		// ����9*��5=45��״̬ nSbCode+bVertѡ���� nStateΪ��
		//--------------------------------------------------
		CRect GetPartRect(int nSbCode, int nState,bool bVert);

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_RECT_ATTRIBUTE(DMAttr::DMScrollBarSkinImplAttr::RECT_margin ,m_rcMargin,DM_ECODE_OK)
		DM_END_ATTRIBUTES()

	public:
		CRect								m_rcMargin;
		
	};

}//namespace DM