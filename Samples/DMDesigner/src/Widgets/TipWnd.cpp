#include "DMDesignerAfx.h"
#include "TipWnd.h"

#pragma execution_character_set("utf-8")

bool AttrTipWnd::InitAttr(DMXmlInitAttrPtr ptr)
{
	bool bRet = false;
	do 
	{
		CStringA strType,strName,strValue,strDesc;
		FindChildByNameT<DUIStatic>("ds_attr")->SetAttribute("text","nil");
		FindChildByNameT<DUIStatic>("ds_type")->SetAttribute("text",strType);
		FindChildByNameT<DUIStatic>("ds_name")->SetAttribute("text",strName);
		FindChildByNameT<DUIStatic>("ds_desc")->SetAttribute("text",strDesc);
		if (NULL == ptr)
		{
			break;
		}
	
		g_pAttr->Parse(ptr->m_pAttr,strType,strName,strValue,strDesc);
		FindChildByNameT<DUIStatic>("ds_attr")->SetAttribute("text",ptr->m_pAttr->GetName());
		FindChildByNameT<DUIStatic>("ds_type")->SetAttribute("text",strType);
		FindChildByNameT<DUIStatic>("ds_name")->SetAttribute("text",strName);
		FindChildByNameT<DUIStatic>("ds_desc")->SetAttribute("text",strDesc);

		bRet = true;
	} while (false);
	return bRet;
}


//AddTipWnd---------------------------------------------------
MultStr g_AddInfo[] = \
{
	{"window",		 "基础控件",		"系统基础窗口",		    ""},
	{"button",		 "基础控件",		"通用的按钮",			""},
	{"label",		 "基础控件",		"标签",				    "标签不响应消息"},
	{"group",		 "基础控件",		"组框",				    ""},
	{"checkbox",	 "基础控件",		"选择框",				""},
	{"link",		 "基础控件",		"链接",				    ""},
	{"combobox",	 "基础控件",		"复合框",				""},
	{"hotkey",		 "基础控件",		"热键",				    ""},

	{"ipaddress",	 "基础控件",		"IP框",			    	""},
	{"radiobutton", "基础控件",		"单选框",				""},
	{"sliderctrl",	 "基础控件",		"滑动条",				""},
	{"processctrl", "基础控件",		"进度条",				""},
	{"tabctrl",	 "基础控件",		"Tab控件",				""},
	{"tabpage",	 "基础控件",		"Tab页",				"仅用于插入到TabCtrl"},
	{"richedit",	 "基础控件",		"编辑框(支持多行)",	    ""},
	{"edit",		 "基础控件",		"编辑框",				""},
	{"listbox",	 "基础控件",		"列表框",				"无水平滚动条"},
	{"listboxex",	 "扩展控件",		"扩展列表框",			""},
	{"treectrl",	 "基础控件",		"树形控件",			    ""},
	{"treectrlex",	 "扩展控件",		"扩展树形控件",		    ""},
	{"headerctrl",	 "基础控件",		"列表头",				""},
	{"listctrlex",	 "扩展控件",		"扩展列表view",	    	""},
	{"gif",		 "基础控件",		"Gif动画",				""},
	{"pnggif",		 "基础控件",		"PngGif控件",		    ""},
	{"ie",			 "基础控件",		"IE控件",				""},
	{"splitlayout", "基础控件",		"SplitLayout控件",		"分隔两个窗口"},
	{"monthcalctrl", "基础控件",		"日历控件",		    	""}, 
	{"scrollwnd",	  "基础控件",		"滚动条容器控件",	    	"允许插入多个超过容器范围的子窗口"}, 
}; 
bool AddTipWnd::InitAdd(CStringA strInfo)
{
	bool bFind = false;
	int nCount = countof(g_AddInfo);
	for (int i=0; i<nCount; i++)
	{
		if (0 == g_AddInfo[i].strOne.CompareNoCase(strInfo))
		{
			FindChildByNameT<DUIStatic>("ds_add_text1")->SetTextA(g_AddInfo[i].strOne);
			FindChildByNameT<DUIStatic>("ds_add_text2")->SetTextA(g_AddInfo[i].strTwo);
			FindChildByNameT<DUIStatic>("ds_add_text3")->SetTextA(g_AddInfo[i].strThree);
			FindChildByNameT<DUIStatic>("ds_add_text4")->SetTextA(g_AddInfo[i].strFour);
			bFind = true;
		}
	}
	if (!bFind)
	{
		FindChildByNameT<DUIStatic>("ds_add_text1")->SetTextA(strInfo);
		FindChildByNameT<DUIStatic>("ds_add_text2")->SetText(L"基础控件");
		FindChildByNameT<DUIStatic>("ds_add_text3")->SetText(L"基础控件");
		FindChildByNameT<DUIStatic>("ds_add_text4")->SetText(L"");
	}
	return true;
}
