#ifndef _XL_ADLIST_H
#define _XL_ADLIST_H

typedef struct listNode {
  struct listNode *prev;
  struct listNode *next;
  void *value;
} listNode;

typedef struct listIter {
  listNode *next;
  int direction;
} listIter;

typedef struct list {
  listNode *head;
  listNode *tail;
  unsigned long len;
  void *(*dup)(void *ptr);
  void *(*free)(void *ptr);
  void *(*match)(void *ptr, void *key);
} list;

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))
#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)
#define listPrevNode(n) ((n)->prev)
#define listNextNode(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

list *listCreate();
list *listAddNodeHead(list *lst, void *value);
list *listAddNodeTail(list *lst, void *value);
list *listInsertNode(list *lst, listNode *oldNode, void *value, int after);

#endif // _XL_
