//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMImgListSkinImpl.h 
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

namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DMImgListSkinImpl"/>��xml���Զ���
	/// </summary>
	class DMImgListSkinImplAttr
	{
	public:
		static const char* STRING_src;                            ///< ״̬��ͼԴ������ͨ��xml����
		static const char* STRING_id;                             ///< Ψһ��ʶskin�����ʾ����id="channel_vz"
		static const char* bool_btitle;                           ///< �Ƿ�ƽ�̻��ƣ�ʾ����btitle="1"
		static const char* bool_bvert;                            ///< ״̬��ͼ�Ƿ�Ϊ��ֱ�ֲ��ģ�ʾ����bvert="1"
		static const char* INT_states;                            ///< ״̬����ʾ����states="1"

	};
	DMAttrValueInit(DMImgListSkinImplAttr,STRING_src)DMAttrValueInit(DMImgListSkinImplAttr,STRING_id)
	DMAttrValueInit(DMImgListSkinImplAttr,bool_btitle)DMAttrValueInit(DMImgListSkinImplAttr,bool_bvert)
	DMAttrValueInit(DMImgListSkinImplAttr,INT_states)
}

namespace DM
{
	/// <summary>
	///		״̬��ͼ����ʵ�֣����ԣ�<see cref="DMAttr::DMImgListSkinImplAttr"/>
	/// </summary>
	class DMImgListSkinImpl:public IDMSkin
	{
		DMDECLARE_CLASS_NAME(DMImgListSkinImpl,"imglist",DMREG_Skin);
	public:
		DMImgListSkinImpl();
	public:
		DMCode GetID(char* lpszId, int iSize);
		DMCode Draw(IDMCanvas *pCanvas, LPCRECT lpRectDraw, int iState,BYTE alpha=0xFF); 
		DMCode GetStateSize(SIZE &sz, int iState=0);                                        
		DMCode GetStates(int &iStates);	
		DMCode GetBitmap(IDMBitmap** ppBitmap);
		DMCode SetBitmap(LPBYTE pBuf,size_t szLen,LPCSTR pszType);
		DMCode UpdateSkin(WPARAM wp, LPARAM lp);
		DMCode IsValid();

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_CUSTOM_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::STRING_src, OnAttributeGetImage)
			DM_STRING_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::STRING_id, m_strID, DM_ECODE_OK)
			DM_bool_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::bool_btitle, m_bTile,  DM_ECODE_OK)	   
			DM_bool_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::bool_bvert, m_bVert, DM_ECODE_OK)	 
			DM_INT_ATTRIBUTE(DMAttr::DMImgListSkinImplAttr::INT_states,m_nStates,DM_ECODE_OK)   
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttributeGetImage(LPCSTR pszValue, bool bLoadXml); // ��ʼ��m_pBitmap
			
	protected:
		DMSmartPtrT<IDMBitmap>						m_pBitmap;      ///< ״̬ͼ         
		int											m_nStates;		///< �ж�����״̬
		bool										m_bTile;		///< �Ƿ�ƽ�̻��ƣ�flase��ʾ�������
		bool										m_bVert;        ///< ��ͼ�Ƿ�ֱ���У�0--ˮƽ����(Ĭ��), ����--��ֱ����
		CStringA                                    m_strID;        ///< Ψһ��ʶ
		CStringA                                    m_strRes;       ///< ��Դ��Ψһ��ʶ�����ڻ���ȷ��
	};
}//namespace DM