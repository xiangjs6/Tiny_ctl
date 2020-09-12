//
// Created by xjs on 2020/9/4.
//

#include "tctl_vector.h"
#include "tctl_allocator.h"
#include <memory.h>

static void fill_allocate(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    size_t old_size = p_private->total_storage_memb;
    p_private->total_storage_memb *= 2;
    p_private->total_storage_memb = p_private->total_storage_memb ? p_private->total_storage_memb : 1;
    p_private->start.ptr = reallocate(p_private->start.ptr, old_size * p_private->memb_size, p_private->total_storage_memb * p_private->memb_size);
    p_private->finish.ptr = p_private->start.ptr + p_private->memb_size * p_private->nmemb;
}
static void *at(int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (pos >= p_private->nmemb)
        return NULL;
    return p_private->start.ptr + p_private->memb_size * pos;
}
static iter_ptr const *begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return (iter_ptr const *)&p_private->start;
}
static iter_ptr const *end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return (iter_ptr const *)&p_private->finish;
}
static void *front(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->start.ptr;
}
static void *back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->finish.ptr - p_private->memb_size;
}
static size_t size(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->nmemb;
}
static size_t capacity(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->total_storage_memb - p_private->nmemb;
}
static bool empty(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->nmemb;
}
static void push_back(void *x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    push_this(this);
    if (!this->capacity()) {
        push_this(this);
        fill_allocate();
    }
    memcpy(p_private->finish.ptr, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish.ptr += p_private->memb_size;
}
static void pop_back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb--;
    p_private->finish.ptr -= p_private->memb_size;
}
static iter_ptr erase(iter_ptr iter)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (iter > p_private->finish.ptr || iter < p_private->start.ptr)
        return NULL;
    size_t back_nmemb = p_private->nmemb - (iter - p_private->start.ptr) / p_private->memb_size - 1;
    memcpy(iter, iter + p_private->memb_size, p_private->memb_size * back_nmemb);
    p_private->nmemb--;
    p_private->finish.ptr -= p_private->memb_size;
    return iter;
}
static iter_ptr insert(iter_ptr iter, void *x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (iter > p_private->finish.ptr || iter < p_private->start.ptr)
        return NULL;
    push_this(this);
    if (!this->capacity()) {
        push_this(this);
        fill_allocate();
    }
    size_t back_nmemb = p_private->nmemb - (iter - p_private->start.ptr) / p_private->memb_size;
    memcpy(iter + p_private->memb_size, iter, p_private->memb_size * back_nmemb);
    memcpy(iter, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish.ptr += p_private->memb_size;
    return iter;
}
static void resize(size_t new_size)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    while (new_size > p_private->total_storage_memb)
    {
        push_this(this);
        fill_allocate();
    }
    p_private->nmemb = new_size;
    p_private->finish.ptr = p_private->start.ptr + new_size * p_private->memb_size;
}
static void clear(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb = 0;
    p_private->finish = p_private->start;
}

void *iter_increment(void *p)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p + p_private->memb_size;
}

void *iter_decrement(void *p)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p - p_private->memb_size;
}

void *iter_add(void *p, int x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p + p_private->memb_size * x;
}

void *iter_sub(void *p, int x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p - p_private->memb_size * x;
}

static __iterator_obj_func  __def_vector_iter = {
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub
};

static const vector __def_vector = {
        at,
        begin,
        end,
        front,
        back,
        size,
        capacity,
        empty,
        push_back,
        pop_back,
        erase,
        insert,
        resize,
        clear
};

void init_vector(vector *p_vector, size_t nmemb, size_t memb_size, void *init_array)
{
    *p_vector = __def_vector;
    __private_vector __private = {memb_size};
    __private.nmemb = __private.total_storage_memb = nmemb;
    __private.start = init_iter(p_vector, reallocate(NULL, 0, nmemb * memb_size), &__def_vector_iter);
    __private.finish = __private.start;
    if (init_array) {
        memcpy(__private.start.ptr, init_array, nmemb * memb_size);
    }
    memcpy(p_vector->__obj_private, &__private, sizeof(__private_vector));
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    deallocate(p_private->start.ptr, p_private->total_storage_memb * p_private->memb_size);
}