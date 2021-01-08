// dict -> hash table -> Entry*[]
#ifndef _XL_DICT_H
#define _XL_DICT_H

#include <stdint.h>

typedef struct dictEntry {
  void *key;
  union {
    void *val;
    uint64_t u64;
    int64_t s64;
  } v;
  struct dictEntry *next; // separate chaining
} dictEntry;

typedef struct dictType {
  unsigned int (*hashFunction)(const void *key); // to generate hash value
  void *(*keyDup)(void *privdata, const void *key);
  void *(*valDup)(void *privdata, const void *obj);
  int (*keyCompare)(void *privdata, const void *key1, const void *key2);
  void (*keyDestructor)(void *privdata, void *key);
  void (*valDestructor)(void *privdata, void *obj);
} dictType;

typedef struct dictht {
  dictEntry **table;
  unsigned long size;
  unsigned long sizemask; // always == size-1, to compute index( = hash&sizemask)
  unsigned long used;
} dictht;

typedef struct dict {
  dictType *type;
  void *privdata;
  dictht ht[2];  // ht[0] for hashtable, ht[1] for rehash
  int rehashidx; // if it's not rehashing, rehashidx == -1
  int iterators; // number of iterators currently running
} dict;

typedef struct dictIterator {
  dict *d;
  int table, index, safe;
  dictEntry *entry, *nextEntry;
  long long fingerprint;
} dictIterator;

#define DICT_OK 0
#define DICT_ERR 1

#define DICT_HT_INITIAL_SIZE     4

#define dictSetKey(d, entry, _key_) \
  do { \
    if ((d)->type->keyDup) \
      (entry)->key = (d)->type->valDup((d)->privdata, (_key_)); \
    else \
      (entry)->key = (_key_); \
  } while(0)

#define dictSetVal(d, entry, _val_) \
  do { \
    if ((d)->type->valDup) \
      (entry)->v.val = (d)->type->valDup((d)->privdata, (_val_)); \
    else \
      (entry)->v.val = (_val_); \
  } while(0)

#define dictIsRehashing(d) ((d)->rehashidx != -1)
#define dictHashKey(d, key) (d)->type->hashFunction((key))

#define dictCompareKeys(d, key1, key2) \
  (((d)->type->keyCompare) ? \
    (d)->type->keyCompare((d)->privdata, (key1), (key2)) : \
    (key1) == (key2))


dict *dictCreate(dictType *type, void *privdata);
int dictAdd(dict *d, void *key, void *value);
dictEntry *dictAddRaw(dict *d, void *key);
int dictExpand(dict *d, unsigned long size);

#endif // _XL_
