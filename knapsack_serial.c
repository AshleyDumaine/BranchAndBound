#include <stdio.h>
#include "priorityQueue.h"
#include <stdlib.h>
#include <pthread.h>

LBound* lb; //lower bound global
int awakeThreads;
pthread_mutex_t mtx;

PQNode* deQueueWork(PQueue* twq);

void enQueueWork(PQueue* twq,PQNode* t);


void pathtranverse(PQueue* theQueue, PQNode* original);

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

PQNode* deQueueWork(PQueue* twq)
{
  pthread_mutex_lock(&twq->_lock);
  PQNode* t = NULL;
  while (isEmpty(twq)) {
    pthread_cond_wait(&twq->_cond,&twq->_lock);
  }
  t = deQueue(twq);
  pthread_mutex_unlock(&twq->_lock);
  return t;
}

void enQueueWork(PQueue* twq,PQNode* t)
{
  pthread_mutex_lock(&twq->_lock);
  enQueue(twq,t);
  pthread_cond_signal(&twq->_cond);
  pthread_mutex_unlock(&twq->_lock);
}

void calculateUpperBound(Item** itemArray,PQNode* node, int len) {
  int cap = node->_cap;
  int index = node->_index;
  double value = (double)node->_value;
  int i;
  
  for (i = index; i >= 0; i--) {
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
      double div = ((double)cap/(double)(item->_weight));
      double newp = div*((double)(item->_profit));
      value = value + newp;
      break;
    }
  }
  //printf("upper bound here %lf \n",value);
  node->_ub = value;
}

/***********still working on this part down here, no touchie, kay? XD**********************************/
void* bb(void* SQueue) {
  PQueue* theQueue = (PQueue*)SQueue;
  //  while(1) {
    //do stuff to one node, basically this is one path
  int count = 7;
 while(count != 0){
    printf("doing work\n %d \n", isEmpty(theQueue));
      pthread_mutex_lock(&mtx);
      awakeThreads--;
      pthread_mutex_unlock(&mtx);
      printf("queue is size is %d \n", theQueue->_sz);

      PQNode* original = deQueueWork(theQueue);

      printf("queue is size is %d \n", theQueue->_sz);
      printf("begining: the value %d\n",original->_value);
      printf("begin: index %d \n",original->_index);
      printf("begin: the capacity is %d \n",original->_cap);
      pthread_mutex_lock(&mtx);
      awakeThreads++;
      pthread_mutex_unlock(&mtx);
      // if (awakeThreads == 0 && isEmpty(theQueue)) break;

      Item** itemArray = theQueue->_itArrayptr;    
      if(original->_index == 0){
	if(original->_cap >= itemArray[0]->_weight){
	  original->_value = itemArray[0]->_profit + original->_value;
	}
        pthread_rwlock_wrlock(&(lb->_lock));
	if(original->_value > lb->_lb){
	  lb->_lb = original->_value;
	  pthread_rwlock_unlock(&(lb->_lock));
	}
      }
      else if(original->_cap == 0){
	if(original->_value> lb->_lb){
	  pthread_rwlock_wrlock(&lb->_lock);
	  lb->_lb= original->_value;
	  pthread_rwlock_unlock(&lb->_lock);
	}
      }
      else{
	pathtranverse(theQueue, original);
      }
      count--;
 }
}
void pathtranverse(PQueue* theQueue, PQNode* original){
  Item** itemArray = theQueue->_itArrayptr;
  printf("got here\n"); 
      while (original->_cap != 0 && original->_index != 0) {
	printf("the value %d\n",original->_value);
	printf("index %d \n",original->_index);
	printf("the capacity is %d \n",original->_cap);
	int index = original->_index;
        calculateUpperBound(itemArray,original,theQueue->_arraylength);
        printf("upper bound %lf \n",original->_ub );

	if(original->_ub < lb->_lb){
	  break;
	}

	//else do stuff with left and right nodes
	original->_left = (PQNode*)malloc(sizeof(PQNode));
	original->_right = (PQNode*)malloc(sizeof(PQNode));
	PQNode* left = original->_left;
	PQNode* right = original->_right;
        
	
	//right and left, set lower bound
	right->_lb = lb;
	left->_lb = lb;
	
	
	right->_index = (original->_index) - 1;
	right->_cap = original->_cap;
	right->_value = original->_value;
	calculateUpperBound(itemArray,right,theQueue->_arraylength);
	printf("at index %d, the right node is value %d, upperbound %lf, capacity %d\n",right->_index,right->_value,right->_ub,right->_cap);
	printf("upperbound right is %lf\n",right->_ub);
        if(right->_ub > lb->_lb){
	  printf("put this into queue\n");
          enQueueWork(theQueue,right);
	}	 
	left->_index = (original->_index) - 1;
	left->_cap =  original->_cap - itemArray[index]->_weight;
	left->_value = itemArray[index]->_profit + original->_value;
	calculateUpperBound(itemArray, left, theQueue->_arraylength);
        printf("at index %d, the left node is value %d, upperbound %lf, capacity %d\n",left->_index,left->_value,left->_ub,left->_cap);
	if(left->_cap < 0){
	  printf("Too much shit, we're done\n");
	  break;
	}
        if(original->_cap < itemArray[index]->_weight){
          pthread_rwlock_wrlock(&lb->_lock);
          if(lb->_lb < original->_value){
            lb->_lb = original->_value;
            pthread_rwlock_unlock(&lb->_lock);
          }
          printf("done with all items \n\n");
          break;
        }


	if(left->_ub < lb->_lb){
	  break;
	}
	free(original);
	original = left;    

      }
      if(original->_value >= lb->_lb){
	lb->_lb= original->_value;
      }
      printf("Done with everything\n\n");
      //  }
      return;
  
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
    //printf("%d ratio: %lf \n", number, ratio);
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

  pthread_mutex_init(&mtx, NULL);
  pthread_mutex_lock(&mtx);
  awakeThreads = nthreads;
  pthread_mutex_unlock(&mtx);

  lb  = (LBound*)malloc(sizeof(LBound));
  pthread_rwlock_init(&lb->_lock, NULL);
  lb->_lb = 0;
  startnode->_lb  = lb;

  startnode->_value = 0;
  startnode->_cap = capacity;
  startnode->_index = len-1;
  enQueueWork(sharedQ, startnode);

  int status;
  pthread_t threads[nthreads];
  for(i=0;i<nthreads;i++){
    status = pthread_create(&threads[i],NULL,bb,(void*)sharedQ);
    if(status){
      printf("ERROR; return code from pthread_create() is %d\n", status);
      exit(-1);
    }
  }
  printf("lower bound is: %d \n",lb->_lb);

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

  pthread_exit(NULL);
  return 0;
}

