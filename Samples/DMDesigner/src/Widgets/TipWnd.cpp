#include "DMDesignerAfx.h"
#include "TipWnd.h"

bool AttrTipWnd::InitAttr(DMXmlInitAttrPtr ptr)
{
	bool bRet = false;
	do 
	{
		CStringW strType,strName,strValue,strDesc;
		FindChildByNameT<DUIStatic>(L"ds_attr")->SetAttribute(L"text",L"未知属性");
		FindChildByNameT<DUIStatic>(L"ds_type")->SetAttribute(L"text",strType);
		FindChildByNameT<DUIStatic>(L"ds_name")->SetAttribute(L"text",strName);
		FindChildByNameT<DUIStatic>(L"ds_desc")->SetAttribute(L"text",strDesc);
		if (NULL == ptr)
		{
			break;
		}
	
		g_pAttr->Parse(ptr->m_pAttr,strType,strName,strValue,strDesc);
		FindChildByNameT<DUIStatic>(L"ds_attr")->SetAttribute(L"text",ptr->m_pAttr->GetName());
		FindChildByNameT<DUIStatic>(L"ds_type")->SetAttribute(L"text",strType);
		FindChildByNameT<DUIStatic>(L"ds_name")->SetAttribute(L"text",strName);
		FindChildByNameT<DUIStatic>(L"ds_desc")->SetAttribute(L"text",strDesc);

		bRet = true;
	} while (false);
	return bRet;
}


//AddTipWnd---------------------------------------------------
MultStr g_AddInfo[] = \
{
	{L"window",		 L"基础控件",		L"系统基础窗口",		L""},
	{L"button",		 L"基础控件",		L"通用的按钮",			L""},
	{L"label",		 L"基础控件",		L"标签",				L"标签不响应消息"},
	{L"group",		 L"基础控件",		L"组框",				L""},
	{L"checkbox",	 L"基础控件",		L"选择框",				L""},
	{L"link",		 L"基础控件",		L"链接",				L""},
	{L"combobox",	 L"基础控件",		L"复合框",				L""},
	{L"hotkey",		 L"基础控件",		L"热键",				L""},

	{L"ipaddress",	 L"基础控件",		L"IP框",				L""},
	{L"radiobutton", L"基础控件",		L"单选框",				L""},
	{L"sliderctrl",	 L"基础控件",		L"滑动条",				L""},
	{L"processctrl", L"基础控件",		L"进度条",				L""},
	{L"tabctrl",	 L"基础控件",		L"Tab控件",				L""},
	{L"tabpage",	 L"基础控件",		L"Tab页",				L"仅用于插入到TabCtrl"},
	{L"richedit",	 L"基础控件",		L"编辑框(支持多行)",	L""},
	{L"edit",		 L"基础控件",		L"编辑框",				L""},
	{L"listbox",	 L"基础控件",		L"列表框",				L"无水平滚动条"},
	{L"listboxex",	 L"扩展控件",		L"扩展列表框",			L""},
	{L"treectrl",	 L"基础控件",		L"树形控件",			L""},
	{L"treectrlex",	 L"扩展控件",		L"扩展树形控件",		L""},
	{L"headerctrl",	 L"基础控件",		L"列表头",				L""},
	{L"listctrlex",	 L"扩展控件",		L"扩展列表view",		L""},
	{L"gif",		 L"基础控件",		L"Gif动画",				L""},
	{L"pnggif",		 L"基础控件",		L"PngGif控件",		    L""},
	{L"ie",			 L"基础控件",		L"IE控件",				L""},
	{L"splitlayout", L"基础控件",		L"SplitLayout控件",		L"分隔两个窗口"},
	{L"monthcalctrl", L"基础控件",		L"日历控件",			L""}, 
	{L"scrollwnd",	L"基础控件",		L"滚动条容器控件",		L"允许插入多个超过容器范围的子窗口"}, 
}; 
bool AddTipWnd::InitAdd(CStringW strInfo)
{
	bool bFind = false;
	int nCount = countof(g_AddInfo);
	for (int i=0; i<nCount; i++)
	{
		if (0 == g_AddInfo[i].strOne.CompareNoCase(strInfo))
		{
			FindChildByNameT<DUIStatic>(L"ds_add_text1")->DV_SetWindowText(g_AddInfo[i].strOne);
			FindChildByNameT<DUIStatic>(L"ds_add_text2")->DV_SetWindowText(g_AddInfo[i].strTwo);
			FindChildByNameT<DUIStatic>(L"ds_add_text3")->DV_SetWindowText(g_AddInfo[i].strThree);
			FindChildByNameT<DUIStatic>(L"ds_add_text4")->DV_SetWindowText(g_AddInfo[i].strFour);
			bFind = true;
		}
	}
	if (!bFind)
	{
		FindChildByNameT<DUIStatic>(L"ds_add_text1")->DV_SetWindowText(strInfo);
		FindChildByNameT<DUIStatic>(L"ds_add_text2")->DV_SetWindowText(L"基础控件");
		FindChildByNameT<DUIStatic>(L"ds_add_text3")->DV_SetWindowText(L"基础控件");
		FindChildByNameT<DUIStatic>(L"ds_add_text4")->DV_SetWindowText(L"");
	}
	return true;
}
