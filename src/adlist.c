#include "adlist.h"
#include "zmalloc.h"

#include <stdlib.h>
#include <assert.h>

list *listCreate()
{
  list *lst = zmalloc(sizeof(list));
  if (NULL == lst)
    return NULL;

  lst->head = lst->tail = NULL;
  lst->len = 0;
  lst->dup = NULL;
  lst->free = NULL;
  lst->match = NULL;

  return lst;
}

list *listAddNodeHead(list *lst, void *value)
{
  assert(lst != NULL);

  listNode *node = zmalloc(sizeof(listNode));
  if (NULL == node)
    return NULL;

  node->value = value;
  if (0 == lst->len) {
    node->prev = node->next = NULL;
    lst->head = lst->tail = node;
  } else {
    node->prev = NULL;
    node->next = lst->head;
    lst->head->prev = node;
    lst->head = node;
  }
  lst->len++;

  return lst;
}

list *listAddNodeTail(list *lst, void *value)
{
  assert(lst != NULL);

  listNode *node = zmalloc(sizeof(listNode));
  if (NULL == node)
    return NULL;

  node->value = value;
  if (0 == lst->len) {
    node->prev = node->next = NULL;
    lst->head = lst->tail = node;
  } else {
    node->prev = lst->tail;
    node->next = NULL;
    lst->tail->next = node;
    lst->tail = node;
  }
  lst->len++;

  return lst;
}

// T = O(1)
list *listInsertNode(list *lst, listNode *oldNode, void *value, int after)
{
  assert(lst != NULL);
  assert(oldNode != NULL);

  listNode *node = zmalloc(sizeof(listNode));
  if (NULL == node)
    return NULL;

  node->value = value;
  if (after) {
    node->prev = oldNode;
    node->next = oldNode->next;
    if (oldNode->next != NULL) {
      oldNode->next->prev = node;
    } else {
      lst->tail = node;
    }
    oldNode->next = node;
  } else {
    node->prev = oldNode->prev;
    node->next = oldNode;
    if (oldNode->prev != NULL) {
      oldNode->prev->next = node;
    } else {
      lst->head = node;
    }
    oldNode->prev = node;
  }
  lst->len++;

  return lst;
}

listIter *listGetIterator(list *lst, int direction)
{
  assert(lst != NULL);

  listIter *it = zmalloc(sizeof(listIter));
  if (NULL == it)
    return NULL;

  if (AL_START_HEAD == direction) {
    it->next = lst->head;
  } else {
    it->next = lst->tail;
  }
  it->direction = direction;

  return it;
}

/*
 * listIter *iter = listGetIterator(list,<direction>);
 * listNode *node;
 * while ((node = listNext(iter)) != NULL) {
 *     doSomethingWith(listNodeValue(node));
 * }
 * listReleaseIterator(iter);
 *
 * T = O(1)
 */
listNode *listNext(listIter *it)
{
  assert(it != NULL);

  listNode *cur = it->next;
  if (cur != NULL) {
    if (it->direction == AL_START_HEAD) {
      it->next = cur->next;
    } else {
      it->next = cur->prev;
    }
  }

  return cur;
}

void listReleaseIterator(listIter *it)
{
  zfree(it);
}

// T = O(n)
listNode *listSearchKey(list *lst, void *key)
{
  assert(lst != NULL);

  listIter *it = listGetIterator(lst, AL_START_HEAD);
  listNode *node;
  while ((node = listNext(it)) != NULL) {
    if (lst->match) { // user match
      if (lst->match(node->value, key)) {
        listReleaseIterator(it);
        return node;
      }
    } else {
      if (node->value == key) {
        listReleaseIterator(it);
        return node;
      }
    }
  }

  listReleaseIterator(it);
  return NULL;
}

// @index: 0-from head, to next. -1-from tail, to prev
listNode *listIndex(list *lst, long index)
{
  assert(lst != NULL);

  listNode *node;
  if (index < 0) {
    index = (-index)-1;
    node = lst->tail;
    while (node && index--)
      node = node->prev;
  } else {
    node = lst->head;
    while (node && index--)
      node = node->next;
  }

  return node;
}

// T = O(1)
void listDelNode(list *lst, listNode *node)
{
  assert(lst != NULL);
  assert(node != NULL);

  if (node->prev)
    node->prev->next = node->next;
  else
    lst->head = node->next;

  if (node->next)
    node->next->prev = node->prev;
  else
    lst->tail = node->prev;

  if (lst->free)
    lst->free(node->value);

  zfree(node);
  lst->len--;
}

list *listDup(list *orig)
{
  if (NULL == orig)
    return NULL;

  list *copy = listCreate();
  if (NULL == copy)
    return NULL;
  copy->dup = orig->dup;
  copy->free = orig->free;
  copy->match = orig->match;

  listIter *it = listGetIterator(orig, AL_START_HEAD);
  listNode *node;
  void *value;
  while ((node = listNext(it)) != NULL) {
    if (orig->dup) {
      value = orig->dup(node->value); // user dup
      if (NULL == value) {
        listReleaseIterator(it);
        listRelease(copy);
        return NULL;
      }
    } else {
      value = node->value; // share same ptr
    }

    copy = listAddNodeTail(copy, value);
    if (NULL == copy) {
      listReleaseIterator(it);
      listRelease(copy);
      return NULL;
    }
  }

  listReleaseIterator(it);
  return copy;
}

void listRelease(list *lst)
{
  if (NULL == lst)
    return;

  unsigned long len = lst->len;
  listNode *cur = lst->head;
  listNode *next;
  while (len--) {
    next = cur->next; // save before free
    if (lst->free)
      lst->free(cur->value);
    zfree(cur);
    cur = next;
  }
  zfree(lst);
}
