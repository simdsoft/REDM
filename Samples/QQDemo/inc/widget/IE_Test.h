#pragma once
#include "IEWnd.h"

class IE_Test
{
public:
	static DMCode OnBtnIeBack();
	static DMCode OnBtnIeRefresh();
	static DMCode OnBtnIefront();
	static DMCode IERefreshWeb(DMEventArgs* pEvent);
	static DMCode OnBtnIeJsTest();

public:
	static IDUIWeb* GetIeObj();

};