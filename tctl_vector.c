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
    p_private->start_iter = reallocate(p_private->start_iter, old_size * p_private->memb_size, p_private->total_storage_memb * p_private->memb_size);
    p_private->finish_iter = p_private->start_iter+ p_private->memb_size * p_private->nmemb;
}
static void *at(int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (pos >= p_private->nmemb)
        return NULL;
    return p_private->start_iter + p_private->memb_size * pos;
}
static void const * const * begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->start.obj_this == NULL) {
        iterator temp = p_private->start;
        temp.obj_this = this;
        memcpy((void*)&p_private->start, &temp, sizeof(iterator));
    }
    return (void*)&p_private->start_iter;
}
static void const * const * end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (p_private->finish.obj_this == NULL) {
        iterator temp = p_private->finish;
        temp.obj_this = this;
        memcpy((void*)&p_private->finish, &temp, sizeof(iterator));
    }
    return (void*)&p_private->finish_iter;
}
static void *front(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->start_iter;
}
static void *back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    return p_private->finish_iter - p_private->memb_size;
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
    memcpy(p_private->finish_iter, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_iter += p_private->memb_size;
}
static void pop_back(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb--;
    p_private->finish_iter -= p_private->memb_size;
}
static vector_iter *erase(vector_iter *iter)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (*iter > p_private->finish_iter || *iter < p_private->start_iter)
        return NULL;
    size_t back_nmemb = p_private->nmemb - (*iter - p_private->start_iter) / p_private->memb_size - 1;
    memmove(*iter, *iter + p_private->memb_size, p_private->memb_size * back_nmemb);
    p_private->nmemb--;
    p_private->finish_iter -= p_private->memb_size;
    return iter;
}
static vector_iter *insert(vector_iter *iter, void *x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    if (*iter > p_private->finish_iter || *iter < p_private->start_iter)
        return NULL;
    push_this(this);
    if (!this->capacity())
        fill_allocate(this);
    size_t back_nmemb = p_private->nmemb - (*iter - p_private->start_iter) / p_private->memb_size;
    memmove(*iter + p_private->memb_size, *iter, p_private->memb_size * back_nmemb);
    memcpy(*iter, x, p_private->memb_size);
    p_private->nmemb++;
    p_private->finish_iter += p_private->memb_size;
    return iter;
}
static void resize(size_t new_size)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    while (new_size > p_private->total_storage_memb)
        fill_allocate(this);
    p_private->nmemb = new_size;
    p_private->finish_iter = p_private->start_iter + new_size * p_private->memb_size;
}
static void clear(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    p_private->nmemb = 0;
    p_private->finish_iter = p_private->start_iter;
}

static void *iter_at(obj_iter iter, int pos)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    vector_iter *_iter = iter;
    return *_iter + pos * p_private->memb_size;
}

static void iter_increment(void *p)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    vector_iter *iter = p;
    *iter += p_private->memb_size;
}

static void iter_decrement(void *p)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    vector_iter *iter = p;
    *iter -= p_private->memb_size;
}

static void iter_add(void *p, int x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    vector_iter *iter = p;
    *iter += x * p_private->memb_size;
}

static void iter_sub(void *p, int x)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    vector_iter *iter = p;
    *iter -= x * p_private->memb_size;
}

static __iterator_obj_func  __def_vector_iter = {
        iter_at,
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
    __private_vector *__private = (__private_vector*)p_vector->__obj_private;
    *(size_t *)&__private->memb_size = memb_size;
    __private->nmemb = __private->total_storage_memb = nmemb;
    __private->start_iter = reallocate(NULL, 0, nmemb * memb_size);
    __private->finish_iter = __private->start_iter;
    __init_iter((void *)&__private->start, p_vector, sizeof(vector_iter),memb_size, &__def_vector_iter);
    __init_iter((void *)&__private->finish, p_vector, sizeof(vector_iter),memb_size, &__def_vector_iter);
    if (init_array) {
        memcpy(__private->start_iter, init_array, nmemb * memb_size);
    }
    //memcpy(p_vector->__obj_private, &__private, sizeof(__private_vector));
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    deallocate(p_private->start_iter, p_private->total_storage_memb * p_private->memb_size);
}

vector creat_vector(size_t nmemb, size_t memb_size, void *init_array)
{
    vector v;
    init_vector(&v, nmemb, memb_size, init_array);
    __private_vector *p_private = (__private_vector*)v.__obj_private;
    iterator temp1 = p_private->start;
    temp1.obj_this = NULL;
    memcpy((void*)&p_private->start, &temp1, sizeof(iterator));
    iterator temp2 = p_private->finish;
    temp2.obj_this = NULL;
    memcpy((void*)&p_private->finish, &temp2, sizeof(iterator));
    return v;
}
