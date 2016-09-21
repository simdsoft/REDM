#pragma once
#include "DMLua.h"
#ifdef _DEBUG
#pragma comment(lib, "lua_d.lib")
#else
#pragma comment(lib, "lua.lib")
#endif

/// <summary>
///		辅助转换工具,特别注意，Src至少要有个虚函数，不然无法转换
/// </summary>
template <class Src,class Dst> Dst DMDyncT(const void* ptr) 
{
	return  dynamic_cast<Dst>((Src)ptr);
}

template<class T,int RegType>T* DMDefRegT()
{
	T* pObj;
	pObj = NULL;
	if (g_pDMApp)
	{
		g_pDMApp->GetDefRegObj((void**)&pObj,RegType);
	}
	return pObj;
}

/// <summary>
///	  Script的实现实例
/// </summary>
class DMScriptImpl : public IDMScript
{
	DMDECLARE_CLASS_NAME(DMScriptImpl,L"DMScriptImpl",DMREG_Script);
public:
	DMScriptImpl();
	DMCode InitScript();
	DMCode RunString(LPCSTR lpszString);
	DMCode RunBuf(LPCSTR pBuf, size_t len);	
	DMCode RunFile(LPCSTR lpszFilePath);
	DMCode RunEvent(DMEventArgs* pEvt);

private:
	DMCode InitDMLua();

private:
	DMCode InitDMLua_Struct();
	DMCode InitDMLua_DUIWnd();
	DMCode InitDMLua_Interface();
	DMCode InitDMLua_DMHWnd();
	DMCode InitDMLua_DMApp();

private:
	DMCode InitDMLua_Global();

public:
	DMLUA::lua_context                m_L;
	bool                              m_bInit;

};