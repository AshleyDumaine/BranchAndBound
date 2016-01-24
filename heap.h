#ifndef __HEAP_H__
#define __HEAP_H__

#include <pthread.h>

typedef struct Item {
  double _ratio;
  long _weight, _profit;
} Item;

typedef struct heap_t_ {
  long last, size, _arraylength;
  void *data;
  int _isDone, _awakeThreads;
  Item** _itArrayptr;
  pthread_mutex_t _lock;
  pthread_cond_t _cond;
} heap_t;

heap_t *heap_create(long size);
void heap_free(heap_t *h);
int heap_resize(heap_t *h, long size);
int heap_insert(heap_t *h, void *item);
void *heap_remove_root(heap_t *h);
#endif
