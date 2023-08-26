#include "DmMainAfx.h"
#include "DMPluginImpl.h"
#include "DMLogImpl.h"
#include "DMAttributeImpl.h"
#include "DMImgDecoderImpl.h"
#include "DMResFolderImpl.h"
#include "DMRenderImpl.h"
#include "DMDrawImpl.h"
#include "DMImg9SkinImpl.h"
#include "DMScrollBarSkinImpl.h"
#include "DMStyleImpl.h"
#include "DMLayoutImpl.h"
#include "DMTooltipImpl.h"
#if !defined(DM_EXCLUDE_MUI)
#include "DMTransImpl.h"
#endif
#include "DMTaskRunnerImpl.h"

#include "DUIButton.h"
#include "DUIScrollBase.h"
#include "DUIScrollWnd.h"
#include "DUITabCtrl.h"
#include "DUITreeCtrl.h"
#include "DUIListBox.h"
#include "DUIListBoxEx.h"
#include "DUIEdit.h"
#include "DUILabel.h"
#include "DUILink.h"
#include "DUIComboBox.h"
#include "DUIGif.h"
#include "DUIPngGif.h"
#if !defined(DM_EXCLUDE_ACTIVEX)
#include "DUIFlash.h"
#if !defined(DM_EXCLUDE_IE)
#include "DUIIE.h"
#endif
#endif
#include "DUIHeaderCtrl.h"
#include "DUIListCtrlEx.h"
#include "DUIGroup.h"
#include "DUICheckBox.h"
#include "DUIRadioButton.h"
#include "DUISliderCtrl.h"
#include "DUIProcessCtrl.h"
#include "DUIHotKeyCtrl.h"
#include "DUIPAddressCtrl.h"
#include "DUITreeCtrlEx.h"
#include "DUIMonthCalCtrl.h"
#include "DUIRectTracker.h"
#include "DUIFlowLayout.h"



namespace DM
{
	const wchar_t* DMPluginImpl::GetName()const 
	{
		return L"{6C27EB26-12F8-4807-95E9-578C781CD61E}";
	}

