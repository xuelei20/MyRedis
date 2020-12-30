cmake_minimum_required(VERSION 2.6)

project(MyRedis)

set(C_FLAGS
  -g
  -Wall)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_subdirectory(src)
add_subdirectory(test)
