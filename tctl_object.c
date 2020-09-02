//
// Created by xjs on 2020/8/31.
//

#include "tctl_object.h"

#include <pthread.h>
#include "allocator.h"

//this指针
static pthread_key_t this_key;
static pthread_once_t this_key_once = PTHREAD_ONCE_INIT;

static void free_this(void *p)
{
    deallocate(p, sizeof(void*));
}

static void make_this_key(void)
{
    pthread_key_create(&this_key, free_this);
}

void **pthis(void)
{
    void **ptr;
    pthread_once(&this_key_once, make_this_key);
    if ((ptr = pthread_getspecific(this_key)) == NULL) {
        ptr = allocate(sizeof(void*));
        pthread_setspecific(this_key, ptr);
    }
    return ptr;
}