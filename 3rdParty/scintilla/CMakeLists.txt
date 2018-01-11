#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-10-14	1.0		
#-------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
PROJECT(scintilla)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB INC_SOURCE 									inc/*.*)
FILE(GLOB SRC_SOURCE 									src/*.*)
FILE(GLOB LEXERS_SOURCE 								lexers/*.*)
FILE(GLOB LEXLIB_SOURCE 								lexlib/*.*)
FILE(GLOB WIN32_SOURCE 									win32/*.*)

SOURCE_GROUP("inc" 								FILES 	${INC_SOURCE})
SOURCE_GROUP("src" 								FILES 	${SRC_SOURCE})
SOURCE_GROUP("lexers" 							FILES 	${LEXERS_SOURCE})
SOURCE_GROUP("lexlib" 							FILES 	${LEXLIB_SOURCE})
SOURCE_GROUP("win32" 							FILES 	${WIN32_SOURCE})

# 增加头文件搜索路径
INCLUDE_DIRECTORIES(inc;
					src;				
					lexers;
					lexlib;
					win32;)
					
# 输出设置
add_definitions(-DUNICODE -D_UNICODE -DSTATIC_BUILD -DSCI_LEXER -DNO_CXX11_REGEX -D_CRT_SECURE_NO_WARNINGS)  # unicode版本

set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.
add_compile_options(/MP /Gm-)
# 输出设置	
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4819")
if(USE_ALLMT_)				
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_LIBRARY(scintilla STATIC 
			${INC_SOURCE};
			${SRC_SOURCE};
			${LEXERS_SOURCE};
			${LEXLIB_SOURCE};
			${WIN32_SOURCE};
			)
SET_TARGET_PROPERTIES(scintilla PROPERTIES OUTPUT_NAME "scintilla")
SET_TARGET_PROPERTIES(scintilla PROPERTIES FOLDER 3rdParty)




