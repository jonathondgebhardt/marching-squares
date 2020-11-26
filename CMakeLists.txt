cmake_minimum_required(VERSION 3.0)
project(marching_squares CXX)

# remove compiler checks if you are using VS2010, it supports
#   1. auto
#   2. lambda
#   3. const iterator
# and, sadly, it doesn't support `for(item, cotainer)'
include( CheckCXXCompilerFlag )
check_cxx_compiler_flag( "-std=c++17"   COMPILER_SUPPORTS_CXX17 )
check_cxx_compiler_flag( "-std=c++0x"   COMPILER_SUPPORTS_CXX0X )
if(COMPILER_SUPPORTS_CXX17)
    if(CMAKE_COMPILER_IS_GNUCXX)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++17")
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    endif()
elseif(COMPILER_SUPPORTS_CXX0X)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
else()
    # MSVC, On by default (if available)
endif()

# add_subdirectory(src)

set(SOURCE_H
    src/Record.hpp
    src/DEMParser.ipp
)

set(SOURCE
    src/main.cpp
    src/Record.cpp
    src/DEMParser.cpp
)

add_executable(${PROJECT_NAME} ${SOURCE_H} ${SOURCE})
