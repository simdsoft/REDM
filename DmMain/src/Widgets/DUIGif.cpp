#include "DmMainAfx.h"
#include "DUIGif.h"

namespace DM
{
	DUIGif::DUIGif()
	{
		Clear();
		m_bAdjustPic  = true;    // 默认缩放gif自适应区域
		m_bCalcClip   = false;
	}

	DUIGif::~DUIGif()
	{
		Clear();
	}

	DMCode DUIGif::LoadFromFile(LPCWSTR pszFileName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			wchar_t szPath[MAX_PATH] = {0};
			if (false == GetRootFullPath((wchar_t*)pszFileName, szPath,MAX_PATH))
			{
				break;
			}
			DWORD dwSize = GetFileSizeW(szPath);
			if (0 == dwSize)
			{
				break;
			}
			DMBufT<byte>pBuf;pBuf.Allocate(dwSize);
			DWORD dwRead;
			if (false == GetFileBufW(szPath, (void**)&pBuf,dwSize,dwRead))
			{
				break;
			}
			iErr = LoadFromMemory(pBuf,dwSize); 
		} while (false);
		return iErr;
	}

	DMCode DUIGif::LoadFromMemory(LPVOID lpBuf, UINT ulSize)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			Clear();
			DMSmartPtrT<IDMImgDecoder> pImgDecoder;DMSmartPtrT<IDMRes> pRes;DMSmartPtrT<IDMRender> pRender;
			g_pDMApp->CreateRegObj((void**)&pImgDecoder,NULL, DMREG_ImgDecoder);
			g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
			g_pDMApp->GetDefRegObj((void**)&pRender,DMREG_Render);
			if (NULL== pImgDecoder||NULL == pRes||NULL == pRender)
			{
				break;
			}

			if (!DMSUCCEEDED(pImgDecoder->LoadFromMemory(lpBuf, ulSize)))
			{
				break;
			}
			pImgDecoder->GetTotalLoopCount(m_ulTotalLoopCount);
			pImgDecoder->GetFrameCount(m_ulFrameCount);
			if (1 >= m_ulFrameCount||0 == m_ulTotalLoopCount)
			{
				break;
			}
			// gif解码格式,wic可以解码帧，所以把图形数据部分去掉了
			DMSmartPtrT<DMGifParse> pObj;
			pObj.Attach(new DMGifParse);
			if (!pObj->LoadFromMemory((BYTE*)lpBuf,ulSize))
			{
				break;
			}
			LPGLOBAL_INFO pGlobalInfo = pObj->GetGlobalInfo();
			DMASSERT_EXPR(pGlobalInfo->frames==m_ulFrameCount,L"解码的帧不同!");
			for (int i=0;i<(int)m_ulFrameCount;i++)
			{
				DMAnimateFrame* pAnimateFrame = new DMAnimateFrame;
				pRender->CreateBitmap(&pAnimateFrame->pBitmap);
				DMSmartPtrT<IDMImgFrame> pImgFrame;
				if (!DMSUCCEEDED(pImgDecoder->GetFrame(i,&pImgFrame)))
				{
					DM_DELETE(pAnimateFrame);
					break;
				}
				if (!DMSUCCEEDED(pAnimateFrame->pBitmap->InitFromFrame(pImgFrame)))
				{
					DM_DELETE(pAnimateFrame);
					break;
				}
				memcpy(&pAnimateFrame->gifFrame,pObj->GetFrame(i),sizeof(GIFFRAME));
				UINT ulDelay = 0;
				pImgFrame->GetDelay(ulDelay);
				DMASSERT_EXPR(ulDelay==pAnimateFrame->gifFrame.ctrlExt.delayTime*10,L"解码的延迟不同!");
				AddObj(pAnimateFrame);
			}
			m_ulFrameCount = (UINT)GetCount();// 怕不定所有帧都能解析成功
			pRender->CreateCanvas(pGlobalInfo->scrWidth,pGlobalInfo->scrHeight,&m_pMemCanvas);
			m_rcGif.SetRect(0,0,pGlobalInfo->scrWidth,pGlobalInfo->scrHeight);
			iErr = DM_ECODE_OK; 
		} while (false);
		if (!DMSUCCEEDED(iErr))
		{
			Clear();
		}
		return iErr;
	}

	DMCode DUIGif::Start()
	{
		Reset();
		if (IsReady())
		{
			GetContainer()->OnRegisterTimeline(this);// 注册
		}
		return DM_ECODE_OK;
	}

	DMCode DUIGif::Pause()
	{
		m_bPause = true;
		return DM_ECODE_OK;
	}

	DMCode DUIGif::Restart()
	{	
		m_bPause = false;
		if (IsReady())
		{
			GetContainer()->OnRegisterTimeline(this);// 注册
		}
		return DM_ECODE_OK;
	}

	DMCode DUIGif::Clear()
	{
		m_ulCurFrame	     = 0;
		m_ulFrameCount		 = 0;
		m_ulCurLoop          = 0;
		m_ulTotalLoopCount   = INFINITE;// 默认是永久循环
		m_dwPreFrameTime     = 0;       // 时间刷新时初始化
		m_bPause             = false;
		m_pMemCanvas.Release();
		RemoveAll();
		return DM_ECODE_OK;
	}

	void DUIGif::DM_OnPaint(IDMCanvas* pCanvas)
	{
		do 
		{
			__super::DM_OnPaint(pCanvas);
			int iState = ReadyCurFrame(m_ulCurFrame);
			if (DMGIF_NOREADY==iState)
			{
				break;
			}
			if (DMGIF_LOOPEND == iState)
			{
				m_ulCurFrame = 0;// 设置回初始帧
				m_ulCurLoop  = 0;// 设置回初始loop
				m_bPause = true; // End时设置为初始帧暂停状态,下次就是重新播放了
			}
			CRect rcClient;
			DV_GetClientRect(rcClient);
			if (m_bAdjustPic)
			{
				CSize szPic(m_rcGif.Width(), m_rcGif.Height());
				MeetRect(rcClient, szPic);
			}
			bool bClip = false;
			if (DMGIF_READY == iState)
			{
				if (m_bCalcClip)
				{
					CRect rcInvalid;
					pCanvas->GetClipBox(rcInvalid);
					CRect rcDest = rcInvalid&rcClient;
					if (!rcDest.IsRectEmpty()&&rcClient!=rcDest)// 无效区和绘制区有交集
					{
						bClip = true;
						pCanvas->PushClip(rcClient,RGN_OR);
					}
				}
			
				// 绘制
				PDMAnimateFrame &pFrame = m_DMArray[m_ulCurFrame];
				CRect rcFrame(pFrame->gifFrame.imageLPos, pFrame->gifFrame.imageTPos, 
					pFrame->gifFrame.imageLPos+pFrame->gifFrame.imageWidth,pFrame->gifFrame.imageTPos+pFrame->gifFrame.imageHeight);

				CRect rcSor = rcFrame;
				rcSor.OffsetRect(-rcSor.TopLeft());
				// If starting a new animation loop
				if (0 == m_ulCurFrame)
				{
					m_pMemCanvas->ClearRect(m_rcGif,0);
				}

				if (DM_BACKGROUND == pFrame->gifFrame.ctrlExt.disposalMethod)
				{
					m_pMemCanvas->ClearRect(rcFrame,PBGRA(0,0,0,0));// 清空frame所在区域
				}

				if (DM_PREVIOUS == pFrame->gifFrame.ctrlExt.disposalMethod)
				{// 回到先前状态
				}
				else
				{
					m_pMemCanvas->DrawBitamp(pFrame->pBitmap,rcSor,rcFrame);
				}
				m_dwPreFrameTime = GetTickCount();// 重设置时间
				m_ulCurFrame++;					  // 增加引用计数
			}
			pCanvas->AlphaBlend(m_pMemCanvas,m_rcGif, rcClient);
			if (bClip)
			{
				pCanvas->PopClip();
			}
		} while (false);
	}


	void DUIGif::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		__super::OnShowWindow(bShow, nStatus);
		if (!bShow)
		{
			Pause();
		}
		else
		{
			Restart();
		}
	}

	void DUIGif::OnDestroy()
	{
		GetContainer()->OnUnregisterTimeline(this);
		__super::OnDestroy();
	}

	DMCode DUIGif::OnTimeline()
	{
		do 
		{
			if (!DM_IsVisible(true))
			{
				break;
			}
			int iState = ReadyCurFrame(m_ulCurFrame);
			if (DMGIF_READY == iState)
			{
				DM_Invalidate();
			}
		} while (false);
		return DM_ECODE_OK;
	}

	bool  DUIGif::IsReady()
	{
		bool bRet = false;
		do 
		{
			if (0 == m_ulTotalLoopCount)
			{
				break;
			}
			if (m_ulFrameCount<2)		// 至少2帧
			{
				break;
			}
			if (m_pMemCanvas.isNull())	// gif解析完后,缓存帧画布必须初始化
			{
				break;
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	bool  DUIGif::Reset()
	{
		bool bRet = false;
		do 
		{
			if (!IsReady())
			{
				break;
			}
			m_ulCurFrame	     = 0;
			m_ulCurLoop          = 0;
			m_dwPreFrameTime     = 0;       // 时间刷新时初始化
			m_pMemCanvas->ClearRect(m_rcGif,0);
			m_bPause             = false;

			bRet = true;
		} while (false);
		return bRet;
	}

	int DUIGif::ReadyCurFrame(UINT ulFrame)
	{
		DMGifState iState = DMGIF_NOREADY;
		do 
		{
			if (!IsReady())
			{
				break;
			}

			//1. 是否暂停
			if (m_bPause)
			{
				iState = DMGIF_PAUSE;
				break;
			}

			//1. 计算循环
			if (m_ulCurFrame>=m_ulFrameCount)
			{
				m_ulCurFrame = 0; // 循环一轮
				if (INFINITE!=m_ulTotalLoopCount)
				{
					m_ulCurLoop++;
				}
			}

			//2. 循环次数已到则暂停
			if (INFINITE!=m_ulTotalLoopCount&&m_ulCurLoop>=m_ulTotalLoopCount)
			{
				iState = DMGIF_LOOPEND;
				break;
			}

			//3. 时间未到
			if (GetTickCount()-m_dwPreFrameTime<(DWORD)m_DMArray[m_ulCurFrame]->gifFrame.ctrlExt.delayTime*10)
			{
				iState = DMGIF_NODELAY;
				break;
			}

			PDMAnimateFrame &pFrameObj = m_DMArray[m_ulCurFrame];
			if (0 == pFrameObj->gifFrame.ctrlExt.delayTime)
			{
				pFrameObj->gifFrame.ctrlExt.delayTime = 10;// 默认0.1s吧
			}
			iState = DMGIF_READY;
		} while (false);
		return iState;
	}

	DMCode DUIGif::OnAttrGif(LPCWSTR lpszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{	
			CStringW  strValue = lpszValue;
			if (PathFileExistsW(strValue))// 这是一个文件绝对路径
			{
				iErr = LoadFromFile(strValue);
				break;
			}

			CStringW strType;
			CStringW strResName;
			CStringWList strList;
			int nCount = SplitStringT(strValue,L':',strList);
			if (1== nCount)// 可能是GIF:IDR_GIF
			{
				strType    = L"GIF";
				strResName = strList[0];
			}
			else if(2 == nCount)
			{
				strType    = strList[0];
				strResName = strList[1];
			}
			else
			{
				CStringW strInfo;
				strInfo.Format(L"gifskin参数%s设置错误",lpszValue);
				DMASSERT_EXPR(0,L"gifskin设置错误!");
				break;
			}
			DMSmartPtrT<IDMRes> pRes;
			g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
			if (!pRes)
			{
				break;
			}
			ULONG ulSize = 0;
			pRes->GetItemSize(strType,strResName,ulSize);
			if (0>=ulSize)
			{
				break;
			}
			DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
			if (!DMSUCCEEDED(pRes->GetItemBuf(strType,strResName, pBuf, ulSize)))
			{
				break;
			}
			iErr = LoadFromMemory(pBuf,ulSize); 
		} while (false);
		if (DMSUCCEEDED(iErr))
		{
			Start(); // 播放
		}
		return iErr;
	}

	void DUIGif::PreArrayObjRemove(const PDMAnimateFrame &obj)
	{
		delete obj;
	}
}//namespace DM