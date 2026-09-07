cmake_minimum_required(VERSION 3.15)
project(bulk_common LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bulk_common STATIC
    block.h
    thread_safe_queue.h
	common.cpp)

target_include_directories(bulk_common PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

if(MSVC)
    target_compile_options(bulk_common PRIVATE /W4)
else()
    target_compile_options(bulk_common PRIVATE -Wall -Wextra -pedantic -Werror)
endif()