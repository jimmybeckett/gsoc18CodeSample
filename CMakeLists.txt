cmake_minimum_required(VERSION 3.9)
project(uBLAS)

find_package(Boost)
IF (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIR})
endif()
find_package (Boost COMPONENTS REQUIRED timer)

set(CMAKE_CXX_STANDARD 17)

add_executable(uBLAS main.cpp Matrix.h)

target_link_libraries (uBLAS boost_timer)