	void DMPluginImpl::Install()
	{
		DMCode iErr = DM_ECODE_OK;
		// 默认Log -----------------------------
#ifdef DMLOG_ON
		iErr = g_pDMApp->Register(DMRegHelperT<DMLogImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "fail DMLogImpl");
		iErr = g_pDMApp->SetDefRegObj(DMLogImpl::GetClassName(),DMLogImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set Log obj fail");
#endif //DMLOG_ON 

		// 默认Attribute,必须在其他解析之前，因为其他解析的实例可能使用到它
		iErr = g_pDMApp->Register(DMRegHelperT<DMAttributeImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMAttributeImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMAttributeImpl::GetClassName(),DMAttributeImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set DMAttributeImpl fail");

		// 默认解码器 ----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMImgDecoderImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMImgDecoderImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMImgDecoderImpl::GetClassName(),DMImgDecoderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set DMImgDecoderImpl fail");

		// 默认Res -------------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMResFolderImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMResFolderImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMResFolderImpl::GetClassName(),DMResFolderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set DMResFolderImpl fail");

		// 默认Render-----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMRenderImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMRenderImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMRenderImpl::GetClassName(),DMRenderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set DMRenderImpl fail");

		// 默认Draw-----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMDrawImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMDrawImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMDrawImpl::GetClassName(),DMDrawImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "set DMDrawImpl fail");

		// 默认Skin -----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMImgListSkinImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMImgListSkinImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMImgListSkinImpl::GetClassName(),DMImgListSkinImpl::GetClassType());
		iErr = g_pDMApp->Register(DMRegHelperT<DMImg9SkinImpl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DMScrollBarSkinImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMImg9SkinImpl fail");

		// 默认Style -----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMStyleImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMStyleImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMStyleImpl::GetClassName(),DMStyleImpl::GetClassType());

		// 默认Layout ----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMLayoutImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMLayoutImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMLayoutImpl::GetClassName(),DMLayoutImpl::GetClassType());

		// 内置Tooltip ---------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMTooltipImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMTooltipImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMTooltipImpl::GetClassName(),DMTooltipImpl::GetClassType());

#if !defined(DM_EXCLUDE_MUI)
		// 内置Trans ---------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMTransImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMTransImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMTransImpl::GetClassName(),DMTransImpl::GetClassType());
#endif

		// 内置TaskRunner-----------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DMTaskRunnerImpl>(),true);
		DMASSERT_MSG(DMSUCCEEDED(iErr), "reg DMTaskRunnerImpl fail");
		iErr = g_pDMApp->SetDefRegObj(DMTaskRunnerImpl::GetClassName(),DMTaskRunnerImpl::GetClassType());

		// 内置Widget ----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DUIWindow>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIButton>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIScrollBase>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIScrollWnd>(), true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIScrollFL>(), true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUITabPage>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUITabCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUITreeCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIListBox>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIListBoxEx>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIRichEdit>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIEdit>(),true);
		new DUIEditHelper;// 和edit相关
		iErr = g_pDMApp->Register(DMRegHelperT<DUILabel>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUILink>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIComboBox>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIGif>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIPngGif>(),true);
#if !defined(_WIN64)// flash暂不支持X64,后续调整下
#if !defined(DM_EXCLUDE_ACTIVEX)
		iErr = g_pDMApp->Register(DMRegHelperT<DUIFlash>(),true);
#endif
#endif
#if !defined(DM_EXCLUDE_ACTIVEX) && !defined(DM_EXCLUDE_IE)
		iErr = g_pDMApp->Register(DMRegHelperT<DUIIE>(),true);
#endif
		iErr = g_pDMApp->Register(DMRegHelperT<DUIHeaderCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIListCtrlEx>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIGroup>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUICheckBox>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIRadioButton>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUISliderCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIProcessCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIHotKeyCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIPEdit>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIPAddressCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUITreeCtrlEx>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIMonthCalCtrl>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIRectTracker>(),true);
		

		// 内置FlowLayout ----------------------------
		iErr = g_pDMApp->Register(DMRegHelperT<DUIVLayout>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIHLayout>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUIWrapLayout>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUITabLayout>(),true);
		iErr = g_pDMApp->Register(DMRegHelperT<DUISplitLayout>(),true);
	}

	void DMPluginImpl::Uninstall()
	{
		g_pDMApp->UnRegister(DUIRichEdit::GetClassName(),DUIRichEdit::GetClassType());
		g_pDMApp->UnRegister(DUIEdit::GetClassName(),DUIEdit::GetClassType());
		delete DUIEditHelper::getSingletonPtr();// 和edit相关


#if 0 // 只是做个示例,默认不开启！！！
		DMCode iErr = DM_ECODE_OK;
		// 默认Log -----------------------------
#ifdef DMLOG_ON
		iErr = g_pDMApp->UnRegister(DMLogImpl::GetClassName(),DMLogImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg DMLogImpl fail");
#endif //DMLOG_ON 

		// 默认Attribute,必须在其他解析之前，因为其他解析的实例可能使用到它
		iErr = g_pDMApp->UnRegister(DMAttributeImpl::GetClassName(),DMAttributeImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg DMAttributeImpl fail");
		

		// 默认解码器 ----------------------------
		iErr = g_pDMApp->UnRegister(DMImgDecoderImpl::GetClassName(),DMImgDecoderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), L"unreg DMImgDecoderImpl fail");
	
		// 默认Res -------------------------------
		iErr = g_pDMApp->UnRegister(DMResFolderImpl::GetClassName(),DMResFolderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg DMResFolderImpl fail");

		// 默认Render-----------------------------
		iErr = g_pDMApp->UnRegister(DMRenderImpl::GetClassName(),DMRenderImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg DMRenderImpl fail");

		// 默认Skin -----------------------------
		iErr = g_pDMApp->UnRegister(DMImgListSkinImpl::GetClassName(),DMImgListSkinImpl::GetClassType());
		iErr = g_pDMApp->UnRegister(DMImg9SkinImpl::GetClassName(),DMImg9SkinImpl::GetClassType());
		iErr = g_pDMApp->UnRegister(DMScrollBarSkinImpl::GetClassName(),DMScrollBarSkinImpl::GetClassType());

		// 默认Style -----------------------------
		iErr = g_pDMApp->UnRegister(DMStyleImpl::GetClassName(),DMStyleImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg fail DMStyleImpl fail");
	
		// 默认Layout ----------------------------
		iErr = g_pDMApp->UnRegister(DMLayoutImpl::GetClassName(),DMLayoutImpl::GetClassType());
		DMASSERT_MSG(DMSUCCEEDED(iErr), "unreg fail DMLayoutImpl fail");
	
		// 内置Widget ----------------------------
		iErr = g_pDMApp->UnRegister(DUIWindow::GetClassName(),DUIWindow::GetClassType());

		// 内置FlowLayout ----------------------------
		iErr = g_pDMApp->UnRegister(DUIVLayout::GetClassName(),DUIVLayout::GetClassType());
		iErr = g_pDMApp->UnRegister(DUIHLayout::GetClassName(),DUIHLayout::GetClassType());
		iErr = g_pDMApp->UnRegister(DUIWrapLayout::GetClassName(),DUIWrapLayout::GetClassType());
#endif //
	}

	void DMPluginImpl::Initialise()
	{
	
	}

	void DMPluginImpl::Shutdown()
	{

	}

}//namespace DM