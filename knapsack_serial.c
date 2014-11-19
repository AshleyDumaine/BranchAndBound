#include <stdio.h>
#include "priorityQueue.h"
#include <stdlib.h>

typedef struct Item {
  double _ratio;
  int _weight;
  int _profit;
} Item;

int compare (Item* arg0, Item* arg1) {
  if(arg0->_ratio < arg1->_ratio)
    return -1;
  else if (arg0->_ratio > arg1->_ratio)
    return 1;
  else
    return 0;
}
//changed ub func
void calculateUpperBound(Item** itemArray, PQNode* node, int len) {
  int cap = node->_cap;
  int index = node->_index;
  int value = node->_value;
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
      int div = (cap/item->_weight);
      int newp = div*(item->_profit);
      value = value + newp;
      break;
    }
  }
  node->_ub = value;
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

  Item** itemArray = (Item**)malloc(sizeof(Item*) * len+1);
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    fscanf(fp,"%d %d %d", &number, &profit, &weight);
    int ratio = profit/weight;
    n->_weight = weight;
    n->_profit = profit;
    n->_ratio = ratio;
    itemArray[i] = n;
  }
  //get max capacity
  fscanf(fp,"%d",&capacity);

  //do stuff

  for (i = 0; i< len; i++) {
    free(itemArray[i]);
  }
  free(itemArray);
  return 0;
}

