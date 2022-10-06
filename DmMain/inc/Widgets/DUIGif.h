//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: 
// File Des:
// File Summary:gif֧����flash�����ϲ���
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-5	1.0			
//--------------------------------------------------------
#pragma once
#include "Widgets/DUIAnimateHelper.h"


namespace DMAttr
{
	/// <summary>
	///		<see cref="DM::DUIGif"/>��xml���Զ���
	/// </summary>
	class DUIGifAttr:public DUIWindowAttr
	{
	public:
		static const char* GIF_gifskin;                                 ///< gif����Դ������ͨ��skinָ��������ֱ��ͨ���ļ���ʶָ��,�����ļ�����·��ָ��,ʾ��:gifskin="gif:4_gif",gifskin="c:\1.gif"                            
		static const char* bool_badjust;                                ///< �Ƿ����ʹͼƬ����Ӧ�ߴ磬Ҳ����ͼƬ���ֱ������ţ�������Σ�Ĭ��Ϊtrue,ʾ��:badjust="1"
		static const char* bool_bcalcclip;                              ///< ���㲢�Զ�����clip����������������,����ʧ����Ч�ʣ�������gif�ؼ��а�����gif�ؼ�,�����gif͸������ÿ��ˢ��ֻ�Ǹ�gif��һ����,ʾ��:bcalcclip="1"
	};
	DMAttrValueInit(DUIGifAttr,GIF_gifskin)DMAttrValueInit(DUIGifAttr,bool_badjust)DMAttrValueInit(DUIGifAttr,bool_bcalcclip)
}

namespace DM
{
	/// <summary>
	///		 gif������ʵ��
	/// </summary>
	class DM_EXPORT DUIGif:public DUIWindow,public DMArrayT<PDMAnimateFrame>,public IDMTimeline
	{
		DMDECLARE_CLASS_NAME(DUIGif, DUINAME_Gif,DMREG_Window)
	public:
		DUIGif();
		~DUIGif();

		enum DMGifState
		{
			DMGIF_NOREADY,
			DMGIF_NODELAY,
			DMGIF_LOOPEND,
			DMGIF_PAUSE,
			DMGIF_READY,
		};
	public:
		//---------------------------------------------------
		// Function Des: �ӿ�
		//---------------------------------------------------
		virtual DMCode LoadFromFile(LPCWSTR pszFileName);
		virtual DMCode LoadFromMemory(LPVOID lpBuf, UINT ulSize);
		virtual DMCode Start();
		virtual DMCode Pause();
		virtual DMCode Restart();
		virtual DMCode Clear();

	public:
		DM_BEGIN_MSG_MAP()
			DM_MSG_WM_PAINT(DM_OnPaint)
			MSG_WM_SHOWWINDOW(OnShowWindow)
		    MSG_WM_DESTROY(OnDestroy)
		DM_END_MSG_MAP()
	public:
		//---------------------------------------------------
		// Function Des: DUI����Ϣ�ַ�ϵ�к���
		//---------------------------------------------------
		void DM_OnPaint(IDMCanvas* pCanvas);
		void OnShowWindow(BOOL bShow, UINT nStatus);
		void OnDestroy();

	public:
		DMCode OnTimeline();

	public:
		bool  IsReady();														///< �Ƿ�׼����gif								
		bool  Reset();														    ///< �ָ��ӵ�һ֡��ʼ״̬
		int   ReadyCurFrame(UINT ulFrame);										///< ���ص�ǰҪ����֡��״̬

	public:
		DM_BEGIN_ATTRIBUTES()
			DM_bool_ATTRIBUTE(DMAttr::DUIGifAttr::bool_bcalcclip,m_bCalcClip,DM_ECODE_NOXMLLOADREFRESH)
			DM_bool_ATTRIBUTE(DMAttr::DUIGifAttr::bool_badjust,m_bAdjustPic,DM_ECODE_NOXMLLOADREFRESH)
			DM_CUSTOM_ATTRIBUTE(DMAttr::DUIGifAttr::GIF_gifskin, OnAttrGif)
		DM_END_ATTRIBUTES()

	public:
		DMCode OnAttrGif(LPCSTR lpszValue, bool bLoadXml);

	public:// ����
		void PreArrayObjRemove(const PDMAnimateFrame &obj);

	public:  
		bool                                         m_bAdjustPic;				 ///< ����ʹͼƬ����Ӧ�ߴ�
		bool                                         m_bCalcClip;                ///< ���㲢�Զ�����clip����������������

		DMSmartPtrT<IDMCanvas>                       m_pMemCanvas;               ///< �ڴ滭��,����ÿһ֡
		DMSmartPtrT<IDMCanvas>                       m_pPreviousCanvas;          ///< ǰһ��Unspecified or Do Not Dispose֡
		CRect                                        m_rcGif;					 ///< gif�Ŀ�ܴ�С,֡��С��һ��Ϊ��ܴ�С

		// ֡����
		UINT                                         m_ulCurFrame;               ///< ��ǰ֡��
		UINT										 m_ulFrameCount;;            ///< ֡��Ŀ
		UINT                                         m_ulCurLoop;                ///< ��ǰѭ������
		UINT										 m_ulTotalLoopCount;		 ///< -1��ʾ����,�����������Σ�����Ϊ0
		DWORD                                        m_dwPreFrameTime;           ///< ��һ֡��ʾ��ʱ�䣬��λms

		bool                                         m_bPause;                   ///< ��ͣ��
		
	};




}//namespace DM