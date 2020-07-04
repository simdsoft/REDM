#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-6	1.0		
#-------------------------------------------------------
PROJECT(DMDemo) 

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
FILE(GLOB ROOT_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp;src/*.c)

SOURCE_GROUP("inc" 		FILES 	${ROOT_HEADER})
SOURCE_GROUP("src"		FILES 	${ROOT_SOURCE})
  
 # 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc)

# 引用DmMain的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/DmMain/inc
					${DM_SOURCE_DIR}/DmMain/inc/Common;
					${DM_SOURCE_DIR}/DmMain/inc/Common/Template;
					${DM_SOURCE_DIR}/DmMain/inc/Core;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Msg;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Dui;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Event;
					${DM_SOURCE_DIR}/DmMain/inc/IDmMain;
					${DM_SOURCE_DIR}/DmMain/inc/Modules;
					${DM_SOURCE_DIR}/DmMain/inc/Widgets;
                    ${DM_SOURCE_DIR}/External;)
					
# 引用Plugin_Expand的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/PlugIns/Plugin_Expand/inc)
  
# 输出设置
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)  # unicode版本
ADD_EXECUTABLE(DMDemo  WIN32 ${ROOT_HEADER};${ROOT_SOURCE})
#if(USE_DMDLL_)
#else
#	set(EXTRA_LIBS rich32.lib)# 增加一个隐式lib
#   target_link_libraries(DMDemo ${EXTRA_LIBS})
#endif()
SET_TARGET_PROPERTIES(DMDemo PROPERTIES OUTPUT_NAME "DMDemo")

# 增加项目依赖项
add_dependencies(DMDemo  DmMain)
if(USE_DMDLL_)
add_dependencies(DMDemo  Plugin_Expand)
endif()

#增加事件后命令
add_custom_command(TARGET DMDemo 
					PRE_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PreBuild.cmd $(TargetDir)
					COMMENT "This command will be executed before build target dmdemo"
					)
add_custom_command(TARGET DMDemo 
					POST_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PostBuild.cmd $(TargetDir)
					COMMENT "This command will be executed after build target dmdemo"
					)
 
 SET_TARGET_PROPERTIES(DMDemo PROPERTIES FOLDER Samples)
 
 # 增加预编译头
use_precompiled_header(DMDemo
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/DMDemoAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/DMDemoAfx.cpp"
  )
