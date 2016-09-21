#pragma once


class DMSkDrawText 
{
public:
	//not support for DT_PREFIXONLY
	void Init(LPCWSTR lpString,int nCount,SkRect skRc, const SkPaint &skPaint, UINT uFormat);
	SkRect Draw(SkCanvas* canvas);

private:
	SkScalar drawLineEndWithEllipsis(SkCanvas *canvas, SkScalar x, SkScalar y, int iBegin,int iEnd,SkScalar fontHei,SkScalar maxWidth);
	SkScalar drawLine(SkCanvas *canvas, SkScalar x, SkScalar y, int iBegin,int iEnd,SkScalar fontHei);
	void buildLines();

private:
	SkTDArray<wchar_t>		m_Text;		 // 文本内容
	SkTDArray<int>			m_Prefix;    // 前缀符索引
	SkTDArray<int>			m_Lines;     // 分行索引，记录了每一行的起始字符索引
	UINT					m_uFormat;   // 显示标志
	SkRect					m_rcBound;   // 限制矩形
	const SkPaint			*m_pSkPaint;
};

SkRect DrawText_Skia(SkCanvas* canvas,const wchar_t *text,int len,SkRect box,const SkPaint& paint,UINT uFormat);

//PS_SOLID
const float  ps_solid[]		 = {1.0f,0.0f};
const float  ps_dash[]		 = {5.0f,5.0f};
const float  ps_dot[]		 = {1.0f,4.0f};
const float  ps_dashdot[]    = {4.0f,1.0f,1.0f,1.0f};
const float  ps_dashdotdot[] = {4.0f,1.0f,1.0f,1.0f,1.0f,1.0f};
const struct LineDashEffect
{
	const float  *fDash;
	int			  nCount;
} LINEDASHEFFECT[] =
{
	{ps_solid,ARRAYSIZE(ps_solid)},
	{ps_dash,ARRAYSIZE(ps_dash)},
	{ps_dot,ARRAYSIZE(ps_dot)},
	{ps_dashdot,ARRAYSIZE(ps_dashdot)},
	{ps_dashdotdot,ARRAYSIZE(ps_dashdotdot)},
};

class DMGetLineDashEffect
{
public:
	DMGetLineDashEffect(int iStyle):pDashPathEffect(NULL)
	{
		if (iStyle>PS_SOLID && iStyle<=PS_DASHDOTDOT)
		{
			const LineDashEffect *pEff=&LINEDASHEFFECT[iStyle-1];
			pDashPathEffect = SkDashPathEffect::Create(pEff->fDash,pEff->nCount,0.0f);
		}
	}

	~DMGetLineDashEffect()
	{
		if(pDashPathEffect) pDashPathEffect->unref();
	}

	SkDashPathEffect *Get() const{return pDashPathEffect;}
private:
	SkDashPathEffect * pDashPathEffect;
};

// 辅助----------------------------------
class ProcXfermode : public SkXfermode 
{
public:
	enum Xfer_Rop2
	{
		Rop2_Clear=0, 
		Rop2_Copy, 
		Rop2_AndInvert, 
		Rop2_CopyInvert, 
		Rop2_AndReverse, 
		Rop2_Invert, 
		Rop2_Xor, 
		Rop2_NAnd, 
		Rop2_And, 
		Rop2_Equiv,
		Rop2_Noop, 
		Rop2_OrInvert, 
		Rop2_OrReverse,
		Rop2_Or,
		Rop2_Set,
		Rop2_End,
	};

	ProcXfermode(int rop);
	// overrides from SkXfermode
	virtual void xfer32(SkPMColor dst[], const SkPMColor src[], int count,const SkAlpha aa[]) const;
	virtual void xfer16(uint16_t dst[], const SkPMColor src[], int count,const SkAlpha aa[]) const;
	virtual void xferA8(SkAlpha dst[], const SkPMColor src[], int count,const SkAlpha aa[]) const;

	// overrides from SkFlattenable
	virtual Factory getFactory() const {return CreateProc;}
	virtual void flatten(SkWriteBuffer&);

#ifndef SK_IGNORE_TO_STRING
	void ProcXfermode::toString(SkString* str) const {
		//         this->INHERITED::toString(str);
	}
#endif
protected:
	ProcXfermode(SkReadBuffer&);
private:
	SkXfermodeProc			fProc;
	int						rop2;
	static SkFlattenable* CreateProc(SkReadBuffer& buffer) {return SkNEW_ARGS(ProcXfermode, (buffer));}
	typedef SkXfermode INHERITED;
};