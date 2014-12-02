#ifndef __HEAP_H__
#define __HEAP_H__

#include <pthread.h>

typedef struct Item {
  double _ratio;
  unsigned long _weight;
  unsigned long _profit;
} Item;



typedef struct heap_t_ {
  long last;
  long size;
  void *data;

  int _isDone;
  int _awakeThreads;
  Item** _itArrayptr;
  unsigned long _arraylength;
  pthread_mutex_t _lock;

  //  pthread_spinlock_t _spin;
  pthread_cond_t _cond;

} heap_t;
#endif


heap_t *heap_create(unsigned long size);
void heap_free(heap_t *h);
int heap_resize(heap_t *h, unsigned long size);
int heap_insert(heap_t *h, void *item);
void *heap_remove_root(heap_t *h);




