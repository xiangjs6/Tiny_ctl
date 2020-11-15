//
// Created by xjs on 2020/9/4.
//

#include "tctl_vector.h"
#include "tctl_allocator.h"
#include <memory.h>
//private
struct inner_iter {
    struct __inner_iterator *start;
    struct __inner_iterator *finish;
};

static void free_inner_iter(void *p)
{
    if (!p)
        return;
    struct inner_iter *__p = p;
    if (!__p->start->used_by_out)
        __destructor_iter(&__p->start);
    if (!__p->finish->used_by_out)
        __destructor_iter(&__p->finish);
    deallocate(p, sizeof(struct inner_iter));
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
    return (((__vector_iter)minuend->val) - ((__vector_iter)subtraction->val)) / (long long)p_private->memb_size;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    pop_this();
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
    if (pos >= p_private->nmemb)
        return NULL;
    return p_private->start_ptr+ p_private->memb_size * pos;
}
static const IterType begin(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    struct inner_iter *pair_iter = pthread_getspecific(p_private->iter_key);
    if (pair_iter == NULL) {
        pair_iter = allocate(sizeof(struct inner_iter));
        pair_iter->finish = pair_iter->start = NULL;
        pthread_setspecific(p_private->iter_key, pair_iter);
    }
    if (pair_iter->start == NULL || pair_iter->start->used_by_out) {
        pair_iter->start = allocate(sizeof(struct __inner_iterator) + sizeof(__vector_iter));
        *pair_iter->start = __creat_iter(sizeof(__vector_iter), this, p_private->memb_size, &__def_vector_iter_func);
    }
    //*(__vector_iter*)pair_iter->start->__inner.__address = p_private->start_ptr;
    memcpy(pair_iter->start->__address, &p_private->start_ptr, sizeof(__vector_iter));
    return pair_iter->start;
}
static const IterType end(void)
{
    vector *this = pop_this();
    __private_vector *p_private = (__private_vector*)this->__obj_private;
    struct inner_iter *pair_iter = pthread_getspecific(p_private->iter_key);
    if (pair_iter == NULL) {
        pair_iter = allocate(sizeof(struct inner_iter));
        pair_iter->finish = pair_iter->start = NULL;
        pthread_setspecific(p_private->iter_key, pair_iter);
    }
    if (pair_iter->finish == NULL || pair_iter->finish->used_by_out) {
        pair_iter->finish = allocate(sizeof(struct __inner_iterator) + sizeof(__vector_iter));
        *pair_iter->finish = __creat_iter(sizeof(__vector_iter), this, p_private->memb_size, &__def_vector_iter_func);
    }
    memcpy(pair_iter->finish->__address, &p_private->finish_ptr, sizeof(__vector_iter));
    //pair_iter->finish_ptr = p_private->finish_ptr;
    return pair_iter->finish;
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

void init_vector(vector *p_vector, size_t memb_size)
{
    *p_vector = __def_vector;
    __private_vector *__private = (__private_vector*)p_vector->__obj_private;
    *(size_t *)&__private->memb_size = memb_size;
    __private->nmemb = __private->total_storage_memb = 0;

    pthread_key_create(&__private->iter_key, free_inner_iter);
    //__private->start_ptr = reallocate(NULL, 0, nmemb * memb_size);
    __private->finish_ptr = __private->start_ptr = NULL;
}

void destory_vector(vector *p_vector)
{
    __private_vector *p_private = (__private_vector *)p_vector->__obj_private;
    deallocate(p_private->start_ptr, p_private->total_storage_memb * p_private->memb_size);
}

vector creat_vector(size_t memb_size)
{
    vector v;
    init_vector(&v, memb_size);
    return v;
}
