#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-7-6	1.0		
#-------------------------------------------------------
PROJECT(DMDesigner) 

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT		 									./*.h;./*.rc;)
FILE(GLOB ROOT_HEADER 									inc/*.h)
FILE(GLOB COMMON_HEADER     							inc/Common/*.h)
FILE(GLOB CORE_HEADER       							inc/Core/*.h)
FILE(GLOB EDITOR_HEADER       							inc/Editor/*.h)
FILE(GLOB MODULES_HEADER    							inc/Modules/*.h)
FILE(GLOB WIDGETS_HEADER    							inc/Widgets/*.h)

FILE(GLOB ROOT_SOURCE 									src/*.cpp;src/*.c)
FILE(GLOB COMMON_SOURCE     							src/Common/*.cpp;src/Common/*.c)
FILE(GLOB CORE_SOURCE       							src/Core/*.cpp;src/Core/*.c)
FILE(GLOB EDITOR_SOURCE       							src/Editor/*.cpp;src/Editor/*.c)
FILE(GLOB MODULES_SOURCE    							src/Modules/*.cpp;src/Modules/*.c)
FILE(GLOB WIDGETS_SOURCE    							src/Widgets/*.cpp;src/Widgets/*.c)

SOURCE_GROUP("other"							FILES 	${ROOT})
SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Common" 						FILES 	${COMMON_HEADER})
SOURCE_GROUP("inc\\Core" 						FILES 	${CORE_HEADER})#必须用\\,不能用/
SOURCE_GROUP("inc\\Editor" 						FILES 	${EDITOR_HEADER})
SOURCE_GROUP("inc\\Modules" 					FILES 	${MODULES_HEADER})
SOURCE_GROUP("inc\\Widgets" 					FILES 	${WIDGETS_HEADER})

SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Common" 						FILES 	${COMMON_SOURCE})
SOURCE_GROUP("src\\Core"						FILES 	${CORE_SOURCE})
SOURCE_GROUP("src\\Editor"						FILES 	${EDITOR_SOURCE})
SOURCE_GROUP("src\\Modules"						FILES 	${MODULES_SOURCE})
SOURCE_GROUP("src\\Widgets"						FILES 	${WIDGETS_SOURCE})

  
 # 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc;
					${CMAKE_CURRENT_SOURCE_DIR}/inc/Common;
					${CMAKE_CURRENT_SOURCE_DIR}/inc/Core;
					${CMAKE_CURRENT_SOURCE_DIR}/inc/Editor;
					${CMAKE_CURRENT_SOURCE_DIR}/inc/Modules;
					${CMAKE_CURRENT_SOURCE_DIR}/inc/Widgets;)

# 引用DmMain的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/DmMain/inc
					${DM_SOURCE_DIR}/DmMain/inc/Common;
					${DM_SOURCE_DIR}/DmMain/inc/Common/Template;
					${DM_SOURCE_DIR}/DmMain/inc/Common/Plugins;
					${DM_SOURCE_DIR}/DmMain/inc/Core;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Msg;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Dui;
					${DM_SOURCE_DIR}/DmMain/inc/Core/Event;
					${DM_SOURCE_DIR}/DmMain/inc/IDmMain;
					${DM_SOURCE_DIR}/DmMain/inc/Modules;
					${DM_SOURCE_DIR}/DmMain/inc/Widgets;
                    ${DM_SOURCE_DIR}/External;)
					
# 引用scintilla的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/3rdParty/scintilla/inc)
					
# 引用Plugin_Expand的头文件
INCLUDE_DIRECTORIES(${DM_SOURCE_DIR}/PlugIns/Plugin_Designer/inc)
  
# 输出设置
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)  # unicode版本
ADD_EXECUTABLE(DMDesigner  WIN32
			   ${ROOT};
			   ${ROOT_HEADER};
			   ${ROOT_SOURCE};
			   ${COMMON_HEADER};
			   ${COMMON_SOURCE};
			   ${CORE_HEADER};
			   ${CORE_SOURCE};
			   ${EDITOR_HEADER};
			   ${EDITOR_SOURCE};
			   ${MODULES_HEADER};
			   ${MODULES_SOURCE};
			   ${WIDGETS_HEADER};
			   ${WIDGETS_SOURCE};
			   )
SET_TARGET_PROPERTIES(DMDesigner PROPERTIES OUTPUT_NAME "DMDesigner")

# 增加项目依赖项
add_dependencies(DMDesigner  DmMain)
if(USE_DMDLL_)
add_dependencies(DMDesigner  Plugin_Designer)
endif()
add_dependencies(DMDesigner  scintilla)

#增加事件后命令
add_custom_command(TARGET DMDesigner 
					PRE_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PreBuild-Designer.cmd $(TargetDir)
					COMMENT "This command will be executed before build target DMDesigner"
					)
add_custom_command(TARGET DMDesigner 
					POST_BUILD 
					COMMAND call ${DM_SOURCE_DIR}/Tools/CMake/PostBuild.cmd $(TargetDir)
					COMMENT "This command will be executed after build target DMDesigner"
					)
SET_TARGET_PROPERTIES(DMDesigner PROPERTIES FOLDER Samples)

# 增加预编译头
use_precompiled_header(DMDesigner
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/DMDesignerAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/DMDesignerAfx.cpp"
  )
 