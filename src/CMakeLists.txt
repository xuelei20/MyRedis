set(xlredisSrc
  adlist.c
  sds.c
  zmalloc
  )

add_library(xlredis ${xlredisSrc})
