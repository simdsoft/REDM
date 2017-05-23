#include "DmMainAfx.h"
#include "DMAttributeImpl.h"

namespace DM
{

	DMCode DMAttributeImpl::ParseInt(LPCWSTR lpszValue,int&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue||0 == wcslen(lpszValue))
			{
				break;
			}

			int iRet = 0;
			if (FALSE == ::StrToIntExW(lpszValue,STIF_SUPPORT_HEX,&iRet))
			{
				break;
			}
			Obj = iRet;
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;
	}

	DMCode DMAttributeImpl::ParseBool(LPCWSTR lpszValue,bool&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue|| 0 == wcslen(lpszValue))
			{
				break;
			}

			if (0 == _wcsicmp(lpszValue, L"yes")
				|| 0 == _wcsicmp(lpszValue, L"true"))
			{
				Obj = true;
				iErr = DM_ECODE_OK;
				break;
			}

			int iValue = 0;
			if (!DMSUCCEEDED(ParseInt(lpszValue,iValue)))
			{
				break;
			}
			Obj = (0!=iValue);
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;
	}

	// #ffffff或rgb(ff,ff,ff)或rgba(ff,ff,ff,ff)
	DMCode DMAttributeImpl::ParseColor(LPCWSTR lpszValue,DMColor&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue|| 0 == wcslen(lpszValue))
			{
				break;
			}

			if (0 == _wcsicmp(lpszValue, L"white"))//白色
			{
				Obj = 0xFFFFFFFF;
				iErr = DM_ECODE_OK;
				break;
			}

			int r = 0;
			int g = 0;
			int b = 0;
			int a = 255;

			int nSuccessNum = 0;
			if (L'#' == lpszValue[0])
			{
				nSuccessNum = swscanf_s(lpszValue,L"#%02x%02x%02x%02x",&r,&g,&b,&a);
				if (4!= nSuccessNum)//没有转换4个
				{
					break;
				}
			}
			else if (wcslen(lpszValue)>5)
			{
				if (0 == _wcsnicmp(lpszValue, L"rgb", 3))
				{
					if (L'a' == lpszValue[3]||L'A' == lpszValue[3])
					{
						nSuccessNum = swscanf_s(lpszValue,L"rgba(%02x,%02x,%02x,%02x)",&r,&g,&b,&a);
						if (4 != nSuccessNum)//没有转换4个
						{
							break;
						}
					}
					else
					{
						nSuccessNum = swscanf_s(lpszValue,L"rgb(%02x,%02x,%02x)",&r,&g,&b);
						if (3 != nSuccessNum)//没有转换3个
						{
							break;
						}
					}
				}
				else if (0 == _wcsnicmp(lpszValue, L"pbgra", 5))
				{
					nSuccessNum = swscanf_s(lpszValue,L"pbgra(%02x,%02x,%02x,%02x)",&b,&g,&r,&a);
					if (4 != nSuccessNum)//没有转换4个
					{
						break;
					}
				}
			}

			//Obj = (RGB(r,g,b)|(a<<24));
			Obj.SetRGBA(r,g,b,a);
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;
	}

	DMCode DMAttributeImpl::ParseSize(LPCWSTR lpszValue,DM::CSize&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue||0 == wcslen(lpszValue))
			{
				break;
			}

			int dx = 0;
			int dy = 0;
			int nSuccessNum = swscanf_s(lpszValue,L"%d,%d",&dx,&dy);
			if (2 != nSuccessNum)
			{
				break;
			}

			Obj.SetSize(dx,dy);
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;;
	}

	DMCode DMAttributeImpl::ParsePoint(LPCWSTR lpszValue,DM::CPoint&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue||0 == wcslen(lpszValue))
			{
				break;
			}

			int dx = 0;
			int dy = 0;
			int nSuccessNum = swscanf_s(lpszValue,L"%d,%d",&dx,&dy);
			if (2 != nSuccessNum)
			{
				break;
			}

			Obj.SetPoint(dx,dy);
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;;
	}

	DMCode DMAttributeImpl::ParseRect(LPCWSTR lpszValue,DM::CRect&Obj)
	{
		int iErr = DM_ECODE_FAIL;
		do 
		{
			if (NULL == lpszValue||0 == wcslen(lpszValue))
			{
				break;
			}
			int left	= 0;
			int top		= 0;
			int right	= 0;
			int bottom  = 0;
			int nSuccessNum = swscanf_s(lpszValue,L"%d,%d,%d,%d",&left,&top,&right,&bottom);
			if (4 != nSuccessNum)
			{
				break;
			}

			Obj.SetRect(left,top,right,bottom);
			iErr = DM_ECODE_OK;
		} while (false);

		return iErr;
	}

}//namespace DM