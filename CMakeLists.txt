cmake_minimum_required(VERSION 2.8.4)
project(csgo_external)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ./build)

set(SOURCE_FILES
        main.cpp
        log/log.cpp log/log.hpp
        utils/timeutils.hpp utils/timeutils.cpp)

add_executable(csgo_external_recode ${SOURCE_FILES})