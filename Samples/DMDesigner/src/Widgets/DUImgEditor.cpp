#include "DMDesignerAfx.h"
#include "DUImgEditor.h"
#pragma comment(lib,"gdiplus")
DUImgEditor::DUImgEditor()
{
	m_bVert = false;
	GdiplusStartupInput input ;
	GdiplusStartup(&m_token, &input, NULL);
}

DUImgEditor::~DUImgEditor()
{
	GdiplusShutdown(m_token) ;
}

void DUImgEditor::DM_OnPaint(IDMCanvas* pCanvas)
{ 
	if (m_BmpList.IsEmpty())
	{
		DUIWindow::DM_OnPaint(pCanvas);
	}
	else 
	{
		CRect rcClient;
		DV_GetClientRect(rcClient);

		// 每一项大小
		IDMBitmap* pFirstBmp = m_BmpList.GetHead();
		CSize szFirstBmp(pFirstBmp->GetWidth(),pFirstBmp->GetHeight());

		CSize sizeAll = GetRangeSize();
		CPoint ptOffset;
		if (m_bVert)
		{
			ptOffset.y = szFirstBmp.cy;
		}
		else
		{
			ptOffset.x = szFirstBmp.cx;
		}

		CRect rcDestItem(0,0,szFirstBmp.cx,szFirstBmp.cy);
		CRect rcAll(0,0,sizeAll.cx,sizeAll.cy);
		rcDestItem.OffsetRect(rcClient.TopLeft()-m_ptCurPos);
		rcAll.OffsetRect(rcClient.TopLeft()-m_ptCurPos);
		if (rcClient.Height()>sizeAll.cy)// 没有坚直滚动条，让它竖起居中
		{
			int iHAvg = (rcClient.Height()-sizeAll.cy)/2;
			rcDestItem.OffsetRect(0,iHAvg);
			rcAll.OffsetRect(0,iHAvg);
		
		}
		if (rcClient.Width()>sizeAll.cx)// 没有水平滚动条，让它水平居中
		{
			int iWAvg = (rcClient.Width()-sizeAll.cx)/2;
			rcDestItem.OffsetRect(iWAvg,0);
			rcAll.OffsetRect(iWAvg,0);
		}

		POSITION pos = m_BmpList.GetHeadPosition();
		while(pos)
		{
			IDMBitmap* pBmp = m_BmpList.GetNext(pos);
			CSize szBmp(pBmp->GetWidth(),pBmp->GetHeight());
			CRect rcSrc,rcDest;
			szBmp.cx = szBmp.cx>szFirstBmp.cx?szFirstBmp.cx:szBmp.cx;
			szBmp.cy = szBmp.cy>szFirstBmp.cy?szFirstBmp.cy:szBmp.cy;
			rcSrc.SetRect(0,0,szBmp.cx,szBmp.cy);
			rcDest = rcSrc;
			rcDest.OffsetRect(rcDestItem.TopLeft());

			//1.绘制图片
			pCanvas->DrawBitamp(pBmp,rcSrc,rcDest);

			//2.继续偏移
			rcDestItem.OffsetRect(ptOffset);
		}
		//3.绘制边框
		if (m_BmpList.GetCount()>1)
		{
			DMSmartPtrT<IDMPen> pPen,oldPen;
			DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			pRender->CreatePen(PBGRA((00),(00),(128),0XFF),PS_DASHDOT,1,&pPen);
			pCanvas->SelectObject(pPen,(IDMMetaFile**)&oldPen);
			pCanvas->DrawRectangle(&rcAll);    
			pCanvas->SelectObject(oldPen);
		}
	}
}

bool DUImgEditor::AddImg(CStringW strPath)
{
	bool bRet = false;
	do 
	{
		DMSmartPtrT<IDMRender> pRender;
		if (!DMSUCCEEDED(g_pDMApp->GetDefRegObj((void**)&pRender, DMREG_Render)))
		{
			DMASSERT_EXPR(0,L"竟然获取默认Render失败,不太可能吧!");
			break;
		}
		DMSmartPtrT<IDMBitmap> pBitmap;
		pRender->CreateBitmap(&pBitmap);
		if (!DMSUCCEEDED(pBitmap->LoadFromFile(strPath,NULL)))
		{
			break;
		}
		 pBitmap->AddRef();
		 m_BmpList.AddTail(pBitmap.get());
		 UpdateScrollRangeSize();
		 DM_Invalidate();
		
		bRet = true;
	} while (false);
	return bRet;
}

bool DUImgEditor::Clear()
{
	POSITION pos = m_BmpList.GetHeadPosition();
	while(pos)
	{
		IDMBitmap* pBmp = m_BmpList.GetNext(pos);
		pBmp->Release();
	}
	m_BmpList.RemoveAll();
	UpdateScrollRangeSize();
	DM_Invalidate();
	return true;
}

bool DUImgEditor::SetVert(bool bVert)
{
	m_bVert = bVert;
	UpdateScrollRangeSize();
	DM_Invalidate();
	return true;
}

