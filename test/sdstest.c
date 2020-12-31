#include "../src/sds.h"

#include <stdio.h>
#include <assert.h>

int main(int argc, char *argv[]) {
  struct sdshdr sh1;
  printf("size:%ld\n", sizeof(struct sdshdr));

  sds sname = sdsnew("hello sds");
  printf("%s\n", sname);
  assert(sdslen(sname) == 9);
  assert(sdsavail(sname) == 0);

  sds sdup = sdsdup(sname);
  printf("%s\n", sdup);
  assert(sdslen(sdup) == 9);
  assert(sdsavail(sdup) == 0);

  sdsclear(sname);
  printf("%s\n", sname);
  assert(sdslen(sname) == 0);
  assert(sdsavail(sname) == 9);

  sdscat(sname, "newdat");
  printf("%s\n", sname);
  assert(sdslen(sname) == 6);
  assert(sdsavail(sname) == 3);

  sdscat(sname, ",data");
  printf("%s\n", sname);
  assert(sdslen(sname) == 11);
  assert(sdsavail(sname) == 11);

  sdscat(sname, sdup);
  printf("%s\n", sname);
  assert(sdslen(sname) == 20);
  assert(sdsavail(sname) == 2);

  sdsfree(sdup); // when free, can't be used anymore
  sdsfree(sname); // when free, can't be used anymore

  return 0;
}
