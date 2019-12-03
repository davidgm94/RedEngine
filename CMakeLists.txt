cmake_minimum_required(VERSION 3.15)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(NAME RedEngine)
project(RedEngine C)

include_directories(src)
include_directories(src/external/glad/include)
include_directories(src/external/glad/include/glad)
include_directories(src/external/glad/include/KHR)
include_directories(src/external/volk)
include_directories(src/graphics)
include_directories(src/graphics/d3d)

add_executable(RedEngine
        src/external/glad/include/glad/glad.h
        src/external/glad/include/KHR/khrplatform.h
        src/external/glad/src/glad.c
        src/external/volk/volk.c
        src/external/volk/volk.h
        src/graphics/d3d/d3d11.h
        src/graphics/d3d/d3d12.h
        src/graphics/d3d/d3dcommon.h
        src/graphics/opengl.h
        src/graphics/vulkan.h
        src/red_common.h
        src/red_engine_win32.c
        src/red_graphics.h
        src/red_os.h
        src/red_platform.c
        src/red_platform.h
        src/os/red_win32.c
        src/os/red_win32.h src/os/red_linux.h src/red_engine_linux.c src/os/linux/args.h src/os/linux/timer.h)
# Sascha
if (NOT CMAKE_VERSION VERSION_LESS 3.7.0)
	message(STATUS "Using module to find Vulkan")
	find_package(Vulkan)
endif()

IF(WIN32)
	IF (NOT Vulkan_FOUND)
		find_library(Vulkan_LIBRARY NAMES vulkan-1 vulkan PATHS ${CMAKE_SOURCE_DIR}/libs/vulkan)
		IF (Vulkan_LIBRARY)
			set(Vulkan_FOUND ON)
			MESSAGE("Using bundled Vulkan library version")
		ENDIF()
ENDIF()
ENDIF(WIN32)
target_include_directories(RedEngine PRIVATE ${Vulkan_INCLUDE_DIRS})
# mine
# cmake_minimum_required(VERSION 3.7 FATAL_ERROR)
# find_package(Vulkan REQUIRED)
target_link_libraries(RedEngine vulkan)
target_link_libraries(RedEngine glfw)
target_link_libraries(RedEngine dl)
target_link_libraries(RedEngine pthread)
find_package(OpenGL REQUIRED)
target_include_directories(RedEngine PUBLIC ${OPENGL_INCLUDE_DIR})
target_link_libraries(RedEngine ${OPENGL_gl_LIBRARY})
set(CMAKE_C_STANDARD 99)
