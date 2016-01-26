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

typedef struct PQNode {
  long _value, _cap, _index;
  double _ub;
  int _leftDone, _rightDone;
  struct LBound* _lb;
  struct PQNode* _left;
  struct PQNode* _right;
} PQNode;

typedef struct LBound {
  long _lb;
  pthread_spinlock_t _lock;
} LBound;

heap_t *heap_create(long size);
void heap_free(heap_t *h);
int isEmpty(heap_t* theQueue);
int heap_resize(heap_t *h, long size);
int heap_insert(heap_t *h, void *item);
void *heap_remove_root(heap_t *h);
#endif
