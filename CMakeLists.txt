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
    src/Record.cpp
    src/DEMParser.cpp
)


add_executable(${PROJECT_NAME} ${SOURCE_H} src/main.cpp ${SOURCE})

find_package(GTest CONFIG REQUIRED)

set(RECORD_TEST record_test)
add_executable(${RECORD_TEST} ${SOURCE_H} src/test/main.cpp ${SOURCE} src/test/Record.test.cpp)
target_link_libraries(${RECORD_TEST} gtest gtest_main)
