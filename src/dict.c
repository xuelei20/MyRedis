#include "dict.h"
#include "zmalloc.h"

#include <stdlib.h>
#include <assert.h>
#include <limits.h>

static int dict_can_resize = 1;
static unsigned int dict_force_resize_ratio = 5;

static void _dictReset(dictht *ht)
{
  assert(ht != NULL);

  ht->table = NULL;
  ht->size = 0;
  ht->sizemask = 0;
  ht->used = 0;
}

static int _dictInit(dict *d, dictType *type, void *privdata)
{
  assert(d != NULL);
  assert(type != NULL);

  _dictReset(&d->ht[0]);
  _dictReset(&d->ht[1]);
  d->type = type;
  d->privdata = privdata;
  d->rehashidx = -1;
  d->iterators = 0;

  return DICT_OK;
}

static void _dictRehashStep(dict *d) 
{
  assert(d != NULL);
}

static int _dictExpandIfNeeded(dict *d)
{
  assert(d != NULL);

  if (dictIsRehashing(d))
    return DICT_OK;

  if (d->ht[0].size == 0)
    return dictExpand(d, DICT_HT_INITIAL_SIZE);

  if (d->ht[0].used >= d->ht[0].size && 
      (dict_can_resize || d->ht[0].used / d->ht[0].size > dict_force_resize_ratio)) 
  {
    return dictExpand(d, d->ht[0].used * 2); // double at least
  }

  return DICT_OK;
}

// T = O(1)
static int _dictKeyIndex(dict *d, const void *key)
{
  assert(d != NULL);
  assert(key != NULL);

  if (_dictExpandIfNeeded(d) == DICT_ERR)
    return -1;

  unsigned int hash = dictHashKey(d, key); // hash value
  unsigned int index;
  for (int i=0; i<=1; ++i)
  {
    index = hash & d->ht[i].sizemask; // hash table index
    dictEntry *entry = d->ht[i].table[index];
    while (entry) {
      if (dictCompareKeys(d, key, entry->key))
        return -1; // if key exist, return -1
      entry = entry->next;
    }

    if (!dictIsRehashing(d))
      break;
  }

  return index;
}

static unsigned long _dictNextPower(unsigned long size)
{
  if (size >= LONG_MAX)
    return LONG_MAX;

  unsigned long i = DICT_HT_INITIAL_SIZE;
  while (i<size) {
    i *= 2; // 2^n
  }
  return i;
}

dict *dictCreate(dictType *type, void *privdata)
{
  assert(type != NULL);

  dict *d = zmalloc(sizeof(dict));
  _dictInit(d, type, privdata);

  return d;
}

int dictAdd(dict *d, void *key, void *value)
{
  assert(d != NULL);
  assert(key != NULL);

  dictEntry *entry = dictAddRaw(d, key);
  if (!entry)
    return DICT_ERR;

  dictSetVal(d, entry, value);
  return DICT_OK;
}

dictEntry *dictAddRaw(dict *d, void *key)
{
  assert(d != NULL);
  assert(key != NULL);

  if (dictIsRehashing(d))
    _dictRehashStep(d);

  int index;
  if ((index = _dictKeyIndex(d, key)) == -1)
    return NULL;

  dictEntry *entry = zmalloc(sizeof(dictEntry));
  dictht *ht = (dictIsRehashing(d) ? &d->ht[1] : &d->ht[0]);
  dictSetKey(d, entry, key);
  entry->next = ht->table[index];
  ht->table[index] = entry;
  ht->used++;

  return entry;
}

int dictExpand(dict *d, unsigned long size)
{
  assert(d != NULL);

  if (dictIsRehashing(d) || size < d->ht[0].used)
    return DICT_ERR;

  dictht ht; // new table
  unsigned long realsize = _dictNextPower(size);
  ht.table = zmalloc(realsize*sizeof(dictEntry*));
  ht.size = realsize;
  ht.sizemask = realsize-1;
  ht.used = 0;

  if (d->ht[0].size == 0) { // first use 
    d->ht[0] = ht;
  } else {
    d->ht[1] = ht;
    d->rehashidx = 0;
  }
  return DICT_OK;
}
