#include <stdio.h>
int compare (Node* arg0, Node* arg1) {
  if(arg0->_ratio < arg1->_ratio)
    return -1;
  else if (arg0->_ration > arg1->_ratio)
    return 1;
  else
    return 0;
}

int main(int argc, char* argv[]) {
  if(argc != 2) {
    fprintf(stderr, "usage: knapsack_serial filename\n");
    return 1;
  }
  char* filename = argv[1];
  FILE* fp = fopen(filename, 'r');
  //scan some  stuff
  
}

