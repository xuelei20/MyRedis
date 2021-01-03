#include "../src/adlist.h"
#include "../src/sds.h"

#include <stdio.h>
#include <assert.h>
#include <string.h>

void printListSds(list *lst)
{
  listIter *it = listGetIterator(lst, AL_START_HEAD);
  listNode *node;
  while ((node = listNext(it)) != NULL) {
    printf("%s,", (const char*)listNodeValue(node));
  }
  listReleaseIterator(it);
  printf("\n");
}

void printListSds_r(list *lst)
{
  listIter *it = listGetIterator(lst, AL_START_TAIL);
  listNode *node;
  while ((node = listNext(it)) != NULL) {
    printf("%s,", (const char *)listNodeValue(node));
  }
  listReleaseIterator(it);
  printf("\n");
}

void *dupSdsFunc(void *ptr)
{
  return sdsdup((sds)ptr);
}

void *freeSdsFunc(void *ptr)
{
  printf("free sds:%s\n", (sds)ptr);
  sdsfree((sds)ptr);
  return NULL;
}

void *matchSdsFunc(void *ptr, void *key)
{
  if (strcmp(ptr, key) == 0) {
    return (void *)1;
  } else {
    return (void *)0;
  }
}

int main()
{
  printf("sizeof:%ld\n", sizeof(listNode));
  printf("sizeof:%ld\n", sizeof(list));
  // list lTest;
  // printf("init, head:%p, len:%ld, match:%p\n", lTest.head, lTest.len, lTest.match);

  list *lName = listCreate();
  listAddNodeHead(lName, sdsnew("li3"));
  listAddNodeHead(lName, sdsnew("wang2"));
  listAddNodeHead(lName, sdsnew("zhang1"));
  assert(listLength(lName) == 3);
  printListSds(lName);

  listAddNodeTail(lName, sdsnew("zhao4"));
  assert(listLength(lName) == 4);
  printListSds(lName);

  listInsertNode(lName, listLast(lName), sdsnew("xue5"), 1);
  assert(listLength(lName) == 5);
  printListSds(lName);

  listInsertNode(lName, listFirst(lName), sdsnew("sun0"), 0);
  assert(listLength(lName) == 6);
  printListSds(lName);

  listInsertNode(lName, listFirst(lName)->next, sdsnew("after1.5"), 1);
  assert(listLength(lName) == 7);
  printListSds(lName);

  listInsertNode(lName, listLast(lName), sdsnew("before4.5"), 0);
  assert(listLength(lName) == 8);
  printListSds(lName);
  printListSds_r(lName);

  listSetMatchMethod(lName, matchSdsFunc);
  listNode *zhaoNode = listSearchKey(lName, "zhao4");
  assert(zhaoNode != NULL);
  listNode *prevNode = listNodePrev(zhaoNode);
  printf("prev:%s\n", (sds)listNodeValue(prevNode));

  listNode *node = listIndex(lName, 2);
  printf("node2:%s\n", (const char *)listNodeValue(node));
  node = listIndex(lName, -1);
  printf("node-1:%s\n", (const char *)listNodeValue(node));

  listSetFreeMethod(lName, freeSdsFunc);
  listDelNode(lName, zhaoNode);
  assert(listLength(lName) == 7);
  printListSds(lName);
  printListSds_r(lName);

  printf("----listDup----\n");
  listSetDupMethod(lName, dupSdsFunc);
  list *lDup = listDup(lName);
  assert(listLength(lDup) == 7);
  printListSds(lDup);
  printListSds_r(lDup);

  listRelease(lName);
  listRelease(lDup);

  return 0;
}

