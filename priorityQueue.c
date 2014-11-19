#include "priorityQueue.h"
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
PQueue* makeQueue(int mxs) {
  PQueue* q = (PQueue*)malloc(sizeof(PQueue));
  q->_mxs = mxs;
  q->_head = q->_tail = q->_sz = 0;
  q->_n = (PQNode**)malloc(sizeof(PQNode*)*mxs);
  pthread_mutex_init(&q->_lock,NULL);
  pthread_cond_init(&q->_condLock,NULL);
  return q;
}

void destroyQueue(PQueue* q) {
  int size = q->_sz;
  int i;
  for (i = 0; i < size; i++) {
    free(q->_n[i]);
    //pthread_rwlock_destroy(&q->_n[i]->_lock);
  }
  free(q->_n);
  free(q);
}
int isEmpty(PQueue* theQueue)
{
  return theQueue->_sz == 0;
}

void enQueue(PQueue* q,PQNode* node)
{
  if (q->_sz == q->_mxs) {
    int mxs = q->_mxs * 2;
    PQNode** nt = malloc(sizeof(PQNode*)*mxs);
    int i = q->_sz;
    int j = 0;
    for(; i > 0; i--) {
      nt[j++] = q->_n[q->_head];
      q->_head = q->_head < q->_mxs-1 ? q->_head+1 : 0;
    }
    q->_head = 0;
    q->_tail = j;
    
    for (j = 0; j < i; j++) {
      free(q->_n[j]);
      //pthread_rwlock_destroy(&q->_n[j]->_lock);
    }

    free(q->_n);
    q->_n = nt;
    q->_mxs = mxs;
    
  }
  q->_sz++;
  q->_n[q->_tail] = node;
  q->_tail = q->_tail < q->_mxs - 1 ? q->_tail + 1 : 0;
}

PQNode* deQueue(PQueue* q)
{
  PQNode* rv = q->_n[q->_head];
  q->_n[q->_head] = NULL;
  q->_head = q->_head < q->_mxs-1 ? q->_head+1 : 0;
  q->_sz--;
  return rv;
}
