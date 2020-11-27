cmake_minimum_required(VERSION 3.0)
project(marching_squares CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -O3")

include_directories(include)

set(SOURCE_H
    include/Record.hpp
    include/DEMParser.ipp
)

set(SOURCE
    src/main.cpp
    src/Record.cpp
    src/DEMParser.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_H} ${SOURCE})

add_subdirectory(src/test)
