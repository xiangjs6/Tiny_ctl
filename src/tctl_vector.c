//
// Created by xjs on 2020/9/4.
//

#include "../include/tctl_vector.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include <memory.h>
//private
static void *iter_at(__iterator *iter, int pos)
{
    __vector_iter const *_iter = &iter->val;
    vector *this = iter->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return *_iter + pos * p_private->memb_size;
}

static void iter_increment(__iterator *iter)
{
    __vector_iter *_iter = (__vector_iter*)&iter->val;
    vector *this = iter->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter += p_private->memb_size;
}

static void iter_decrement(__iterator *iter)
{
    __vector_iter *_iter = &iter->val;
    vector *this = iter->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter -= p_private->memb_size;
}

static void iter_add(__iterator *iter, int x)
{
    __vector_iter *_iter = &iter->val;
    vector *this = iter->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter += x * p_private->memb_size;
}

static void iter_sub(__iterator *iter, int x)
{
    __vector_iter *_iter = &iter->val;
    vector *this = iter->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    *_iter -= x * p_private->memb_size;
}

static long long iter_diff(const __iterator *minuend, const __iterator *subtraction)
{
    vector *this = minuend->__inner.obj_this;
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return (((__vector_iter)minuend->val) - ((__vector_iter)subtraction->val)) / (long long)p_private->memb_size;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    return it1->val == it2->val;
}

static const __iterator_obj_func  __def_vector_iter = {
        iter_at,
        iter_increment,
        iter_decrement,
        iter_add,
        iter_sub,
        iter_diff,
        iter_equal
};

static const iterator_func __def_vector_iter_func = INIT_ITER_FUNC(&__def_vector_iter);

static void fill_allocate(vector *this)
{
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    size_t old_size = p_private->total_storage_memb;
    p_private->total_storage_memb *= 2;
    p_private->total_storage_memb = p_private->total_storage_memb ? p_private->total_storage_memb : 1;
    p_private->start_ptr = reallocate(p_private->start_ptr, old_size * p_private->memb_size, p_private->total_storage_memb * p_private->memb_size);
    p_private->finish_ptr = p_private->start_ptr + p_private->memb_size * p_private->nmemb;
}
//public
static void *at(int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    //if (pos >= p_private->nmemb)
    //    return NULL;
    return p_private->start_ptr + p_private->memb_size * pos;
}
static IterType begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    struct __inner_iterator *start = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__vector_iter));
    init_iter(start, sizeof(__vector_iter), this, p_private->memb_size, &__def_vector_iter_func);
    //*(__vector_iter*)pair_iter->start->__inner.__address = p_private->start_ptr;
    memcpy(start->__address, &p_private->start_ptr, sizeof(__vector_iter));
    return start;
}
static IterType end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    struct __inner_iterator *finish = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__vector_iter));
    init_iter(finish, sizeof(__vector_iter), this, p_private->memb_size, &__def_vector_iter_func);
    memcpy(finish->__address, &p_private->finish_ptr, sizeof(__vector_iter));
    //pair_iter->finish_ptr = p_private->finish_ptr;
    return finish;
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
    return !p_private->nmemb;
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
static IterType erase(IterType iter)
{
    vector *this = pop_this();
    __iterator *p_iter = iter;
    __vector_iter const *_iter = &p_iter->val;//获取vector迭代器的地址
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (*_iter > p_private->finish_ptr || *_iter < p_private->start_ptr)
        return NULL;
    size_t back_nmemb = p_private->nmemb - (*_iter - p_private->start_ptr) / p_private->memb_size - 1;
    memmove(*_iter, *_iter + p_private->memb_size, p_private->memb_size * back_nmemb);
    p_private->nmemb--;
    p_private->finish_ptr -= p_private->memb_size;
    return iter;
}
static IterType insert(IterType iter, void *x)
{
    vector *this = pop_this();
    __iterator *p_iter = iter;
    __vector_iter const *_iter = &p_iter->val;
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

static void swap(struct vector *v)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    __private_vector *p_v_private = (__private_vector*)v->__obj_private;
    if (p_private->memb_size != p_v_private->memb_size)
        return;
    __private_vector tmp = *p_private;
    memcpy(p_private, p_v_private, sizeof(__private_vector));
    memcpy(p_v_private, &tmp, sizeof(__private_vector));
}

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
        clear,
        swap
};

void init_vector(vector *p_vector, size_t memb_size)
{
    *p_vector = __def_vector;
    __private_vector *__private = (__private_vector*)p_vector->__obj_private;
    *(size_t *)&__private->memb_size = memb_size;
    __private->nmemb = __private->total_storage_memb = 0;

    //__private->start_ptr = reallocate(NULL, 0, nmemb * memb_size);
    __private->finish_ptr = __private->start_ptr = NULL;
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    if (p_private->total_storage_memb)
        deallocate(p_private->start_ptr, p_private->total_storage_memb * p_private->memb_size);
}

vector creat_vector(size_t memb_size)
{
    vector v;
    init_vector(&v, memb_size);
    return v;
}
