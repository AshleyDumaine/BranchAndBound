#include "heap.h"
#include <algorithm>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>

LBound* lb; //lower bound global
pthread_mutex_t mtx;

bool compare (const Item* a, const Item* b) {
  return a->_ratio < b->_ratio;
}

PQNode* deQueueWork(heap_t* twq) {
  pthread_mutex_lock(&twq->_lock);
  while (isEmpty(twq)) {
    if(twq->_isDone == 0) 
      pthread_cond_wait(&twq->_cond,&twq->_lock);
    else {
      pthread_mutex_unlock(&twq->_lock);
      return NULL; 
    }
  }
  PQNode* n = (PQNode*)heap_remove_root(twq);
  pthread_mutex_unlock(&twq->_lock);
  return n;
}

void enQueueWork(heap_t* twq,PQNode* t) {
  pthread_mutex_lock(&twq->_lock);
  heap_insert(twq,t);
  pthread_cond_signal(&twq->_cond);
  pthread_mutex_unlock(&twq->_lock);
}

void calculateUpperBound(Item** itemArray,PQNode* node, long len) {
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

void pathtranverse(heap_t* theQueue, PQNode* original_x){
  Item** itemArray = theQueue->_itArrayptr;
  heap_t* myQueue = heap_create(theQueue->size);
  heap_insert(myQueue,original_x);
  while(!isEmpty(myQueue)){
    PQNode* original = (PQNode*)heap_remove_root(myQueue);
  while (original->_cap != 0 && original->_index != 0) {
    long index = original->_index;
    calculateUpperBound(itemArray,original,theQueue->_arraylength);
    if(original->_ub < lb->_lb)
      break;
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
    if(right->_ub > lb->_lb){
      if(theQueue->_awakeThreads == 1){
      enQueueWork(theQueue,right);
      }
      else{
	heap_insert(myQueue,right);
      }
    }
    else
      free(right);
    left->_index = (original->_index) - 1;
    left->_cap =  original->_cap - itemArray[index]->_weight;
    left->_value = itemArray[index]->_profit + original->_value;
    calculateUpperBound(itemArray, left, theQueue->_arraylength);
    if(left->_cap < 0){
      free(left);
      break;
    }
    if(original->_cap < itemArray[index]->_weight){
      if(lb->_lb < original->_value){
	pthread_spin_lock(&lb->_lock); //lock
	lb->_lb = original->_value; //write
	pthread_spin_unlock(&lb->_lock); //unlock
      }
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
  if(original->_value >= lb->_lb){
    pthread_spin_lock(&lb->_lock); //lock
    lb->_lb= original->_value; //write
    pthread_spin_unlock(&lb->_lock); //unlock
  }
  free(original);
  }
  heap_free(myQueue);
  return;
}
void* bb(void* SQueue) {
  heap_t* theQueue = (heap_t*)SQueue;
  while(1){   
    pthread_mutex_lock(&theQueue->_lock); //lock
    if (isEmpty(theQueue) && theQueue->_awakeThreads == 1){
      pthread_cond_broadcast(&(theQueue->_cond));
      theQueue->_isDone = 1;
    }
    else
      theQueue->_awakeThreads--;
    pthread_mutex_unlock(&theQueue->_lock); //unlock
    PQNode* original = deQueueWork(theQueue);
    if (original==NULL)
      pthread_exit(0);
    pthread_mutex_lock(&theQueue->_lock); //lock
    theQueue->_awakeThreads++;
    pthread_mutex_unlock(&theQueue->_lock); //unlock
    Item** itemArray = theQueue->_itArrayptr;    
    if(original->_index == 0){
      if(original->_cap >= itemArray[0]->_weight)
	original->_value = itemArray[0]->_profit + original->_value;
      if(original->_value > lb->_lb){
	pthread_spin_lock(&lb->_lock); //lock
	lb->_lb = original->_value; //write
	pthread_spin_unlock(&lb->_lock); //unlock
      }
      free(original);
    }
    else if(original->_cap == 0){      
      if(original->_value > lb->_lb){
	pthread_spin_lock(&lb->_lock); //lock
	lb->_lb= original->_value; //write
	pthread_spin_unlock(&lb->_lock); //unlock
      }
      free(original);
    }
    else{
      pathtranverse(theQueue, original);
    }
    if(isEmpty(theQueue))
      continue;
  }
}

int main(int argc, char* argv[]) {
  if(argc != 3) {
    fprintf(stderr, "usage: knapsack_serial filename thread count\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp; 
  fp = fopen(filename, "r");
  long len, i, capacity;
  if (!fscanf(fp,"%lu",&len)) std::cout << "fscanf failed" << std::endl;
  long number,weight,profit;
  std::vector<Item*> itemArray(len);
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    if (!fscanf(fp,"%lu %lu %lu", &number, &profit, &weight)) std::cout << "fscanf failed" << std::endl;
    double ratio = (double)profit/(double)weight;
    n->_weight = weight;
    n->_profit = profit;
    n->_ratio = ratio;
    itemArray[i] = n;
  }
  //get max capacity
  if (!fscanf(fp,"%lu",&capacity)) std::cout << "fscanf failed" << std::endl;
  std::sort(itemArray.begin(), itemArray.end(), compare);
  fclose(fp);
  int nthreads = atoi(argv[2]);
  int status;
  void* exitStatus;
  pthread_t threads[nthreads];
  heap_t* sharedQ = heap_create(len*10);
  sharedQ->_isDone = 0;
  sharedQ->_itArrayptr = itemArray.data();
  sharedQ->_arraylength = len;
  PQNode* startnode = (PQNode*)malloc(sizeof(PQNode));
  pthread_mutex_init(&mtx, NULL);
  sharedQ->_awakeThreads = nthreads;
  lb  = (LBound*)malloc(sizeof(LBound));
  pthread_spin_init(&lb->_lock, 0);
  lb->_lb = 0;
  startnode->_lb  = lb;
  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;
  enQueueWork(sharedQ, startnode);
  clock_t tic = clock();
  for(i = 0; i < nthreads; i++){
    status = pthread_create(&threads[i],NULL,bb,(void*)sharedQ);
    if(status){
      std::cout << "ERROR; return code from pthread_create() is " << status << std::endl;
      exit(-1);
    }
  }
  for(i = 0; i < nthreads; i++)
    pthread_join(threads[i], &exitStatus);
  printf("optimal value: %lu \n",lb->_lb);
  clock_t toc = clock();
  printf("Elapsed: %f seconds \n", (double)(toc-tic)/CLOCKS_PER_SEC);
  heap_free(sharedQ);
  pthread_mutex_destroy(&mtx);
  pthread_spin_destroy(&lb->_lock);
  free(lb);
  for (i = 0; i < len; i++)
    free(itemArray[i]);
  return 0;
}
