#include "DmMainAfx.h"
#include "DUIDropTargetDispatch.h"

namespace DM
{
	// DUIDropTargetDispatch------------------------------------
	DUIDropTargetDispatch::DUIDropTargetDispatch(DUIWindow* pOwner)
		:m_pOwner(pOwner)
	{
		m_pDataObj		 = NULL;
		m_hDUIHoverWnd   = 0;
	}

	DUIDropTargetDispatch::~DUIDropTargetDispatch(void)
	{
		DragLeave();
		POSITION pos = m_mapDropTarget.GetStartPosition();
		while (pos)
		{
			DTMAP::CPair *pPair = m_mapDropTarget.GetNext(pos);
			pPair->m_value->Release();
		}
	}

	bool DUIDropTargetDispatch::RegisterDragDrop(DUIWND hDUIWnd, IDropTarget* pDropTarget)
	{
		bool bRet = false;
		do 
		{
			if (m_mapDropTarget.Lookup(hDUIWnd))
			{
				break;// 已注册了
			}

			m_mapDropTarget[hDUIWnd] = pDropTarget;
			pDropTarget->AddRef();
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIDropTargetDispatch::UnRegisterDragDrap(DUIWND hDUIWnd)
	{
		bool bRet = false;
		do 
		{
			DTMAP::CPair* pPair = m_mapDropTarget.Lookup(hDUIWnd);
			if (NULL == pPair)
			{
				break;
			}

			pPair->m_value->Release();
			m_mapDropTarget.RemoveKey(hDUIWnd);
			bRet = true;
		} while (false);
		return bRet;
	}

	//IUnkown
	HRESULT STDMETHODCALLTYPE DUIDropTargetDispatch::QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject)
	{
		HRESULT hr = S_OK;// E_NOINTERFACE;
		do 
		{
			if (__uuidof(IUnknown) == riid)
			{
				*ppvObject = (IUnknown*) this;
				break;
			}
			if (__uuidof(IDropTarget) == riid)
			{
				*ppvObject = (IDropTarget*)this;
				break;
			}

			hr = E_NOINTERFACE;
		} while (false);
		return hr;
	}

	ULONG STDMETHODCALLTYPE DUIDropTargetDispatch::AddRef(void)
	{
		return 1;
	}

	ULONG STDMETHODCALLTYPE DUIDropTargetDispatch::Release(void)
	{ 
		return 1;
	}

	HRESULT STDMETHODCALLTYPE DUIDropTargetDispatch::DragEnter(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		m_pDataObj = pDataObj;
		m_pDataObj->AddRef();
		return DragOver(grfKeyState,pt,pdwEffect);
	}

	HRESULT STDMETHODCALLTYPE DUIDropTargetDispatch::DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		do 
		{
			DUIWND hDUIHoverWnd = m_pOwner->HitTestPoint(PointL2FrameClient(pt));
			if (NULL == hDUIHoverWnd)
			{
				break;
			}
			*pdwEffect = DROPEFFECT_NONE;//表示此窗口不能接受拖放。
			if (hDUIHoverWnd != m_hDUIHoverWnd)
			{
				DTMAP::CPair* pPair = m_mapDropTarget.Lookup(m_hDUIHoverWnd);
				if (m_hDUIHoverWnd && pPair)
				{
					pPair->m_value->DragLeave();
				}
				m_hDUIHoverWnd = hDUIHoverWnd;
				pPair = m_mapDropTarget.Lookup(m_hDUIHoverWnd);
				if (m_hDUIHoverWnd && pPair)
				{
					pPair->m_value->DragEnter(m_pDataObj,grfKeyState,pt,pdwEffect);
				}
			}
			else
			{
				DTMAP::CPair *pPair = m_mapDropTarget.Lookup(m_hDUIHoverWnd);
				if (m_hDUIHoverWnd && pPair)
				{
					pPair->m_value->DragOver(grfKeyState,pt,pdwEffect);
				}
			}
		} while (false);
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DUIDropTargetDispatch::DragLeave(void)
	{
		if (m_pDataObj)
		{
			m_pDataObj->Release();
			m_pDataObj = NULL;
		}
		return S_OK;
	}

	HRESULT STDMETHODCALLTYPE DUIDropTargetDispatch::Drop(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState, POINTL pt,__RPC__inout DWORD *pdwEffect)
	{
		DTMAP::CPair* pPair = m_mapDropTarget.Lookup(m_hDUIHoverWnd);
		if (m_hDUIHoverWnd && pPair)
		{
			pPair->m_value->Drop(pDataObj,grfKeyState,pt,pdwEffect);
		}
		m_hDUIHoverWnd = NULL;
		m_pDataObj->Release();
		m_pDataObj = NULL;
		return S_OK;
	}

	// 辅助
	POINT DUIDropTargetDispatch::PointL2FrameClient(const POINTL& pt)
	{
		CPoint ptClient(pt.x,pt.y);
		::ScreenToClient(m_pOwner->GetContainer()->OnGetHWnd(),&ptClient);
		return ptClient;
	}

}//namespace DM