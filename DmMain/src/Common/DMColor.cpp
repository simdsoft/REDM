#include "DmMainAfx.h"
#include "DMColor.h"

namespace DM
{
	DMColor::DMColor()
	{
		b = g = r = 0;
		a = 0xFF;
	}

	DMColor::DMColor(BYTE r,BYTE g,BYTE b,BYTE a/*=0xFF*/)
		:a(a),b(b),g(g),r(r)
	{

	}

	DMColor::DMColor(DWORD clr)
	{
		memcpy(this,&clr,4);
	}

	const COLORREF DMColor::ToCOLORREF() const
	{
		return RGB(r,g,b)|(0<<24);
	}

	const DWORD DMColor::ToBGRA() const
	{
		DWORD crRet;
		memcpy(&crRet,this,4);
		return crRet;
	}

	void DMColor::SetRGBA(BYTE R,BYTE G,BYTE B,BYTE A)
	{
		r = R;
		g = G;
		b = B;
		a = A;
	}


	void DMColor::SetbyBGRA(DWORD Clr)
	{
		memcpy(this,&Clr,4);
	}

	DMColor DMColor::PreMultiPly(DMColor clr,BYTE alpha)
	{
		DMColor temp = clr;
		if (alpha!=0xff)
		{
			temp.r = clr.r*alpha/0xff;
			temp.g = clr.g*alpha/0xff;
			temp.b = clr.b*alpha/0xff;
			temp.a = clr.a*alpha/0xff;
		}
		return temp;
	}

	void DMColor::PreMultiply(BYTE alpha)
	{
		if (alpha!=0xff)
		{
			r = r*alpha/0xff;
			g = g*alpha/0xff;
			b = b*alpha/0xff;
			a = a*alpha/0xff;
		}
	}

}//namespace DM