//
// Created by xjs on 2020/8/31.
//

#include "tctl_object.h"

#include <pthread.h>
#include <malloc.h>
#include <memory.h>
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

//临时变量
static pthread_key_t tmp_val_key;
static pthread_once_t tmp_val_key_once = PTHREAD_ONCE_INIT;
struct __temp {
    size_t size;
    void *p;
};

static void free_temp(void *p)
{
    deallocate(((struct __temp*)p)->p, ((struct __temp*)p)->size);
    deallocate(p, sizeof(struct __temp));
}

static void make_tmp_val_key(void)
{
    pthread_key_create(&tmp_val_key, free_temp);
}

void *tmp_val(void *p, size_t n)
{
    struct __temp *ptr;
    pthread_once(&tmp_val_key_once, make_tmp_val_key);
    if ((ptr = pthread_getspecific(tmp_val_key)) == NULL) {
        ptr = allocate(sizeof(struct __temp));
        ptr->size = 0;
        ptr->p = NULL;
    }
    if (n > ptr->size)
        ptr->p = reallocate(ptr->p, ptr->size, n);
    memcpy(ptr->p, p, n);
    return ptr->p;
}