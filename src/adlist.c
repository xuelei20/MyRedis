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
