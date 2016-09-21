#include "DMDesignerAfx.h"
#include "Plugin.h"
#include "ProjTree.h"
#include "ResMultFolder.h"
#include "Layout.h"
#include "Draw.h"
#include "DUIPropCtrl.h"
#include "TipsImpl.h"
#include "DUIRoot.h"
#include"NewResDlg.h"

  
    
const wchar_t* Plugin::GetName()const 
{
	return L"{93FC7099-2F60-4e8f-B2E1-6C0BEBEB7F01}";
} 

void Plugin::Install()
{
	g_pDMApp->Register(DMRegHelperT<Layout>(),true);
	g_pDMApp->Register(DMRegHelperT<DMDraw>(),true);
	g_pDMApp->Register(DMRegHelperT<ProjTree>(),true);
	g_pDMApp->Register(DMRegHelperT<AttrTree>(),true);
	g_pDMApp->Register(DMRegHelperT<ObjTree>(),true);
    g_pDMApp->Register(DMRegHelperT<ResMultFolder>(),true);
	g_pDMApp->Register(DMRegHelperT<DSEdit>(),true);
	g_pDMApp->Register(DMRegHelperT<DSizeEdit>(),true);
	g_pDMApp->Register(DMRegHelperT<DUITreeEx>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIDragFrame>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIRect>(),true);
	g_pDMApp->Register(DMRegHelperT<PosList>(),true);
	g_pDMApp->Register(DMRegHelperT<PosEdit>(),true);
	g_pDMApp->Register(DMRegHelperT<PosItem>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIPos>(),true);
	g_pDMApp->Register(DMRegHelperT<DUISize>(),true);
	g_pDMApp->Register(DMRegHelperT<DUImgEditor>(),true);
	g_pDMApp->Register(DMRegHelperT<DUILimitEdit>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIPropFrame>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIPropList>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIPropDesc>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIList>(),true);
	g_pDMApp->Register(DMRegHelperT<TipsImpl>(),true);	
	g_pDMApp->Register(DMRegHelperT<DUIObjEditor>(),true);	
	g_pDMApp->Register(DMRegHelperT<DUIRoot>(),true);
	g_pDMApp->Register(DMRegHelperT<DUIRecentListBox>(),true);
	g_pDMApp->SetDefRegObj(Layout::GetClassName(),Layout::GetClassType());
	g_pDMApp->SetDefRegObj(ResMultFolder::GetClassName(),ResMultFolder::GetClassType());
}

void Plugin::Uninstall()
{ 
}

void Plugin::Initialise()
{
}

void Plugin::Shutdown()
{
}