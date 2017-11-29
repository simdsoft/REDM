#include "DmMainAfx.h"
#include "DUIFontPool.h"

namespace DM
{
	DUIFontPool::DUIFontPool()
	{
		wcscpy_s(m_szDefFontFace, L"宋体");
		::GetObjectW(::GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONTW), &m_lfDefault);
		m_lfDefault.lfHeight = -14;// 负值
		wcscpy_s(m_lfDefault.lfFaceName, countof(m_lfDefault.lfFaceName), m_szDefFontFace);
	}

	DUIFontPool::~DUIFontPool()
	{
		RemoveAll();
	}

	// font="face:宋体,size:0,weight:400,charset:0,underline:1,italic:1,strike:1",face:、weight:后多位，其余:后限制1位
	bool DUIFontPool::SetDefaultFont(const CStringW& strFont)
	{
		bool bRet = false;
		do 
		{
			LOGFONTW lf = {0};
			if (false == GetLogFont(strFont,&lf))
			{
				break;
			}
			memcpy(&m_lfDefault, &lf, sizeof(LOGFONTW));
			wcscpy_s(m_szDefFontFace, countof(m_lfDefault.lfFaceName), m_lfDefault.lfFaceName);
			bRet = true;
		} while (false);
		return bRet;
	}

	IDMFontPtr DUIFontPool::GetFont(const CStringW& strFont)
	{
		IDMFontPtr pFont = NULL;
		do 
		{
			// 解析并创建
			LOGFONTW lf={0};
			CStringW szFont = strFont;
			szFont.Trim();
			szFont.Remove(VK_SPACE);// 移除空格
			szFont.MakeLower();
			if (false == GetLogFont(szFont,&lf))
			{
				if (GetObjByKey(szFont,pFont))
				{
					break;// 已找到
				}
				g_pDMRender->CreateFont(&m_lfDefault,&pFont); // 默认字体
				AddKey(szFont, pFont);
				break;// 已找到
			}
			
			CStringW szKey  = GetFontKey(&lf); // 生成标准key
			if (GetObjByKey(szKey,pFont))
			{
				break;// 已找到
			}

			g_pDMRender->CreateFont(&lf,&pFont);
			if (pFont)
			{
				AddKey(szKey, pFont);// 加入标准key
			}
		} while (false);
		return pFont;
	}

	CStringW DUIFontPool::GetFontKey(const LPLOGFONTW lpLogFont)
	{
		CStringW szKey;
		CStringW strFaceName = lpLogFont->lfFaceName;strFaceName.MakeLower();
		szKey.Format(L"weight:%d,charset:%d,underline:%d,italic:%d,strike:%d,size:%d,face:%s",lpLogFont->lfWeight,lpLogFont->lfCharSet,lpLogFont->lfUnderline,lpLogFont->lfItalic,lpLogFont->lfStrikeOut,lpLogFont->lfHeight,strFaceName);
		return szKey;
	}

	bool DUIFontPool::GetLogFont(const CStringW& strFont,LPLOGFONTW lpLogFont)
	{
		bool bRet = false;
		do 
		{
			CStringW szFont = strFont;
			szFont.Trim();
			szFont.Remove(VK_SPACE);// 移除空格
			szFont.MakeLower();
			if (szFont.IsEmpty()||NULL==lpLogFont)// 为NULL
			{
				break;
			}

			// 解析并创建
			bool bItalic=0,bUnderline=0,bStrike=0;   
			CStringW strFace = L""; int lFontSize=0;int lWightSize=0;int lCharSet=0;

			//------------------------------------------
			int nPosBegin = szFont.Find(L"face:");                     
			if(nPosBegin!=-1)                                         
			{                                                         
				nPosBegin+=5;                                             
				int nPosEnd=szFont.Find(L";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(L",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				strFace=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
			} 
			else
			{
				strFace = m_lfDefault.lfFaceName; 
			}

			//------------------------------------------
			nPosBegin=szFont.Find(L"underline:");                    
			if(nPosBegin!=-1)                                         
			{                                                         
				bUnderline=szFont.Mid(nPosBegin+10,1)!=L"0";             
			}
			else
			{
				bUnderline=!!m_lfDefault.lfUnderline;
			}

			//------------------------------------------
			nPosBegin=szFont.Find(L"italic:");                       
			if(nPosBegin!=-1)                                         
			{                                                         
				bItalic=szFont.Mid(nPosBegin+7,1)!=L"0";                 
			}
			else
			{
				bItalic=!!m_lfDefault.lfItalic;
			}

			//------------------------------------------
			nPosBegin=szFont.Find(L"strike:");                       
			if(nPosBegin!=-1)                                         
			{                                                         
				bStrike=szFont.Mid(nPosBegin+7,1)!=L"0";                 
			}
			else
			{
				bStrike=!!m_lfDefault.lfStrikeOut;
			}

			//------------------------------------------
			nPosBegin=szFont.Find(L"size:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=5;                                             
				int nPosEnd=szFont.Find(L";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(L",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringW strFontSize=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lFontSize = DMABS(m_lfDefault.lfHeight);
				DMAttributeDispatch::ParseInt(strFontSize,lFontSize);
			}
			else
			{
				lFontSize = DMABS(m_lfDefault.lfHeight);
			}

			//------------------------------------------
			nPosBegin=szFont.Find(L"weight:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=7;                                             
				int nPosEnd=szFont.Find(L";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(L",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringW strWeightSize=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lWightSize = DMABS(m_lfDefault.lfWeight);
				DMAttributeDispatch::ParseInt(strWeightSize,lWightSize);
			}
			else
			{
				lWightSize = DMABS(m_lfDefault.lfWeight);
			}
			//------------------------------------------
			nPosBegin=szFont.Find(L"charset:");
			if (nPosBegin!=-1)
			{
				nPosBegin+=8;  
				int nPosEnd=szFont.Find(L";",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.Find(L",",nPosBegin);
				if(nPosEnd==-1) nPosEnd=szFont.GetLength();
				CStringW strCharSet=szFont.Mid(nPosBegin,nPosEnd-nPosBegin);
				lCharSet = DMABS(m_lfDefault.lfCharSet);
				DMAttributeDispatch::ParseInt(strCharSet,lCharSet);
			}
			else
			{
				lCharSet = DMABS(m_lfDefault.lfCharSet);
			}

			//------------------------------------------
			memcpy(lpLogFont, &m_lfDefault, sizeof(LOGFONTW));
			if (0 != lFontSize)
			{
				lpLogFont->lfHeight = -DMABS(lFontSize);// 负值
			}
			if (0 != lWightSize)
			{
				lpLogFont->lfWeight  = DMABS(lWightSize);
			}

			lpLogFont->lfCharSet	 = DMABS(lCharSet);// charset可为0
			lpLogFont->lfUnderline   = (false != bUnderline);
			lpLogFont->lfItalic      = (false != bItalic);
			lpLogFont->lfStrikeOut   = (false != bStrike);
			lpLogFont->lfQuality	 = CLEARTYPE_NATURAL_QUALITY;
			if (!strFace.IsEmpty())
			{
				wcscpy_s(lpLogFont->lfFaceName,countof(lpLogFont->lfFaceName),strFace);
			}
			bRet = true;
		} while (false);
		return bRet;
	}

	/// @brief 预处理移除项
	/// @return
	void DUIFontPool::PreMapKeyRemove(const IDMFontPtr &obj)
	{
		obj->Release(); // 如引用计数为0，则自动delete this
	}

}//namespace DM