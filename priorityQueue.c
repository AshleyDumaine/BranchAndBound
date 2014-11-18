#include "priorityQueue.h"
#include <stdlib>

PQueue* makeQueue(int mxs) {
  PQueue* q = (PQueue*)malloc(sizeof(PQueue));
  q->_mxs = mxs;
  q->_head = q->_tail = q->_sz = 0;
  q->_n = (Node**)malloc(sizeof(Node*)*mxs);
  return q;
}

void destroyQueue(PQueue* q) {
  int size = q->_sz;
  int i;
  for (i = 0; i < size; i++) {
    pthread_rwlock_destroy(&q->n[i]->_lock);
  }
  free(q->_n);
  free(q);
}
int isEmpty(PQueue* theQueue)
{
  return theQueue->_sz == 0;
}

void enQueue(PQueue* q,Node* node)
{
  if (q->_sz == q->_mxs) {
    int mxs = q->_mxs * 2;
    Node** n = malloc(sizeof(Node*)*mxs);
    int i = q->_sz;
    int j = 0;
    for(; i > 0; i--) {
      n[j++] = q->_n[q->_head];
      q->_head = q->_head < q->_mxs-1 ? q->_head+1 : 0;
    }
    q->_head = 0;
    q->_tail = j;
    for (j = 0; j < i; j++) {
      pthread_rwlock_destroy(&q->n[j]->_lock);
    }
    free(q->_n);
    q->_n = n;
    q->_mxs = mxs;
  }
  q->_sz++;
  q->_n[q->_tail] = node;
  q->_tail = q->_tail < q->_mxs - 1 ? q->_tail + 1 : 0;
}

Node* deQueue(PQueue* q)
{
  Node* rv = q->_n[q->_head];
  q->_n[q->_head] = NULL;
  q->_head = q->_head < q->_mxs-1 ? q->_head+1 : 0;
  q->_sz--;
  return rv;
}
