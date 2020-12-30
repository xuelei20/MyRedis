#include "../src/sds.h"

#include <stdio.h>

int main(int argc, char *argv[]) {
  struct sdshdr sh1;
  printf("size:%ld\n", sizeof(struct sdshdr));

  sds sname = sdsnew("hello sds");
  printf("\"%s\" len:%ld, avail:%ld\n", sname, sdslen(sname), sdsavail(sname));
  sdsfree(sname); // when you free, can't be used anymore

  return 0;
}
