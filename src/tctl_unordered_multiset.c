//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_unordered_multiset.h"
//private
static void *get_key(const void *x)
{
    return (void*)x;
}

//public
static IterType begin(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).begin();
}

static IterType end(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).end();
}

static bool empty(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).empty();
}

static size_t size(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).size();
}

static size_t max_size(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static IterType insert(void *x)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).insert_equal(x);
}

static void erase(IterType it)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    THIS(&p_private->ht).erase(it);
}

static void clear(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    THIS(&p_private->ht).clear();
}

static IterType find(void *x)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).find(x);
}

static size_t count(void *x)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).count(x);
}

static size_t bucket_count(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).bucket_count();
}

static size_t max_bucket_count(void)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static void reserve(size_t n)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    THIS(&p_private->ht).resize(n);
}

static void swap(struct unordered_multiset *s)
{
    unordered_multiset *this = pop_this();
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)this->__obj_private;
    __private_unordered_multiset *p_s_private = (__private_unordered_multiset*)s->__obj_private;
    THIS(&p_private->ht).swap(&p_s_private->ht);
}

static const unordered_multiset _def_unordered_multiset = {
        begin,
        end,
        empty,
        size,
        max_size,
        insert,
        erase,
        clear,
        find,
        count,
        bucket_count,
        max_bucket_count,
        reserve,
        swap
};

void init_unordered_multiset(unordered_multiset *p_s, size_t memb_size, Compare equal, HashFunc hash)
{
    *p_s = _def_unordered_multiset;
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)p_s->__obj_private;
    init_hashtable(&p_private->ht, memb_size, equal, hash, get_key);
}

void destory_unordered_multiset(unordered_multiset *p_s)
{
    __private_unordered_multiset *p_private = (__private_unordered_multiset*)p_s->__obj_private;
    destory_hashtable(&p_private->ht);
}

unordered_multiset creat_unordered_multiset(size_t memb_size, Compare equal, HashFunc hash)
{
    unordered_multiset s;
    init_unordered_multiset(&s, memb_size, equal, hash);
    return s;
}
