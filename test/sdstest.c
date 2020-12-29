#include "../src/sds.h"

#include <stdio.h>

int main() {
  struct sdshdr s1;
  printf("size:%ld\n", sizeof(struct sdshdr));
  printf("size:%ld\n", sizeof(s1));
  return 0;
}
