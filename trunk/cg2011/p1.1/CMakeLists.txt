cmake_minimum_required(VERSION 2.6)
project(segments_intersection)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(main main.cpp geometry.cpp)
