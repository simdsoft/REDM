#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-3-15	1.0		
#-------------------------------------------------------
PROJECT(Plugin_Expand)

if(USE_DMSKIA_)
ADD_DEFINITIONS(-DUSE_DMSKIA_RENDER_)
endif()

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 							inc/*.h)
FILE(GLOB RES_HEADER 							inc/Res/*.h)
FILE(GLOB ANIMATE_HEADER 					    inc/Animate/*.h)
FILE(GLOB ANIMATE_3D_HEADER 					inc/Animate/3d/*.h)

FILE(GLOB ROOT_SOURCE 							src/*.cpp;src/*.c)
FILE(GLOB RES_SOURCE  							src/Res/*.cpp;src/Res/*.c)
FILE(GLOB ANIMATE_SOURCE  						src/Animate/*.cpp;src/Animate/*.c)
FILE(GLOB ANIMATE_3D_SOURCE  					src/Animate/3d/*.cpp;src/Animate/3d/*.c)

if(USE_DMSKIA_)
FILE(GLOB RENDER_HEADER    						inc/Render/*.h)
FILE(GLOB RENDER_SOURCE    						src/Render/*.cpp;src/Render/*.c)
endif()

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Res" 						FILES 	${RES_HEADER})
SOURCE_GROUP("inc\\Animate" 					FILES 	${ANIMATE_HEADER})
SOURCE_GROUP("inc\\Animate\\3d" 				FILES 	${ANIMATE_3D_HEADER})

SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Res"							FILES 	${RES_SOURCE})
SOURCE_GROUP("src\\Animate"						FILES 	${ANIMATE_SOURCE})
SOURCE_GROUP("src\\Animate\\3d"					FILES 	${ANIMATE_3D_SOURCE})

if(USE_DMSKIA_)
SOURCE_GROUP("inc\\Render" 						FILES 	${RENDER_HEADER})
SOURCE_GROUP("src\\Render"						FILES 	${RENDER_SOURCE})
endif()
  
  
# 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc;)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc/Animate;)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc/Animate/3d;)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc/Res;)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc/Render;)

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

# 引用ZLIB的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/3rdParty/zlib)

# 引用skia的头文件
if(USE_DMSKIA_)
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/3rdParty/skia;
					${DM_SOURCE_DIR}/3rdParty/skia/inc/config;
					${DM_SOURCE_DIR}/3rdParty/skia/inc;
					${DM_SOURCE_DIR}/3rdParty/skia/inc/core;
					)
endif()

# 输出设置
add_definitions(-DUNICODE -D_UNICODE)  # unicode版本

set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.

# 设置输出路径在Plugins/分支下
set(LIBRARY_OUTPUT_PATH  ${DM_BINARY_DIR}/bin/plugins)
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
if(USE_DMSKIA_)
ADD_LIBRARY(Plugin_Expand SHARED 
			${ROOT_HEADER};
			${RES_HEADER};
			${ANIMATE_HEADER};
			${ANIMATE_3D_HEADER};

			${RENDER_HEADER};
			${RENDER_SOURCE};
			
			${ROOT_SOURCE};
			${RES_SOURCE};
			${ANIMATE_SOURCE};
			${ANIMATE_3D_SOURCE};
			)
else()
ADD_LIBRARY(Plugin_Expand SHARED 
			${ROOT_HEADER};
			${RES_HEADER};
			${ANIMATE_HEADER};
			${ANIMATE_3D_HEADER};
			${ROOT_SOURCE};
			${RES_SOURCE};
			${ANIMATE_SOURCE};
			${ANIMATE_3D_SOURCE};
			)
endif()


# 设置输出路径在Plugins/分支下
set_target_properties(Plugin_Expand PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${DM_BINARY_DIR}/bin/plugins)
set_target_properties(Plugin_Expand PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO  ${DM_BINARY_DIR}/bin/plugins)
#set_target_properties(Plugin_Expand PROPERTIES LINKER_LANGUAGE CXX)#解决CMake can not determine linker language for target

# 增加项目依赖项
add_dependencies(Plugin_Expand  DmMain)
add_dependencies(Plugin_Expand  zlib)
if(USE_DMSKIA_)
TARGET_LINK_LIBRARIES(Plugin_Expand  Usp10.lib;Opengl32.lib)
endif()

SET_TARGET_PROPERTIES(Plugin_Expand PROPERTIES FOLDER PlugIns)

 # 增加预编译头
use_precompiled_header(Plugin_Expand
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/Plugin_ExpandAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/Plugin_ExpandAfx.cpp"
  )
