#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H
#include <pthread.h>

#include "heap.h"
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

typedef struct priorityQueue {
  long _mxs, _tail, _sz, _arraylength;
  int _head, _isDone, _awakeThreads;
  PQNode** _n;
  Item** _itArrayptr;
  pthread_mutex_t _lock;
  pthread_cond_t _cond;
} PQueue;

heap_t* makeQueue(long mxs);
void  destroyQueue(heap_t* theQueue);
void enQueue(heap_t* theQueue, PQNode* n);
PQNode* deQueue(heap_t* theQueue);
int isEmpty(heap_t* theQueue);
#endif
