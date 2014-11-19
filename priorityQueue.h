#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H
#include <pthread.h>

typedef struct PQNode {
  struct PQNode* _next;
  int _key;
  int _value;
  int _cap;
  int _leftDone;
  int _rightDone;
  double _ub;
  struct LBound* _lb;
  int _index;
} PQNode;

typedef struct LBound {
  pthread_rwlock_t _lock;
  int _lb;
} LBound;

typedef struct priorityQueue {
  int _mxs;
  int _head;
  int _tail;
  int _sz;
  PQNode** _n;
} PQueue;

PQueue* makeQueue(int mxs);
void  destroyQueue(PQueue* theQueue);
void enQueue(PQueue* theQueue, PQNode* n);
PQNode* deQueue(PQueue* theQueue);
int isEmpty(PQueue* theQueue);
#endif