bool DUImgEditor::Save(CStringW strPath)
{
	bool bRet = false;
	do 
	{
		CSize sz = GetRangeSize();
		if (0 == sz.cx
			||0 == sz.cy)
		{
			break;
		}

		DMSmartPtrT<IDMCanvas> pCanvas;
		DMSmartPtrT<IDMRender> pRender;
		g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
		pRender->CreateCanvas(sz.cx,sz.cy,&pCanvas);

		// 绘制
		IDMBitmap* pFirstBmp = m_BmpList.GetHead();
		CSize szFirstBmp(pFirstBmp->GetWidth(),pFirstBmp->GetHeight());
		CPoint ptOffset;
		if (m_bVert)
		{
			ptOffset.y = szFirstBmp.cy;
		}
		else
		{
			ptOffset.x = szFirstBmp.cx;
		}

		CRect rcDestItem(0,0,szFirstBmp.cx,szFirstBmp.cy);
		POSITION pos = m_BmpList.GetHeadPosition();
		while(pos)
		{
			IDMBitmap* pBmp = m_BmpList.GetNext(pos);
			CSize szBmp(pBmp->GetWidth(),pBmp->GetHeight());
			CRect rcSrc,rcDest;
			szBmp.cx = szBmp.cx>szFirstBmp.cx?szFirstBmp.cx:szBmp.cx;
			szBmp.cy = szBmp.cy>szFirstBmp.cy?szFirstBmp.cy:szBmp.cy;
			rcSrc.SetRect(0,0,szBmp.cx,szBmp.cy);
			rcDest = rcSrc;
			rcDest.OffsetRect(rcDestItem.TopLeft());

			//1.绘制图片
			pCanvas->DrawBitamp(pBmp,rcSrc,rcDest);

			//2.继续偏移
			rcDestItem.OffsetRect(ptOffset);
		}

		// 保存文件
		DMSmartPtrT<IDMBitmap> pBitmap;
		pCanvas->GetObject((IDMMetaFile**)&pBitmap,DMF_BITMAP);

		// 保存到文件
		EncoderParameters encoderParameters;  
		encoderParameters.Count = 1;  
		encoderParameters.Parameter[0].Guid = EncoderQuality;  
		encoderParameters.Parameter[0].Type = EncoderParameterValueTypeLong;  
		encoderParameters.Parameter[0].NumberOfValues = 1;  
		ULONG quality = 100;  
		encoderParameters.Parameter[0].Value = &quality;  

		LPVOID pBits = pBitmap->GetPixelBits();
		Bitmap bmp(sz.cx,sz.cy,sz.cx*4,PixelFormat32bppPARGB,(BYTE*)pBits);
		Image *gdipImg = &bmp;
		CLSID encoderClsid;
		CStringW strAfx = strPath.Right(strPath.GetLength()-1-strPath.ReverseFind(L'.'));
		if (0 == strAfx.CompareNoCase(L"png"))
		{
			strAfx = L"image/png";
		}
		else if (0 == strAfx.CompareNoCase(L"jpg")||0 == strAfx.CompareNoCase(L"jpeg"))
		{
			strAfx = L"image/jpeg";
		}
		else if (0 == strAfx.CompareNoCase(L"bmp"))
		{
			strAfx = L"image/bmp";
		}
		GetEncoderClsid(strAfx,&encoderClsid) ; 
		Status st = gdipImg->Save(strPath, &encoderClsid, &encoderParameters);
		if (Ok!=st)
		{
			break;
		}

		bRet = true;
	} while (false);
	return bRet;
}

int DUImgEditor::GetImgCount()
{
	return m_BmpList.GetCount();
}

void DUImgEditor::UpdateScrollRangeSize()
{
	SetRangeSize(GetRangeSize()); 
}

CSize DUImgEditor::GetRangeSize()
{
	CSize size;
	do 
	{
		int iHei = 0;
		int iWid = 0;
		int iCount = m_BmpList.GetCount();
		if (0 == iCount)
		{
			break;
		}
		IDMBitmap* pFirstBmp = m_BmpList.GetHead();
		CSize szBmp(pFirstBmp->GetWidth(),pFirstBmp->GetHeight());
		if (m_bVert)
		{
			iHei = szBmp.cy*iCount;
			iWid = szBmp.cx;
		}
		else
		{
			iWid = szBmp.cx*iCount;
			iHei = szBmp.cy;
		}
		size.SetSize(iWid,iHei);
	} while (false);
	return size;
}

int DUImgEditor::GetEncoderClsid(const WCHAR* format, CLSID* pClsid)  
{  
	UINT num = 0; // number of image encoders   
	UINT size = 0; // size of the image encoder array in bytes   
	ImageCodecInfo* pImageCodecInfo = NULL;  
	GetImageEncodersSize(&num, &size);  
	if(size == 0)  
		return -1; // Failure   
	pImageCodecInfo = (ImageCodecInfo*)(malloc(size));  
	if(pImageCodecInfo == NULL)  
		return -1; // Failure   
	GetImageEncoders(num, size, pImageCodecInfo);  
	for(UINT j = 0; j < num; ++j)  
	{  
		if( wcscmp(pImageCodecInfo[j].MimeType, format) == 0 )  
		{  
			*pClsid = pImageCodecInfo[j].Clsid;  
			free(pImageCodecInfo);  
			return j; // Success   
		}  
	}  
	free(pImageCodecInfo);  
	return -1;   
}


