//
// Created by xjs on 2020/12/20.
//

#include "../include/tctl_unordered_multimap.h"
//private
static void *get_key(const void *x)
{
    return (void*)x;
}

//public
static IterType begin(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).begin();
}

static IterType end(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).end();
}

static bool empty(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).empty();
}

static size_t size(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).size();
}

static size_t max_size(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static IterType insert(void *x)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).insert_equal(x);
}

static void erase(IterType it)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    THIS(&p_private->ht).erase(it);
}

static void clear(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    THIS(&p_private->ht).clear();
}

static IterType find(void *x)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).find(x);
}

static size_t count(void *x)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).count(x);
}

static size_t bucket_count(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).bucket_count();
}

static size_t max_bucket_count(void)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static void reserve(size_t n)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    THIS(&p_private->ht).resize(n);
}

static void swap(struct unordered_multimap *m)
{
    unordered_multimap *this = pop_this();
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)this->__obj_private;
    __private_unordered_multimap *p_m_private = (__private_unordered_multimap*)m->__obj_private;
    THIS(&p_private->ht).swap(&p_m_private->ht);
}

static const unordered_multimap _def_unordered_multimap = {
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

void init_unordered_multimap(unordered_multimap *p_m, size_t key_size, size_t val_size, Compare equal, HashFunc hash)
{
    *p_m = _def_unordered_multimap;
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)p_m->__obj_private;
    init_hashtable(&p_private->ht, key_size + val_size, equal, hash, get_key);
}

void destory_unordered_multimap(unordered_multimap *p_m)
{
    __private_unordered_multimap *p_private = (__private_unordered_multimap*)p_m->__obj_private;
    destory_hashtable(&p_private->ht);
}

unordered_multimap creat_unordered_multimap(size_t key_size, size_t val_size, Compare equal, HashFunc hash)
{
    unordered_multimap m;
    init_unordered_multimap(&m, key_size, val_size, equal, hash);
    return m;
}
