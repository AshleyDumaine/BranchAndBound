#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

typedef struct PQNode {
  struct PQNode* _next;
  double _ratio;
  int _key;
  int _value;
  int _cap;
  int _leftDone;
  int _rightDone;
  double _ub;
  struct LBound* _lb;
} PQNode;

typedef struct LBound {
  pthread_rwlock_t _lock;
  int _lb;
} LBound;

typedef struct priorityQueue {
  int _msx;
  int _head;
  int _tail;
  int _sz;
  Node** _n;
} PQueue;

PQueue* makeQueue(int mxs);
void  destroyQueue(PQueue* theQueue);
void enQueue(PQueue* theQueue, Node* n);
Node* deQueue(PQueue* theQueue);
int isEmpty(PQueue* theQueue);
#endif

