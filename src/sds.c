#include "sds.h"
#include "zmalloc.h"

#include <string.h>

sds sdsnew(const char *init) {
  size_t len = (init == NULL ? 0 : strlen(init));
  return sdsnewlen(init, len);
}

// T = O(n)
sds sdsnewlen(const void *init, size_t len) {
  struct sdshdr *sh;

  if (init) {
    sh = zmalloc(sizeof(struct sdshdr)+len+1);
  } else {
    sh = zcalloc(sizeof(struct sdshdr)+len+1);
  }

  if (NULL == sh) 
    return NULL;

  sh->len = len;
  sh->free = 0;
  if (init && len > 0)
    memcpy(sh->buf, init, len);
  sh->buf[len] = '\0';

  return (char *)sh->buf;
}

// T = O(n)
void sdsfree(sds s)
{
  if (NULL == s) 
    return;
  zfree(s-sizeof(struct sdshdr));
}

sds sdsdup(const sds s) {
  return sdsnewlen(s, sdslen(s));
}

// T = O(1)
void sdsclear(sds s) {
  struct sdshdr *sh = (void *)(s-sizeof(struct sdshdr));
  sh->free += sdslen(s);
  sh->len = 0;
  sh->buf[0] = '\0';
}

sds sdscat(sds s, const char *append) {
  return sdscatlen(s, append, strlen(append));
}

sds sdscatlen(sds s, const char *append, size_t addlen) {
  s = sdsMakeRoomFor(s, addlen);
  if (NULL == s) 
    return NULL;

  struct sdshdr *sh = (void *)(s-sizeof(struct sdshdr));
  size_t curlen = sdslen(s);
  memcpy(s+curlen, append, addlen);
  sh->len = curlen+addlen;
  sh->free = sh->free-addlen;
  sh->buf[curlen+addlen] = '\0';

  return s;
}

sds sdsMakeRoomFor(sds s, size_t addlen) {
  struct sdshdr *sh = (void *)(s-sizeof(struct sdshdr));
  size_t free = sh->free;
  if (free >= addlen)
    return s;

  size_t len = sh->len;
  size_t newlen = len+addlen;
  if (newlen*2 < SDS_MAX_PREALLOC)
    newlen *= 2; // double
  else
    newlen = SDS_MAX_PREALLOC;

  struct sdshdr *newsh = zrealloc(sh, sizeof(struct sdshdr)+newlen+1); // will copy old data
  if (NULL == newsh)
    return NULL;
  newsh->free = newlen-len;
  return newsh->buf;
}

sds sdscatsds(sds s, const sds append) {
  return sdscatlen(s, append, sdslen(append));
}
