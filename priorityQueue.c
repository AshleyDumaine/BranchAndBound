#include "heap.h"
#include "priorityQueue.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

heap_t* makeQueue(long mxs) {
  heap_t* h = heap_create(mxs);
  pthread_mutex_init(&h->_lock,NULL);
  pthread_cond_init(&h->_cond,NULL);
  return h;
}

void destroyQueue(heap_t* q) {
  pthread_cond_destroy(&q->_cond);
  pthread_mutex_destroy(&q->_lock);
  heap_free(q);
}

int isEmpty(heap_t* theQueue) {
  return theQueue->last < 0;
}

void enQueue(heap_t* q,PQNode* node) {
  heap_insert(q, (void*)node);
}

PQNode* deQueue(heap_t* q) {
  PQNode* rv = (PQNode*)heap_remove_root(q);
  return rv;
}
