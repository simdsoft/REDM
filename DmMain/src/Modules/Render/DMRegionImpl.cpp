#include "DmMainAfx.h"
#include "DMRegionImpl.h"

namespace DM
{
	DMRegionImpl::DMRegionImpl()
		:m_hRgn(NULL)
	{
		m_hRgn = ::CreateRectRgn(0,0,0,0);
	}

	DMRegionImpl::~DMRegionImpl()
	{
		DM_DELETE_OBJECT(m_hRgn);
	}

	bool DMRegionImpl::CombineRect(LPCRECT lpRect,int fnCombineMode)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			HRGN hRgn = ::CreateRectRgnIndirect(lpRect);
			if (fnCombineMode == RGN_DIFF)
			{
				::CombineRgn(m_hRgn,m_hRgn,hRgn,fnCombineMode);
			}
			else
			{
				::CombineRgn(m_hRgn,hRgn,m_hRgn,fnCombineMode);//注意，hRgn放第一位，因为RGN_COPY需要
			}
			DM_DELETE_OBJECT(hRgn);

			bRet = true;
		} while (false);
		return bRet;
	}

	bool DMRegionImpl::PtInRegion(const POINT &pt)
	{
		return TRUE == ::PtInRegion(m_hRgn,pt.x,pt.y);
	}

	bool DMRegionImpl::RectInRegion(LPCRECT lpRect)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			bRet = (TRUE == ::RectInRegion(m_hRgn,lpRect));
		} while (false);
		return bRet;
	}

	void DMRegionImpl::GetRgnBox(LPRECT lpRect)
	{
		::GetRgnBox(m_hRgn,lpRect);
	}

	bool DMRegionImpl::IsEmpty()
	{
		RECT rcBox;
		GetRgnBox(&rcBox);
		return (rcBox.left == rcBox.right) || (rcBox.top== rcBox.bottom);
	}

	void DMRegionImpl::Offset(const POINT &pt)
	{
		::OffsetRgn(m_hRgn,pt.x,pt.y);
	}

	void DMRegionImpl::Clear()
	{
		::SetRectRgn(m_hRgn,0,0,0,0);
	}

	HRGN DMRegionImpl::GetRegion() const
	{
		return m_hRgn;
	}

	void DMRegionImpl::SetRegion(const HRGN  rgn)
	{
		::CombineRgn(m_hRgn,rgn,NULL,RGN_COPY);
	}

	DMCode DMRegionImpl::GetRectNum(int &iNum)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DWORD dwSize = GetRegionData(m_hRgn,0,NULL);
			if (0 == dwSize)
			{
				break;
			}
			iNum = (dwSize - sizeof(RGNDATAHEADER))/sizeof(RECT);
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMRegionImpl::GetRectArray(RECT* pRect,int iNum)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMBufT<RGNDATA>pData;
			DWORD dwSize = sizeof(RGNDATAHEADER)+iNum*sizeof(RECT);
			pData.AllocateBytes(dwSize);
			pData->rdh.dwSize = sizeof(RGNDATAHEADER);
			pData->rdh.iType  = RDH_RECTANGLES;
			pData->rdh.nCount = iNum;
			if (0 == GetRegionData(m_hRgn,dwSize,pData))
			{
				break;
			}
			memcpy(pRect,pData->Buffer,iNum*sizeof(RECT));
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

}//namespace DM