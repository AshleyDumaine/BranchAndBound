#ifndef __PRIORITYQUEUE_H
#define __PRIORITYQUEUE_H

typedef struct PQNode {
  struct PQNode* _next;
  int _key;
  int _value;
  int _cap;
  int _ub;
  int _lb;
  pthread_rwlock_t _lock;
} PQNode;

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

