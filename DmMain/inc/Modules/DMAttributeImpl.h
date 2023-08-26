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
		DMDECLARE_CLASS_NAME(DMAttributeImpl,"DMAttributeImpl",DMREG_Attribute);
	public:
		virtual DMCode ParseInt(LPCSTR lpszValue,int&Obj);
		virtual DMCode ParseBool(LPCSTR lpszValue,bool&Obj);
		virtual DMCode ParseColor(LPCSTR lpszValue,DMColor&Obj);
		virtual DMCode ParseSize(LPCSTR lpszValue,DM::CSize&Obj);
		virtual DMCode ParsePoint(LPCSTR lpszValue,DM::CPoint&Obj);
		virtual DMCode ParseRect(LPCSTR lpszValue,DM::CRect&Obj);
	};

}//namespace DM