cmake_minimum_required(VERSION 3.12)
project(Delaunay)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O2")

add_executable(Delaunay main.cpp Auxilliary.h)
