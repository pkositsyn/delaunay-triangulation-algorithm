cmake_minimum_required(VERSION 3.10)
project(Delaunay)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(Tests test/test_all.cpp
        src/scanline_triangulation.cpp src/models.cpp)
target_link_libraries(Tests ${GTEST_LIBRARIES} pthread)

add_executable(Delaunay src/main.cpp src/scanline_triangulation.cpp src/models.cpp)