#include <stdio.h>
#include "priorityQueue.h"
#include <stdlib.h>

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
//changed ub func
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
    pthread_cond_wait(theQueue->_condLock, &theQueue->_lock);
  }
  //do stuff
  PQNode* node = dequeue(theQueue);
  pthread_mutex_unlock(&theQueue->_lock);
  while (node->_cap != 0 || node->_index != 0) {
    
  }
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

  //for the start node
  PQNode* startnode = (PQNode*)malloc(sizeof(PQNode));
  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;


  //This is just a test to make sure its organized, it works, go test it, etc 
  for(i = 0; i <len; i++){
    printf("%lf \n", itemArray[i]->_ratio);
  }
  

  for (i = 0; i< len; i++) {
    free(itemArray[i]);
  }
  free(itemArray);
  return 0;
}

