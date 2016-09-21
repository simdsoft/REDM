#include "Plugin_ExpandAfx.h"
#include "DMSkiaHelper.h"


#define DT_ELLIPSIS (DT_PATH_ELLIPSIS|DT_END_ELLIPSIS|DT_WORD_ELLIPSIS)
#define CH_ELLIPSIS L"..."
#define MAX(a,b)    (((a) > (b)) ? (a) : (b))

static size_t breakTextEx(const SkPaint *pPaint, const wchar_t* textD, size_t length, SkScalar maxWidth,
						  SkScalar* measuredWidth) 
{
	// 返回的是字节数，breakText方法，该方法能够检测一行显示多少文字
	size_t nLineLen = pPaint->breakText(textD,length*sizeof(wchar_t),maxWidth,measuredWidth,SkPaint::kForward_TextBufferDirection);
	if (nLineLen==0)
	{
		return 0;
	}

	// 换算成字符数
	nLineLen/=sizeof(wchar_t);

	const wchar_t * p = textD;//  有\r\n或\n就断行
	for (size_t i=0;i<nLineLen;i++, p++)
	{
		if (*p == L'\r')
		{
			if (i<nLineLen-1 && p[1]==L'\n')
				return i+2;
			else 
				return i;
		}
		else if (*p == L'\n')
		{
			return i+1;
		}
	}
	return nLineLen;
}

SkRect DrawText_Skia(SkCanvas* canvas,const wchar_t *text,int len,SkRect box,const SkPaint& paint,UINT uFormat)
{
	if (len<0)	
	{
		len = (int)wcslen(text);
	}

	DMSkDrawText layout;
	layout.Init(text,len,box,paint,uFormat);

	return layout.Draw(canvas);
}

//////////////////////////////////////////////////////////////////////////
void DMSkDrawText::Init(LPCWSTR lpString,int nCount,SkRect skRc, const SkPaint &skPaint, UINT uFormat)
{
	if (uFormat & DT_NOPREFIX)//不考虑前缀
	{
		m_Text.setCount(nCount);
		memcpy(m_Text.begin(),lpString,nCount*sizeof(wchar_t));
	}
	else
	{
		m_Prefix.reset();
		SkTDArray<wchar_t> tmp;
		tmp.setCount(nCount);
		memcpy(tmp.begin(),lpString,nCount*sizeof(wchar_t));
		for (int i=0;i<tmp.count();i++)
		{
			if (L'&' == tmp[i])// 前缀
			{
				tmp.remove(i,1);
				if (i<tmp.count()-1)
				{
					m_Prefix.push(i);
				}
				if (i<tmp.count()-1 && tmp[i+1]==L'&') // 如果是&&，则后一个&跳过，因为要显示
				{
					i++;  
				}
			}
		}
		m_Text = tmp;
	}

	m_pSkPaint = &skPaint;
	m_rcBound = skRc;
	m_uFormat = uFormat;
	buildLines();
}

void DMSkDrawText::buildLines()
{
	m_Lines.reset();
	if (m_uFormat & DT_SINGLELINE)
	{
		m_Lines.push(0);
	}
	else
	{
		const wchar_t *text = m_Text.begin();
		const wchar_t* stop = m_Text.begin() + m_Text.count();
		SkScalar maxWid = m_rcBound.width();// 宽度
		if (m_uFormat & DT_CALCRECT && maxWid < 1.0f)
		{
			maxWid = 10000.0f;
		}
		int lineHead=0;
		while (lineHead<m_Text.count())
		{
			m_Lines.push(lineHead);
			size_t line_len = breakTextEx(m_pSkPaint,text,stop-text,maxWid,0);
			text += line_len;
			lineHead += (int)line_len;
		};
	}
}

