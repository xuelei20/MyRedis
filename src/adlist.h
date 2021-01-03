// a double linked list
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

#define listNodePrev(n) ((n)->prev)
#define listNodeNext(n) ((n)->next)
#define listNodeValue(n) ((n)->value)

#define listSetDupMethod(l,m) ((l)->dup = (m))
#define listSetFreeMethod(l,m) ((l)->free = (m))
#define listSetMatchMethod(l,m) ((l)->match = (m))
#define listGetDupMethod(l) ((l)->dup)
#define listGetFreeMethod(l) ((l)->free)
#define listGetMatchMethod(l) ((l)->match)
#define listLength(l) ((l)->len)
#define listFirst(l) ((l)->head)
#define listLast(l) ((l)->tail)

list *listCreate();
list *listAddNodeHead(list *lst, void *value);
list *listAddNodeTail(list *lst, void *value);
list *listInsertNode(list *lst, listNode *oldNode, void *value, int after);
listNode *listSearchKey(list *lst, void *key);
listNode *listIndex(list *lst, long index);
void listDelNode(list *lst, listNode *node);
list *listDup(list *orig);
void listRelease(list *lst);

listIter *listGetIterator(list *lst, int direction);
listNode *listNext(listIter *it);
void listReleaseIterator(listIter *it);

#define AL_START_HEAD 0
#define AL_START_TAIL 1

#endif // _XL_
