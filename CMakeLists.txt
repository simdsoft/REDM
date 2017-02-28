#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-5	1.0		
#-------------------------------------------------------
cmake_minimum_required(VERSION 2.8)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

# 工程名
PROJECT(DM)   
 
# 支持简版if else
set(CMAKE_ALLOW_LOOSE_LOOP_CONSTRUCTS true) 

# 使用相对路径
set(CMAKE_USE_RELATIVE_PATHS true)
set(CMAKE_SUPPRESS_REGENERATION true) 

IF(DEBUG_MODE)  
  ADD_DEFINITIONS(-D_DEBUG)  
ENDIF() 

# 定义工程的顶级路径,CMAKE_CURRENT_SOURCE_DIR:当前处理的CMakeLists.txt所在的路径
set(PROJDIR ${CMAKE_CURRENT_SOURCE_DIR})

# 设置输出路径,SET(EXECUTABLE_OUTPUT_PATH <新路径>)并不会对此变量有影响,只是改变了最终目标文件的存储路径
set(EXECUTABLE_OUTPUT_PATH ${DM_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH  ${DM_BINARY_DIR}/bin)

# 设置引用库路径
set(LIBS ${LIBRARY_OUTPUT_PATH};${DM_SOURCE_DIR}/3rdParty/lib)
LINK_DIRECTORIES(${LIBS})

# 开启DMMain默认为编译成dll 
OPTION(USE_DMDLL_ "Compile DmMain as dll"   ON) 
if(USE_DMDLL_)
# 给解决方案中所有工程增加宏定义DLL_DMMAIN
ADD_DEFINITIONS(-DDLL_DMMAIN)
endif()

# 开启MT模式
OPTION(USE_ALLMT_ "Compile all projects as mt"   ON)

# 开启设计器模式
OPTION(USE_DESIGNER_ "Compile DmMain for designer"   OFF)
 if(USE_DESIGNER_)
# 给解决方案中所有工程增加宏定义_DMDesigner_
ADD_DEFINITIONS(-D_DMDesigner_)
endif()

# 是否开启LOG功能
OPTION(USE_DMLOG_  "DM open log"    ON)
if(USE_DMLOG_)
# 给解决方案中所有工程增加宏定义LOG_ON
ADD_DEFINITIONS(-DDMLOG_ON)
endif()

# 是否使用Skia引擎
OPTION(USE_DMSKIA_  "DM user skia render draw"    OFF)

# 添加CMake的模块文件的查找路径
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/Tools/CMake)
INCLUDE(PrecompiledHeader)

# 增加子文件夹
ADD_SUBDIRECTORY(${PROJDIR}/DmMain)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMDemo)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/QQDemo)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMSpy)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMUnitTest)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMDesigner)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/TGPDemo)
if(USE_DMDLL_)# 仅dll模式下支持插件
ADD_SUBDIRECTORY(${PROJDIR}/PlugIns/Plugin_Expand)
ADD_SUBDIRECTORY(${PROJDIR}/PlugIns/Plugin_Designer)
endif()
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/zlib)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/gtest)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/lua)
if(USE_DMSKIA_)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/skia)
endif()