SkScalar DMSkDrawText::drawLine( SkCanvas *canvas, SkScalar x, SkScalar y, int iBegin,int iEnd,SkScalar fontHei )
{
	const wchar_t *text=m_Text.begin()+iBegin;

	if (!(m_uFormat & DT_CALCRECT))
	{
		canvas->drawText(text,(iEnd-iBegin)*sizeof(wchar_t),x,y,*m_pSkPaint);
		int i=0;
		while (i<m_Prefix.count())
		{
			if(m_Prefix[i]>=iBegin)
				break;
			i++;
		}

		SkScalar xBase = x;
		if (m_pSkPaint->getTextAlign() != SkPaint::kLeft_Align)
		{
			SkScalar nTextWidth = m_pSkPaint->measureText(text,(iEnd-iBegin)*sizeof(wchar_t));
			switch (m_pSkPaint->getTextAlign())
			{
			case SkPaint::kCenter_Align:
				xBase = x - nTextWidth/2.0f;
				break;
			case SkPaint::kRight_Align:
				xBase = x- nTextWidth;
				break;
			}
		}

		while (i<m_Prefix.count() && m_Prefix[i]<iEnd)
		{
			SkScalar x1 = m_pSkPaint->measureText(text,(m_Prefix[i]-iBegin)*sizeof(wchar_t));
			SkScalar x2 = m_pSkPaint->measureText(text,(m_Prefix[i]-iBegin+1)*sizeof(wchar_t));
			canvas->drawLine(xBase+x1,y+1,xBase+x2,y+1,*m_pSkPaint); //绘制下划线
			i++;
		}
	}
	return m_pSkPaint->measureText(text,(iEnd-iBegin)*sizeof(wchar_t));
}

SkScalar DMSkDrawText::drawLineEndWithEllipsis( SkCanvas *canvas, SkScalar x, SkScalar y, int iBegin,int iEnd,SkScalar fontHei,SkScalar maxWidth )
{
	SkScalar widReq = m_pSkPaint->measureText(m_Text.begin()+iBegin,(iEnd-iBegin)*sizeof(wchar_t));
	if (widReq<=m_rcBound.width())
	{
		return drawLine(canvas,x,y,iBegin,iEnd,fontHei);
	}
	else
	{
		SkScalar fWidEllipsis = m_pSkPaint->measureText(CH_ELLIPSIS,sizeof(CH_ELLIPSIS)-sizeof(wchar_t));
		maxWidth-=fWidEllipsis;

		int i=0;
		const wchar_t *text=m_Text.begin()+iBegin;
		SkScalar fWid=0.0f;
		while (i<(iEnd-iBegin))
		{
			SkScalar fWord = m_pSkPaint->measureText(text+i,sizeof(wchar_t));
			if (fWid + fWord > maxWidth)
				break;
			fWid += fWord;
			i++;
		}
		if (!(m_uFormat & DT_CALCRECT))
		{
			wchar_t *pbuf = new wchar_t[i+3];
			memcpy(pbuf,text,i*sizeof(wchar_t));
			memcpy(pbuf+i,CH_ELLIPSIS,3*sizeof(wchar_t));
			canvas->drawText(pbuf,(i+3)*sizeof(wchar_t),x,y,*m_pSkPaint);
			DM_DELETE_ARRAY(pbuf);
		}

		return fWid + fWidEllipsis;
	}
}

