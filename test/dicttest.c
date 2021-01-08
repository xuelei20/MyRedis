#include "../src/dict.h"
#include "../src/sds.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

unsigned int strHashFunc(const void *key)
{
  return strlen((const char*)key);
}

void *sdsKeyDupFunc(void *privdata, const void *value)
{
  // TODO
  return NULL;
}

typedef struct Student {
  int age;
  double score;
} Student;

void printHashTable(dict *d)
{
  dictht *ht = &d->ht[0];
  for (int i=0; i<ht->size; i++) {
    if (ht->table[i] != NULL) {
      Student *stu = (Student *)&ht->table[i]->v.val;
      printf("%s:%d\n", (const char*)ht->table[i]->key, stu->age);
    }
  }
}


int main()
{
  dictType type;
  type.hashFunction = strHashFunc;
  // type.keyDup = sdsKeyDupFunc;

  dict *d = dictCreate(&type, NULL);

  Student stu1;
  stu1.age = 23;
  stu1.score = 93.5;
  dictAdd(d, "xiaoming", &stu1);

  printHashTable(d);

  return 0;
}
