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
  int _value;
  int _cap;
  int _leftDone;
  int _rightDone;
  double _ub;
  struct LBound* _lb;
  int _index;
  struct PQNode* _left;
  struct PQNode* _right;
} PQNode;

typedef struct LBound {
  pthread_mutex_t _lock;
  int _lb;
} LBound;

typedef struct priorityQueue {
  int _mxs;
  int _head;
  int _tail;
  int _sz;
  int _isDone;
  int _awakeThreads;
  PQNode** _n;
  Item** _itArrayptr;
  int _arraylength;
  pthread_mutex_t _lock;
  pthread_cond_t _cond;
} PQueue;

heap_t* makeQueue(long mxs);
void  destroyQueue(heap_t* theQueue);
void enQueue(heap_t* theQueue, PQNode* n);
PQNode* deQueue(heap_t* theQueue);
int isEmpty(heap_t* theQueue);
#endif

