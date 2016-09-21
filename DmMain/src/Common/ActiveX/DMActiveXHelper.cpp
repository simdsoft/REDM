#include "DmMainAfx.h"
#include "DMActiveXHelper.h"

namespace DM
{
	//--------------------------------------------------------------------------
	DMBStr::DMBStr(const wchar_t* non_bstr)
		: bstr_(SysAllocString(non_bstr)) {}

	DMBStr::~DMBStr()
	{
		SysFreeString(bstr_);
	}

	void DMBStr::Reset(BSTR bstr)
	{
		if (bstr != bstr_)
		{
			SysFreeString(bstr_);
			bstr_ = bstr;
		}
	}

	BSTR DMBStr::Release()
	{
		BSTR bstr = bstr_;
		bstr_ = NULL;
		return bstr;
	}

	void DMBStr::Swap(DMBStr& bstr2)
	{
		BSTR tmp = bstr_;
		bstr_ = bstr2.bstr_;
		bstr2.bstr_ = tmp;
	}

	BSTR* DMBStr::Receive()
	{
		return &bstr_;
	}

	BSTR DMBStr::Allocate(const wchar_t* str)
	{
		Reset(SysAllocString(str));
		return bstr_;
	}

	BSTR DMBStr::AllocateBytes(size_t bytes)
	{
		Reset(SysAllocStringByteLen(NULL, static_cast<UINT>(bytes)));
		return bstr_;
	}

	void DMBStr::SetByteLen(size_t bytes)
	{
		unsigned int* data = reinterpret_cast<unsigned int*>(bstr_);
		data[-1] = static_cast<unsigned int>(bytes);
	}

	size_t DMBStr::Length() const
	{
		return SysStringLen(bstr_);
	}

	size_t DMBStr::ByteLength() const
	{
		return SysStringByteLen(bstr_);
	}

}//namespace DM
