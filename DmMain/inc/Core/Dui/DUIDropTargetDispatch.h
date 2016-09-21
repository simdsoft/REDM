//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DUIDropTargetDispatch.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-17	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	/// <summary>
	///		管理DropTarget在DUI窗口中的分发
	/// </summary>
	class DM_EXPORT DUIDropTargetDispatch : public IDropTarget
	{
	public:
		DUIDropTargetDispatch(DUIWindow* pOwner);
		virtual~DUIDropTargetDispatch();

		/// -------------------------------------------------
		/// @brief			 把DUIWND注册进去,在分发时，如果是当前DUIWND，则使用pDropTarget调用IDropTarget相关函数
		/// @param[in]		 hDUIWnd		DUI窗口句柄
		/// @param[in]		 pDropTarget	DropTarget对象
		/// @return  true成功,false失败
		bool RegisterDragDrop(DUIWND hDUIWnd, IDropTarget *pDropTarget);

		/// -------------------------------------------------
		/// @brief			 反注册
		/// @param[in]		 hDUIWnd		DUI窗口句柄
		/// @return			 true成功,false失败
		bool UnRegisterDragDrap(DUIWND hDUIWnd);

	public:
		//IUnkown
		virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, __RPC__deref_out void __RPC_FAR *__RPC_FAR *ppvObject);
		virtual ULONG STDMETHODCALLTYPE AddRef(void);
		virtual ULONG STDMETHODCALLTYPE Release(void);

		//IDropTarget
		virtual HRESULT STDMETHODCALLTYPE DragEnter(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE DragOver(DWORD grfKeyState,POINTL pt,__RPC__inout DWORD *pdwEffect);
		virtual HRESULT STDMETHODCALLTYPE DragLeave(void);
		virtual HRESULT STDMETHODCALLTYPE Drop(__RPC__in_opt IDataObject *pDataObj,DWORD grfKeyState, POINTL pt,__RPC__inout DWORD *pdwEffect);

	public:// 辅助
		POINT PointL2FrameClient(const POINTL& pt);

	public:

		typedef CMap<DUIWND,IDropTarget*> DTMAP;
		DTMAP							  m_mapDropTarget;
		IDataObject*					  m_pDataObj;
		DUIWindow*						  m_pOwner;
		DUIWND						      m_hDUIHoverWnd;
	};
}// namespace DM