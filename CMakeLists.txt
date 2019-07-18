cmake_minimum_required(VERSION 3.13)
project(ButterworthFilterTools)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(lib/googletest-master)
include_directories(/lib/googletest-master/googletest/include)

add_executable(ButterworthFilterTools main.cpp ButterTool.cpp ButterTool.h)
target_link_libraries(ButterworthFilterTools gtest gtest_main)