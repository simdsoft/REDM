#include "Plugin_ExpandAfx.h"
#include "DMSkiaRegionImpl.h"

namespace DM
{
	DMSkiaRegionImpl::DMSkiaRegionImpl()
	{
		m_SkRegion.setEmpty();
	}

	DMSkiaRegionImpl::~DMSkiaRegionImpl()
	{
		m_SkRegion.setEmpty();
	}

	bool DMSkiaRegionImpl::CombineRect(LPCRECT lpRect,int fnCombineMode)
	{
		bool bRet = false;
		do 
		{
			SkIRect skiRc;
			if (false == Rect2SkIRect(lpRect, skiRc))
			{
				break;
			}

			bRet = m_SkRegion.op(skiRc,GdiRgnOp2SkRgnOp(fnCombineMode));
		} while (false);

		return bRet;
	}

	bool DMSkiaRegionImpl::PtInRegion(const POINT &pt)
	{
		return m_SkRegion.contains(pt.x,pt.y);
	}

	bool DMSkiaRegionImpl::RectInRegion(LPCRECT lpRect)
	{
		bool bRet = false;
		do 
		{
			SkIRect skiRc;
			if (false == Rect2SkIRect(lpRect,skiRc))
			{
			}

			bRet = m_SkRegion.intersects(skiRc);;
		} while (false);

		return bRet;
	}

	void DMSkiaRegionImpl::GetRgnBox(LPRECT lpRect)
	{
		SkIRect skiRc	= m_SkRegion.getBounds();
		lpRect->left	= skiRc.left();
		lpRect->top		= skiRc.top();
		lpRect->right	= skiRc.right();
		lpRect->bottom	= skiRc.bottom();
	}

	bool DMSkiaRegionImpl::IsEmpty()
	{
		 return m_SkRegion.isEmpty();
	}

	void DMSkiaRegionImpl::Offset(const POINT &pt)
	{
		m_SkRegion.translate(pt.x,pt.y);
	}

	void DMSkiaRegionImpl::Clear()
	{
		m_SkRegion.setEmpty();
	}

	SkRegion DMSkiaRegionImpl::GetRegion() const
	{
		return m_SkRegion;
	}

	void DMSkiaRegionImpl::SetRegion(const SkRegion& skRgn)
	{
		m_SkRegion = skRgn;
	}

	bool DMSkiaRegionImpl::Rect2SkIRect(LPCRECT lpRect,SkIRect &skiRc)
	{
		bool bRet = false;
		do 
		{
			if (NULL == lpRect)
			{
				break;
			}

			skiRc.setLTRB(lpRect->left,lpRect->top,lpRect->right,lpRect->bottom);
			bRet = true;
		} while (false);

		return bRet;
	}

	SkRegion::Op DMSkiaRegionImpl::GdiRgnOp2SkRgnOp(int fnCombineMode)
	{
		SkRegion::Op op = SkRegion::kReplace_Op;
		switch (fnCombineMode)
		{
		case RGN_COPY: 
			op = SkRegion::kReplace_Op;
			break;

		case RGN_AND:
			op = SkRegion::kIntersect_Op;
			break;

		case RGN_OR:
			op = SkRegion::kUnion_Op;
			break;

		case RGN_DIFF:
			op = SkRegion::kDifference_Op;
			break;

		case RGN_XOR: 
			op = SkRegion::kXOR_Op;
			break;
		default:
			DMASSERT_EXPR(0,L"无法识别的fnCombineMode");
			break;
		}
		return op;
	}




}//namespace DM