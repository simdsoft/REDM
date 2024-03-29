#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-5	1.0
#      k000     2017-07-11            修复项目作为子项目引入时PrecompiledHeader.cmake路径错误
#-------------------------------------------------------
cmake_minimum_required(VERSION 3.21)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)

cmake_policy(SET CMP0079 NEW)

# 工程名
PROJECT(DM)

message(STATUS "CMAKE_SYSTEM_PROCESSOR=${CMAKE_SYSTEM_PROCESSOR}")

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
set(LIBRARY_OUTPUT_PATH  ${DM_BINARY_DIR}/lib)

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
OPTION(USE_ALLMT_ "Compile all projects as mt"   OFF)


# 是否开启LOG功能
OPTION(USE_DMLOG_  "DM open log"    ON)
if(USE_DMLOG_)
# 给解决方案中所有工程增加宏定义LOG_ON
ADD_DEFINITIONS(-DDMLOG_ON)
endif()

# 是否使用Skia引擎
OPTION(USE_DMSKIA_  "DM user skia render draw"    OFF)

# 添加CMake的模块文件的查找路径
LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Tools/CMake)
INCLUDE(PrecompiledHeader)

# Use extern pugixml
option(USE_EXTERN_PUGIXML "Use extern pugixml" OFF)
set(USE_EXTERN_PUGIXML ${USE_EXTERN_PUGIXML} CACHE INTERNAL "Use extern pugixml")

if(USE_EXTERN_PUGIXML)
    # 使用外部动态pugixml库
    ADD_DEFINITIONS(-DPUGIXML_API=__declspec\(dllimport\))
else()
    ADD_DEFINITIONS(-DPUGIXML_API=) 
endif()

# 增加子文件夹
macro(dm_config_pred target_name pred)
    if(${pred})
        target_compile_definitions(${target_name} PUBLIC ${pred}=1)
    endif()
endmacro()
ADD_SUBDIRECTORY(${PROJDIR}/DmMain)
dm_config_pred(DmMain DM_EXCLUDE_MUI)
dm_config_pred(DmMain DM_EXCLUDE_ACTIVEX)
dm_config_pred(DmMain DM_EXCLUDE_IE)

set_target_properties(DmMain PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${DM_BINARY_DIR}/bin)

ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/gtest)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/lua)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/scintilla)
if(USE_DMSKIA_)
    ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/skia)
endif()

option(DM_BUILD_DESIGNER "Build DM Designer" ON)
if (DM_BUILD_DESIGNER)
    ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMDesigner)
endif()

option(DM_BUILD_TESTS "Build DM Tests" ON)
if (DM_BUILD_TESTS)
    ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMUnitTest)
    target_link_libraries(DMUnitTest gtest)

    ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMDemo)
    ADD_SUBDIRECTORY(${PROJDIR}/Samples/QQDemo)
    ADD_SUBDIRECTORY(${PROJDIR}/Samples/TGPDemo)
endif()


if(NOT USE_EXTERN_ZLIB)
    set(ZLIB_BUILD_STATIC OFF CACHE BOOL "Build zlib static library" FORCE)
    set(ZLIB_BUILD_SHARED ON CACHE BOOL "Build zlib shared library" FORCE)
    set(ZLIB_BUILD_EXAMPLES OFF CACHE BOOL "Build zlib examples" FORCE)
    set(ZLIB_INSTALL OFF CACHE BOOL "Create installation targets (chain builders may want to disable this)" FORCE)
    ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/zlib EXCLUDE_FROM_ALL)
    set_target_properties(zlib PROPERTIES 
        RUNTIME_OUTPUT_DIRECTORY ${DM_BINARY_DIR}/bin
        FOLDER "3rdParty"
        )
endif()

if(USE_DMDLL_)# 仅dll模式下支持插件
    ADD_SUBDIRECTORY(${PROJDIR}/PlugIns/Plugin_Expand)
    ADD_SUBDIRECTORY(${PROJDIR}/PlugIns/Plugin_Designer)
endif()
