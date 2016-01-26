#include "heap.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros for accessing left/right/parent indices for a heap node             
#define HEAP_LEFT(i) (2*i+1)
#define HEAP_RIGHT(i) (2*i+2)
#define HEAP_PARENT(i) ((i-1)/2)

int heap_compare(void *a, void *b) { 
  return ((PQNode*)b)->_ub - ((PQNode*)a)->_ub;
}
                                                                              
heap_t *heap_create(long size) {
  heap_t *h = (heap_t *) calloc(1, sizeof(heap_t));
  if (!h) return NULL;
  h->last = -1;
  h->size = size;
  h->data = calloc(size, sizeof(void*));
  if (!(h->data)) return NULL;
  return h;
}

void heap_free(heap_t *h) {
  free(h->data);
  free(h);
}

int isEmpty(heap_t* theQueue) {
  return theQueue->last < 0;
}

int heap_resize(heap_t *h, long size) {
  if (size <= h->size) return -1;
  void **H = (void **) calloc(size, sizeof(void*));
  if (!H) {
    printf("OUT OF MEMORY\n");
    exit(-1);
  }
  memcpy(H, h->data, h->size * sizeof(void*));
  free(h->data);
  h->size = size;
  h->data = H;
  return 0;
}

int heap_insert(heap_t *h, void *item) {
  if (h->last == h->size || !item) return -1;
  long i = ++h->last;
  void **H = (void **) h->data;
  H[i] = item;
  while (i > 0 && heap_compare(item, H[HEAP_PARENT(i)])) {
    H[i] = H[HEAP_PARENT(i)];
    H[HEAP_PARENT(i)] = item;
    i = HEAP_PARENT(i);
  }
  return 0;
}

void *heap_remove_root(heap_t *h) {
  void **H = (void **) h->data;
  void *temp, *root = H[0];
  long i = 0, left, right, child;
  if (h->last < 0) return NULL;
  H[0] = H[h->last];
  H[h->last] = NULL;
  h->last--;
  while (1) {
    child = 0;
    left = HEAP_LEFT(i);
    right = HEAP_RIGHT(i);
    if (right > h->last) child = left;
    if (left > h->last) break;
    if (child == 0) child = heap_compare(H[left], H[right]) ? left : right;
    if (heap_compare(H[child], H[i])) {
      temp = H[i];
      H[i] = H[child];
      H[child] = temp;
    }
    i = child;
  }
  return root;
}
