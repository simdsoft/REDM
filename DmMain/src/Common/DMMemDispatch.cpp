#include "DmMainAfx.h"

namespace DM
{
	//
	int g_DM_rgInitData[] = { -1, 0, 0, 0, 0, 0, 0, 0 };
	void *DMMemDispatch::DM_malloc(size_t size)
	{
		return malloc(size);
	}

	void *DMMemDispatch::DM_realloc(void* memblock, size_t size)
	{
		return realloc(memblock,size);
	}

	void *DMMemDispatch::DM_calloc(size_t num,size_t size)
	{
		return calloc(num,size);
	}

	void DMMemDispatch::DM_free(void* memblock)
	{
		return free(memblock);
	}

	void* DMMemDispatch::Get_CString_InitData()
	{
		return &g_DM_rgInitData;
	}

}//namespace DM