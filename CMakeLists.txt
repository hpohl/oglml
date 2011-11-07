cmake_minimum_required(VERSION 2.6)
project(oglml)

set(CMAKE_CXX_COMPILER g++-4.5)

add_definitions(-std=c++0x -Wall -Wextra -ftemplate-depth=100)

set(CMAKE_BINARY_DIR build)

add_subdirectory(src)
