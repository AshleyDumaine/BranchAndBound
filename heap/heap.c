#include "heap.h"
#include <errno.h>
#include <string.h>
#include <stdlib.h>
/*                                                                              
 * Macros for accessing left/right/parent indices for a heap node               
 */
#define HEAP_LEFT(i) (2*i+1)
#define HEAP_RITE(i) (2*i+2)
#define HEAP_PARENT(i) ((i-1)/2)
int heap_compare(void *a, void *b) { return (int)b > (int)a; }
/*                                                                              
 * Allocate and initialize a binary heap of a given size (capacity)             
 */
heap_t *heap_create(int size)
{
  heap_t *h = calloc(1, sizeof(heap_t));
  if (h == NULL) return NULL;
  h->last = -1;
  h->size = size;
  h->data = calloc(size, sizeof(void*));
  if (h->data == NULL) return NULL;
  return h;
}
/*                                                                              
 * Free a heap                                                                  
 */
void heap_free(heap_t *h)
{
  free(h->data);
  free(h);
}
/*                                                                              
 * Resize a heap (increase only)                                                
 */
int heap_resize(heap_t *h, int size)
{
  if (size <= h->size) return -EINVAL;
  void **H = calloc(size, sizeof(void*));
  if (H == NULL) return -ENOMEM;
  memcpy(H, h->data, h->size * sizeof(void*));
  free(h->data);
  h->size = size;
  h->data = H;
  return 0;
}
/*                                                                              
 * Insert an item in a heap                                                     
 */
int heap_insert(heap_t *h, void *item)
{
  if (h->last == h->size) return -ENOMEM;
  if (item == NULL) return -EINVAL;
  int i = ++h->last;
  void **H = h->data;
  /*                                                                            
   * Put the new item at the end of the heap and start up-heap operation        
   */
  H[i] = item;
  while (i > 0 && heap_compare(item, H[HEAP_PARENT(i)])) {
    H[i] = H[HEAP_PARENT(i)];
    H[HEAP_PARENT(i)] = item;
    i = HEAP_PARENT(i);
  }
  return 0;
}
/*                                                                              
 * Remove the root element of a heap and return it                              
 */
void *heap_remove_root(heap_t *h)
{
  void **H = h->data;
  void *temp, *root = H[0];
  int i = 0, left, rite, child;
  if (h->last < 0) return NULL;
  /*                                                                            
   * Replace root item with the last item in the heap                           
   */
  H[0] = H[h->last];
  H[h->last] = NULL;
  h->last--;
  /*                                                                            
   * Down-heap operation:                                                       
   */
  while (1) {
    child = 0; left = HEAP_LEFT(i); rite = HEAP_RITE(i);
    if (rite > h->last) child = left;
    if (left > h->last) break;
    if (child == 0)
      child = heap_compare(H[left], H[rite]) ? left : rite;
    if (heap_compare(H[child], H[i])) {
      temp = H[i];
      H[i] = H[child];
      H[child] = temp;
    }
    i = child;
  }
  return root;
}
int main(int argc, char *argv[])
{
  heap_t *heap = heap_create(12);
  heap_insert(heap, (void*)5);
  heap_insert(heap, (void*)1);
  heap_insert(heap, (void*)3);
  heap_insert(heap, (void*)9);
  heap_insert(heap, (void*)2);
  heap_insert(heap, (void*)3);
  heap_insert(heap, (void*)4);
  heap_insert(heap, (void*)6);
  void * v;
  while (v = heap_remove_root(heap)) {
    printf("%d\n", (int)v);
  }
  return 0;
}

