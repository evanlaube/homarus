cmake_minimum_required(VERSION 3.21)
set (CMAKE_CXX_STANDARD 11)

project(homarus)

include(CTest)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(demos)
