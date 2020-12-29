cmake_minimum_required(VERSION 2.6)

project(MyRedis)
set(C_FLAGS
  -g
  -Wall)

add_subdirectory(src)
add_subdirectory(test)
