// simple dynamic string
#ifndef _XL_SDS_H
#define _XL_SDS_H

#include <sys/types.h>

typedef char *sds; // point to buf

struct sdshdr {
  int len;
  int free;
  char buf[];
};

// T = O(1)
inline static size_t sdslen(const sds s) {
  struct sdshdr *sh = (void*)(s - sizeof(struct sdshdr));
  return sh->len;
}

// T = O(1)
inline static size_t sdsavil(const sds s) {
  struct sdshdr *sh = (void*)(s - sizeof(struct sdshdr));
  return sh->free;
}

sds sdsnew(const char* init);

#endif // _XL_
