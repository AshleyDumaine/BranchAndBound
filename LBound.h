#ifndef __LBOUND_H
#define __LBOUND_H
typedef struct LBound {
  int _lb;
  pthread_rwlock_t _lock;
} LBound;
#endif
