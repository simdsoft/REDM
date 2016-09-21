//-------------------------------------------------------
// Copyright (c) DuiMagic
// All rights reserved.
// 
// File Name: DMError.h 
// File Des: 错误码
// File Summary: 命名规则:
// 如类名为ANAME，则归于enum ANAME_ERROR_CODE枚举中
// 如函数名为funNAME,则归于ANAME_FUNNAME_XX_ERR,XX表示具体原因
// Cur Version: 1.0
// Author:
// Create Data:
// History:
// 		<Author>	<Time>		<Version>	  <Des>
//      guoyou		2015-1-7	1.0			
//-------------------------------------------------------
#pragma once

/// <summary>
///		错误码列表
/// </summary>
namespace DM
{
	#define DMSUCCEEDED(dmcode)   (0 == (int)(dmcode))
	enum DM_ERROR_CODE///通用错误码ENUM
	{
		DM_ECODE_OK = 0,											///< 通用的成功	
		DM_ECODE_FAIL = 1,											///< 通用的失败,在xml解析中表示未处理xml	
		DM_ECODE_NOXMLLOADREFRESH = 2,                              ///< 非xml方式加载时需要刷新自身
		DM_ECODE_NOXMLLOADPARENTREFRESH,							///< 非xml方式加载时需要刷新父窗口（如tabpage）
		DM_ECODE_NOXMLRELAYOUT,										///< 非xml方式加载时重新布局
		DM_ECODE_NOTIMPL,                                           ///< 未实现接口
		DM_ECODE_SMALLBUF,                                          ///< BUF太小
		DM_ECODE_GETROOTDIR_FAIL,									///< 获取exe根目录失败
		DM_ECODE_PATHCOMBINE_FAIL,                                  ///< PathCombineW失败
		DM_ECODE_NOLOOP,                                            ///< 结束循环
		DM_ECODE_END  = 100,                                        ///< 通用的错识码结束
	};

	enum DMDATABASE_ERROR_CODE///XML错误码ENUM
	{
		// LoadDMData
		DMDATABASE_LOADDMDATA_PARAM_INVALID = 200,					///< 参数非法

	};

	enum DMAPPDATA_ERROR_CODE
	{
		DMAPPDATA_BASE  = 300,
		// Run
		DMAPP_RUN_ADDMESSAGELOOP_FAIL,							    ///< 加入消息循环列表失败
	};

	enum DMREGMGR_ERROR_CODE///REG错误码ENUM
	{
		// RegisterByType
		DMREGMGR_REGISTERBYTYPE_REG_EXIST = 400,					///< 注册已存在

		// CreateRegObjByType
		DMREGMGR_CREATEREGOBJBYTYPE_PARAM_INVALID,                  ///< 传入参数非法
		DMREGMGR_CREATEREGOBJBYTYPE_COUNT_ISZERO,                   ///< 注册数为0

		// SetDefRegObjByType
		DMREGMGR_SETDEFREGOBJBYTYPE_DEFREGNAME_SAME,                ///< SetDefRegObj和原来的一致
	};

	enum DMPLUGINSTOOL_ERROR_CODE///插件错误码
	{
		// LoadPlugins
		DMPLUGINSTOOL_LOADPLUGINS_CFGFILE_NOTEXIST,                 ///< CFG文件不存在 

		// LoadPlugin
		DMPLUGINSTOOL_LOADPLUGIN_LOAD_FAIL,                         ///< dll加载失败
		DMPLUGINSTOOL_LOADPLUGIN_ALREADY_EXIST,                     ///< dll已被加载过
		DMPLUGINSTOOL_LOADPLUGIN_NOFIND_START_EXPORT,               ///< dll没有导出dllStartPlugin
	};
}
