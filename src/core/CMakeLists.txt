cmake_minimum_required(VERSION 3.15)
project(bulk_core LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(bulk_core STATIC
    interfaces.h
    bulkprocessor.h
    bulkprocessor.cpp
    consoleoutputhandler.h
    consoleoutputhandler.cpp
    fileoutputhandler.h
    fileoutputhandler.cpp
	сonnection_manager.h
    connection_manager.cpp
	session_manager.h
    session_manager.cpp
)

target_include_directories(bulk_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(bulk_core PRIVATE /W4)
else()
    target_compile_options(bulk_core PRIVATE -Wall -Wextra -pedantic -Werror)
endif()