SkRect DMSkDrawText::Draw(SkCanvas* canvas)
{
	// 可参考http://hgy413.com/1855.html
	SkPaint::FontMetrics metrics;
	m_pSkPaint->getFontMetrics(&metrics);
	float fontHeight = metrics.fDescent-metrics.fAscent;// 字符高
	float lineSpan = metrics.fBottom-metrics.fTop;// 行高
	SkRect rcDraw = m_rcBound;

	float  x = 0.0f;
	switch (m_pSkPaint->getTextAlign()) 
	{
	case SkPaint::kCenter_Align:
		x = SkScalarHalf(m_rcBound.width());
		break;
	case SkPaint::kRight_Align:
		x = m_rcBound.width();
		break;
	default://SkPaint::kLeft_Align:
		x = 0;
		break;
	}
	x += m_rcBound.fLeft;

	canvas->save();
	canvas->clipRect(m_rcBound);// 设置裁剪区
	float height = m_rcBound.height();
	float y=m_rcBound.fTop - metrics.fAscent;
	if (m_uFormat&DT_SINGLELINE||1==m_Lines.count())//单行显示
	{
		rcDraw.fBottom = rcDraw.fTop + lineSpan;
		if (m_uFormat&DT_VCENTER) 
		{
			y += (height - fontHeight)/2.0f;
		}
		if (m_uFormat & DT_ELLIPSIS)// 只支持在行尾增加省略号
		{
			rcDraw.fRight = rcDraw.fLeft + drawLineEndWithEllipsis(canvas,x,y,0,m_Text.count(),fontHeight,m_rcBound.width());
		}
		else
		{
			rcDraw.fRight = rcDraw.fLeft + drawLine(canvas,x,y,0,m_Text.count(),fontHeight);
		}
	}
	else// 多行显示
	{
		SkScalar maxLineWid = 0;
		int iLine = 0;
		while(iLine<m_Lines.count())
		{
			if (abs(y + lineSpan + metrics.fAscent) >= abs(m_rcBound.fBottom)) 
				break;  //the last visible line
			int iBegin = m_Lines[iLine];
			int iEnd = iLine<(m_Lines.count()-1)?m_Lines[iLine+1]:m_Text.count();
			SkScalar lineWid = drawLine(canvas,x,y,iBegin,iEnd,fontHeight);
			maxLineWid = MAX(maxLineWid,lineWid);
			y += lineSpan;
			iLine ++;
		}
		if (iLine<m_Lines.count())//Draw the last visible line
		{
			int iBegin=m_Lines[iLine];
			int iEnd = iLine<(m_Lines.count()-1)?m_Lines[iLine+1]:m_Text.count();
			SkScalar lineWid;
			if (m_uFormat & DT_ELLIPSIS)//只支持在行尾增加省略号
			{
				lineWid = drawLineEndWithEllipsis(canvas,x,y,iBegin,iEnd,fontHeight,m_rcBound.width());
			}
			else
			{
				lineWid = drawLine(canvas,x,y,iBegin,iEnd,fontHeight);
			}
			maxLineWid = MAX(maxLineWid,lineWid);
			y += lineSpan;
		}
		rcDraw.fRight = rcDraw.fLeft+maxLineWid-fontHeight;// 记得减fontHeight
		rcDraw.fBottom = y-abs(metrics.fAscent);// 记得减fAscent
	}
	canvas->restore();
	return rcDraw;
}

