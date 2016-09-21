//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMAttributeImpl.h 
// File Des: 
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

namespace DM
{
	class DMAttributeImpl:public IDMAttribute
	{
		DMDECLARE_CLASS_NAME(DMAttributeImpl,L"DMAttributeImpl",DMREG_Attribute);
	public:
		virtual DMCode ParseInt(LPCWSTR lpszValue,int&Obj);				
		virtual DMCode ParseBool(LPCWSTR lpszValue,bool&Obj);			
		virtual DMCode ParseColor(LPCWSTR lpszValue,DMColor&Obj);		
		virtual DMCode ParseSize(LPCWSTR lpszValue,DM::CSize&Obj);		
		virtual DMCode ParsePoint(LPCWSTR lpszValue,DM::CPoint&Obj);	
		virtual DMCode ParseRect(LPCWSTR lpszValue,DM::CRect&Obj);		
	};

}//namespace DM