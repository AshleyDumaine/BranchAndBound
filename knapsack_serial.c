#include <stdio.h>
#include "priorityQueue.h"
#include <stdlib.h>

LBound* lb; //lower bound global

int compare (const void* a, const void* b) {
  const Item** arg0  = (const Item**)a;
  const Item** arg1 = (const Item**)b;
  if((*arg0)->_ratio < (*arg1)->_ratio)
    return -1;
  else if ((*arg0)->_ratio > (*arg1)->_ratio)
    return 1;
  else
    return 0;
}

void calculateUpperBound(Item** itemArray,PQNode* node, int len) {
  int cap = node->_cap;
  int index = node->_index;
  double value = (double)node->_value;
  int i;
  for (i = index; i < len; i++) {
    Item* item = itemArray[i];
    int diff = cap - item->_weight;
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
      double div = ((double)cap/(double)item->_weight);
      double newp = div*(double)(item->_profit);
      value = value + newp;
      break;
    }
  }
  node->_ub = value;
}

void* bb(void* sharedQ) {
  PQueue* theQueue = (PQueue*)sharedQ;
  pthread_mutex_lock(&theQueue->_lock);
  while (isEmpty(theQueue)) {
    pthread_cond_wait(&theQueue->_condLock, &theQueue->_lock);
  }
  //do stuff to one node, basically this is one path
  PQNode* original = deQueue(theQueue);
  pthread_mutex_unlock(&theQueue->_lock);
  Item** itemArray = theQueue->_itArrayptr;

  if(original->_index == 0){
    if(original->_value > lb->_lb){
      //************************************************
      //need to put a write lock first???
      lb->_lb = original->_value;
      //unlock afterwards???
      //P.S, whats the read lock? put the code below commented out =D
      //***********************************************
    }
    return;
  }
  if(original->_cap == 0){
    if(original->_value> lb->_lb){
      lb->_lb= original->_value;
    }
    return;
  }
  while (original->_cap != 0 && original->_index != 0) {
    int index = original->_index;
    PQNode* left = original->_left;
    original->_left = (PQNode*)malloc(sizeof(PQNode));
    PQNode* right = original->_right;
    original->_right = (PQNode*)malloc(sizeof(PQNode));

    //right and left, set lower bound
    right->_lb = lb;
    left->_lb = lb;

    right->_index = original->_index++;
    right->_cap = original->_cap;
    right->_value = original->_value;
    calculateUpperBound(itemArray,right,theQueue->_arraylength);
    
    if(left->_cap < itemArray[index]->_weight){
      if(lb->_lb < left->_value){
	lb->_lb = left->_value;
      }
      break;
    }
    //else you continue below 
    left->_cap =  original->_cap - itemArray[index]->_weight;
    left->_value = itemArray[index]->_profit + original->_value;
    calculateUpperBound(itemArray, left, theQueue->_arraylength);
    
    enQueue(theQueue,right);
    free(original);
    original = left;    
  }
  if(original->_value > lb->_lb){
    lb->_lb= original->_value;
  }

  //***********************************
  //should bb be called again here so the same thread will go back for more work??????????????????????????????????????????????????
  //**********************************

}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "usage: knapsack_serial filename\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp; 
  fp = fopen(filename, "r");
  //scan some  stuff
  int len, i, capacity;
  fscanf(fp,"%d",&len);

  //number is useless, we just need it as a parameter for scanning
  int number,weight,profit;

  Item** itemArray = (Item**)malloc(sizeof(Item*) * len);
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    fscanf(fp,"%d %d %d", &number, &profit, &weight);
    double ratio = (double)profit/(double)weight;
    n->_weight = weight;
    n->_profit = profit;
    n->_ratio = ratio;
    printf("%d ratio: %lf \n", number, ratio);
    itemArray[i] = n;
  }
  //get max capacity
  fscanf(fp,"%d",&capacity);
  qsort(itemArray, len, sizeof(Item*),compare); 
  //do stuff involving 1 thread currently

  //the main part of the assignment
  int nthreads = 1;

  //The shared queue 
  PQueue* sharedQ = makeQueue(len); //dynamically grows
  sharedQ->_itArrayptr = itemArray;
  sharedQ->_arraylength = len;
  //for the start node
  PQNode* startnode = (PQNode*)malloc(sizeof(PQNode));

  lb  = (LBound*)malloc(sizeof(LBound));
  pthread_rwlock_init(&lb->_lock, NULL);
  lb->_lb = 0;
  startnode->_lb  = lb;

  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;
  enQueue(sharedQ, startnode);

  //*****************************************************
  //pthread create here, call bb!

  //***************************************************

  /*
  //This is just a test to make sure its organized, it works, go test it, etc 
  for(i = 0; i <len; i++){
    printf("%lf \n", itemArray[i]->_ratio);
  }
  */

  for (i = 0; i< len; i++) {
    free(itemArray[i]);
  }
  free(itemArray);
  free(lb);
  return 0;
}

