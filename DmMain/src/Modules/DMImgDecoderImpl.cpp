#include "DmMainAfx.h"
#include "DMImgDecoderImpl.h"
#include "DMGifParse.h"

namespace DM
{
	DMImgFrameImpl::DMImgFrameImpl(IWICBitmapSource* pBmpSource,UINT uFrameDelay/*=0*/)
		:m_pBmpSource(pBmpSource),m_uFrameDelay(uFrameDelay)
	{

	}

	DMCode DMImgFrameImpl::GetSize(UINT &ulWid,UINT &ulHei)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (m_pBmpSource.isNull())
			{
				iErr = DM_ECODE_FAIL;
				break;
			}

			if (S_OK != m_pBmpSource->GetSize(&ulWid, &ulHei))
			{
				iErr = DM_ECODE_FAIL;
				break;
			}
		} while (false);
		return iErr;
	}

    DMCode DMImgFrameImpl::SetDelay(UINT ulDelay)
	{
		m_uFrameDelay = ulDelay;
		return DM_ECODE_OK;
	}

	DMCode DMImgFrameImpl::GetDelay(UINT &ulDelay)
	{
		ulDelay = m_uFrameDelay;
		return DM_ECODE_OK;
	}

	DMCode DMImgFrameImpl::CopyPixels(LPVOID lpBuf, UINT ulSize,UINT cbStride,const RECT *lpRect/*=NULL*/)
	{
		DMCode iErr = DM_ECODE_OK;
		do 
		{
			if (m_pBmpSource.isNull())
			{
				iErr = DM_ECODE_FAIL;
				break;
			}

			if (NULL == lpRect)
			{
				if (S_OK != m_pBmpSource->CopyPixels(NULL,cbStride,ulSize,(BYTE*)lpBuf))
				{
					iErr = DM_ECODE_FAIL;
					break;
				}
			}
			else
			{
				WICRect rc = {lpRect->left,lpRect->top,lpRect->right-lpRect->left,lpRect->bottom-lpRect->top};
				if (S_OK != m_pBmpSource->CopyPixels(&rc,cbStride,ulSize,(BYTE*)lpBuf))
				{
					iErr = DM_ECODE_FAIL;
					break;
				}
			}
		} while (false);
		return iErr;
	}

	/// DMImgDecoderImpl-----------------------------------
	DMSmartPtrT<IWICImagingFactory>  DMImgDecoderImpl::ms_ImgFactory;
	DMImgDecoderImpl::DMImgDecoderImpl()
	{
		m_uTotalLoopCount = INFINITE;
		m_ulFrameCount = 0;
	}

	DMImgDecoderImpl::~DMImgDecoderImpl()
	{
		RemoveAll();
	}

	DMCode DMImgDecoderImpl::LoadFromMemory(const void *pBuf,size_t bufLen)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (ms_ImgFactory.isNull())
			{
				HRESULT hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&ms_ImgFactory)
					);
				if (FAILED(hr))
				{
					DMFAIL_MSG("CoCreateInstance fail");
					break;
				}
			}

			DMSmartPtrT<IWICStream> pStream;
			if (FAILED(ms_ImgFactory->CreateStream(&pStream)))
			{
				break;
			}

			if (FAILED(pStream->InitializeFromMemory((BYTE*)pBuf,(DWORD)bufLen)))
			{
				break;
			}

			DMSmartPtrT<IWICBitmapDecoder> pBmpDecoder;
			if (FAILED(ms_ImgFactory->CreateDecoderFromStream(pStream,NULL,WICDecodeMetadataCacheOnDemand,&pBmpDecoder)))
			{
				break;
			}

			if (DoDecode(pBmpDecoder))
			{
				iErr = DM_ECODE_OK;
				if (!CheckFramesValid())
				{
					LOG_ERR("[mid]��֡���ӳ�ʱ���ȡʧ��\n");
					RepairFrames(pBuf,bufLen);
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DMImgDecoderImpl::LoadFromFile(LPCWSTR pszFileName)
	{
		DMCode iErr = DM_ECODE_FAIL;
		DMSmartPtrT<IWICBitmapDecoder> pBmpDecoder;
		do 
		{
			wchar_t szExeDir[MAX_PATH] = {0};
			if (!GetRootDirW(szExeDir, MAX_PATH))
			{
				break;
			}

			wchar_t szPath[MAX_PATH] = {0};
			if (0 == PathCombineW(szPath, szExeDir, pszFileName))
			{
				break;
			}

			if (!CheckFileExistW(szPath))
			{
				break;
			}

			if (ms_ImgFactory.isNull())
			{
				HRESULT hr = CoCreateInstance(
					CLSID_WICImagingFactory,
					NULL,
					CLSCTX_INPROC_SERVER,
					IID_PPV_ARGS(&ms_ImgFactory)
					);
				if (FAILED(hr))
				{
					break;
				}
			}

			if (FAILED(ms_ImgFactory->CreateDecoderFromFilename(
				szPath,
				NULL,
				GENERIC_READ,
				WICDecodeMetadataCacheOnLoad,
				&pBmpDecoder
				)))
			{
				break;
			}

			if (DoDecode(pBmpDecoder))
			{
				iErr = DM_ECODE_OK;
				if (!CheckFramesValid())
				{
					LOG_ERR("[mid]��֡���ӳ�ʱ���ȡʧ��\n");
					RepairFrames(szPath);
				}
			}
		} while (false);
		return iErr;
	}

	DMCode DMImgDecoderImpl::GetFrameCount(UINT &ulCount)
	{
		ulCount = (UINT)GetCount();//����DMArrayT
		return DM_ECODE_OK;
	}

	DMCode DMImgDecoderImpl::GetTotalLoopCount(UINT &ulCount)
	{
		ulCount = m_uTotalLoopCount;
		return DM_ECODE_OK;
	}

	DMCode DMImgDecoderImpl::GetFrame(UINT iFrame, IDMImgFrame**ppFrame)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == ppFrame)
			{
				break;
			}

			DMImgFrameImplPtr pObj = NULL;
			if (false == GetObj(iFrame, pObj))//DMArrayT��ȫ����
			{
				break;
			}
			*ppFrame = pObj;
			(*ppFrame)->AddRef();//
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}


	///���� ------------------------------------------
	bool DMImgDecoderImpl::DoDecode(IWICBitmapDecoder* pBmpDecoder)
	{
		bool bRet =  false;
		do 
		{
			// ��������DMImgFrameImpl*
			if (FAILED(pBmpDecoder->GetFrameCount(&m_ulFrameCount)))
			{
				break;
			}

			if (0 == m_ulFrameCount)
			{
				break;
			}
			m_uTotalLoopCount = INFINITE;// -1Ĭ����infinitely loop
			if (m_ulFrameCount>1)
			{
				m_uTotalLoopCount = GetTotalLoopCount(pBmpDecoder); 
			}

			for (int i=0; i<(int)m_ulFrameCount; i++)
			{
				ParseFrame(pBmpDecoder,i);
			}
			UINT ulFrameCount = 0;
			GetFrameCount(ulFrameCount);
			if (m_ulFrameCount != ulFrameCount)
			{
				DMASSERT_MSG(m_ulFrameCount == ulFrameCount,"some frame can't decode");
				m_ulFrameCount = ulFrameCount;
			}

			bRet = true;
		} while (false);

		return bRet;
	}

	bool DMImgDecoderImpl::ParseFrame(IWICBitmapDecoder* pBmpDecoder,unsigned int iFrame)
	{
		bool bRet =  false;
		do 
		{
			DMSmartPtrT<IWICFormatConverter> pFormatConvert;
			if (FAILED(ms_ImgFactory->CreateFormatConverter(&pFormatConvert)))
			{
				break;
			}

			DMSmartPtrT<IWICBitmapFrameDecode> pBmpFrameDecoder;
			if (SUCCEEDED(pBmpDecoder->GetFrame(iFrame, &pBmpFrameDecoder)))
			{
				WICPixelFormatGUID pixelFormat;
			    auto hr = pBmpFrameDecoder->GetPixelFormat(&pixelFormat);

				unsigned int uFrameDelay = 0;        // ��֡ʹ��
				if (SUCCEEDED(
					pFormatConvert->Initialize(
					pBmpFrameDecoder,				      //Input bitmap to convert
					GUID_WICPixelFormat32bppPBGRA,        //Destination pixel format
					WICBitmapDitherTypeNone,              //Specified dither patterm
					NULL,                                 //Specify a particular palette
					0.f,								  //Alpha threshold
					WICBitmapPaletteTypeCustom			  //Palette translation type
					))
					)
				{
					if (m_ulFrameCount>1)
					{// ��֡
						uFrameDelay = GetFrameDelay(pBmpFrameDecoder);

					}

					DMSmartPtrT<IWICBitmapSource> pBmpSource;
					if (SUCCEEDED(pFormatConvert->QueryInterface(IID_PPV_ARGS(&pBmpSource))))
					{
						DMImgFrameImpl *pObj = new DMImgFrameImpl(pBmpSource,uFrameDelay);
						AddObj(pObj);// �˺�������DMArrayTģ��
					}
				}
			}

			bRet = true;
		} while (false);
		return bRet;
	}

	UINT DMImgDecoderImpl::GetFrameDelay(IWICBitmapFrameDecode* pBmpFrameDecoder)
	{
		unsigned int uFrameDelay = 0;        // ��֡ʹ��
		do 
		{
			if (NULL == pBmpFrameDecoder)
			{
				break;
			}

			DMSmartPtrT<IWICMetadataQueryReader> pFrameMetadataQueryReader;

			PROPVARIANT propValue;
			PropVariantInit(&propValue);
			HRESULT hr = pBmpFrameDecoder->GetMetadataQueryReader(&pFrameMetadataQueryReader);
			if(!SUCCEEDED(hr))
			{
				LOG_ERR("[mid]-GetMetadataQueryReader:0x%08x\n",hr);
				break;
			}
			// note: ��XP�£��᷵��WINCODEC_ERR_UNSUPPORTEDOPERATION��0x88982f81��
			if (SUCCEEDED(hr))
			//if (SUCCEEDED(pBmpFrameDecoder->GetMetadataQueryReader(&pFrameMetadataQueryReader)))
			{// Get Metadata Query Reader from the frame
				if (SUCCEEDED(pFrameMetadataQueryReader->GetMetadataByName(L"/grctlext/Delay", &propValue)))
				{// ȡ��gif��֡�ӳ�
					if (propValue.vt == VT_UI2)
					{// Convert the delay retrieved in 10 ms units to a delay in 1 ms units	
						HRESULT hr = UIntMult(propValue.uiVal, 10, &uFrameDelay);
					}
				}
			}
			PropVariantClear(&propValue);
		} while (false);
		return uFrameDelay;
	}

	UINT DMImgDecoderImpl::GetTotalLoopCount(IWICBitmapDecoder* pBmpDecoder)
	{
		unsigned int uTotalLoopCount = INFINITE; // -1Ĭ����infinitely loop
		do 
		{
			if (NULL == pBmpDecoder)
			{
				break;
			}

			DMSmartPtrT<IWICMetadataQueryReader> pMetadataQueryReader;

			HRESULT hr = pBmpDecoder->GetMetadataQueryReader(&pMetadataQueryReader);
			if(!SUCCEEDED(hr))
			{
				LOG_DEBUG("[mid]-GetMetadataQueryReader:0x%08x\n",hr);
				break;
			}

			// ����״̬��1.û��ѭ����Ϣ��1�Σ� 2.��ѭ����Ϣ��������3.��ѭ����Ϣ��0�α�ʾ��Զѭ����
			PROPVARIANT propValue;
			PropVariantInit(&propValue);
			if (SUCCEEDED(pMetadataQueryReader->GetMetadataByName(
				L"/appext/application", 
				&propValue)) &&
				propValue.vt == (VT_UI1 | VT_VECTOR) &&
				propValue.caub.cElems == 11 &&  // Length of the application block
				(!memcmp(propValue.caub.pElems, "NETSCAPE2.0", propValue.caub.cElems)
				||!memcmp(propValue.caub.pElems, "ANIMEXTS1.0", propValue.caub.cElems)))
			{
				PropVariantClear(&propValue);
				if (SUCCEEDED(pMetadataQueryReader->GetMetadataByName(L"/appext/data", &propValue)))
				{
					//  The data is in the following format:
					//  byte 0: extsize (must be > 1)
					//  byte 1: loopType (1 == animated gif)
					//  byte 2: loop count (least significant byte)
					//  byte 3: loop count (most significant byte)
					//  byte 4: set to zero
					if (propValue.vt == (VT_UI1 | VT_VECTOR) &&
						propValue.caub.cElems >= 4 &&
						propValue.caub.pElems[0] > 0 &&
						propValue.caub.pElems[1] == 1)
					{
						uTotalLoopCount = MAKEWORD(propValue.caub.pElems[2], propValue.caub.pElems[3]);

						// If the total loop count is not zero, we then have a loop count
						// If it is 0, then we repeat infinitely
						if (0 == uTotalLoopCount) 
						{
							uTotalLoopCount = INFINITE;// hgy���ã�-1���ڱ�ʶ��
						}
						else
						{
							uTotalLoopCount++;// �����Ǵ�0��ʼ�ģ�����+1
						}
					}
				}
			}
			else
			{
				uTotalLoopCount = 1;
			}
			PropVariantClear(&propValue);

		} while (false);
		return uTotalLoopCount;
	}

	bool DMImgDecoderImpl::CheckFramesValid()
	{
		bool bRet = false;
		do 
		{
			if (1>=m_ulFrameCount)// ��У�鵥֡
			{
				bRet = true;
				break;
			}
			
			UINT ulDelay = 0;
			for (int i=0;i<(int)m_ulFrameCount;i++)
			{
				DMImgFrameImplPtr &ptr = m_DMArray[i];
				ptr->GetDelay(ulDelay);
				if (0!=ulDelay)// ֻҪ��һ֡�ӳٲ�Ϊ0������ΪУ��ͨ��
				{
					bRet = true;
					break;
				}
			}
		} while (false);
		return bRet;
	}

	void DMImgDecoderImpl::RepairFrames(const void *pBuf,size_t bufLen)
	{
		do 
		{
			DMSmartPtrT<DMGifParse> pObj;
			pObj.Attach(new DMGifParse);
			if (pObj->LoadFromMemory((const BYTE*)pBuf, (int)bufLen))
			{
				LPGLOBAL_INFO gi = pObj->GetGlobalInfo();
				DMASSERT_MSG(gi->frames==m_ulFrameCount,"frameCount mismatch");
				for (int i=0;i<(int)m_ulFrameCount;i++)
				{
					GIFFRAMEPTR fm = pObj->GetFrame(i);
					if (fm)
					{
						m_DMArray[i]->SetDelay(fm->ctrlExt.delayTime*10);
					}
				}
				if (gi->bLoop)
				{
					if (gi->totalLoopCount)
					{
						m_uTotalLoopCount = gi->totalLoopCount;
					}
					else
					{
						m_uTotalLoopCount = INFINITE;
					}
				}
				else
				{
					m_uTotalLoopCount = 1;
				}
			}
		} while (false);
	}

	void DMImgDecoderImpl::RepairFrames(LPCWSTR pszFileName)
	{
		do 
		{
			DMSmartPtrT<DMGifParse> pObj;
			pObj.Attach(new DMGifParse);
			if (pObj->LoadFromFile(pszFileName))
			{
				LPGLOBAL_INFO gi = pObj->GetGlobalInfo();
				DMASSERT_MSG(gi->frames==m_ulFrameCount,L"frameCount mismatch");
				for (int i=0;i<(int)m_ulFrameCount;i++)
				{
					GIFFRAMEPTR fm = pObj->GetFrame(i);
					if (fm)
					{
						m_DMArray[i]->SetDelay(fm->ctrlExt.delayTime*10);
					}
				}
				if (gi->bLoop)
				{
					if (gi->totalLoopCount)
					{
						m_uTotalLoopCount = gi->totalLoopCount;
					}
					else
					{
						m_uTotalLoopCount = INFINITE;
					}
				}
				else
				{
					m_uTotalLoopCount = 1;
				}
			}
		} while (false);
	}

	void DMImgDecoderImpl::PreArrayObjRemove(const DMImgFrameImplPtr &obj)
	{
		obj->Release();
	}
}//namespace DM