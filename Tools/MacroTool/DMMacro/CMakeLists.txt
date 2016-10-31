PROJECT(DMMacro) 

FILE(GLOB ROOT_HEADER 		${CMAKE_CURRENT_SOURCE_DIR}/inc/*.h)
FILE(GLOB ROOT_SOURCE 		${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp;src/*.c)

SOURCE_GROUP("inc" 				FILES 	${ROOT_HEADER})
SOURCE_GROUP("src"				FILES 	${ROOT_SOURCE})

use_precompiled_header(DMMacro
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/StdAfx.h"
   "${CMAKE_CURRENT_SOURCE_DIR}/src/StdAfx.cpp"
  )
  
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/inc
					${CMAKE_CURRENT_SOURCE_DIR}/)

INCLUDE_DIRECTORIES(${MacroTool_SOURCE_DIR}/DmMain/inc
					${MacroTool_SOURCE_DIR}/DmMain/inc/Common;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Common/Template;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Core;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Core/Msg;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Core/Dui;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Core/Event;
					${MacroTool_SOURCE_DIR}/DmMain/inc/IDmMain;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Modules;
					${MacroTool_SOURCE_DIR}/DmMain/inc/Widgets;)
					
				
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
ADD_DEFINITIONS(-DUNICODE -D_UNICODE)  # unicode�汾
ADD_EXECUTABLE(DMMacro  WIN32 ${ROOT};${ROOT_HEADER};${ROOT_SOURCE};)
SET_TARGET_PROPERTIES(DMMacro PROPERTIES OUTPUT_NAME "DMMacro")

add_dependencies(DMMacro  DmMain)

