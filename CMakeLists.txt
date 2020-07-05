#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-5	1.0
#      k000     2017-07-11            修复项目作为子项目引入时PrecompiledHeader.cmake路径错误
#-------------------------------------------------------
cmake_minimum_required(VERSION 3.10)
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

# --- The compiler flags
message("-- Building REDM with cpp${_CXX_STD} support")
if ( IOS )
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fembed-bitcode" CACHE INTERNAL "CMAKE_CXX_FLAGS")
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fembed-bitcode" CACHE INTERNAL "CMAKE_C_FLAGS")
endif ()
if (NOT WIN32 OR CYGWIN)
    if (_CXX_STD EQUAL 17)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1z" CACHE INTERNAL "CMAKE_CXX_FLAGS")
        if ( IOS )
            # Aligned deallocation function of type 'void (void *, std::align_val_t) noexcept' is only available on iOS 11 or newer
            # most of time, low level malloc will alloc a aligned address for new operator,
            # so it's ok to add -faligned-allocation, certainly, still need find a ios9.0 device
            # to test does it works well?
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -faligned-allocation" CACHE INTERNAL "CMAKE_CXX_FLAGS")
        endif()
    elseif(_CXX_STD EQUAL 14)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y" CACHE INTERNAL "CMAKE_CXX_FLAGS")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11" CACHE INTERNAL "CMAKE_CXX_FLAGS")
    endif()
else()
    if(_CXX_STD EQUAL 17)
        # target_compile_features(REDM PUBLIC cxx_std_17) works
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++17" CACHE INTERNAL "CMAKE_CXX_FLAGS")
    elseif(_CXX_STD EQUAL 14)
        # target_compile_features(REDM PUBLIC cxx_std_14) not works
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /std:c++14" CACHE INTERNAL "CMAKE_CXX_FLAGS")
    endif()
endif()

# 增加子文件夹
ADD_SUBDIRECTORY(${PROJDIR}/DmMain)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/DMDemo)
ADD_SUBDIRECTORY(${PROJDIR}/Samples/QQDemo)
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
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/scintilla)
if(USE_DMSKIA_)
ADD_SUBDIRECTORY(${PROJDIR}/3rdParty/skia)
endif()
