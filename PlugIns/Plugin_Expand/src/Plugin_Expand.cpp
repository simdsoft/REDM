#include "Plugin_ExpandAfx.h"
#include "Plugin_Expand.h"
#include "DM3dAnimate.h"
#include "DMLineAnimate.h"
#include "DMScreenAnimate.h"
#include "DMResZipImpl.h"
#include "DMResMultZipImpl.h"

#ifdef USE_DMSKIA_RENDER_
#include "DMSkiaRenderImpl.h"
#endif

Plugin_Expand::Plugin_Expand(void)
{
}

Plugin_Expand::~Plugin_Expand(void)
{

}

const wchar_t* Plugin_Expand::GetName()const 
{
	return L"Plugin_Expand";
}

void Plugin_Expand::Install()
{
	g_pDMApp->Register(DMRegHelperT<DM3DAnimate>(),true);
	g_pDMApp->Register(DMRegHelperT<DMLineAnimate>(),true);
	g_pDMApp->Register(DMRegHelperT<DMScreenAnimate>(),true);
	g_pDMApp->Register(DMRegHelperT<DMResZipImpl>(),true);
	g_pDMApp->Register(DMRegHelperT<DMResMultZipImpl>(),true);
#ifdef USE_DMSKIA_RENDER_
	DMCode iErr = g_pDMApp->Register(DMRegHelperT<DMSkiaRenderImpl>(),true);
	DMASSERT_EXPR(DMSUCCEEDED(iErr), L"×¢²áDMSkiaRenderImplÊ§°Ü!");
	iErr = g_pDMApp->SetDefRegObj(DMSkiaRenderImpl::GetClassName(),DMSkiaRenderImpl::GetClassType());
	DMASSERT_EXPR(DMSUCCEEDED(iErr), L"ÉèÖÃDMSkiaRenderImplÎªÄ¬ÈÏRender¶ÔÏóÊ§°Ü!");
#endif
}

void Plugin_Expand::Uninstall()
{
	g_pDMApp->UnRegister(DM3DAnimate::GetClassName(),DM3DAnimate::GetClassType());
	g_pDMApp->UnRegister(DMLineAnimate::GetClassName(),DMLineAnimate::GetClassType());
	g_pDMApp->UnRegister(DMScreenAnimate::GetClassName(),DMScreenAnimate::GetClassType());
	g_pDMApp->UnRegister(DMResZipImpl::GetClassName(),DMResZipImpl::GetClassType());
	g_pDMApp->UnRegister(DMResMultZipImpl::GetClassName(),DMResMultZipImpl::GetClassType());
#ifdef USE_DMSKIA_RENDER_
	DMCode iErr = g_pDMApp->UnRegister(DMSkiaRenderImpl::GetClassName(),DMSkiaRenderImpl::GetClassType());
	DMASSERT_EXPR(DMSUCCEEDED(iErr), L"·´×¢²áDMImgDecoderImplÊ§°Ü!");
#endif

}

void Plugin_Expand::Initialise()
{
	printf("Plugin_Expand::initialise\n");
}

void Plugin_Expand::Shutdown()
{
	printf("Plugin_Expand::shutdown\n");
}