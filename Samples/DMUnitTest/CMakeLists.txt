#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-7	1.0		
#-------------------------------------------------------
PROJECT(DMUnitTest) 

# 支持简版if else
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) 

# 文件分组（过滤）
FILE(GLOB ROOT_HEADER 									./*.h)
FILE(GLOB ROOT_SOURCE 									./*.cpp)# 如果使用相对路径，一定要加.直接写在/*.cpp是不被识别的
FILE(GLOB TESTCASE_SOURCE     							./TestCase/*.cpp)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\TestCase" 					FILES 	${TESTCASE_SOURCE})

  
# 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/;
					${CMAKE_CURRENT_SOURCE_DIR}/TestCase;
					${CMAKE_CURRENT_SOURCE_DIR}/GtestInc;
					)
					
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
									
# 输出设置
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)  # unicode版本
ADD_EXECUTABLE(DMUnitTest  ${ROOT_HEADER};${ROOT_SOURCE};${TESTCASE_SOURCE};)
SET_TARGET_PROPERTIES(DMUnitTest PROPERTIES OUTPUT_NAME "DMUnitTest")

# 增加项目依赖项
add_dependencies(DMUnitTest  DmMain)
if(USE_DMDLL_)
add_dependencies(DMUnitTest  Plugin_Expand)
endif()
add_dependencies(DMUnitTest  lua)
add_dependencies(DMUnitTest  gtest)


#增加事件后命令
add_custom_command(TARGET DMUnitTest 
					PRE_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PreBuild-ut.cmd $(TargetDir)
					COMMENT "This command will be executed before build target DMUnitTest"
					)
add_custom_command(TARGET DMUnitTest 
					POST_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PostBuild.cmd $(TargetDir)
					COMMENT "This command will be executed after build target DMUnitTest"
					)


# 增加预编译头
use_precompiled_header(DMUnitTest
   "${CMAKE_CURRENT_SOURCE_DIR}/DMUintTestAfx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/DMUintTestAfx.cpp"
  )

