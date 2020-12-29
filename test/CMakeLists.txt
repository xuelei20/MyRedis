set(xxtest sdstest)
add_executable(${xxtest} ${xxtest}.c)
add_link_options(${xxtest} xlredis)
