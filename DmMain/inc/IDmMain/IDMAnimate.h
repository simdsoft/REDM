//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: IDMAnimate.h 
// File Des:
// File Summary: 
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-3-10	1.0			
//-------------------------------------------------------
#pragma once
namespace DM
{
	class IDMAnimate;
	/// <summary>
	///		动画拥有者,classtype=<see cref="DMREG_Attribute"/>
	/// </summary>
	class IDMAnimateOwner
	{
	public:
		virtual DUIWindow* GetAnimateOwnerWnd() = 0;											///< 动画拥有者
		virtual DMCode AnimateBegin_Callback(IDMAnimate*pAni,WPARAM wp=0, LPARAM lp=0) = 0;		///< 动画开始时回调
		virtual DMCode AnimateMid_Callback(IDMAnimate*pAni, WPARAM wp=0, LPARAM lp=0) = 0;      ///< 动画中间回调		
		virtual DMCode AnimateEnd_Callback(IDMAnimate*pAni,WPARAM wp=0, LPARAM lp=0) = 0;		///< 动画结束回调
	};

	/// <summary>
	///		动画对外扩展接口,classtype=<see cref="DMREG_Animate"/>
	/// </summary>
	class IDMAnimate:public DMBase
	{
		DMDECLARE_CLASS_NAME(IDMAnimate,L"IDMAnimate",DMREG_Animate);
	public:
		virtual DMCode AnimateInit(IDMAnimateOwner*pOwner,IDMCanvas* pStartCanvas,IDMCanvas* pEndCanvas,WPARAM wp=0, LPARAM lp=0) = 0;   ///< 初始化动画
		virtual DMCode AnimateBegin(WPARAM wp=0, LPARAM lp=0) = 0;																		 ///< 开始动画
		virtual DMCode AnitmateGet(WPARAM wp=0, LPARAM lp=0) = 0;                                                                        ///< 从动画接口获取某些信息
		virtual DMCode AnimateEnd(WPARAM wp=0, LPARAM lp=0) = 0;																		 ///< 结束动画
	};
}//namespace DM