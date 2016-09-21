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
	{L"Window",		 L"基础控件",		L"系统基础窗口",		L""},
	{L"Button",		 L"基础控件",		L"通用的按钮",			L""},
	{L"Static",		 L"基础控件",		L"静态框",				L"静态框不响应消息"},
	{L"Group",		 L"基础控件",		L"组框",				L""},
	{L"CheckBox",	 L"基础控件",		L"选择框",				L""},
	{L"Link",		 L"基础控件",		L"链接",				L""},
	{L"Combobox",	 L"基础控件",		L"复合框",				L""},
	{L"HotKey",		 L"基础控件",		L"热键",				L""},

	{L"IPAddress",	 L"基础控件",		L"IP框",				L""},
	{L"RadioButton", L"基础控件",		L"单选框",				L""},
	{L"SliderCtrl",	 L"基础控件",		L"滑动条",				L""},
	{L"ProcessCtrl", L"基础控件",		L"进度条",				L""},
	{L"TabCtrl",	 L"基础控件",		L"Tab控件",				L""},
	{L"TabPage",	 L"基础控件",		L"Tab页",				L"仅用于插入到TabCtrl"},
	{L"RichEdit",	 L"基础控件",		L"编辑框(支持多行)",	L""},
	{L"Edit",		 L"基础控件",		L"编辑框",				L""},
	{L"ListBox",	 L"基础控件",		L"列表框",				L"无水平滚动条"},
	{L"ListBoxEx",	 L"扩展控件",		L"扩展列表框",			L""},
	{L"TreeCtrl",	 L"基础控件",		L"树形控件",			L""},
	{L"TreeCtrlEX",	 L"扩展控件",		L"扩展树形控件",		L""},
	{L"HeaderCtrl",	 L"基础控件",		L"列表头",				L""},
	{L"ListCtrlEx",	 L"扩展控件",		L"扩展列表view",		L""},
	{L"Gif",		L"基础控件",		L"Gif动画",				L""},
};
bool AddTipWnd::InitAdd(CStringW strInfo)
{
	int nCount = countof(g_AddInfo);
	for (int i=0; i<nCount; i++)
	{
		if (0 == g_AddInfo[i].strOne.CompareNoCase(strInfo))
		{
			FindChildByNameT<DUIStatic>(L"ds_add_text1")->DV_SetWindowText(g_AddInfo[i].strOne);
			FindChildByNameT<DUIStatic>(L"ds_add_text2")->DV_SetWindowText(g_AddInfo[i].strTwo);
			FindChildByNameT<DUIStatic>(L"ds_add_text3")->DV_SetWindowText(g_AddInfo[i].strThree);
			FindChildByNameT<DUIStatic>(L"ds_add_text4")->DV_SetWindowText(g_AddInfo[i].strFour);
		}
	}
	return true;
}
