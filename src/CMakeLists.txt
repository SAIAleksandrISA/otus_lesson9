cmake_minimum_required(VERSION 3.15)
project(BulkApp_Src LANGUAGES CXX)

add_subdirectory(core)
add_subdirectory(common)
add_subdirectory(threading)

add_subdirectory(async)