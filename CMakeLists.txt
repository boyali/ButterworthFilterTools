cmake_minimum_required(VERSION 3.13)
project(ButterworthFilterTools)

set(CMAKE_CXX_STANDARD 17)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(FILTER_SOURCE ButterworthFilterTools
        main.cpp
        include/ButterTool.h
        src/ButterTool.cpp)

add_library(filter_lib SHARED ${FILTER_SOURCE})

#target_link_libraries(ButterworthFilterTools PUBLIC gtest gtest_main)
target_link_libraries(filter_lib gtest gtest_main)
target_include_directories(filter_lib PUBLIC ${FILTER_INCLUDE})

add_executable(butterfilter main.cpp)
target_link_libraries(butterfilter filter_lib)

add_executable(test_butterfilter test/src/test_butterworth_filter.cpp)
target_include_directories(test_butterfilter PUBLIC include)
target_link_libraries(test_butterfilter filter_lib)