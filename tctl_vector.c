//
// Created by xjs on 2020/9/4.
//

#include "tctl_vector.h"
#include "tctl_allocator.h"
#include <memory.h>

static void fill_allocate(vector *this)
{
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    size_t old_size = p_private->total_storage_memb;
    p_private->total_storage_memb *= 2;
    p_private->total_storage_memb = p_private->total_storage_memb ? p_private->total_storage_memb : 1;
    p_private->start_iter.val = reallocate(p_private->start_iter.val, old_size * p_private->memb_size, p_private->total_storage_memb * p_private->memb_size);
    p_private->finish_iter.val = p_private->start_iter.val + p_private->memb_size * p_private->nmemb;
}
static void *at(int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (pos >= p_private->nmemb)
        return NULL;
    return p_private->start_iter.val + p_private->memb_size * pos;
}
static void const * const * begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->start_iter.iterator_func_p != &p_private->iter_func)
        p_private->start_iter.iterator_func_p = &p_private->iter_func;
    return (void*)&p_private->start_iter;
}
static void const * const * end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->finish_iter.iterator_func_p != &p_private->iter_func)
        p_private->finish_iter.iterator_func_p = &p_private->iter_func;
    return (void*)&p_private->finish_iter;
}
static void *front(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->start_iter.val;
}
static void *back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->finish_iter.val - p_private->memb_size;
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
    if (!THIS(this).capacity())
        fill_allocate(this);
    memcpy(p_private->finish_iter.val, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_iter.val += p_private->memb_size;
}
static void pop_back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb--;
    p_private->finish_iter.val -= p_private->memb_size;
}
static __iterator *erase(__iterator *iter)
{
    vector *this = pop_this();
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (_iter->val > p_private->finish_iter.val || _iter->val < p_private->start_iter.val)
        return NULL;
    size_t back_nmemb = p_private->nmemb - (_iter->val - p_private->start_iter.val) / p_private->memb_size - 1;
    memmove(_iter->val, _iter->val + p_private->memb_size, p_private->memb_size * back_nmemb);
    p_private->nmemb--;
    p_private->finish_iter.val -= p_private->memb_size;
    return iter;
}
static __iterator *insert(__iterator *iter, void *x)
{
    vector *this = pop_this();
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (_iter->val > p_private->finish_iter.val || _iter->val < p_private->start_iter.val)
        return NULL;
    push_this(this);
    if (!this->capacity())
        fill_allocate(this);
    size_t back_nmemb = p_private->nmemb - (_iter->val - p_private->start_iter.val) / p_private->memb_size;
    memmove(_iter->val + p_private->memb_size, _iter->val, p_private->memb_size * back_nmemb);
    memcpy(_iter->val, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_iter.val += p_private->memb_size;
    return iter;
}
static void resize(size_t new_size)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    while (new_size > p_private->total_storage_memb)
        fill_allocate(this);
    p_private->nmemb = new_size;
    p_private->finish_iter.val = p_private->start_iter.val + new_size * p_private->memb_size;
}
static void clear(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb = 0;
    p_private->finish_iter.val = p_private->start_iter.val;
}

static void *iter_at(struct __iterator *iter, int pos)
{
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)container_of(_iter->iterator_func_p, __private_vector, iter_func);
    return _iter->val + pos * p_private->memb_size;
}

static void iter_increment(struct __iterator *iter)
{
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)container_of(_iter->iterator_func_p, __private_vector, iter_func);
    _iter->val += p_private->memb_size;
}

static void iter_decrement(struct __iterator *iter)
{
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)container_of(_iter->iterator_func_p, __private_vector, iter_func);
    _iter->val -= p_private->memb_size;
}

static void iter_add(struct __iterator *iter, int x)
{
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)container_of(_iter->iterator_func_p, __private_vector, iter_func);
    _iter->val += x * p_private->memb_size;
}

static void iter_sub(struct __iterator *iter, int x)
{
    __vector_iter *_iter = (__vector_iter*)iter;
    __private_vector *p_private = (__private_vector*)container_of(_iter->iterator_func_p, __private_vector, iter_func);
    _iter->val -= x * p_private->memb_size;
}

static long long iter_diff(struct __iterator *minuend, struct __iterator *subtraction)
{
    __private_vector *p_private = (__private_vector*)container_of(minuend->iterator_func_p, __private_vector, iter_func);
    return (((__vector_iter*)minuend)->val - ((__vector_iter*)subtraction)->val) / p_private->memb_size;
}

static __iterator_obj_func  __def_vector_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff
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
    __private_vector *__private = (__private_vector*)p_vector->__obj_private;
    *(size_t *)&__private->memb_size = memb_size;
    __private->nmemb = __private->total_storage_memb = nmemb;

    struct __iterator temp_iter = __creat_iter(sizeof(__vector_iter), memb_size, &__private->iter_func);
    memcpy(&__private->start_iter, &temp_iter, sizeof(__vector_iter));
    memcpy(&__private->finish_iter, &temp_iter, sizeof(__vector_iter));
    __init_iter_func((void*)&__private->iter_func, &__def_vector_iter);

    __private->start_iter.val = reallocate(NULL, 0, nmemb * memb_size);
    __private->finish_iter.val = __private->start_iter.val;
    if (init_array) {
        memcpy(__private->start_iter.val, init_array, nmemb * memb_size);
    }
    //memcpy(p_vector->__obj_private, &__private, sizeof(__private_vector));
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    deallocate(p_private->start_iter.val, p_private->total_storage_memb * p_private->memb_size);
}

vector creat_vector(size_t nmemb, size_t memb_size, void *init_array)
{
    vector v;
    init_vector(&v, nmemb, memb_size, init_array);
    return v;
}
