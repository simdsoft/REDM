#-------------------------------------------------------
# Copyright (c) DuiMagic
# All rights reserved.
# History:
# 		<Author>	<Time>		<Version>	  <Des>
#      guoyou		2015-8-18	1.0		
#-------------------------------------------------------
PROJECT(skia)

# 文件分组（过滤），同时必须加入到ADD_LIBRARY中才能显示
FILE(GLOB ROOT_HEADER 									inc/*.h)
FILE(GLOB CORE_HEADER     								inc/core/*.h)
FILE(GLOB UTILS_HEADER     								inc/utils/*.h)
FILE(GLOB UTILS_WIN_HEADER     							inc/utils/win/*.h)
FILE(GLOB PATHOPS_HEADER     							inc/pathops/*.h)
FILE(GLOB EFFECTS_HEADER     							inc/effects/*.h)
FILE(GLOB PORTS_HEADER     								inc/ports/*.h)
FILE(GLOB GPU_HEADER     								inc/gpu/*.h)
FILE(GLOB GPU_GL_HEADER     							inc/gpu/gl/*.h)
FILE(GLOB IMAGES_HEADER     							inc/images/*.h)

FILE(GLOB ROOT_SOURCE 									src/*.*)
FILE(GLOB FONTS_SOURCE     								src/fonts/*.*)
FILE(GLOB UTILS_SOURCE     								src/utils/*.*)
FILE(GLOB UTILS_WIN_SOURCE     							src/utils/win/*.*)
FILE(GLOB CORE_SOURCE     								src/core/*.*)
FILE(GLOB DOC_SOURCE     								src/doc/*.*)
FILE(GLOB IMAGE_SOURCE     								src/image/*.*)
FILE(GLOB LAZY_SOURCE     								src/lazy/*.*)
FILE(GLOB PATHOPS_SOURCE     							src/pathops/*.*)
FILE(GLOB EFFECTS_SOURCE     							src/effects/*.*)
FILE(GLOB EFFECTS_GRADIENTS_SOURCE     					src/effects/gradients/*.*)
FILE(GLOB OPTS_SOURCE     								src/opts/*.*)
FILE(GLOB PORTS_SOURCE     								src/ports/*.*)
FILE(GLOB GPU_SOURCE     								src/gpu/*.*)
FILE(GLOB GPU_EFFECTS_SOURCE     						src/gpu/effects/*.*)
FILE(GLOB GPU_GL_SOURCE     							src/gpu/gl/*.*)
FILE(GLOB GPU_GL_WIN_SOURCE     						src/gpu/gl/win/*.*)
FILE(GLOB GPU_GL_DEBUG_SOURCE     						src/gpu/gl/debug/*.*)
FILE(GLOB GPU_GL_BUILDERS_SOURCE     					src/gpu/gl/builders/*.*)
FILE(GLOB SFNT_SOURCE     								src/sfnt/*.*)
FILE(GLOB IMAGES_SOURCE     							src/images/*.*)


SOURCE_GROUP("inc" 								FILES 	${ROOT_HEADER})
SOURCE_GROUP("inc\\core" 						FILES 	${CORE_HEADER})
SOURCE_GROUP("inc\\utils" 						FILES 	${UTILS_HEADER})
SOURCE_GROUP("inc\\utils\\win" 					FILES 	${UTILS_WIN_HEADER})
SOURCE_GROUP("inc\\pathops" 					FILES 	${PATHOPS_HEADER})
SOURCE_GROUP("inc\\effects" 					FILES 	${EFFECTS_HEADER})
SOURCE_GROUP("inc\\ports" 						FILES 	${PORTS_HEADER})
SOURCE_GROUP("inc\\gpu" 						FILES 	${GPU_HEADER})
SOURCE_GROUP("inc\\gpu\\gl" 					FILES 	${GPU_GL_HEADER})
SOURCE_GROUP("inc\\images" 						FILES 	${IMAGES_HEADER})

SOURCE_GROUP("src"								FILES 	${ROOT_SOURCE})
SOURCE_GROUP("src\\fonts"						FILES 	${FONTS_SOURCE})
SOURCE_GROUP("src\\utils"						FILES 	${UTILS_SOURCE})
SOURCE_GROUP("src\\utils\\win"					FILES 	${UTILS_WIN_SOURCE})
SOURCE_GROUP("src\\core"						FILES 	${CORE_SOURCE})
SOURCE_GROUP("src\\doc"							FILES 	${DOC_SOURCE})
SOURCE_GROUP("src\\image"						FILES 	${IMAGE_SOURCE})
SOURCE_GROUP("src\\lazy"						FILES 	${LAZY_SOURCE})
SOURCE_GROUP("src\\pathops"						FILES 	${PATHOPS_SOURCE})
SOURCE_GROUP("src\\effects"						FILES 	${EFFECTS_SOURCE})
SOURCE_GROUP("src\\effects\\gradients"			FILES 	${EFFECTS_GRADIENTS_SOURCE})
SOURCE_GROUP("src\\opts"						FILES 	${OPTS_SOURCE})
SOURCE_GROUP("src\\ports"						FILES 	${PORTS_SOURCE})
SOURCE_GROUP("src\\gpu"							FILES 	${GPU_SOURCE})
SOURCE_GROUP("src\\gpu\\gl"						FILES 	${GPU_GL_SOURCE})
SOURCE_GROUP("src\\gpu\\gl\\effects"			FILES 	${GPU_EFFECTS_SOURCE})
SOURCE_GROUP("src\\gpu\\gl\\win"				FILES 	${GPU_GL_WIN_SOURCE})
SOURCE_GROUP("src\\gpu\\gl\\debug"				FILES 	${GPU_GL_DEBUG_SOURCE})
SOURCE_GROUP("src\\gpu\\gl\\builder"			FILES 	${GPU_GL_BUILDERS_SOURCE})
SOURCE_GROUP("src\\sfnt"						FILES 	${SFNT_SOURCE})
SOURCE_GROUP("src\\images"						FILES 	${IMAGES_SOURCE})

# 增加头文件搜索路径
INCLUDE_DIRECTORIES(inc/config;
					inc/core;				
					inc/gpu;
					inc/images;
					inc/utils;
					inc/utils/win;	
					inc/ports;
					inc/text;
					inc/effects;				
					inc/pathops;
					src/gpu;
					src/utils;
					src/ports;
					src/sfnt;
					src/core;
					src/image;
					src/opts;
					src/lazy;)

# 输出设置
add_definitions(-DUNICODE -D_UNICODE)  # unicode版本

set(CMAKE_DEBUG_POSTFIX "_d")# Debug下生成文件增加后缀,对exe无效.

# 输出设置
ADD_DEFINITIONS(-DSK_IGNORE_ETC1_SUPPORT) 						
ADD_DEFINITIONS(-D_CRT_SECURE_NO_WARNINGS) 						# disable CRT
if(USE_DMDLL_)
else()
SET(CMAKE_CXX_FLAGS_DEBUG "/D_DEBUG /MTd /Zi /Ob0 /Od /RTC1")# 直接抄gui的默认，然后 改成mtd
SET(CMAKE_CXX_FLAGS_RELEASE "/MT /O2 /Ob2 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_MINSIZEREL "/MT /O1 /Ob1 /D NDEBUG")
SET(CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MT /Zi /O2 /Ob1 /D NDEBUG")
endif()
ADD_LIBRARY(skia STATIC 
			${ROOT_HEADER};
			${CORE_HEADER};
			${UTILS_HEADER};
			${PATHOPS_HEADER};
			${EFFECTS_HEADER};
			${PORTS_HEADER};
			${GPU_HEADER};
			${GPU_GL_HEADER};
			${IMAGES_HEADER};
			
			${ROOT_SOURCE};
			${FONTS_SOURCE};
			${UTILS_SOURCE};
			${UTILS_WIN_SOURCE}
			${CORE_SOURCE};
			${DOC_SOURCE};
			${IMAGE_SOURCE};
			${LAZY_SOURCE};
			${PATHOPS_SOURCE};
			${EFFECTS_SOURCE};
			${EFFECTS_GRADIENTS_SOURCE};
			${OPTS_SOURCE};
			${PORTS_SOURCE};
			${GPU_SOURCE};
			${GPU_EFFECTS_SOURCE};
			${GPU_GL_SOURCE};
			${GPU_GL_WIN_SOURCE};
			${GPU_GL_DEBUG_SOURCE};
			${GPU_GL_BUILDERS_SOURCE};
			${SFNT_SOURCE};
			${IMAGES_SOURCE};
			)
SET_TARGET_PROPERTIES(skia PROPERTIES OUTPUT_NAME "skia")
SET_TARGET_PROPERTIES(skia PROPERTIES FOLDER 3rdParty)