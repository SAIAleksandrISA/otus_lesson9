cmake_minimum_required(VERSION 3.15)
project(bulk_threading LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bulk_threading STATIC
    thread_manager.h
    thread_manager.cpp
)

target_include_directories(bulk_threading PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(bulk_threading PRIVATE /W4)
else()
    target_compile_options(bulk_threading PRIVATE -Wall -Wextra -pedantic -Werror)
endif()