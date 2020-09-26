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
    p_private->start_ptr = reallocate(p_private->start_ptr, old_size * p_private->memb_size, p_private->total_storage_memb * p_private->memb_size);
    p_private->finish_ptr = p_private->start_ptr + p_private->memb_size * p_private->nmemb;
}
static void *at(int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (pos >= p_private->nmemb)
        return NULL;
    return p_private->start_ptr+ p_private->memb_size * pos;
}
static const __iterator *begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->start_iter.obj_this != this)
        p_private->start_iter.obj_this = this;
    return (__iterator*)&p_private->start_iter;
}
static const __iterator *end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->finish_iter.obj_this != this)
        p_private->finish_iter.obj_this = this;
    return (__iterator*)&p_private->finish_iter;
}
static void const *front(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->start_ptr;
}
static void const *back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->finish_ptr - p_private->memb_size;
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
    memcpy(p_private->finish_ptr, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_ptr += p_private->memb_size;
}
static void pop_back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb--;
    p_private->finish_ptr -= p_private->memb_size;
}
static __iterator *erase(__iterator *iter)
{
    vector *this = pop_this();
    __vector_iter const *_iter = &iter->val;//获取vector迭代器的地址
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (*_iter > p_private->finish_ptr || *_iter < p_private->start_ptr)
        return NULL;
    size_t back_nmemb = p_private->nmemb - (*_iter - p_private->start_ptr) / p_private->memb_size - 1;
    memmove(*_iter, *_iter + p_private->memb_size, p_private->memb_size * back_nmemb);
    p_private->nmemb--;
    p_private->finish_ptr -= p_private->memb_size;
    return iter;
}
static __iterator *insert(__iterator *iter, void *x)
{
    vector *this = pop_this();
    __vector_iter const *_iter = &iter->val;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (*_iter > p_private->finish_ptr || *_iter < p_private->start_ptr)
        return NULL;
    push_this(this);
    if (!this->capacity())
        fill_allocate(this);
    size_t back_nmemb = p_private->nmemb - (*_iter - p_private->start_ptr) / p_private->memb_size;
    memmove(*_iter + p_private->memb_size, *_iter, p_private->memb_size * back_nmemb);
    memcpy(*_iter, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_ptr += p_private->memb_size;
    return iter;
}
static void resize(size_t new_size)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    while (new_size > p_private->total_storage_memb)
        fill_allocate(this);
    p_private->nmemb = new_size;
    p_private->finish_ptr = p_private->start_ptr + new_size * p_private->memb_size;
}
static void clear(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb = 0;
    p_private->finish_ptr = p_private->start_ptr;
}

static void *iter_at(__iterator *iter, int pos)
{
    __vector_iter const *_iter = &iter->val;
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return *_iter + pos * p_private->memb_size;
}

static void iter_increment(__iterator *iter)
{
    __vector_iter *_iter = (__vector_iter*)&iter->val;
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter += p_private->memb_size;
}

static void iter_decrement(__iterator *iter)
{
    __vector_iter *_iter = &iter->val;
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter -= p_private->memb_size;
}

static void iter_add(__iterator *iter, int x)
{
    __vector_iter *_iter = &iter->val;
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter += x * p_private->memb_size;
}

static void iter_sub(__iterator *iter, int x)
{
    __vector_iter *_iter = &iter->val;
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter -= x * p_private->memb_size;
}

static long long iter_diff(const __iterator *minuend, const __iterator *subtraction)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return (((__vector_iter)minuend->val) - ((__vector_iter)subtraction->val)) / p_private->memb_size;
}

static const __iterator_obj_func  __def_vector_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff
};
static const iterator_func __def_vector_iter_func = INIT_ITER_FUNC(&__def_vector_iter);

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

    __private->finish_iter = __creat_iter(sizeof(__vector_iter), p_vector, memb_size, &__def_vector_iter_func);
    __private->start_iter = __creat_iter(sizeof(__vector_iter), p_vector, memb_size, &__def_vector_iter_func);

    __private->start_ptr = reallocate(NULL, 0, nmemb * memb_size);
    __private->finish_ptr = __private->start_ptr;
    if (init_array) {
        memcpy(__private->start_ptr, init_array, nmemb * memb_size);
    }
    //memcpy(p_vector->__obj_private, &__private, sizeof(__private_vector));
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    deallocate(p_private->start_ptr, p_private->total_storage_memb * p_private->memb_size);
}

vector creat_vector(size_t nmemb, size_t memb_size, void *init_array)
{
    vector v;
    init_vector(&v, nmemb, memb_size, init_array);
    return v;
}
