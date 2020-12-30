set(xxtest sdstest)
add_executable(${xxtest} ${xxtest}.c)
target_link_libraries(${xxtest} xlredis)
