cmake_minimum_required(VERSION 3.0)

project(GLSLCompiler)

if(${CMAKE_SYSTEM_NAME} MATCHES "Windows")
	set(OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
	set(OS_MACOSX 1)
endif()

if(OS_MACOSX)
	find_package(OpenGL REQUIRED)
endif()

set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(LIB_SFML "Sdk/SFML" CACHE PATH "Location of library SFML.")

include_directories(${LIB_SFML}/include)
link_directories(${LIB_SFML}/lib)

include_directories(${PROJECT_SOURCE_DIR})

if(OS_MACOSX)
	include_directories(${OPENGL_INCLUDE_DIR})
endif()

add_executable(glslcompiler main.cpp ogl.cpp)

if(OS_WINDOWS)
	target_link_libraries(glslcompiler sfml-window sfml-system opengl32)
elseif(OS_MACOSX)
	target_link_libraries(glslcompiler sfml-window sfml-system ${OPENGL_LIBRARIES})
endif()

install(TARGETS glslcompiler DESTINATION bin)