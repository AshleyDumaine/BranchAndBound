#ifndef __QUEUEWRAPPER_H
#define __QUEUEWRAPPER_H
typedef struct queueWrapper {
  struct Queue* _queue;
  pthread_mutex_t _mutex;
  pthread_cond_t _cond;
} QWrapper;
#endif
