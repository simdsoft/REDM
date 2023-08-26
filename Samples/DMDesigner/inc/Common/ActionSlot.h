// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ActionSlot.h
// File mark:   
// File summary:
// Author:		lzlong
// Edition:     1.0
// Create date: 2019-3-20
// ----------------------------------------------------------------
#pragma once

enum ActionSlotType
{
	ActionSlotTypeBase = -1,
	NullSlot,
	EditorElemPosChangeSlot,
};

class ActionSlot
{
public:
	ActionSlot();
	virtual ~ActionSlot();

	virtual DMCode PerformUndoActionSlot() = 0;
	virtual DMCode PerformRedoActionSlot() = 0;
	bool   IsActExCuteAble()
	{
		return (GetSlotID() != NullSlot);
	}
	virtual UINT GetSlotID() = 0;

private:
};

//---------------------------------------------------
// Function Des: 空动作类型  每个动作之间需要空动作来填充 
//---------------------------------------------------
class NullActionSlot : public ActionSlot
{
public:
	NullActionSlot();
	~NullActionSlot();

	virtual DMCode PerformUndoActionSlot();
	virtual DMCode PerformRedoActionSlot();
	virtual UINT GetSlotID()
	{
		return NullSlot;
	}
};


//---------------------------------------------------
// Function Des: Editor里面的元素位置改变项动作描述
//---------------------------------------------------
class EditorElemPosChgActionSlot : public ActionSlot
{ 
public:
	EditorElemPosChgActionSlot(HDMTREEITEM hItem, CRect OldRect, CRect NewRect, bool bMain, ObjTree*, DUIDragFrame*);
	~EditorElemPosChgActionSlot();

	virtual UINT GetSlotID()
	{
		return EditorElemPosChangeSlot;
	}
	virtual DMCode PerformUndoActionSlot();
	virtual DMCode PerformRedoActionSlot();

private:

	DUIDragFrame*			m_pDragFrame;
	ObjTree*				m_pObjTree;
	HDMTREEITEM				m_hItem;
	DM::LPTVITEMEX			m_pTreeDataLaw;
	LPARAM					m_pTreeParam;
	bool					m_bMain;
	CRect					m_OldRect;
	CRect					m_NewRect;
};