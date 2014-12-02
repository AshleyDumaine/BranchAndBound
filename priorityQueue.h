#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H
#include <pthread.h>

#include "heap.h"
/*
typedef struct Item {
  double _ratio;
  int _weight;
  int _profit;
} Item;
*/

typedef struct PQNode {
  unsigned long _value;
  unsigned long _cap;
  int _leftDone;
  int _rightDone;
  double _ub;
  struct LBound* _lb;
  unsigned long _index;
  struct PQNode* _left;
  struct PQNode* _right;
} PQNode;

typedef struct LBound {
  pthread_spinlock_t _lock;
  unsigned long _lb;
} LBound;

typedef struct priorityQueue {
  unsigned long _mxs;
  int _head;
  unsigned long _tail;
  unsigned long _sz;
  int _isDone;
  int _awakeThreads;
  PQNode** _n;
  Item** _itArrayptr;
  unsigned long _arraylength;
  pthread_mutex_t _lock;
  pthread_cond_t _cond;
} PQueue;

heap_t* makeQueue(long mxs);
void  destroyQueue(heap_t* theQueue);
void enQueue(heap_t* theQueue, PQNode* n);
PQNode* deQueue(heap_t* theQueue);
int isEmpty(heap_t* theQueue);
#endif

