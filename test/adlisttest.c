#include "../src/adlist.h"
#include "../src/sds.h"

#include <stdio.h>
#include <assert.h>

void printListSds(list *lst)
{
  for (listNode *n=listFirst(lst); n!=NULL; n=listNextNode(n))
  {
    printf("%s,", (const char*)listNodeValue(n));
  }
  printf("\n");
}

int main()
{
  listNode node;
  printf("sizeof:%ld\n", sizeof(listNode));
  printf("sizeof:%ld\n", sizeof(list));

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
}

