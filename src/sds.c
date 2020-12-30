#include "sds.h"
#include "zmalloc.h"

#include <string.h>

sds sdsnew(const char *init) {
  size_t len = (init == NULL ? 0 : strlen(init));
  return sdsnewlen(init, len);
}

// T = O(n)
sds sdsnewlen(const void *init, size_t len) {
  struct sdshdr *hd;

  if (init) {
    hd = zmalloc(sizeof(struct sdshdr)+len+1);
  } else {
    hd = zcalloc(sizeof(struct sdshdr)+len+1);
  }

  if (NULL == hd) return NULL;

  hd->len = len;
  hd->free = 0;
  if (init && len > 0)
    memcpy(hd->buf, init, len);
  hd->buf[len] = '\0';

  return (char *)hd->buf;
}

// T = O(n)
void sdsfree(sds s)
{
  if (NULL == s) return;
  zfree(s-sizeof(struct sdshdr));
}
