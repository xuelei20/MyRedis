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
  int rehashidx; // rehashing not in progress if rehashidx == -1
  int iterators; // number of iterators currently running
} dict;

#endif // _XL_
