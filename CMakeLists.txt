cmake_minimum_required( VERSION 3.1 )

set( CMAKE_BUILD_TYPE "Release" )
if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wno-unused-variable -pthread" )
else()
	set( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -pthread -fopenmp" )
endif()
message( STATUS "Build Type: ${CMAKE_BUILD_TYPE} ${CMAKE_CXX_FLAGS}" )

project(correction)
aux_source_directory(. DIR_SRCS)
add_executable(${PROJECT_NAME} correction.cpp)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS}) # Not needed for CMake >= 2.8.11
target_link_libraries(${PROJECT_NAME} ${OpenCV_LIBS})