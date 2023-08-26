// ----------------------------------------------------------------
// Copyright (c)  
// All rights reserved.
// 
// File name:	ActionSlotMgr.h
// File mark:   
// File summary:负责各类动作的前进后退队列维护
//				链表节点分布 NullNode->ActionNode->NullNode->ActionNode->NullNode 
//				1.当前节点一定是指向NullNode  Undo的时候当前节点后退一步  指向ActionNode 然后执行动作，执行完再后退一步指向NullNode，Redo的时候动作相反
//				2.每次添加完节点后马上跟着添加一个NullNode
//				3.当前动作如果执行失败，忽略这个动作，然后往前或往后推进执行
// Author:		lzlong
// Edition:     1.0
// Create date: 2019-3-20
// ----------------------------------------------------------------
#pragma once

template<class T>
class ActionSlotMgr
{
	///-----------------------------------------
	///  节点结构体
	typedef struct _ACTIONLINKNODE
	{
		struct _ACTIONLINKNODE *hPrevSibling; ///< 兄节点
		struct _ACTIONLINKNODE *hNextSibling; ///< 弟节点
		T* data;							  ///< 数据
	}ACTIONLINKNODE, *HACTIONLINKNODE;

public:
	ActionSlotMgr()
	{
		m_bMuteAddSlot = false;
		m_hCurAction = NULL;
		m_hActionFirst = NULL;
	}

	virtual ~ActionSlotMgr()
	{
		FreeAllActionSlot();
	}

	void FreeAllActionSlot()
	{
		m_hCurAction = NULL;
		if (m_hActionFirst) FreeNode(m_hActionFirst);
		m_hActionFirst = NULL;
	}

	void InsertNewAction(T* action)
	{
		if (m_bMuteAddSlot) //屏蔽 直接delete掉
		{
			OnNodeFree(action);
			return;
		}
		HACTIONLINKNODE hInserted = new ACTIONLINKNODE;
		hInserted->data = action;
		hInserted->hPrevSibling = NULL;
		hInserted->hNextSibling = NULL;
		if (!m_hCurAction) //first item
		{
			m_hCurAction = hInserted;
			m_hCurAction->hPrevSibling = NULL;
			m_hCurAction->hNextSibling = NULL;
			m_hActionFirst = m_hCurAction;
		}
		else 
		{//建立链表
			if (m_hCurAction->hNextSibling) //有下一项 后面的释放掉  新加入的就是最后一项了
			{
				FreeNode(m_hCurAction->hNextSibling);
			}
			m_hCurAction->hNextSibling = hInserted;
			hInserted->hPrevSibling = m_hCurAction;
			m_hCurAction = hInserted;
		}
	}

	DMCode ExcutePrevSiblingAction()
	{
		DMCode iRet = DM_ECODE_FAIL;
		do 
		{
			if (!m_hCurAction)
				break;
			bool bActExcuteAble = m_hCurAction->data->IsActExCuteAble(); //当前一定是NullNode
			DMASSERT(!bActExcuteAble);
			if (!m_hCurAction->hPrevSibling)
				break;
			m_hCurAction = m_hCurAction->hPrevSibling;
			DMASSERT(m_hCurAction);
				
			iRet = m_hCurAction->data->PerformUndoActionSlot();
			m_hCurAction = m_hCurAction->hPrevSibling;
			DMASSERT(m_hCurAction && !m_hCurAction->data->IsActExCuteAble());//可执行动作前面一定是NullNode
		} while (DM_ECODE_OK != iRet); //直到成功的动作
		return iRet;
	}

	DMCode ExcuteNextSiblingAction()
	{
		DMCode iRet = DM_ECODE_FAIL;
		do
		{
			if (!m_hCurAction)
				break;
			bool bActExcuteAble = m_hCurAction->data->IsActExCuteAble(); //当前一定是NullNode
			DMASSERT(!bActExcuteAble);
			if (!m_hCurAction->hNextSibling)
				break;
			m_hCurAction = m_hCurAction->hNextSibling;
			DMASSERT(m_hCurAction);

			iRet = m_hCurAction->data->PerformRedoActionSlot();
			m_hCurAction = m_hCurAction->hNextSibling;
			DMASSERT(m_hCurAction && !m_hCurAction->data->IsActExCuteAble());//可执行动作后面一定是NullNode
		} while (DM_ECODE_OK != iRet); //直到成功的动作
		return iRet;
	}

	//是否可以undo了
	bool IsExistPrevSiblingSteps()
	{
		if (!m_hCurAction)
			return false;
		return (NULL != m_hCurAction->hPrevSibling);
	}

	//是否可以redo了
	bool IsExistNextSiblingSteps()
	{
		if (!m_hCurAction)
			return false;
		return (NULL != m_hCurAction->hNextSibling);
	}

	T* GetCurActData()
	{
		return m_hCurAction->data;
	}
	
	DMCode WalkPrevSiblingStep()
	{
		DMCode iRet = DM_ECODE_FAIL;
		do
		{
			if (!m_hCurAction)
				break;
			if (!m_hCurAction->hPrevSibling)
				break;

			m_hCurAction = m_hCurAction->hPrevSibling;
			iRet = DM_ECODE_OK;
		} while (DM_ECODE_OK != iRet); //直到成功的动作
		return iRet;
	}

	DMCode WalkNexSiblingStep()
	{
		DMCode iRet = DM_ECODE_FAIL;
		do
		{
			if (!m_hCurAction)
				break;
			if (!m_hCurAction->hNextSibling)
				break;

			m_hCurAction = m_hCurAction->hNextSibling;
			iRet = DM_ECODE_OK;
		} while (DM_ECODE_OK != iRet); //直到成功的动作
		return iRet;
	}

	void FreeNode(HACTIONLINKNODE& hsNode)
	{
		DMASSERT(hsNode);
		HACTIONLINKNODE hSibling = hsNode->hNextSibling;
		if (hSibling) FreeNode(hSibling);
		hsNode->hNextSibling = NULL;
		hsNode->hPrevSibling = NULL;
		OnNodeFree(hsNode->data);
		delete hsNode;
		hsNode = NULL;
	}

	virtual void OnNodeFree(T* & data)
	{
		delete data;
	}

	bool& GetMemberbMuteAddSlot()
	{
		return m_bMuteAddSlot;
	}

	bool IsActionArrayChainEmpty()
	{
		return NULL == m_hCurAction;
	}
private:
	HACTIONLINKNODE m_hCurAction;
	HACTIONLINKNODE	m_hActionFirst;
	bool			m_bMuteAddSlot;
};