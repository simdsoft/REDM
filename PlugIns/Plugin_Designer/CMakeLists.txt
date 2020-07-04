#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-11-13	1.0		
#-------------------------------------------------------
PROJECT(Plugin_Designer)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 							inc/*.h)
FILE(GLOB ROOT_SOURCE 							src/*.cpp;src/*.c)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})

  
# 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc;)

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
					
# 引用Plugin_Designer的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/PlugIns/Plugin_Designer/inc)

# 输出设置
add_definitions(-DUNICODE -D_UNICODE)  # unicode版本
set(CMAKE_DEBUG_POSTFIX "_d")		   # Debug下生成文件增加后缀,对exe无效.

# 设置输出路径在Plugins/分支下
set(LIBRARY_OUTPUT_PATH  ${DM_BINARY_DIR}/bin/plugins)
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_LIBRARY(Plugin_Designer SHARED 
			${ROOT_HEADER};
			${ROOT_SOURCE};
			)

# 设置输出路径在Plugins/分支下
set_target_properties(Plugin_Designer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Designer PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${DM_BINARY_DIR}/bin/plugins)
#set_target_properties(Plugin_Designer PROPERTIES LINKER_LANGUAGE CXX)#解决CMake can not determine linker language for target

# 增加项目依赖项
add_dependencies(Plugin_Designer  DmMain)

SET_TARGET_PROPERTIES(Plugin_Designer PROPERTIES FOLDER PlugIns)

 # 增加预编译头
use_precompiled_header(Plugin_Designer
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/Plugin_DesignerAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin_DesignerAfx.cpp"
  )
