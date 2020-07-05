#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-6	1.0		
#-------------------------------------------------------
PROJECT(QQDemo) 

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT		 		./*.h;./*.rc;)
FILE(GLOB ROOT_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
FILE(GLOB SPY_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/spy/*.h)
FILE(GLOB SCRIPT_HEADER 	${CMAKE_CURRENT_SOURCE_DIR}/inc/script/*.h)
FILE(GLOB WIDGET_HEADER 	${CMAKE_CURRENT_SOURCE_DIR}/inc/widget/*.h)
FILE(GLOB ROOT_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp;src/*.c)
FILE(GLOB SPY_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/spy/*.cpp;src/spy/*.c;/src/spy/*.cur)
FILE(GLOB SCRIPT_SOURCE 	${CMAKE_CURRENT_SOURCE_DIR}/src/script/*.cpp;src/script/*.c)
FILE(GLOB WIDGET_SOURCE 	${CMAKE_CURRENT_SOURCE_DIR}/src/widget/*.cpp;src/widget/*.c;/src/widget/*.cur)

SOURCE_GROUP("other"			FILES 	${ROOT})
SOURCE_GROUP("inc" 				FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\spy" 		FILES 	${SPY_HEADER})
SOURCE_GROUP("inc\\script" 		FILES 	${SCRIPT_HEADER})
SOURCE_GROUP("inc\\widget" 		FILES 	${WIDGET_HEADER})
SOURCE_GROUP("src"				FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\spy"			FILES 	${SPY_SOURCE})
SOURCE_GROUP("src\\script"		FILES 	${SCRIPT_SOURCE})
SOURCE_GROUP("src\\widget"		FILES 	${WIDGET_SOURCE})

  
 # 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc;${CMAKE_CURRENT_SOURCE_DIR}/;${CMAKE_CURRENT_SOURCE_DIR}/inc/spy;${CMAKE_CURRENT_SOURCE_DIR}/inc/script;${CMAKE_CURRENT_SOURCE_DIR}/inc/widget;)

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
ADD_EXECUTABLE(QQDemo  WIN32 ${ROOT};${ROOT_HEADER};${SPY_HEADER};${SCRIPT_HEADER};${WIDGET_HEADER};${ROOT_SOURCE};${SPY_SOURCE};${SCRIPT_SOURCE};${WIDGET_SOURCE})
#if(USE_DMDLL_)
#else
#	set(EXTRA_LIBS rich32.lib)# 增加一个隐式lib
#   target_link_libraries(QQDemo ${EXTRA_LIBS})
#endif()
SET_TARGET_PROPERTIES(QQDemo PROPERTIES OUTPUT_NAME "QQDemo")

# 增加项目依赖项
add_dependencies(QQDemo  DmMain)
add_dependencies(QQDemo  lua)
if(USE_DMDLL_)
add_dependencies(QQDemo  Plugin_Expand)
endif()

#增加事件后命令
add_custom_command(TARGET QQDemo 
					PRE_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PreBuild-QQDemo.cmd $(TargetDir)
					COMMENT "This command will be executed before build target QQDemo"
					)
add_custom_command(TARGET QQDemo 
					POST_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PostBuild.cmd $(TargetDir)
					COMMENT "This command will be executed after build target QQDemo"
					)
SET_TARGET_PROPERTIES(QQDemo PROPERTIES FOLDER Samples)

# 增加预编译头
use_precompiled_header(QQDemo
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/QQDemoAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/QQDemoAfx.cpp"
  )
