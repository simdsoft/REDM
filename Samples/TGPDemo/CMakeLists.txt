#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-4-20	1.0		
#-------------------------------------------------------
PROJECT(TGPDemo) 

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT		 		./*.h;./*.rc;)
FILE(GLOB ROOT_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
FILE(GLOB ROOT_RES_HEADER 	${CMAKE_CURRENT_SOURCE_DIR}/inc/Res/*.h)
FILE(GLOB ROOT_MENU_HEADER 	${CMAKE_CURRENT_SOURCE_DIR}/inc/Menu/*.h)

FILE(GLOB ROOT_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp;src/*.c)
FILE(GLOB ROOT_RES_SOURCE 	${CMAKE_CURRENT_SOURCE_DIR}/src/Res/*.cpp;src/Res/*.c)
FILE(GLOB ROOT_MENU_SOURCE 	${CMAKE_CURRENT_SOURCE_DIR}/src/Menu/*.cpp;src/Menu/*.c)

SOURCE_GROUP("other"			FILES 	${ROOT})
SOURCE_GROUP("inc" 				FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Res" 		FILES 	${ROOT_RES_HEADER})
SOURCE_GROUP("inc\\Menu" 		FILES 	${ROOT_MENU_HEADER})
SOURCE_GROUP("src"				FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Res"			FILES 	${ROOT_RES_SOURCE})
SOURCE_GROUP("src\\Menu" 		FILES 	${ROOT_MENU_SOURCE})

  
 # 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc;${CMAKE_CURRENT_SOURCE_DIR}/;${CMAKE_CURRENT_SOURCE_DIR}/inc/Res;${CMAKE_CURRENT_SOURCE_DIR}/inc/Menu;)

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
					
# 引用lua的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/3rdParty/lua
					${DM_SOURCE_DIR}/3rdParty/lua/wrapper)	
					
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
ADD_EXECUTABLE(TGPDemo  WIN32 ${ROOT};${ROOT_HEADER};${ROOT_SOURCE};${ROOT_RES_HEADER};${ROOT_RES_SOURCE};${ROOT_MENU_HEADER};${ROOT_MENU_SOURCE})
#if(USE_DMDLL_)
#else
#	set(EXTRA_LIBS rich32.lib)# 增加一个隐式lib
#   target_link_libraries(TGPDemo ${EXTRA_LIBS})
#endif()
SET_TARGET_PROPERTIES(TGPDemo PROPERTIES OUTPUT_NAME "TGPDemo")

# 增加项目依赖项
add_dependencies(TGPDemo  DmMain)
add_dependencies(TGPDemo  lua)
if(USE_DMDLL_)
add_dependencies(TGPDemo  Plugin_Expand)
endif()

#增加事件后命令
add_custom_command(TARGET TGPDemo 
					PRE_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PreBuild-TGPDemo.cmd $(TargetDir)
					COMMENT "This command will be executed before build target TGPDemo"
					)
add_custom_command(TARGET TGPDemo 
					POST_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PostBuild.cmd $(TargetDir)
					COMMENT "This command will be executed after build target TGPDemo"
					)
SET_TARGET_PROPERTIES(TGPDemo PROPERTIES FOLDER Samples)

# 增加预编译头
use_precompiled_header(TGPDemo
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/TGPDemoAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/TGPDemoAfx.cpp"
  )
