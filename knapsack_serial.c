#include <stdio.h>
#include "priorityQueue.h"
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include "heap.h"

LBound* lb; //lower bound global
//pthread_mutex_t mtx;
PQNode* deQueueWork(heap_t* twq);
void enQueueWork(heap_t* twq,PQNode* t);
void pathtranverse(heap_t* theQueue, PQNode* original);

int compare (const void* a, const void* b) {
  const Item** arg0  = (const Item**)a;
  const Item** arg1 = (const Item**)b;
  if((*arg0)->_ratio < (*arg1)->_ratio)return -1;
  else if ((*arg0)->_ratio > (*arg1)->_ratio) return 1;
  else return 0;
}

PQNode* deQueueWork(heap_t* twq)
{
  if (isEmpty(twq))return NULL; 
  PQNode* n = deQueue(twq);
  return n;
}

void enQueueWork(heap_t* twq,PQNode* t)
{
  enQueue(twq,t);
}

void calculateUpperBound(Item** itemArray,PQNode* node, unsigned long len) {
  long i, cap = node->_cap, index = node->_index;
  double value = (double)node->_value;
  for (i = index; i >= 0; i--) {
    Item* item = itemArray[i];
    long diff = cap - item->_weight;
    if(diff > 0){
      cap = diff;
      value = value + item->_profit;
    }
    else if(diff == 0){
      cap = diff;
      value = value + item->_profit;
      break;
    }
    else if(diff < 0){
      double div = ((double)cap/(double)(item->_weight));
      double newp = div*((double)(item->_profit));
      value = value + newp;
      break;
    }
  }
  node->_ub = value;
}

void* bb(void* SQueue) {
  heap_t* theQueue = (heap_t*)SQueue;
  while(1){
    PQNode* original = deQueueWork(theQueue);
    if (original==NULL)return;
    Item** itemArray = theQueue->_itArrayptr;    
    if(original->_index == 0){
      if(original->_cap >= itemArray[0]->_weight)
	original->_value = itemArray[0]->_profit + original->_value;
      if(original->_value > lb->_lb)
	lb->_lb = original->_value;
      free(original);
    }
    else if(original->_cap == 0){
      if(original->_value > lb->_lb)lb->_lb= original->_value;
      free(original);
    }
    else
      pathtranverse(theQueue, original);
    if(isEmpty(theQueue)) continue;
  }
}

void pathtranverse(heap_t* theQueue, PQNode* original){
  Item** itemArray = theQueue->_itArrayptr;
  while (original->_cap != 0 && original->_index != 0) {
    unsigned long index = original->_index;
    calculateUpperBound(itemArray,original,theQueue->_arraylength);
    if(original->_ub < lb->_lb) break;
    original->_left = (PQNode*)malloc(sizeof(PQNode));
    original->_right = (PQNode*)malloc(sizeof(PQNode));
    PQNode* left = original->_left;
    PQNode* right = original->_right;
    right->_lb = lb;
    left->_lb = lb;
    right->_index = (original->_index) - 1;
    right->_cap = original->_cap;
    right->_value = original->_value;
    calculateUpperBound(itemArray,right,theQueue->_arraylength);
    if(right->_ub > lb->_lb) enQueueWork(theQueue,right);
    else free(right);
    left->_index = (original->_index) - 1;
    left->_cap =  original->_cap - itemArray[index]->_weight;
    left->_value = itemArray[index]->_profit + original->_value;
    calculateUpperBound(itemArray, left, theQueue->_arraylength);
    if(left->_cap < 0){
      free(left);
      break;
    }
    if(original->_cap < itemArray[index]->_weight){
      if(lb->_lb < original->_value)
	lb->_lb = original->_value;
      break;
    }
    if(left->_ub < lb->_lb){
      free(left);
      break;
    }
    PQNode* temp = original;
    free(temp);
    original = left;   
  }
  if(original->_value >= lb->_lb)
    lb->_lb= original->_value;
  free(original);
  return;
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "usage: knapsack_serial filename\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp; 
  fp = fopen(filename, "r");
  unsigned long len, i, capacity;
  fscanf(fp,"%lu",&len);
  //number is useless, we just need it as a parameter for scanning
  unsigned long number,weight,profit;
  Item** itemArray = (Item**)malloc(sizeof(Item*)*len);
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    fscanf(fp,"%lu %lu %lu", &number, &profit, &weight);
    double ratio = (double)profit/(double)weight;
    n->_weight = weight;
    n->_profit = profit;
    n->_ratio = ratio;
    itemArray[i] = n;
  }
  //get max capacity
  fscanf(fp,"%lu",&capacity);
  qsort(itemArray, len, sizeof(Item*),compare); 
  fclose(fp);
  //the main part of the assignment
  int nthreads = 1;
  int status;
  void* exitStatus;
  pthread_t threads[nthreads];
  heap_t* sharedQ = makeQueue(len*10);
  sharedQ->_isDone = 0;
  sharedQ->_itArrayptr = itemArray;
  sharedQ->_arraylength = len;
  PQNode* startnode = (PQNode*)malloc(sizeof(PQNode));
  //pthread_mutex_init(&mtx, NULL);
  lb  = (LBound*)malloc(sizeof(LBound));
  //pthread_rwlock_init(&lb->_lock, NULL);
  lb->_lb = 0;
  startnode->_lb  = lb;
  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;
  enQueueWork(sharedQ, startnode);
  clock_t tic = clock();
  for(i=0;i<nthreads;i++){
    status = pthread_create(&threads[i],NULL,bb,(void*)sharedQ);
    if(status){
      printf("ERROR; return code from pthread_create() is %d\n", status);
      exit(-1);
    }
  }
  for(i=0;i<nthreads;i++)
    pthread_join(threads[i],&exitStatus);
   printf("lower bound is: %lu \n",lb->_lb);
  clock_t toc = clock();
  printf("Elapsed: %f seconds \n", (double)(toc-tic)/CLOCKS_PER_SEC);
  destroyQueue(sharedQ);
  //pthread_mutex_destroy(&mtx);
  //pthread_rwlock_destroy(&lb->_lock);
  free(lb);
  for (i = 0; i< len; i++)
    free(itemArray[i]);
  free(itemArray);
  return 0;
}
