cmake_minimum_required(VERSION 2.8.4)
project(csgo_external)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32 -std=c++11")
set(EXECUTABLE_OUTPUT_PATH ./build)

set(SOURCE_FILES
        main.cpp
        logger/logger.cpp logger/logger.hpp
        utils/time.hpp utils/time.cpp)

add_executable(csgo_external_recode ${SOURCE_FILES})