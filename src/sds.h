// simple dynamic string
#ifndef _XL_SDS_H
#define _XL_SDS_H

#include <sys/types.h>

#define SDS_MAX_PREALLOC (1024*1024)

typedef char *sds; // point to buf

struct sdshdr {
  int len;
  int free;
  char buf[];
};

// T = O(1). If use C string, strlen T = O(n)
inline static size_t sdslen(const sds s) 
{
  struct sdshdr *sh = (void*)(s - sizeof(struct sdshdr));
  return sh->len;
}

// T = O(1)
inline static size_t sdsavail(const sds s) 
{
  struct sdshdr *sh = (void*)(s - sizeof(struct sdshdr));
  return sh->free;
}

sds sdsnew(const char *init);
sds sdsnewlen(const void *init, size_t len);
void sdsfree(sds s);
sds sdsdup(const sds s);
void sdsclear(sds s);
sds sdscat(sds s, const char *append);
sds sdscatlen(sds s, const char *append, size_t addlen);
sds sdscatsds(sds s, const sds append);
sds sdscpy(sds s, const char *src);
sds sdscpylen(sds s, const char *src, size_t len);
int sdscmp(const sds s1, const sds s2);

/* private */
sds sdsMakeRoomFor(sds ptr, size_t addlen);

#endif // _XL_
