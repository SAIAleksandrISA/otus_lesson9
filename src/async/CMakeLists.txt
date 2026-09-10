cmake_minimum_required(VERSION 3.15)
project(async_lib LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(async_lib SHARED
    async.cpp
    async.h
)

target_include_directories(async_lib PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(async_lib PUBLIC
    bulk_core
    bulk_common
    bulk_threading
    Threads::Threads
)

target_compile_definitions(async_lib PRIVATE ASYNC_EXPORTS)

if(MSVC)
    target_compile_options(async_lib PRIVATE /W4)
else()
    target_compile_options(async_lib PRIVATE -Wall -Wextra -pedantic -Werror)
endif()

install(TARGETS async_lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
)