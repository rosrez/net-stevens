#ifndef __unpthreads_h
#define __unpthreads_h

#include "unp.h"
#include <pthread.h>

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg);

void Pthread_exit(void *retval);

int Pthread_join(pthread_t thread, void **retval);

int Pthread_detach(pthread_t thread);

pthread_t Pthread_self(void);

int Pthread_once(pthread_once_t *once_control,
       void (*init_routine)(void));

int Pthread_attr_init(pthread_attr_t *attr);
int Pthread_attr_destroy(pthread_attr_t *attr); 

int Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate);
int Pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate);

int Pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize);
int Pthread_attr_getguardsize(pthread_attr_t *attr, size_t *guardsize);

int Pthread_attr_setschedparam(pthread_attr_t *attr,
                       const struct sched_param *param);
int Pthread_attr_getschedparam(pthread_attr_t *attr,
                       struct sched_param *param);

int Pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy);
int Pthread_attr_getschedpolicy(pthread_attr_t *attr, int *policy);

int Pthread_attr_setinheritsched(pthread_attr_t *attr,
                       int inheritsched);
int Pthread_attr_getinheritsched(pthread_attr_t *attr,
                       int *inheritsched);

int Pthread_attr_setscope(pthread_attr_t *attr, int scope);
int Pthread_attr_getscope(pthread_attr_t *attr, int *scope);

int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize);
int Pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize); 

#ifdef _GNU_SOURCE

int Pthread_attr_setaffinity_np(pthread_attr_t *attr,
                   size_t cpusetsize, const cpu_set_t *cpuset);
int Pthread_attr_getaffinity_np(pthread_attr_t *attr,
                   size_t cpusetsize, cpu_set_t *cpuset);

int Pthread_getattr_np(pthread_t thread, pthread_attr_t *attr);

#endif /* _GNU_SOURCE */

int Pthread_setschedparam(pthread_t thread, int policy,
                      const struct sched_param *param);
int Pthread_getschedparam(pthread_t thread, int *policy,
                      struct sched_param *param);

int Pthread_setschedprio(pthread_t thread, int prio);

#ifdef _GNU_SOURCE

int Pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
                           const cpu_set_t *cpuset);
int Pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
                           cpu_set_t *cpuset);
#endif /* _GNU_SOURCE */

int Pthread_setcancelstate(int state, int *oldstate);
int Pthread_setcanceltype(int type, int *oldtype);

int Pthread_cancel(pthread_t thread);
void Pthread_testcancel(void);

int Pthread_mutex_destroy(pthread_mutex_t *mutex);
int Pthread_mutex_init(pthread_mutex_t *restrict mutex,
       const pthread_mutexattr_t *restrict attr);

int Pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int Pthread_mutex_unlock(pthread_mutex_t *mutex);

int Pthread_cond_destroy(pthread_cond_t *cond);
int Pthread_cond_init(pthread_cond_t *restrict cond,
       const pthread_condattr_t *restrict attr);

int Pthread_cond_broadcast(pthread_cond_t *cond);
int Pthread_cond_signal(pthread_cond_t *cond);

int pthread_cond_timedwait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex,
       const struct timespec *restrict abstime);
int Pthread_cond_wait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex);

int Pthread_key_create(pthread_key_t *key, void (*destructor)(void*));
int Pthread_key_delete(pthread_key_t key);

void *Pthread_getspecific(pthread_key_t key);
int Pthread_setspecific(pthread_key_t key, const void *value);

#endif      /* __unpthreads_h */
