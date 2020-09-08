//
// Created by xjs on 2020/9/1.
//

#include "tctl_common.h"
#include <pthread.h>
#include <memory.h>
#include "tctl_allocator.h"

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