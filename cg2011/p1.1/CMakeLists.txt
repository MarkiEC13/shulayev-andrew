cmake_minimum_required(VERSION 2.6)
project(segments_intersection)

add_definitions(-Wall -W -std=c++0x)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(main main.cpp geometry.cpp)
