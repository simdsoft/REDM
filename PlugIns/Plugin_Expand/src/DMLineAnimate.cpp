#include "Plugin_ExpandAfx.h"
#include "DMLineAnimate.h"
#include <math.h>

namespace DM
{
	DMLineAnimate::DMLineAnimate()
	{
		m_pOwner = NULL;
	}

	DMCode DMLineAnimate::AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp, LPARAM lp)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pOwner)
			{
				break;
			}
			m_pOwner = pOwner;
			
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMLineAnimate::AnimateBegin(WPARAM wp, LPARAM lp)
	{	
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == m_pOwner)
			{
				break;
			}
			CRect rcWnd = m_pOwner->GetAnimateOwnerWnd()->m_rcWindow;
			HWND hWnd = m_pOwner->GetAnimateOwnerWnd()->GetContainer()->OnGetHWnd();
			::ClientToScreen(hWnd, (LPPOINT)&rcWnd);
			::ClientToScreen(hWnd, ((LPPOINT)&rcWnd)+1);
			DMLineAniEffect anim;
			if (wp)
			{
				anim.Open(rcWnd);
			}
			else
			{
				anim.Close(rcWnd);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	DMCode DMLineAnimate::AnitmateGet(WPARAM wp, LPARAM lp)
	{
		return DM_ECODE_OK;
	}

	DMCode DMLineAnimate::AnimateEnd(WPARAM wp, LPARAM lp)
	{
		return DM_ECODE_OK;
	}

	//----------------------------------------------
#define FIXED_SHIFT 16
#define FIXED int

	typedef struct tag_MATRIXF {
		FIXED fM11;
		FIXED fM12;
		int	  iM13;
		FIXED fM21;
		FIXED fM22;
		int   iM23;
		int   iM31;
		int   iM32;
		int   iM33;
	} MATRIXF;

	const FIXED fixed1= 1 << FIXED_SHIFT;
	const MATRIXF matrix1 = { fixed1, 0, 0, 0, fixed1, 0, 0, 0, 1 };
	const POINT point0 = { 0, 0 };

	int		fixedMul(int iMultiplicand, FIXED fMultiplier);
	FIXED	fixedDiv(int iNumerator, int iDenominator);
	POINT	operator*( const POINT &point, const MATRIXF &matrix);
	MATRIXF	operator*( const MATRIXF &matrix1, const MATRIXF &matrix2);
	MATRIXF	mix( const MATRIXF &matrix1, const MATRIXF &matrix2, FIXED fMix);
	POINT	mix( const POINT &point1, const POINT &point2, FIXED fMix);

	MATRIXF scaleMatrix( FIXED scaleX, FIXED scaleY, POINT ptOrg = point0 );
	MATRIXF rotateMatrix( FIXED angle, POINT ptOrg = point0 );
	MATRIXF offsetMatrix( int offsetX, int offsetY );

	BOOL	efSpinFrame( DMLineAniData *pDMLineAniData );
	BOOL	efVortexFrames( DMLineAniData *pDMLineAniData );
	BOOL	efScatterGatherFrames( DMLineAniData *pDMLineAniData );
	BOOL	efSpikeFrames( DMLineAniData *pDMLineAniData );
	BOOL	efFireworxFrames( DMLineAniData *pDMLineAniData );



	inline POINT operator+(POINT &point1, POINT &point2)
	{
		POINT ptResult;
		ptResult.x = point1.x + point2.x;
		ptResult.y = point1.y + point2.y;
		return ptResult;
	}

	inline POINT operator-(POINT &point1, POINT &point2)
	{
		POINT ptResult;
		ptResult.x = point1.x - point2.x;
		ptResult.y = point1.y - point2.y;
		return ptResult;
	}

	inline MATRIXF& operator *=(MATRIXF &matrix1, const MATRIXF &matrix2)
	{
		return matrix1 = matrix1*matrix2;
	}


	void drawBox( HDC hDC, POINT &ptCenter, POINT &ptRelRightTop );
	void drawPoly( HDC hDC, POINT *pPts, DWORD dwPoints );

	//////////////////////////////////////////////////////////////////////
	// Construction/Destruction
	//////////////////////////////////////////////////////////////////////

	DMLineAniEffect::DMLineAniEffect()
	{
		m_hPen = ::CreatePen( PS_SOLID, 1, RGB(0,0,0) );

		Defaults();
	}

	DMLineAniEffect::~DMLineAniEffect()
	{
		::DeleteObject( m_hPen );
	}

	void DMLineAniEffect::Defaults()
	{
		m_rcScreen = CRect(0,0,GetSystemMetrics(SM_CXFULLSCREEN),GetSystemMetrics(SM_CYFULLSCREEN));
		m_iAfterimages = 6;
		m_iTotalSteps = 50;
		m_iDelay = 5;
		Effect( Random );
	}

	void DMLineAniEffect::Setup(int iSteps, int iAfterimages, int iDelay)
	{
		if (iSteps > 255) iSteps = 255;
		else if (iSteps < 1) iSteps = 1;
		m_iTotalSteps = iSteps;

		if (iAfterimages > 32) iAfterimages = 32;
		else if (iAfterimages < 0) iAfterimages = 0;
		m_iAfterimages = iAfterimages;

		if (iDelay > 100) iDelay = 100;
		else if (iDelay < 0) iDelay = 0;
	}

	void DMLineAniEffect::Effect( EffectType effect )
	{
		switch ( effect ) {
	case Random:
		break;
	case Spin:
		m_iParameter = 360;
		break;
	case Vortex:
		m_iParameter = 180;
		break;
	case ScatterGather:
		m_iParameter = 4;
		break;
	case Spike:
		m_iParameter = 180;
		break;
	case Fireworks:
		m_iParameter = 360;
		break;
		}
		m_Effect = effect;
	}

	void DMLineAniEffect::ChooseFunc()
	{
		bool bRandom = false;

		if (m_Effect == Random) {
			bRandom = true;
			Effect((EffectType)(rand() % 5));
		}

		switch (m_Effect) {
	case Spin:
		m_Func = efSpinFrame;
		break;
	case Vortex:
		m_Func = efVortexFrames;
		break;
	case ScatterGather:
		m_Func = efScatterGatherFrames;
		break;
	case Spike:
		m_Func = efSpikeFrames;
		break;
	case Fireworks:
		m_Func = efFireworxFrames;
		break;
	default:
		m_Func = 0;
		}

		if (bRandom) {
			m_Effect = Random;
		}
	}


	void DMLineAniEffect::Open( const CRect &rcWnd )
	{
		HPEN oldpen;
		int	 oldrop2;
		DMLineAniData ad;

		oldpen = (HPEN)::SelectObject(m_hDC, m_hPen);
		oldrop2= ::SetROP2(m_hDC, R2_NOT);

		ChooseFunc();
		ad.bOpen = TRUE;
		ad.hDC = m_hDC;
		ad.iAfterimages = m_iAfterimages;
		ad.iTotalSteps = m_iTotalSteps;
		ad.rcWnd = rcWnd;
		ad.ptCenter = rcWnd.CenterPoint();
		ad.ptRelRightTop = CPoint( rcWnd.right - ad.ptCenter.x,rcWnd.top - ad.ptCenter.y );
		ad.iParameter = m_iParameter;

		DMLineAniate(ad);

		::SetROP2(m_hDC, oldrop2);
		::SelectObject(m_hDC, oldpen);
	}

	void DMLineAniEffect::Close( const CRect &rcWnd )
	{
		HPEN oldpen;
		int	 oldrop2;
		DMLineAniData ad;

		oldpen = (HPEN)::SelectObject(m_hDC, m_hPen);
		oldrop2= ::SetROP2(m_hDC, R2_NOT);

		ChooseFunc();
		ad.bOpen = FALSE;
		ad.hDC = m_hDC;
		ad.iAfterimages = m_iAfterimages;
		ad.iTotalSteps = m_iTotalSteps;
		ad.rcWnd = rcWnd;
		ad.ptCenter = rcWnd.CenterPoint();
		ad.ptRelRightTop = CPoint( rcWnd.right - ad.ptCenter.x,rcWnd.top - ad.ptCenter.y );
		ad.iParameter = m_iParameter;

		DMLineAniate(ad);

		::SetROP2(m_hDC, oldrop2);
		::SelectObject(m_hDC, oldpen);
	}

	void DMLineAniEffect::DMLineAniate(DMLineAniData &animData)
	{
		animData.animType = DMLineAniInit;
		m_Func(&animData);

		if (animData.bOpen) {
			for(int frame=0;frame < animData.iTotalSteps+animData.iAfterimages;frame++) {
				// draw 
				if (frame < animData.iTotalSteps) {
					animData.animType = DMLineAniDraw;
					animData.iStep = frame;
					m_Func( &animData );
					::GdiFlush();
				}
				Sleep(m_iDelay);
				// erase
				if (frame >= animData.iAfterimages) {
					animData.animType = DMLineAniErase;
					animData.iStep = frame - animData.iAfterimages;
					m_Func( &animData );
					::GdiFlush();
				}
			}
		} else {
			for(int frame=animData.iTotalSteps+animData.iAfterimages-1;frame >= 0 ;frame--) {
				// draw 
				if (frame >= animData.iAfterimages) {
					animData.animType = DMLineAniDraw;
					animData.iStep = frame - animData.iAfterimages;
					m_Func( &animData );
					::GdiFlush();
				}
				Sleep(m_iDelay);
				// erase
				if (frame < animData.iTotalSteps) {
					animData.animType = DMLineAniErase;
					animData.iStep = frame;
					m_Func( &animData );
					::GdiFlush();
				}
			}
		}

		animData.animType = DMLineAniTerm;
		m_Func(&animData);
	}
	//////////////////////////////////////////////////////////////////////
	// Supp. functions
	//////////////////////////////////////////////////////////////////////

	int		fixedMul(int iMultiplicand, FIXED fMultiplier)
	{
		return MulDiv(iMultiplicand, fMultiplier, 65536);
	}

	FIXED	fixedDiv(int iNumerator, int iDenominator)
	{
		if ( iNumerator == 0 || iDenominator == 0) return 0;

		return MulDiv(65536,iNumerator,iDenominator);
	}

	POINT	operator*( const POINT &point, const MATRIXF &matrix)
	{
		POINT ptResult;
		ptResult.x = fixedMul(point.x, matrix.fM11) + fixedMul(point.y,matrix.fM21) + matrix.iM31;
		ptResult.y = fixedMul(point.x, matrix.fM12) + fixedMul(point.y,matrix.fM22) + matrix.iM32;
		return ptResult;
	}

	MATRIXF	operator*( const MATRIXF &m1, const MATRIXF &m2)
	{
		MATRIXF mtRes;

		mtRes.fM11 = fixedMul(m1.fM11,m2.fM11) + fixedMul(m1.fM12,m2.fM21);
		mtRes.fM12 = fixedMul(m1.fM11,m2.fM12) + fixedMul(m1.fM12,m2.fM22);
		mtRes.iM13 = 0;
		mtRes.fM21 = fixedMul(m1.fM21,m2.fM11) + fixedMul(m1.fM22,m2.fM21);
		mtRes.fM22 = fixedMul(m1.fM21,m2.fM12) + fixedMul(m1.fM22,m2.fM22);
		mtRes.iM23 = 0;
		mtRes.iM31 = fixedMul(m1.iM31,m2.fM11) + fixedMul(m1.iM32,m2.fM21) + m2.iM31;
		mtRes.iM32 = fixedMul(m1.iM31,m2.fM12) + fixedMul(m1.iM32,m2.fM22) + m2.iM32;
		mtRes.iM33 = 1;

		return mtRes;
	}

	MATRIXF	mix( const MATRIXF &matrix1, const MATRIXF &matrix2, FIXED fMix)
	{
		MATRIXF mtRes;

		mtRes.fM11 = fixedMul(matrix1.fM11, fMix) + fixedMul(matrix2.fM11,fixed1-fMix);
		mtRes.fM12 = fixedMul(matrix1.fM12, fMix) + fixedMul(matrix2.fM12,fixed1-fMix);
		mtRes.iM13 = fixedMul(matrix1.iM13, fMix) + fixedMul(matrix2.iM13,fixed1-fMix);
		mtRes.fM21 = fixedMul(matrix1.fM21, fMix) + fixedMul(matrix2.fM21,fixed1-fMix);
		mtRes.fM22 = fixedMul(matrix1.fM22, fMix) + fixedMul(matrix2.fM22,fixed1-fMix);
		mtRes.iM23 = fixedMul(matrix1.iM23, fMix) + fixedMul(matrix2.iM23,fixed1-fMix);
		mtRes.iM31 = fixedMul(matrix1.iM31, fMix) + fixedMul(matrix2.iM31,fixed1-fMix);
		mtRes.iM32 = fixedMul(matrix1.iM32, fMix) + fixedMul(matrix2.iM32,fixed1-fMix);
		mtRes.iM33 = fixedMul(matrix1.iM33, fMix) + fixedMul(matrix2.iM33,fixed1-fMix);

		return mtRes;
	}

	POINT	mix( const POINT &point1, const POINT &point2, FIXED fMix)
	{
		POINT ptRes;

		ptRes.x = fixedMul(point1.x,fMix) + fixedMul(point2.x,fixed1-fMix);
		ptRes.y = fixedMul(point1.y,fMix) + fixedMul(point2.y,fixed1-fMix);

		return ptRes;
	}

	MATRIXF offsetMatrix( int offsetX, int offsetY )
	{
		MATRIXF mRes = matrix1;

		mRes.iM31 = offsetX;
		mRes.iM32 = offsetY;

		return mRes;
	}

	MATRIXF scaleMatrix( FIXED scaleX, FIXED scaleY, POINT ptOrg)
	{
		MATRIXF mRes = matrix1;

		mRes.fM11 = scaleX;
		mRes.fM22 = scaleY;

		return offsetMatrix(-ptOrg.x,-ptOrg.y) * mRes * offsetMatrix(ptOrg.x,ptOrg.y);
	}

	MATRIXF rotateMatrix( FIXED angle, POINT ptOrg)
	{
		MATRIXF mRes = matrix1;

		double dAngle = (angle / 65536.0) * 3.141592654 / 180.0;
		FIXED fCos = (FIXED)(65536.0 * cos( dAngle ));
		FIXED fSin = (FIXED)(65536.0 * sin( dAngle ));

		mRes.fM11 =  fCos;
		mRes.fM21 =  -fSin;
		mRes.fM12 =  fSin;
		mRes.fM22 =  fCos;

		return offsetMatrix(-ptOrg.x,-ptOrg.y) * mRes * offsetMatrix(ptOrg.x,ptOrg.y);
	}

	/////////////////////////////////////////////////////////
	// Effects
	/////////////////////////////////////////////////////////
	BOOL	efSpinFrame( DMLineAniData *pDMLineAniData )
	{
		if (pDMLineAniData->animType == DMLineAniDraw ||
			pDMLineAniData->animType == DMLineAniErase ) {
				POINT ptRect[4];

				ptRect[1].x = ptRect[0].x = pDMLineAniData->ptRelRightTop.x;
				ptRect[1].y = -(ptRect[0].y = pDMLineAniData->ptRelRightTop.y);

				MATRIXF matrix;
				FIXED	fxScale;
				fxScale = fixedDiv(pDMLineAniData->iStep,pDMLineAniData->iTotalSteps);

				matrix = scaleMatrix( fxScale, fxScale ) * 	rotateMatrix( 
					fixedDiv( pDMLineAniData->iParameter * ( pDMLineAniData->iStep - pDMLineAniData->iTotalSteps ), pDMLineAniData->iTotalSteps ) );

				ptRect[0] = ptRect[0] * matrix;
				ptRect[1] = ptRect[1] * matrix;
				ptRect[2] = pDMLineAniData->ptCenter + ptRect[0];
				ptRect[3] = pDMLineAniData->ptCenter + ptRect[1];
				ptRect[0] = pDMLineAniData->ptCenter - ptRect[0];
				ptRect[1] = pDMLineAniData->ptCenter - ptRect[1];

				drawPoly(pDMLineAniData->hDC,ptRect,4);
		}

		return TRUE;
	}

	BOOL efVortexFrames( DMLineAniData *pDMLineAniData )
	{
		switch ( pDMLineAniData->animType ) {
		case DMLineAniInit :
			*(MATRIXF*)pDMLineAniData->bBuffer = rotateMatrix( fixed1 * 72 , pDMLineAniData->ptCenter);
			break;
		case DMLineAniDraw:
		case DMLineAniErase: 
			{
				POINT ptBoxRel;

				ptBoxRel.x = pDMLineAniData->ptRelRightTop.x * pDMLineAniData->iStep / pDMLineAniData->iTotalSteps;
				ptBoxRel.y = pDMLineAniData->ptRelRightTop.y * pDMLineAniData->iStep / pDMLineAniData->iTotalSteps;

				MATRIXF matrix;
				FIXED	fxScale;

				fxScale = fixedDiv( (pDMLineAniData->iTotalSteps - pDMLineAniData->iStep) * 4,
					pDMLineAniData->iTotalSteps * 3 );

				matrix = offsetMatrix( pDMLineAniData->ptRelRightTop.x, pDMLineAniData->ptRelRightTop.y ) *
					scaleMatrix( fxScale, fxScale, pDMLineAniData->ptCenter ) *
					rotateMatrix( fixedDiv(pDMLineAniData->iParameter * pDMLineAniData->iStep, pDMLineAniData->iTotalSteps) , pDMLineAniData->ptCenter );

				POINT ptBoxCenter;
				ptBoxCenter = pDMLineAniData->ptCenter * matrix;

				for( int iLoop = 0; iLoop < 5; iLoop++) {
					drawBox( pDMLineAniData->hDC, ptBoxCenter, ptBoxRel );
					ptBoxCenter = ptBoxCenter * *(MATRIXF *)pDMLineAniData->bBuffer;
				}

				break;
			}
		}

		return TRUE;
	}

	BOOL efScatterGatherFrames( DMLineAniData *pDMLineAniData )
	{
		if ( pDMLineAniData->animType == DMLineAniDraw ||
			pDMLineAniData->animType == DMLineAniErase ) {
				int iDivisor = pDMLineAniData->iParameter;
				POINT ptBoxRel;

				ptBoxRel.x = pDMLineAniData->ptRelRightTop.x *  pDMLineAniData->iStep / (pDMLineAniData->iTotalSteps * iDivisor);
				ptBoxRel.y = pDMLineAniData->ptRelRightTop.y *  pDMLineAniData->iStep / (pDMLineAniData->iTotalSteps * iDivisor);

				MATRIXF matrix;
				FIXED fxScale;

				fxScale = fixedDiv( pDMLineAniData->iTotalSteps * 3 - pDMLineAniData->iStep * 2, pDMLineAniData->iTotalSteps );
				matrix = scaleMatrix( fxScale, fxScale ) * offsetMatrix( pDMLineAniData->ptCenter.x, pDMLineAniData->ptCenter.y );

				for(int iRow = 0; iRow < iDivisor; iRow++) {
					for( int iCol=0; iCol < iDivisor; iCol++) {
						POINT ptTileCenter;

						ptTileCenter.x = (iRow * 2 - iDivisor + 1) * pDMLineAniData->ptRelRightTop.x / iDivisor;
						ptTileCenter.y = (iCol * 2 - iDivisor + 1) * pDMLineAniData->ptRelRightTop.y / iDivisor;
						ptTileCenter = ptTileCenter * matrix;

						drawBox( pDMLineAniData->hDC, ptTileCenter, ptBoxRel );
					}
				}
		}

		return TRUE;
	}

	BOOL efSpikeFrames( DMLineAniData *pDMLineAniData )
	{
		struct SpikeData {
			POINT ptTriangleEnd[16][3], ptEndCenter[16], ptTriangleCenter[16];
			MATRIXF matrixCircle[16];
		};

		switch (pDMLineAniData->animType) {
	case DMLineAniInit: {
		int xLeft	= pDMLineAniData->rcWnd.left;
		int xRight	= pDMLineAniData->rcWnd.right;
		int yTop	= pDMLineAniData->rcWnd.bottom;
		int yBottom = pDMLineAniData->rcWnd.top;

		POINT *pTriangle = ((SpikeData*)pDMLineAniData->bBuffer)->ptTriangleEnd[0],
			*pCenter	 = ((SpikeData*)pDMLineAniData->bBuffer)->ptEndCenter;

		for( int iNdx = 0; iNdx < 16; iNdx++, pTriangle +=3, pCenter++) {
			pTriangle[0] = pDMLineAniData->ptCenter;

			if (iNdx < 4) {
				pTriangle[1].y = pTriangle[2].y = yTop;
				pTriangle[1].x = (xLeft * (4 - iNdx) + xRight * iNdx) / 4;
				pTriangle[2].x = (xLeft * (3 - iNdx) + xRight * (iNdx+1)) / 4;
			} else if (iNdx < 8) {
				pTriangle[1].x = pTriangle[2].x = xRight;
				pTriangle[1].y = (yTop * (8 - iNdx) + yBottom * (iNdx-4)) / 4;
				pTriangle[2].y = (yTop * (7 - iNdx) + yBottom * (iNdx-3)) / 4;
			} else if (iNdx < 12) {
				pTriangle[1].y = pTriangle[2].y = yBottom;
				pTriangle[1].x = (xRight * (12 - iNdx) + xLeft * (iNdx-8)) / 4;
				pTriangle[2].x = (xRight * (11 - iNdx) + xLeft * (iNdx-7)) / 4;
			} else {
				pTriangle[1].x = pTriangle[2].x = xLeft;
				pTriangle[1].y = (yBottom * (16 - iNdx) + yTop * (iNdx-12)) / 4;
				pTriangle[2].y = (yBottom * (15 - iNdx) + yTop * (iNdx-11)) / 4;
			}

			pCenter->x = (pTriangle[0].x + pTriangle[1].x + pTriangle[2].x) / 3;
			pCenter->y = (pTriangle[0].y + pTriangle[1].y + pTriangle[2].y) / 3;
		}

		pCenter = ((SpikeData*)pDMLineAniData->bBuffer)->ptTriangleCenter;
		POINT ptTrgCenter;

		ptTrgCenter.x = pDMLineAniData->ptCenter.x;
		ptTrgCenter.y = pDMLineAniData->ptCenter.y + (pDMLineAniData->ptRelRightTop.x + pDMLineAniData->ptRelRightTop.y) * 4/5;

		for(int iNdx =0;iNdx < 16; iNdx++ ) {
			MATRIXF matrix;

			matrix = rotateMatrix( (33 * fixed1) + (-22 * fixed1) * iNdx, pDMLineAniData->ptCenter );
			pCenter[iNdx] = ptTrgCenter * matrix;

			POINT ptTemp = pCenter[iNdx] - ((SpikeData*)pDMLineAniData->bBuffer)->ptEndCenter[iNdx];
			((SpikeData*)pDMLineAniData->bBuffer)->matrixCircle[iNdx] = offsetMatrix( ptTemp.x, ptTemp.y );
		}

		break;
				   }
	case DMLineAniDraw:
	case DMLineAniErase: {
		POINT ptTriangle[3];
		FIXED fixedFactor;

		MATRIXF matrix;
		FIXED fxScale;
		fxScale = fixedDiv(pDMLineAniData->iStep, pDMLineAniData->iTotalSteps);

		if (pDMLineAniData->iStep < pDMLineAniData->iTotalSteps / 2) {
			fixedFactor = (fixed1 - fixedDiv(pDMLineAniData->iStep*2, pDMLineAniData->iTotalSteps)) *
				pDMLineAniData->iParameter;

			for( int iNdx=0;iNdx < 16; iNdx++ ) {
				matrix = scaleMatrix( fxScale, fxScale, ((SpikeData*)pDMLineAniData->bBuffer)->ptEndCenter[iNdx] ) *
					rotateMatrix( fixedFactor, ((SpikeData*)pDMLineAniData->bBuffer)->ptEndCenter[iNdx] );
				matrix = matrix * ((SpikeData*)pDMLineAniData->bBuffer)->matrixCircle[iNdx];

				for(int iAngle=0;iAngle < 3;iAngle++) {
					ptTriangle[iAngle] = 
						((SpikeData*)pDMLineAniData->bBuffer)->ptTriangleEnd[iNdx][iAngle] * matrix;
				}

				drawPoly( pDMLineAniData->hDC, ptTriangle, 3 );
			}
		} else {
			fixedFactor = fixedDiv(pDMLineAniData->iStep*2 - pDMLineAniData->iTotalSteps, pDMLineAniData->iTotalSteps);

			for( int iNdx=0; iNdx < 16; iNdx++) {
				matrix = scaleMatrix( fxScale, fxScale, ((SpikeData*)pDMLineAniData->bBuffer)->ptEndCenter[iNdx]);
				matrix *= mix( matrix1, ((SpikeData*)pDMLineAniData->bBuffer)->matrixCircle[iNdx], fixedFactor );

				for( int iAngle=0; iAngle < 3; iAngle++ ) {
					ptTriangle[iAngle] = 
						((SpikeData*)pDMLineAniData->bBuffer)->ptTriangleEnd[iNdx][iAngle] * matrix;
				}

				drawPoly(pDMLineAniData->hDC, ptTriangle, 3 );
			}
		}


		break;
					}
		}

		return TRUE;
	}

	BOOL efFireworxFrames( DMLineAniData *pDMLineAniData )
	{
		const int NRECT = 10;
		struct FWData {
			POINT ptCenter[NRECT];
		};

		switch ( pDMLineAniData->animType ) {
	case DMLineAniInit: {
		POINT *ptCenter = ((FWData*)pDMLineAniData->bBuffer)->ptCenter;
		POINT ptRectCenter;

		ptRectCenter.x = pDMLineAniData->ptCenter.x;
		ptRectCenter.y = pDMLineAniData->ptCenter.y + (pDMLineAniData->ptRelRightTop.x + pDMLineAniData->ptRelRightTop.y) * 5/3;

		for(int iNdx=0;iNdx < NRECT;iNdx++) {
			MATRIXF matrix = rotateMatrix( iNdx * (360 * fixed1 / NRECT), pDMLineAniData->ptCenter );

			ptCenter[iNdx] = ptRectCenter * matrix;

		}
		break;
				   }
	case DMLineAniDraw:
	case DMLineAniErase: {
		POINT ptTemp;
		FIXED fixedFactor = fixedDiv( pDMLineAniData->iStep, pDMLineAniData->iTotalSteps );

		MATRIXF matrix;
		POINT ptRect[4], ptTmp[4];
		ptRect[0].x = ptRect[3].x = -pDMLineAniData->ptRelRightTop.x;
		ptRect[1].x = ptRect[2].x = pDMLineAniData->ptRelRightTop.x;
		ptRect[0].y = ptRect[1].y = pDMLineAniData->ptRelRightTop.y;
		ptRect[2].y = ptRect[3].y = -pDMLineAniData->ptRelRightTop.y;

		for(int iNdx=0; iNdx < NRECT; iNdx++) {
			matrix = scaleMatrix( fixedFactor, fixedFactor ) *
				rotateMatrix( (fixed1-fixedFactor)*pDMLineAniData->iParameter);

			ptTemp = mix(pDMLineAniData->ptCenter, ((FWData*)pDMLineAniData->bBuffer)->ptCenter[iNdx], fixedFactor);
			matrix = matrix * offsetMatrix( ptTemp.x, ptTemp.y );

			for(int iAngle=0; iAngle < 4; iAngle++) 
				ptTmp[iAngle] = ptRect[iAngle] * matrix;

			drawPoly( pDMLineAniData->hDC, ptTmp, 4 );
		}

		break;
					}
		}

		return TRUE;
	}

	/////////////////////////////////////////////////////////
	// Primitives
	/////////////////////////////////////////////////////////

	void drawBox( HDC hDC, POINT &ptCenter, POINT &ptRelRightTop )
	{
		if (ptRelRightTop.x == 0 && ptRelRightTop.y == 0) return;

		POINT ptTemp[4];

		ptTemp[0].x = ptCenter.x - ptRelRightTop.x;
		ptTemp[0].y = ptCenter.y + ptRelRightTop.y;
		ptTemp[1].x = ptCenter.x + ptRelRightTop.x;
		ptTemp[1].y = ptCenter.y + ptRelRightTop.y;
		ptTemp[2].x = ptCenter.x + ptRelRightTop.x;
		ptTemp[2].y = ptCenter.y - ptRelRightTop.y;
		ptTemp[3].x = ptCenter.x - ptRelRightTop.x;
		ptTemp[3].y = ptCenter.y - ptRelRightTop.y;

		::MoveToEx( hDC, ptTemp[3].x, ptTemp[3].y , NULL );
		::PolylineTo( hDC, ptTemp, 4 );
	}

	void drawPoly( HDC hDC, POINT *pPts, DWORD dwPoints )
	{
		if (pPts == NULL || dwPoints == 0) return;

		if (pPts[dwPoints-1].x == pPts[0].x &&
			pPts[dwPoints-1].y == pPts[0].y ) return;
		::MoveToEx(hDC, pPts[dwPoints-1].x,pPts[dwPoints-1].y,NULL);
		::PolylineTo( hDC, pPts, dwPoints );
	}

}//namespace DM