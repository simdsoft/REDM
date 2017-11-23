#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-1-5	1.0
#     k000      2017-07-11            使用target_include_directories指定DmMain的包含目录
#     k000      2017-07-12            启用时 USE_DESIGNER_ 为DmMain定义宏 _DMDesigner_
#-------------------------------------------------------
PROJECT(DmMain)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 									inc/*.h)
FILE(GLOB COMMON_HEADER     							inc/Common/*.h)
FILE(GLOB COMMON_TEMPLATE_HEADER     					inc/Common/Template/*.h)
FILE(GLOB COMMON_PLUGINS_HEADER     					inc/Common/Plugins/*.h)
FILE(GLOB COMMON_ACTIVEX_HEADER     					inc/Common/ActiveX/*)# 存在flash.tlh
FILE(GLOB CORE_HEADER       							inc/Core/*.h)
FILE(GLOB CORE_MSG_HEADER       						inc/Core/Msg/*.h)
FILE(GLOB CORE_DUI_HEADER       						inc/Core/Dui/*.h)
FILE(GLOB CORE_EVENT_HEADER       						inc/Core/Event/*.h)
FILE(GLOB IDMMAIN_HEADER    							inc/IDmMain/*.h)
FILE(GLOB MODULES_HEADER    							inc/Modules/*.h)
FILE(GLOB MODULES_RENDER_HEADER    						inc/Modules/Render/*.h)
FILE(GLOB MODULES_SKIN_HEADER    						inc/Modules/Skin/*.h)
FILE(GLOB WIDGETS_HEADER    							inc/Widgets/*.h)

FILE(GLOB ROOT_SOURCE 									src/*.cpp;src/*.c;src/*.rc)
FILE(GLOB COMMON_SOURCE     							src/Common/*.cpp;src/Common/*.c)
FILE(GLOB COMMON_PLUGINS_SOURCE     					src/Common/Plugins/*.cpp;src/Common/Plugins/*.c)
FILE(GLOB COMMON_ACTIVEX_SOURCE     					src/Common/ActiveX/*.cpp;src/Common/ActiveX/*.c)
FILE(GLOB CORE_SOURCE       							src/Core/*.cpp;src/Core/*.c)
FILE(GLOB CORE_MSG_SOURCE       						src/Core/Msg/*.cpp;src/Core/Msg/*.c)
FILE(GLOB CORE_DUI_SOURCE       						src/Core/Dui/*.cpp;src/Core/Dui/*.c)
FILE(GLOB CORE_EVENT_SOURCE       						src/Core/Event/*.cpp;src/Core/Event/*.c)
FILE(GLOB IDMMAIN_SOURCE    							src/IDmMain/*.cpp;src/IDmMain/*.c)
FILE(GLOB MODULES_SOURCE    							src/Modules/*.cpp;src/Modules/*.c)
FILE(GLOB MODULES_RENDER_SOURCE    						src/Modules/Render/*.cpp;src/Modules/Render/*.c)
FILE(GLOB MODULES_SKIN_SOURCE    						src/Modules/Skin/*.cpp;src/Modules/Skin/*.c)
FILE(GLOB WIDGETS_SOURCE    							src/Widgets/*.cpp;src/Widgets/*.c)

SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\Common" 						FILES 	${COMMON_HEADER})
SOURCE_GROUP("inc\\Common\\Template" 			FILES 	${COMMON_TEMPLATE_HEADER})
SOURCE_GROUP("inc\\Common\\Plugins" 			FILES 	${COMMON_PLUGINS_HEADER})
SOURCE_GROUP("inc\\Common\\ActiveX" 			FILES 	${COMMON_ACTIVEX_HEADER})
SOURCE_GROUP("inc\\Core" 						FILES 	${CORE_HEADER})#必须用\\,不能用/
SOURCE_GROUP("inc\\Core\\Msg" 					FILES 	${CORE_MSG_HEADER})
SOURCE_GROUP("inc\\Core\\Dui" 					FILES 	${CORE_DUI_HEADER})
SOURCE_GROUP("inc\\Core\\Event" 				FILES 	${CORE_EVENT_HEADER})
SOURCE_GROUP("inc\\IDmMain" 					FILES 	${IDMMAIN_HEADER})
SOURCE_GROUP("inc\\Modules" 					FILES 	${MODULES_HEADER})
SOURCE_GROUP("inc\\Modules\\Render" 			FILES 	${MODULES_RENDER_HEADER})
SOURCE_GROUP("inc\\Modules\\Skin" 				FILES 	${MODULES_SKIN_HEADER})
SOURCE_GROUP("inc\\Widgets" 					FILES 	${WIDGETS_HEADER})
SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\Common" 						FILES 	${COMMON_SOURCE})
SOURCE_GROUP("src\\Common\\Plugins" 			FILES 	${COMMON_PLUGINS_SOURCE})
SOURCE_GROUP("src\\Common\\ActiveX" 			FILES 	${COMMON_ACTIVEX_SOURCE})
SOURCE_GROUP("src\\Core"						FILES 	${CORE_SOURCE})
SOURCE_GROUP("src\\Core\\Msg"					FILES 	${CORE_MSG_SOURCE})
SOURCE_GROUP("src\\Core\\Dui"					FILES 	${CORE_DUI_SOURCE})
SOURCE_GROUP("src\\Core\\Event"					FILES 	${CORE_EVENT_SOURCE})
SOURCE_GROUP("src\\IDmMain"						FILES 	${IDMMAIN_SOURCE})
SOURCE_GROUP("src\\Modules"						FILES 	${MODULES_SOURCE})
SOURCE_GROUP("src\\Modules\\Render"				FILES 	${MODULES_RENDER_SOURCE})
SOURCE_GROUP("src\\Modules\\Skin"				FILES 	${MODULES_SKIN_SOURCE})
SOURCE_GROUP("src\\Widgets"						FILES 	${WIDGETS_SOURCE})

# 输出设置
add_definitions(-DUNICODE -D_UNICODE)  # unicode版本
set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.
if(USE_ALLMT_)
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
if(USE_DMDLL_)
ADD_LIBRARY(DmMain SHARED
			${ROOT_HEADER};
			${COMMON_HEADER};
			${COMMON_TEMPLATE_HEADER};
			${COMMON_PLUGINS_HEADER};
			${COMMON_ACTIVEX_HEADER};
			${CORE_HEADER};
			${CORE_MSG_HEADER};
			${CORE_DUI_HEADER};
			${CORE_EVENT_HEADER};
			${IDMMAIN_HEADER};
			${MODULES_HEADER};
			${MODULES_RENDER_HEADER};
			${MODULES_SKIN_HEADER};
			${WIDGETS_HEADER};
			${ROOT_SOURCE};
			${COMMON_SOURCE};
			${COMMON_PLUGINS_SOURCE};
			${COMMON_ACTIVEX_SOURCE};
			${CORE_SOURCE};
			${CORE_MSG_SOURCE};
			${CORE_DUI_SOURCE};
			${CORE_EVENT_SOURCE};
			${IDMMAIN_SOURCE};
			${MODULES_SOURCE};
			${MODULES_RENDER_SOURCE};
			${MODULES_SKIN_SOURCE};
			${WIDGETS_SOURCE};
			)
else()
ADD_LIBRARY(DmMain STATIC
			${ROOT_HEADER};
			${COMMON_HEADER};
			${COMMON_TEMPLATE_HEADER};
			${COMMON_PLUGINS_HEADER};
			${COMMON_ACTIVEX_HEADER};
			${CORE_HEADER};
			${CORE_MSG_HEADER};
			${CORE_DUI_HEADER};
			${CORE_EVENT_HEADER};
			${IDMMAIN_HEADER};
			${MODULES_HEADER};
			${MODULES_RENDER_HEADER};
			${MODULES_SKIN_HEADER};
			${WIDGETS_HEADER};
			${ROOT_SOURCE};
			${COMMON_SOURCE};
			${COMMON_PLUGINS_SOURCE};
			${COMMON_ACTIVEX_SOURCE};
			${CORE_SOURCE};
			${CORE_MSG_SOURCE};
			${CORE_DUI_SOURCE};
			${CORE_EVENT_SOURCE};
			${IDMMAIN_SOURCE};
			${MODULES_SOURCE};
			${MODULES_RENDER_SOURCE};
			${MODULES_SKIN_SOURCE};
			${WIDGETS_SOURCE};
			)
endif()

if (USE_DESIGNER_)
  target_compile_definitions(DmMain
    PRIVATE _DMDesigner_
  )
endif()

target_include_directories(DmMain
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Common
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Common/Template
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Common/Plugins
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Common/ActiveX
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Core
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Core/Msg
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Core/Dui
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Core/Event
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/IDmMain
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Modules
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Modules/Render
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Modules/Skin
  PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/inc/Widgets
)
SET_TARGET_PROPERTIES(DmMain PROPERTIES OUTPUT_NAME "DmMain")

# 增加预编译头
use_precompiled_header(DmMain
   "${CMAKE_CURRENT_SOURCE_DIR}/inc/DmMainAfx.h"
  "${CMAKE_CURRENT_SOURCE_DIR}/src/DmMainAfx.cpp"
  )
