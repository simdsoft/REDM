﻿#include "DmMainAfx.h"

#if !defined(DM_EXCLUDE_ACTIVEX)

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

			CStringA strType;
			CStringA strResName;
			CStringA strSource = DMW2A(m_strUrl, CP_UTF8);
			int iFind = strSource.ReverseFind((':'));
			if (-1 != iFind)
			{
				strType    = strSource.Left(iFind);
				strResName = strSource.Right(strSource.GetLength()-iFind-1);
				if (-1!=strType.MakeLower().Find("http")||-1!=strType.MakeLower().Find("ftp"))// http在线url
				{
					Play(m_strUrl);
				}
				else// 本地资源包,使用内存方式播放
				{
					DMBufT<byte> pBuf;
					ULONG ulSize = 0;
					if (DMSUCCEEDED(pRes->GetItemBuf(strType, strResName, pBuf, &ulSize)))
					{
						Play(pBuf, ulSize);
					}
					else
					{
						DMBufT<byte> pBuf;
						DWORD dwSize = 0;
						if (DMSUCCEEDED(IDMRes::ReadFileBuf(m_strUrl, pBuf, &dwSize)))
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

	DMCode DUIFlash::OnAttrUrl(LPCSTR pszValue, bool bLoadXml)
	{
		DMCode iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == pszValue||0 == strlen(pszValue))
			{
				break;
			}

			CStringW wstrUrl = DMCA2W(pszValue, -1, CP_UTF8);
			if (L'.' == pszValue[0])
			{
				wchar_t szPath[MAX_PATH] = {0};
				GetRootDirW(szPath, MAX_PATH);
				PathCombineW(szPath,szPath, wstrUrl);
				m_strUrl = szPath;
			}
			else
			{
				m_strUrl = wstrUrl;
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

#endif
