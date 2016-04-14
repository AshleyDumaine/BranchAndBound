#include "heap.h"
#include <algorithm>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <vector>
#include <unistd.h>

long node_count = 0;
LBound* lb; //lower bound global

bool compare (const Item* a, const Item* b) {
  return a->_ratio < b->_ratio;
}

void calculateUpperBound(Item** itemArray, PQNode* node, long len) {
  long i, cap = node->_cap, index = node->_index;
  double value = (double)node->_value;
  for (i = index; i >= 0; i--) {
    Item* item = itemArray[i];
    long diff = cap - item->_weight;
    // if it fits...
    if(diff >= 0) {
      // put the item in the knapsack and adjust profit and cap
      cap = diff;
      value += item->_profit;
      // stop if full
      if(diff == 0) break;
    }
    else {
      // take a fraction of the item that will fit in the knapsack
      double div = ((double)cap/(item->_weight));
      double newp = div*((double)(item->_profit));
      value += newp;
      break;
    }
  }
  node->_ub = value;
}

void pathtranverse(heap_t* theQueue, PQNode* original){
  Item** itemArray = theQueue->_itArrayptr;
  while (original->_cap != 0 && original->_index != 0) {
    long index = original->_index;
    calculateUpperBound(itemArray, original, theQueue->_arraylength);
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
    calculateUpperBound(itemArray, right, theQueue->_arraylength);
    if(right->_ub > lb->_lb) heap_insert(theQueue, right);
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
  if(original->_value > lb->_lb)
    lb->_lb = original->_value;
  free(original);
  return;
}

void* bb(heap_t* theQueue) {
  while(1){
    PQNode* original = (PQNode*)heap_remove_root(theQueue);
    if (!original) return lb; 
    node_count++;
    Item** itemArray = theQueue->_itArrayptr;    
    if(original->_index == 0){
      if(original->_cap >= itemArray[0]->_weight)
	original->_value += itemArray[0]->_profit;
      if(original->_value >= lb->_lb)
	lb->_lb = original->_value;
      free(original);
    }
    else if(original->_cap == 0){
      if(original->_value >= lb->_lb)
	lb->_lb= original->_value;
      free(original);
    }
    else
      pathtranverse(theQueue, original);
    if(isEmpty(theQueue)) continue;
  }
}

int main(int argc, char* argv[]) {
  clock_t tic = clock();
  if(argc != 2) {
    fprintf(stderr, "usage: knapsack_serial filename\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp = fopen(filename, "r");
  long len, i, capacity, number, weight, profit;
  if (!fscanf(fp,"%lu",&len)) {
    std::cout << "fscanf failed" << std::endl;
    return 1;
  }
  std::vector<Item*> itemArray(len); 
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    if (!fscanf(fp,"%lu %lu %lu", &number, &profit, &weight)) {
      std::cout << "fscanf failed" << std::endl;
      return 1;
    }
    // maybe instead of ratio use a different heuristic
    n->_weight = weight;
    n->_profit = profit;
    n->_ratio = ((double)profit)/weight;
    itemArray[i] = n;
  }
  if (!fscanf(fp,"%lu",&capacity)) {
    std::cout << "fscanf failed" << std::endl;
    return 1;
  }

  if (capacity * len < 5000000) {
	int ret = execl("./dp_knapsack", "./dp_knapsack", argv[1], (char *)0);
	return 0;
  }
  std::sort(itemArray.begin(), itemArray.end(), compare);
  fclose(fp);
  int status;
  void* exitStatus;
  heap_t* sharedQ = heap_create(len*2);
  sharedQ->_isDone = 0;
  sharedQ->_itArrayptr = itemArray.data();
  sharedQ->_arraylength = len;
  PQNode* startnode = (PQNode*)malloc(sizeof(PQNode));
  lb = (LBound*)malloc(sizeof(LBound));
  lb->_lb = 0;
  startnode->_lb  = lb;
  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;
  heap_insert(sharedQ, startnode);
  lb = (LBound*)bb(sharedQ);
  clock_t toc = clock();
  printf("Optimal value: %lu \nOptimality: 1\n", lb->_lb);
  printf("%f seconds \n", (double)(toc-tic)/CLOCKS_PER_SEC);
  printf("Nodes traversed: %lu \n", node_count);
  heap_free(sharedQ);
  free(lb);
  for (i = 0; i < len; i++)
    free(itemArray[i]);
  return 0;
}
