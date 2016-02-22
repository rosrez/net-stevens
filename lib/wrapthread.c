#include "unpthread.h"

#define pterror(code, msg...) \
    do { fprintf(stderr, msg); fprintf(stderr, ": %s\n", strerror(code));  exit(1); } while (0)

int Pthread_create(pthread_t *thread, const pthread_attr_t *attr,
           void *(*start_routine) (void *), void *arg)
{
    int s = pthread_create(thread, attr, start_routine, arg);
    if (s != 0)
        pterror(s, "()");
    return s;
}

void Pthread_exit(void *retval)
{
    pthread_exit(retval);
}

int Pthread_join(pthread_t thread, void **retval)
{
    int s = pthread_join(thread, retval);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_detach(pthread_t thread)
{
    int s = pthread_detach(thread);
    if (s != 0)
        pterror(s, "()");
    return s;
}

pthread_t Pthread_self(void)
{
    return pthread_self();      /* always succeeds */
}

int Pthread_once(pthread_once_t *once_control,
       void (*init_routine)(void))
{
    int s = pthread_once(once_control, init_routine);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_init(pthread_attr_t *attr)
{
    int s = pthread_attr_init(attr);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_destroy(pthread_attr_t *attr)
{
    int s = pthread_attr_destroy(attr);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setdetachstate(pthread_attr_t *attr, int detachstate)
{
    int s = pthread_attr_setdetachstate(attr, detachstate);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getdetachstate(pthread_attr_t *attr, int *detachstate)
{
    int s = pthread_attr_getdetachstate(attr, detachstate);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setguardsize(pthread_attr_t *attr, size_t guardsize)
{
    int s = pthread_attr_setguardsize(attr, guardsize);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getguardsize(pthread_attr_t *attr, size_t *guardsize)
{
    int s = pthread_attr_getguardsize(attr, guardsize);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setschedparam(pthread_attr_t *attr,
                       const struct sched_param *param)
{
    int s = pthread_attr_setschedparam(attr, param);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getschedparam(pthread_attr_t *attr,
                       struct sched_param *param)
{
    int s = pthread_attr_getschedparam(attr, param);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setschedpolicy(pthread_attr_t *attr, int policy)
{
    int s = pthread_attr_setschedpolicy(attr, policy);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getschedpolicy(pthread_attr_t *attr, int *policy)
{
    int s = pthread_attr_getschedpolicy(attr, policy);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setinheritsched(pthread_attr_t *attr,
                       int inheritsched)
{
    int s = pthread_attr_setinheritsched(attr, inheritsched);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getinheritsched(pthread_attr_t *attr,
                       int *inheritsched)
{
    int s = pthread_attr_getinheritsched(attr, inheritsched);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setscope(pthread_attr_t *attr, int scope)
{
    int s = pthread_attr_setscope(attr, scope);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getscope(pthread_attr_t *attr, int *scope)
{
    int s = pthread_attr_getscope(attr, scope);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_setstacksize(pthread_attr_t *attr, size_t stacksize)
{
    int s = pthread_attr_setstacksize(attr, stacksize);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getstacksize(pthread_attr_t *attr, size_t *stacksize)
{
    int s = pthread_attr_getstacksize(attr, stacksize);
    if (s != 0)
        pterror(s, "()");
    return s;
}

#ifdef _GNU_SOURCE

int Pthread_attr_setaffinity_np(pthread_attr_t *attr,
                   size_t cpusetsize, const cpu_set_t *cpuset)
{
    int s = pthread_attr_setaffinity_np(attr, cpusetsize, cpuset);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_attr_getaffinity_np(pthread_attr_t *attr,
                   size_t cpusetsize, cpu_set_t *cpuset)
{
    int s = pthread_attr_getaffinity_np(attr, cpusetsize, cpuset);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_getattr_np(pthread_t thread, pthread_attr_t *attr)
{
    int s = pthread_getattr_np(thread, attr);
    if (s != 0)
        pterror(s, "()");
    return s;
}

#endif /* _GNU_SOURCE */

int Pthread_setschedparam(pthread_t thread, int policy,
                      const struct sched_param *param)
{
    int s = pthread_setschedparam(thread, policy, param);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_getschedparam(pthread_t thread, int *policy,
                      struct sched_param *param)
{
    int s = pthread_getschedparam(thread, policy, param);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_setschedprio(pthread_t thread, int prio)
{
    int s = pthread_setschedprio(thread, prio);
    if (s != 0)
        pterror(s, "()");
    return s;
}

#ifdef _GNU_SOURCE

int Pthread_setaffinity_np(pthread_t thread, size_t cpusetsize,
                           const cpu_set_t *cpuset)
{
    int s = pthread_setaffinity_np(thread, cpusetsize, cpuset);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_getaffinity_np(pthread_t thread, size_t cpusetsize,
                           cpu_set_t *cpuset)
{
    int s = pthread_getaffinity_np(thread, cpusetsize, cpuset);
    if (s != 0)
        pterror(s, "()");
    return s;
}

#endif /* _GNU_SOURCE */

int Pthread_setcancelstate(int state, int *oldstate)
{
    int s = pthread_setcancelstate(state, oldstate);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_setcanceltype(int type, int *oldtype)
{
    int s = pthread_setcanceltype(type, oldtype);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cancel(pthread_t thread)
{
    int s = pthread_cancel(thread);
    if (s != 0)
        pterror(s, "()");
    return s;
}

void Pthread_testcancel(void)
{
    pthread_testcancel();
}

int Pthread_mutex_destroy(pthread_mutex_t *mutex)
{
    int s = pthread_mutex_destroy(mutex);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_mutex_init(pthread_mutex_t *restrict mutex,
       const pthread_mutexattr_t *restrict attr)
{
    int s = pthread_mutex_init(mutex, attr);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_mutex_lock(pthread_mutex_t *mutex)
{
    int s = pthread_mutex_lock(mutex);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int pthread_mutex_trylock(pthread_mutex_t *mutex)
{
    int s = pthread_mutex_trylock(mutex);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_mutex_unlock(pthread_mutex_t *mutex)
{
    int s = pthread_mutex_unlock(mutex);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cond_destroy(pthread_cond_t *cond)
{
    int s = pthread_cond_destroy(cond);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cond_init(pthread_cond_t *restrict cond,
       const pthread_condattr_t *restrict attr)
{
    int s = pthread_cond_init(cond, attr);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cond_broadcast(pthread_cond_t *cond)
{
    int s = pthread_cond_broadcast(cond);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cond_signal(pthread_cond_t *cond)
{
    int s = pthread_cond_signal(cond);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int pthread_cond_timedwait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex,
       const struct timespec *restrict abstime)
{
    int s = pthread_cond_timedwait(cond, mutex, abstime);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_cond_wait(pthread_cond_t *restrict cond,
       pthread_mutex_t *restrict mutex)
{
    int s = pthread_cond_wait(cond, mutex);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_key_create(pthread_key_t *key, void (*destructor)(void*))
{
    int s = pthread_key_create(key, destructor);
    if (s != 0)
        pterror(s, "()");
    return s;
}

int Pthread_key_delete(pthread_key_t key)
{
    int s = pthread_key_delete(key);
    if (s != 0)
        pterror(s, "()");
    return s;
}

void *Pthread_getspecific(pthread_key_t key)
{
    return pthread_getspecific(key);        /* always succeeds; returns NULL if key not present */
}

int Pthread_setspecific(pthread_key_t key, const void *value)
{
    int s = pthread_setspecific(key, value);
    if (s != 0)
        pterror(s, "()");
    return s;
}
