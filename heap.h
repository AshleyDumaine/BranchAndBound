#ifndef __HEAP_H__
#define __HEAP_H__
typedef struct heap_t_ {
  int last;
  int size;
  void *data;
} heap_t;
#endif

heap_t *heap_create(int size);
void heap_free(heap_t *h);
int heap_resize(heap_t *h, int size);
int heap_insert(heap_t *h, void *item);
void *heap_remove_root(heap_t *h);




