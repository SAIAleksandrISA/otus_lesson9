cmake_minimum_required(VERSION 3.15)

project(bulk LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(bulk 
    main.cpp
)

find_package(Threads REQUIRED)

target_include_directories(bulk
    PRIVATE
        ${CMAKE_SOURCE_DIR}/src
)

target_link_libraries(bulk
    PRIVATE
        async_lib
        Threads::Threads
)

if(MSVC)
    target_compile_options(bulk
        PRIVATE
            /W4
    )
else()
    target_compile_options(bulk
        PRIVATE
            -Wall
            -Wextra
            -pedantic
            -Werror
    )
endif()

install(TARGETS bulk
    RUNTIME DESTINATION bin
)