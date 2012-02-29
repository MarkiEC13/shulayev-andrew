cmake_minimum_required(VERSION 2.6)
project(convex_hull)

add_definitions(-O2)

set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")
set(CMAKE_BUILD_TYPE RelWithDebInfo)

add_executable(main shared/io.cpp shared/geometry.cpp)
