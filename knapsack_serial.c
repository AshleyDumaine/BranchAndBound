#include <stdio.h>
typedef struct Item {
  double _ratio;
  int _weight;
  int profit;
} Item

int compare (Item* arg0, Item* arg1) {
  if(arg0->_ratio < arg1->_ratio)
    return -1;
  else if (arg0->_ration > arg1->_ratio)
    return 1;
  else
    return 0;
}

int calculateUpperBound(int fk, int cap) {
  int rc = cap;
  double p = 0.0;
  int i;
  for (i = fk; i < cap; i++) {
    
  }
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "usage: knapsack_serial filename\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp = fopen(filename, 'r');
  //scan some  stuff
  int len, i;
  Item** itemArray = (Item**)malloc(sizeof(Item*) * len);
  for (i = 0; i < len; i++) {
    Item* n = (Item*)malloc(sizeof(Item));
    itemArray[i] = n;
  }
  //do stuff

  for (i = 0; i< len; i++) {
    free(itemArray[i]);
  }
  free(itemArray);
  return 0;
}

