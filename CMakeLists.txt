cmake_minimum_required(VERSION 3.13)
project(ButterworthFilterTools)

set(CMAKE_CXX_STANDARD 17)

#add_subdirectory(lib/googletest-master)
include_directories(/lib/googletest-master/googletest/include include)

set(FILTER_INCLUDE /lib/googletest-master/googletest/include
        include)

set(FILTER_SOURCE ButterworthFilterTools
        main.cpp
        src/ButterTool.cpp)

add_library(filter_lib SHARED ${FILTER_SOURCE})

#target_link_libraries(ButterworthFilterTools PUBLIC gtest gtest_main)
target_link_libraries(filter_lib gtest gtest_main)
target_include_directories(filter_lib PUBLIC ${FILTER_INCLUDE})

add_executable(butterfilter main.cpp)
target_link_libraries(butterfilter filter_lib)

add_executable(test_butterfilter test/src/test_butterworth_filter.cpp)
target_link_libraries(test_butterfilter filter_lib)