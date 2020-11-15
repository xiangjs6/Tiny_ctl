//
// Created by xjs on 2020/8/31.
//

#include "tctl_allocator.h"
#include "tctl_common.h"
#include <malloc.h>
#include <memory.h>
#include <pthread.h>

#define THROW_BAD_ALLOC

//第一级分配器
static void (*__malloc_handler)(void) = NULL;

static void *__oom_malloc(size_t n)
{
    void *res = NULL;
    for (;;) {
        if (!__malloc_handler) THROW_BAD_ALLOC;
        __malloc_handler();
        res = malloc(n);
        if (res) return res;
    }
}

static void *__oom_realloc(void *p, size_t n)
{
    void *res = NULL;
    for (;;) {
        if (!__malloc_handler) THROW_BAD_ALLOC;
        __malloc_handler();
        res = realloc(p, n);
        if (res) return res;
    }
}

static void *__malloc_alloc(size_t n)
{
    if (n == 0)
        return NULL;
    void *res = malloc(n);
    if (!res)
        return __oom_malloc(n);
    return res;
}

static void *__malloc_realloc(void *p, size_t n)
{
    void *res = realloc(p, n);
    if (!res)
        return __oom_realloc(p, n);
    return res;
}

static void __malloc_dealloc(void *p, size_t n)
{
    free(p);
}

//第二级分配器
#define ALIGN 8
#define NFREELISTS 16
#define FRAGMENT_MAX_SIZE 128
#define ROUND_UP(n) (((n) + ALIGN - 1) & ~(ALIGN - 1))
#define FREELIST_INDEX(n) (((n) + ALIGN - 1) / ALIGN - 1)

union obj {
    union obj *free_list_link;
    char data[1];
};

static union obj *free_list[NFREELISTS];
static void *start_free, *end_free;

static union obj *chunk_alloc(size_t n, int *nobjs)
{
    size_t total_bytes = n * (*nobjs);
    size_t left_bytes = end_free - start_free;
    union obj *res;
    if (left_bytes >= total_bytes) {
        res = start_free;
        start_free += total_bytes;
        return res;
    } else if (left_bytes >= n) {
        *nobjs = left_bytes / n;
        total_bytes = n * (*nobjs);
        res = start_free;
        start_free += total_bytes;
        return res;
    } else {
        size_t get_bytes = 2 * total_bytes;
        if (left_bytes > 0) {
            union obj **p_free_list = free_list + FREELIST_INDEX(left_bytes);
            ((union obj*)start_free)->free_list_link = *p_free_list;
            *p_free_list = start_free;
        }
        start_free = __malloc_alloc(get_bytes);
        end_free = start_free + get_bytes;
        return chunk_alloc(n, nobjs);
    }
}

static void refill(size_t n)
{
    int nobjs = 20;
    void *chunk = chunk_alloc(n, &nobjs);
    union obj **p_free_list = free_list + FREELIST_INDEX(n);
    for (int i = 0; i < nobjs; i++) {
        union obj *curent_obj = chunk + n * i;
        curent_obj->free_list_link = *p_free_list;
        *p_free_list = curent_obj;
    }
}

static pthread_mutex_t fragment_lock = PTHREAD_MUTEX_INITIALIZER;
static void *__fragment_alloc(size_t n)
{
    if (n == 0)
        return NULL;
    pthread_mutex_lock(&fragment_lock);
    union obj **p_free_list = free_list + FREELIST_INDEX(n);
    union obj *res = *p_free_list;
    if (!res) {
        refill(ROUND_UP(n));
        pthread_mutex_unlock(&fragment_lock);
        return __fragment_alloc(n);
    }
    *p_free_list = res->free_list_link;
    pthread_mutex_unlock(&fragment_lock);
    return res;
}

static void __fragment_dealloc(void *p, size_t n)
{
    union obj *q = p;
    pthread_mutex_lock(&fragment_lock);
    union obj **p_free_list = free_list + FREELIST_INDEX(n);
    q->free_list_link = *p_free_list;
    *p_free_list = q;
    pthread_mutex_unlock(&fragment_lock);
}

//对外接口
void (*set_malloc_handler(void(*f)(void)))(void)
{
    void (*old)(void) = __malloc_handler;
    __malloc_handler = f;
    return old;
}

void *allocate(size_t n)
{
    if (n <= FRAGMENT_MAX_SIZE)
        return __fragment_alloc(n);
    else
        return __malloc_alloc(n);
}

void *reallocate(void *p, size_t old_size, size_t new_size)
{
    if (old_size <= FRAGMENT_MAX_SIZE && ROUND_UP(old_size) == ROUND_UP(new_size))
        return p;
    else if (new_size <= FRAGMENT_MAX_SIZE) {
        void *__p = __fragment_alloc(new_size);
        if (old_size > 0) {
            if (__p)
                memcpy(__p, p, MIN(old_size, new_size));
            if (old_size <= FRAGMENT_MAX_SIZE)
                __fragment_dealloc(p, old_size);
            else
                __malloc_dealloc(p, old_size);
        }
        return __p;
    } else {
        void *__p;
        if (old_size <= FRAGMENT_MAX_SIZE) {
            __p = __malloc_realloc(NULL, new_size);
            memcpy(__p, p, old_size);
            __fragment_dealloc(p, old_size);
        } else {
            __p = __malloc_realloc(p, new_size);
        }
        return __p;
    }
}

void deallocate(void *p, size_t n)
{
    if (n <= FRAGMENT_MAX_SIZE)
        __fragment_dealloc(p, n);
    else
        __malloc_dealloc(p, n);
}
