#pragma once

class TabCtrl_Test
{
public:
	static DMCode OnInsert(DMEventArgs *pEvt);
	static DMCode OnDelete(DMEventArgs *pEvt);
	static DMCode OnSelChanging(DMEventArgs *pEvt);

public:
	static int                m_nInsertCount;
};