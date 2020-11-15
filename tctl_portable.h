//
// Created by xjs on 2020/11/15.
//

#ifndef TINY_CTL_TCTL_PORTABLE_H
#define TINY_CTL_TCTL_PORTABLE_H
#include <pthread.h>
typedef pthread_key_t thread_key_t;
typedef pthread_mutex_t thread_mutex_t;
typedef pthread_once_t thread_once_t;
#define THREAD_ONCE_INIT PTHREAD_ONCE_INIT
#define thread_once pthread_once
#define THREAD_MUTEX_INITIALIZER PTHREAD_MUTEX_INITIALIZER
#define thread_getspecific pthread_getspecific
#define thread_setspecific pthread_setspecific
#define thread_key_create pthread_key_create
#define thread_mutex_lock pthread_mutex_lock
#define thread_mutex_unlock pthread_mutex_unlock
#endif //TINY_CTL_TCTL_PORTABLE_H
