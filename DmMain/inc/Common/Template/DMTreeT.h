//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMTreeT.h 
// File Des: Tree基础抽离成模板
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-2-22	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
#pragma warning(push)
#pragma warning(disable: 4312) 
#define DMTVI_ROOT     ((HDMTREEITEM)0xFFFF0000)///<=TVI_ROOT
#define DMTVI_FIRST    ((HDMTREEITEM)0xFFFF0001)///<=TVI_FIRST
#define DMTVI_LAST     ((HDMTREEITEM)0xFFFF0002)///<=TVI_LAST
#define DMTVN_ROOT     ((HDMTREENODE)(ULONG_PTR)0xFFFF0000)///<=DMTVN_ROOT
#define DMTVN_FIRST    ((HDMTREENODE)(ULONG_PTR)0xFFFF0001)///<=DMTVN_FIRST
#define DMTVN_LAST     ((HDMTREENODE)(ULONG_PTR)0xFFFF0002)///<=DMTVN_LAST
#define STVL_ROOT      ((PDMTREELINK)0xFFFF0000)

	template<class T>
	class DMTreeT  
	{
		///-----------------------------------------
		///  节点结构体
		typedef struct _DMTREENODE
		{
			struct _DMTREENODE *hParent;      ///< 父节点
			struct _DMTREENODE *hChildFirst;  ///< 第一个子节点
			struct _DMTREENODE *hChildLast;   ///< 最后一个子节点
			struct _DMTREENODE *hPrevSibling; ///< 兄节点
			struct _DMTREENODE *hNextSibling; ///< 弟节点
			T data;                           ///< 数据
		}DMTREENODE,*HDMTREENODE;

		///-----------------------------------------
		/// 此结构体用于连接树节点
		typedef struct _DMTREELINK
		{
			HDMTREENODE  hParent;			 ///< 父节点
			HDMTREENODE  hChildFirst;		 ///< 第一个子节点
			HDMTREENODE  hChildLast;		 ///< 最后一个子节点
			HDMTREENODE  hPrevSibling;		 ///< 兄节点
			HDMTREENODE  hNextSibling;		 ///< 弟节点
		}DMTREELINK,*PDMTREELINK;

		///-----------------------------------------
		/// 遍历一个树结点的回调函数原型
		typedef bool (*CBTRAVERSING)(T*,LPARAM);

	public:
		DMTreeT()
		{
			m_hRootFirst = NULL;
			m_hRootLast  = NULL;
		}

		virtual ~DMTreeT()
		{
			DeleteAllItems();
		}

		void DeleteAllItems()
		{
			if (m_hRootFirst)
			{
				FreeNode(DMTVN_ROOT);
				m_hRootFirst = NULL;
				m_hRootLast  = NULL;
			}
		}

		///-----------------------------------------
		/// 获取下一个兄弟
		static HDMTREEITEM GetNextSiblingItem(HDMTREEITEM hItem)
		{
			PDMTREELINK pLink= (PDMTREELINK)hItem;
			DMASSERT(pLink&&pLink!=STVL_ROOT);
			return (HDMTREEITEM)pLink->hNextSibling;
		}

		///-----------------------------------------
		/// 获取上一个兄弟
		static HDMTREEITEM GetPrevSiblingItem(HDMTREEITEM hItem)
		{
			PDMTREELINK pLink  = (PDMTREELINK)hItem;
			DMASSERT(pLink&&pLink!=STVL_ROOT);
			return (HDMTREEITEM)pLink->hPrevSibling;
		}

		///-----------------------------------------
		/// 获取父结点
		static HDMTREEITEM GetParentItem(HDMTREEITEM hItem)
		{
			PDMTREELINK pLink= (PDMTREELINK)hItem;
			DMASSERT(pLink&&pLink!=STVL_ROOT);
			return (HDMTREEITEM)pLink->hParent;
		}

		///< 获取结点层数
		static int GetItemLevel(HDMTREEITEM hItem)
		{
			int nRet = -1;
			if (hItem==DMTVI_ROOT) 
			{
				hItem = NULL;
			}
			while (hItem)
			{
				nRet++;
				hItem = GetParentItem(hItem);
			}
			return nRet;
		}

		///< HDMTREEITEM 根节点
		static HDMTREEITEM GetRootItem(HDMTREEITEM hItem)
		{
			HDMTREEITEM hParent=hItem;
			while(GetParentItem(hParent))
			{
				hParent = GetParentItem(hParent);
			}
			return hParent;
		}

		///< 获取第一个子结点,
		HDMTREEITEM GetChildItem(HDMTREEITEM hItem,bool bFirst=true)
		{
			HDMTREENODE hsNode = (HDMTREENODE)hItem;
			DMASSERT(hsNode);
			if (hsNode==DMTVN_ROOT)
			{
				if (bFirst)
				{
					return (HDMTREEITEM)m_hRootFirst;
				}
				else
				{
					return (HDMTREEITEM)m_hRootLast;
				}
			}
			else
			{
				if (bFirst)
				{
					return (HDMTREEITEM)hsNode->hChildFirst;
				}
				else
				{
					return (HDMTREEITEM)hsNode->hChildLast;
				}
			}
		}

		///< 获取子结点数量
		int GetChildrenCount(HDMTREEITEM hItem)
		{
			int nRet = 0;
			HDMTREEITEM hChild = GetChildItem(hItem);
			while (hChild)
			{
				nRet++;
				hChild = GetNextSiblingItem(hChild);
			}
			return nRet;
		}

		///< 删除一个节点，可以被派生类重载
		virtual void DeleteItem(HDMTREEITEM hItem)
		{
			HDMTREENODE hsNode = (HDMTREENODE)hItem;
			DMASSERT(hsNode);
			if (hsNode==DMTVN_ROOT)
			{
				FreeNode(DMTVN_ROOT);
				m_hRootFirst = NULL;
				m_hRootLast = NULL;
				return;
			}
			DMTREENODE nodeCopy = *hsNode;
			bool bRootFirst=hsNode==m_hRootFirst;
			bool bRootLast=hsNode==m_hRootLast;
			FreeNode(hsNode);

			if (nodeCopy.hPrevSibling)///<has prevsibling
			{
				nodeCopy.hPrevSibling->hNextSibling=nodeCopy.hNextSibling;
			}
			else if (nodeCopy.hParent)///<parent's first child
			{
				nodeCopy.hParent->hChildFirst=nodeCopy.hNextSibling;
			}
			if (nodeCopy.hNextSibling)///< update next sibling's previous sibling
			{
				nodeCopy.hNextSibling->hPrevSibling=nodeCopy.hPrevSibling;
			}
			else if (nodeCopy.hParent)///<parent's last child
			{
				nodeCopy.hParent->hChildLast=nodeCopy.hPrevSibling;
			}
			///<update root item
			if (bRootFirst)   
			{
				m_hRootFirst=nodeCopy.hNextSibling;
			}
			if (bRootLast) 
			{
				m_hRootLast=nodeCopy.hPrevSibling;
			}
		}

		///< 删除一个结点分枝，如果该结点的父结点没有其它子节点则一起删除
		bool DeleteItemEx(HDMTREEITEM hItem)
		{
			if(GetChildItem(hItem)) return false;
			while(hItem && !GetChildItem(hItem))
			{
				HDMTREEITEM hParent=GetParentItem(hItem);
				DeleteItem(hItem);
				hItem=hParent;
			}
			return true;
		}

		///< 获取结点中保存的数据
		static T GetItem(HDMTREEITEM hItem)
		{
			DMASSERT(hItem!=DMTVI_ROOT);
			HDMTREENODE hsNode = (HDMTREENODE)hItem;
			DMASSERT(hsNode);
			return hsNode->data;
		}

		///< 获取结点中保存的数据
		static T *GetItemPt(HDMTREEITEM hItem)
		{
			DMASSERT(hItem!=DMTVI_ROOT);
			HDMTREENODE hsNode = (HDMTREENODE)hItem;
			DMASSERT(hsNode);
			return &hsNode->data;
		}

		///< 插入一个新结点,data:结点数据hParent:新结点的父结点,hInsertAfter:新结点的前一个兄弟结点,返回HDMTREEITEM 新结点的指针
		HDMTREEITEM InsertItem(const T &data,HDMTREEITEM hParent=DMTVI_ROOT,HDMTREEITEM hInsertAfter=DMTVI_LAST)
		{
			HDMTREENODE hParentNode      = (HDMTREENODE) hParent;
			HDMTREENODE hInsertAfterNode = (HDMTREENODE) hInsertAfter;
			if (hParentNode==DMTVN_ROOT)
			{
				hParentNode = NULL;
			}
			DMASSERT(hInsertAfter);
			if (hInsertAfterNode!=DMTVN_FIRST && hInsertAfterNode!=DMTVN_LAST)
			{
				if (hInsertAfterNode->hParent!=hParentNode) 
				{
					return NULL;
				}
				if (hInsertAfterNode->hNextSibling==NULL)
				{
					hInsertAfterNode=DMTVN_LAST;
				}
			}

			HDMTREENODE hInserted=new DMTREENODE;
			hInserted->data=data;
			hInserted->hParent=hParentNode;
			hInserted->hChildFirst=NULL;
			hInserted->hChildLast=NULL;

			if (hInsertAfterNode==DMTVN_FIRST)
			{
				hInserted->hPrevSibling=NULL;
				if(hParentNode==NULL)///<root
				{
					hInserted->hNextSibling=m_hRootFirst;
					if (m_hRootFirst)
					{
						m_hRootFirst->hPrevSibling=hInserted;
					}
					m_hRootFirst=hInserted;
					if (m_hRootLast==NULL) 
					{
						m_hRootLast=hInserted;
					}
				}
				else ///<has parent
				{
					hInserted->hNextSibling=hParentNode->hChildFirst;
					if (hInserted->hNextSibling)
					{
						hInserted->hNextSibling->hPrevSibling=hInserted;
						hParentNode->hChildFirst=hInserted;
					}
					else
					{
						hParentNode->hChildLast=hParentNode->hChildFirst=hInserted;
					}
				}
			}
			else if(hInsertAfterNode==DMTVN_LAST)
			{
				hInserted->hNextSibling=NULL;
				if (hParentNode==NULL)///<root
				{
					hInserted->hPrevSibling=m_hRootLast;
					if (m_hRootLast) 
					{
						m_hRootLast->hNextSibling=hInserted;
					}
					m_hRootLast=hInserted;
					if (!m_hRootFirst) 
					{
						m_hRootFirst=hInserted;
					}
				}else
				{
					hInserted->hPrevSibling=hParentNode->hChildLast;
					if (hParentNode->hChildLast) 
					{
						hInserted->hPrevSibling->hNextSibling=hInserted;
						hParentNode->hChildLast=hInserted;
					}
					else
					{
						hParentNode->hChildLast=hParentNode->hChildFirst=hInserted;
					}                
				}
			}
			else
			{
				HDMTREENODE hNextSibling=hInsertAfterNode->hNextSibling;
				hInserted->hPrevSibling=hInsertAfterNode;
				hInserted->hNextSibling=hNextSibling;
				hNextSibling->hPrevSibling = hInsertAfterNode->hNextSibling = hInserted;
			}
			return (HDMTREEITEM)hInserted;
		}

		///< 采用递归方式遍历一个树结点,CBTRAVERSING为回调函数,返回被中止的树结点
		HDMTREEITEM TraversingRecursion(HDMTREEITEM hItem,CBTRAVERSING funTraversing,LPARAM lParam)
		{
			DMASSERT(hItem);
			if (hItem!=DMTVI_ROOT)
			{
				if (funTraversing(GetItemPt(hItem),lParam))
				{
					return hItem;
				}
			}

			HDMTREEITEM hChild = GetChildItem(hItem);
			while (hChild)
			{
				HDMTREEITEM hTmp = GetChildItem(hChild);
				if (hTmp)
				{
					HDMTREEITEM hRet=TraversingRecursion(hTmp,funTraversing,lParam);
					if (hRet) 
					{
						return hRet;
					}
				}
				else
				{
					if (funTraversing(GetItemPt(hChild),lParam)) 
					{
						return hChild;
					}
				}
				hChild = GetNextSiblingItem(hChild);
			}
			return NULL;
		}

		///< 按顺序方式从指定结点开始查找后面的结点，包括自己的子节点及自己向下的兄弟结点
		HDMTREEITEM TraversingSequence(HDMTREEITEM hItem,CBTRAVERSING funTraversing,LPARAM lParam)
		{
			if (!m_hRootFirst) 
			{
				return NULL;
			}
			if (hItem!=DMTVI_ROOT)
			{
				if (funTraversing(GetItemPt(hItem),lParam)) 
				{
					return hItem;
				}
			}
			HDMTREEITEM hNext = GetNextItem(hItem);
			while (hNext)
			{
				if (funTraversing(GetItemPt(hNext),lParam))
				{
					return hNext;
				}
				hNext = GetNextItem(hNext);
			}
			return NULL;
		}

		///< 获取根节点
		HDMTREEITEM GetRootItem(bool bFirst=true)
		{
			return (HDMTREEITEM)(bFirst?m_hRootFirst:m_hRootLast);
		}

		///< 获取树形控件的总数目
		int GetCount()
		{
			HDMTREEITEM hRoot = (HDMTREEITEM)m_hRootFirst;
			if (NULL == hRoot)
			{
				return 0;
			}
			return GetDesendants(hRoot)+1;
		}

		///< 获取结点的子孙结点数
		int GetDesendants(HDMTREEITEM hItem)
		{
			int nRet = 0;
			HDMTREEITEM hChild = GetChildItem(hItem);
			while (hChild)
			{
				nRet += 1+GetDesendants(hChild);
				hChild = GetNextSiblingItem(hChild);
			}
			return nRet;
		}

		///< 获取当前结点的下一个结点
		/*Describe  获取当前结点的下一个结点
		* 如果当前结点有子结点，则返回自己的第一个子结点，
		* 否则如果有向下的兄弟结点，则返回自己向下兄弟结点、
		* 否则搜索自己的父结点的向下兄弟结点
		*/
		HDMTREEITEM GetNextItem(HDMTREEITEM hItem)
		{
			if (hItem==DMTVI_ROOT)
			{
				return (HDMTREEITEM)m_hRootFirst;
			}

			HDMTREEITEM hRet = GetChildItem(hItem);
			if (hRet)
			{
				return hRet;
			}
			HDMTREEITEM hParent = hItem;
			while (hParent)
			{
				hRet = GetNextSiblingItem(hParent);
				if (hRet) 
				{
					return hRet;
				}
				hParent = GetParentItem(hParent);
			}
			return NULL;
		}

		///< 获取当前结点的下一个结点
		/* int &nLevel -- 当前结点(hItem)与目标结点(return)的层次关系:1-父子关系，0－兄弟关系，-n－子->父的兄弟
		* Describe  获取当前结点的下一个结点
		*           如果当前结点有子结点，则返回自己的第一个子结点，
		*           否则如果有向下的兄弟结点，则返回自己向下兄弟结点、
		*           否则搜索自己的父结点的向下兄弟结点
		*/
		HDMTREEITEM GetNextItem(HDMTREEITEM hItem,int &nLevel)
		{
			if (hItem==DMTVI_ROOT)
			{
				nLevel = 1;
				return (HDMTREEITEM)m_hRootFirst;
			}

			HDMTREEITEM hRet = GetChildItem(hItem);
			if (hRet)
			{
				nLevel = 1;
				return hRet;
			}
			HDMTREEITEM hParent = hItem;
			nLevel = 0;
			while (hParent)
			{
				hRet = GetNextSiblingItem(hParent);
				if (hRet) 
				{
					return hRet;
				}
				nLevel--;
				hParent = GetParentItem(hParent);
			}
			return NULL;
		}
	public:///< 辅助
		///< 采用后序遍历的方式释放结点占用的空间
		void FreeNode(HDMTREENODE hsNode)
		{
			DMASSERT(hsNode);
			if (hsNode != DMTVN_ROOT)
			{
				OnNodeFree(hsNode->data);// 在树形结构破坏前释放
			}
			HDMTREENODE hSibling=(HDMTREENODE)GetChildItem((HDMTREEITEM)hsNode);
			while (hSibling)
			{
				HDMTREENODE hNextSibling=hSibling->hNextSibling;
				FreeNode(hSibling);
				hSibling=hNextSibling;
			}
			if (hsNode!=DMTVN_ROOT)
			{
				delete hsNode;
			}
		}

	public:///< 可重载
		///< 在派生类中实现数据的释放操作
		virtual void OnNodeFree(T & data){}

	public:
		HDMTREENODE  m_hRootFirst;  ///< 第一个根节点
		HDMTREENODE  m_hRootLast;   ///< 最后一个根节点
	};

#pragma warning(pop)
}//namespace DM