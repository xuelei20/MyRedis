cmake_minimum_required(VERSION 2.6)
project(xlRedis)

add_compile_options(-g -Wall -std=c99)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(test)
