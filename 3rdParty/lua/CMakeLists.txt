#-------------------------------------------------------
# Copyright (c) DMLUA
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2016-3-21	1.0		
#-------------------------------------------------------
PROJECT(lua)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 							*.h)
FILE(GLOB ROOT_SOURCE 							*.cpp;*.c)
FILE(GLOB WRAPPER_SOURCE 						wrapper/*.h;wrapper/*.cpp;wrapper/*.c)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("wrapper"							FILES 	${WRAPPER_SOURCE})

 # 增加头文件搜索路径
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR}/;
					${CMAKE_CURRENT_SOURCE_DIR}/wrapper;)


set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.

# 输出设置
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /wd4996")
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
ADD_LIBRARY(lua STATIC ${ROOT_HEADER};${ROOT_SOURCE};${WRAPPER_SOURCE})
SET_TARGET_PROPERTIES(lua PROPERTIES OUTPUT_NAME "lua")
SET_TARGET_PROPERTIES(lua PROPERTIES FOLDER 3rdParty)

