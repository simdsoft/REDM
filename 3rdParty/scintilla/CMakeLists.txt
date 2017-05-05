#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-10-14	1.0		
#-------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
PROJECT(scintilla)

# �ļ����飨���ˣ���ͬʱ������뵽ADD_LIBRARY�в�����ʾ
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

# ����ͷ�ļ�����·��
INCLUDE_DIRECTORIES(inc;
					src;				
					lexers;
					lexlib;
					win32;)
					
# �������
add_definitions(-DUNICODE -D_UNICODE -DSTATIC_BUILD -DSCI_LEXER -DNO_CXX11_REGEX)  # unicode�汾

set(CMAKE_DEBUG_POSTFIX "_d")# Debug�������ļ����Ӻ�׺,��exe��Ч.

# �������	
if(USE_ALLMT_)				
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) 						# disable CRT
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996 /wd4819")
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# ֱ�ӳ�gui��Ĭ�ϣ�Ȼ�� �ĳ�mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
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



