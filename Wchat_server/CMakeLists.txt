project(server_test)
cmake_minimum_required(VERSION 3.10)

include_directories(./include/)

if(CMAKE_COMPILER_IS_GNUCXX)
    add_compile_options(-std=c++11)
    message(STATUS "optional:-std=c++11")   
endif(CMAKE_COMPILER_IS_GNUCXX)

add_definitions("-g")

add_subdirectory(src)
add_subdirectory(test)
