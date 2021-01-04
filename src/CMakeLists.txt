set(xlredisSrc
  adlist.c
  dict.c
  sds.c
  zmalloc
  )

add_library(xlredis ${xlredisSrc})
