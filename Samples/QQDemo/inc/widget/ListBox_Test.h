#pragma once

class ListBox_Test
{
public:
	static DMCode OnInsert(DMEventArgs *pEvt);
	static DMCode OnDelete(DMEventArgs *pEvt);

public:
	static int                m_nInsertCount;
};