// 扩展Skia, 实现Rop2操作
static SkPMColor rdRop2Proc_Clear(SkPMColor src, SkPMColor dst)
{
	return 0;
}
static SkPMColor rdRop2Proc_Copy(SkPMColor src, SkPMColor dst)
{
	return src;
}
static SkPMColor rdRop2Proc_CopyInvert(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = ~src;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Invert(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = ~dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Xor(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = src ^ dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Or(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = src | dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_And(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = src & dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_AndReverse(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = src & (~dst);
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_AndInvert(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = (~src) & dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_NAnd(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = (~src) | (~dst);
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Equiv(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = (~src) ^ dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Noop(SkPMColor src, SkPMColor dst)
{
	return dst;
}
static SkPMColor rdRop2Proc_OrInvert(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = (~src) | dst;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_OrReverse(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = src | (~dst);
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}
static SkPMColor rdRop2Proc_Set(SkPMColor src, SkPMColor dst)
{
	SkPMColor res = 1;
	res |= (SK_A32_MASK << SK_A32_SHIFT); // force it to be opaque
	return res;
}

// rop2操作，结束
typedef struct _Rop2Proc
{
	SkXfermodeProc proc;
}Rop2Proc;

static Rop2Proc rop2_map[] =
{
	{rdRop2Proc_Clear}, 
	{rdRop2Proc_Copy}, 
	{rdRop2Proc_AndInvert}, 
	{rdRop2Proc_CopyInvert}, 
	{rdRop2Proc_AndReverse}, 
	{rdRop2Proc_Invert}, 
	{rdRop2Proc_Xor}, 
	{rdRop2Proc_NAnd}, 
	{rdRop2Proc_And}, 
	{rdRop2Proc_Equiv},
	{rdRop2Proc_Noop}, 
	{rdRop2Proc_OrInvert}, 
	{rdRop2Proc_OrReverse},
	{rdRop2Proc_Or},
	{rdRop2Proc_Set},
};

ProcXfermode::ProcXfermode( int rop ) : rop2(rop)
{
	if(rop < Rop2_End)
	{
		fProc = rop2_map[rop].proc;
	}
	else
	{
		fProc = NULL;
	}
}

void ProcXfermode::xfer32(SkPMColor dst[], const SkPMColor src[], int count,const SkAlpha aa[]) const 
{
	SkASSERT(dst && src && count >= 0);
	SkXfermodeProc proc = fProc;
	if (NULL != proc)
	{
		if (NULL == aa) 
		{
			for (int i = count - 1; i >= 0; --i) 
			{
				dst[i] = proc(src[i], dst[i]);
			}
		} 
		else 
		{
			for (int i = count - 1; i >= 0; --i) 
			{
				unsigned a = aa[i];
				if (0 != a)
				{
					SkPMColor dstC = dst[i];
					SkPMColor C = proc(src[i], dstC);
					if (a != 0xFF) 
					{
						C = SkFourByteInterp(C, dstC, a);
					}
					dst[i] = C;
				}
			}
		}
	}
}
void ProcXfermode::xfer16(uint16_t dst[],const  SkPMColor src[], int count, const  SkAlpha aa[]) const
{
	SkASSERT(dst && src && count >= 0);
	SkXfermodeProc proc = fProc;
	if (NULL != proc)
	{
		if (NULL == aa) 
		{
			for (int i = count - 1; i >= 0; --i) 
			{
				SkPMColor dstC = SkPixel16ToPixel32(dst[i]);
				dst[i] = SkPixel32ToPixel16_ToU16(proc(src[i], dstC));
			}
		} 
		else
		{
			for (int i = count - 1; i >= 0; --i)
			{
				unsigned a = aa[i];
				if (0 != a)
				{
					SkPMColor dstC = SkPixel16ToPixel32(dst[i]);
					SkPMColor C = proc(src[i], dstC);
					if (0xFF != a)
					{
						C = SkFourByteInterp(C, dstC, a);
					}
					dst[i] = SkPixel32ToPixel16_ToU16(C);
				}
			}
		}
	}
}

void ProcXfermode::xferA8(SkAlpha dst[],const  SkPMColor src[], int count, const  SkAlpha aa[]) const
{
	SkASSERT(dst && src && count >= 0);
	SkXfermodeProc proc = fProc;
	if (NULL != proc) 
	{
		if (NULL == aa)
		{
			for (int i = count - 1; i >= 0; --i) 
			{
				SkPMColor res = proc(src[i], dst[i] << SK_A32_SHIFT);
				dst[i] = SkToU8(SkGetPackedA32(res));
			}
		} 
		else 
		{
			for (int i = count - 1; i >= 0; --i)
			{
				unsigned a = aa[i];
				if (0 != a) 
				{
					SkAlpha dstA = dst[i];
					SkPMColor res = proc(src[i], dstA << SK_A32_SHIFT);
					unsigned A = SkGetPackedA32(res);
					if (0xFF != a) 
					{
						A = SkAlphaBlend(A, dstA, SkAlpha255To256(a));
					}
					dst[i] = SkToU8(A);
				}
			}
		}
	}
}

ProcXfermode::ProcXfermode(SkReadBuffer& buffer)
: SkXfermode(buffer)
{
	fProc = (SkXfermodeProc)buffer.readFunctionPtr();
	rop2 = buffer.readInt();
}

void ProcXfermode::flatten(SkWriteBuffer& buffer)
{
	buffer.writeFunctionPtr((void*)fProc);
	buffer.write32(rop2);
}
