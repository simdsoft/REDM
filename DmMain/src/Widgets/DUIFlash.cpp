#include "DmMainAfx.h"
#include "DUIFlash.h"

namespace DM
{
	DUIFlash::DUIFlash()
	{
		m_ClsId    = __uuidof(ShockwaveFlashObjects::ShockwaveFlash);
		m_curFrame = 0;
	}

	DMCode DUIFlash::DV_OnAxActivate(IUnknown *pUnknwn)// 初始化时会调用到此处
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> pFlash = Ptr();
			if (!pFlash)
			{
				break;
			}
			pFlash->put_WMode(bstr_t(_T("transparent")));// 设置背景透明
			if (m_strUrl.IsEmpty())
			{
				break;
			}

			DMSmartPtrT<IDMRes> pRes;
			g_pDMApp->GetDefRegObj((void**)&pRes, DMREG_Res);
			if (NULL==pRes)
			{
				break;
			}

			CStringW strType;
			CStringW strResName;
			int iFind = m_strUrl.ReverseFind(_T(':'));
			if (-1 != iFind)
			{
				strType    = m_strUrl.Left(iFind);
				strResName = m_strUrl.Right(m_strUrl.GetLength()-iFind-1);
				if (-1!=strType.MakeLower().Find(L"http")||-1!=strType.MakeLower().Find(L"ftp"))// http在线url
				{
					Play(m_strUrl);
				}
				else// 本地资源包,使用内存方式播放
				{
					ULONG ulSize = 0;
					pRes->GetItemSize(strType,strResName,ulSize);
					if (ulSize)
					{
						DMBufT<byte>pBuf;pBuf.Allocate(ulSize);
						if (DMSUCCEEDED(pRes->GetItemBuf(strType,strResName, pBuf, ulSize)))
						{
							Play(pBuf, ulSize);
						}
					}
					else
					{
						DWORD dwSize = GetFileSizeW((LPCWSTR)m_strUrl);
						DMBufT<byte> pBuf;
						pBuf.Allocate(dwSize);
						DWORD dwRead;
						GetFileBufW((LPCWSTR)m_strUrl,(void**)&pBuf,dwSize,dwRead);
						if (pBuf && dwSize)
						{
							Play(pBuf, dwSize);
						}
					}
				}
			}
			else// 本地swf路径
			{
				Play(m_strUrl);
			}
			iErr = DM_ECODE_OK;
		} while (false);
		return iErr;
	}

	typedef struct _FLASH_STREAM_HEADER
	{
		DWORD m_dwSignature;
		DWORD m_dwDataSize;
	} FLASH_STREAM_HEADER, *PFLASH_STREAM_HEADER;
	bool DUIFlash::Play(void* pBuf, DWORD dwSize)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pBuf||0 == dwSize)
			{
				break;
			}

			DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> pFlash = Ptr();
			if (!pFlash)
			{
				break;
			}

			IStream *spStream = NULL;
			if (S_OK != ::CreateStreamOnHGlobal(NULL, TRUE, &spStream))
			{
				break;
			}
			FLASH_STREAM_HEADER fsh = {0x55665566, dwSize};
			ULARGE_INTEGER uli = {sizeof(fsh) + dwSize};
			if (S_OK != spStream->SetSize(uli))// 设置大小
			{
				break;
			}

			// 写flash头
			if (S_OK != spStream->Write(&fsh, sizeof (fsh), NULL))
			{
				break;
			}

			if (S_OK != spStream->Write(reinterpret_cast<void*>(pBuf), dwSize, NULL))
			{
				break;
			}

			// 设置起始处播放
			uli.QuadPart = 0;
			spStream->Seek(*reinterpret_cast<PLARGE_INTEGER>(&uli), STREAM_SEEK_SET, NULL);
			m_pAxContainer->InitControl(spStream);
			bRet = true;
		} while (false);
		return bRet;
	}

	bool DUIFlash::Play(const LPCWSTR pszUrl)
	{
		bool bRet = false;
		do 
		{
			if (NULL == pszUrl||0 == wcslen(pszUrl))
			{
				break;
			}
			DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> pFlash = Ptr();
			if (!pFlash)
			{
				break;
			}

			if (L'.' == pszUrl[0])// 相对路径
			{
				wchar_t szPath[MAX_PATH] = {0};
				GetRootDirW(szPath, MAX_PATH);
				PathCombineW(szPath,szPath, pszUrl);
				m_strUrl = szPath;
			}
			else
			{
				m_strUrl = pszUrl;
			}
			bRet = SUCCEEDED(pFlash->put_Movie(bstr_t(m_strUrl)));
		} while (false);
		return bRet;
	}

	DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> DUIFlash::Ptr()
	{
		DMComQIPtr<ShockwaveFlashObjects::IShockwaveFlash> ptr;
		if (m_pAxContainer)
		{
			ptr = m_pAxContainer->ActiveXControl();
		}
		return ptr;
	}

	void DUIFlash::OnShowWindow(BOOL bShow, UINT nStatus)
	{
		SetMsgHandled(FALSE);
		DMComPtr<ShockwaveFlashObjects::IShockwaveFlash> pFlash = Ptr();
		if (pFlash)
		{
			if (!bShow)
			{
				if (m_pAxContainer->ActiveXControl())
				{
					if (pFlash->IsPlaying())
					{
						m_curFrame = pFlash->CurrentFrame();
						pFlash->StopPlay();	
					}
				}
			}
			else
			{
				if (m_pAxContainer->ActiveXControl())
				{
					if (FALSE == pFlash->IsPlaying())
					{

						pFlash->GotoFrame(m_curFrame);
						pFlash->Play();	
					}
				}
			}
		}
	}

	DMCode DUIFlash::OnAttrUrl(LPCWSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pszValue||0 == wcslen(pszValue))
			{
				break;
			}

			if (L'.' == pszValue[0])
			{
				wchar_t szPath[MAX_PATH] = {0};
				GetRootDirW(szPath, MAX_PATH);
				PathCombineW(szPath,szPath, pszValue);
				m_strUrl = szPath;
			}
			else
			{
				m_strUrl = pszValue;
			}

			if(!bLoadXml)
			{
				Play(m_strUrl);
			}
			iErr =  DM_ECODE_OK;
		} while (false);
		return iErr;
	}
}//namespace DM