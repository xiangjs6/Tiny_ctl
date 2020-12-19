//
// Created by xjs on 2020/12/19.
//

#include "../include/tctl_unordered_set.h"
//private
static void *get_key(const void *x)
{
    return (void*)x;
}

//public
static IterType begin(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).begin();
}

static IterType end(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).end();
}

static bool empty(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).empty();
}

static size_t size(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).size();
}

static size_t max_size(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static IterType insert(void *x)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).insert_unique(x);
}

static void erase(IterType it)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    THIS(&p_private->ht).erase(it);
}

static void clear(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    THIS(&p_private->ht).clear();
}

static IterType find(void *x)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).find(x);
}

static size_t count(void *x)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).count(x);
}

static size_t bucket_count(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).bucket_count();
}

static size_t max_bucket_count(void)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    return THIS(&p_private->ht).max_bucket_count();
}

static void reserve(size_t n)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    THIS(&p_private->ht).resize(n);
}

static void swap(struct unordered_set *s)
{
    unordered_set *this = pop_this();
    __private_unordered_set *p_private = (__private_unordered_set*)this->__obj_private;
    __private_unordered_set *p_s_private = (__private_unordered_set*)s->__obj_private;
    THIS(&p_private->ht).swap(&p_s_private->ht);
}

static const unordered_set _def_unordered_set = {
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

void init_unordered_set(unordered_set *p_s, size_t memb_size, Compare equal, HashFunc hash)
{
    *p_s = _def_unordered_set;
    __private_unordered_set *p_private = (__private_unordered_set*)p_s->__obj_private;
    init_hashtable(&p_private->ht, memb_size, equal, hash, get_key);
}

void destory_unordered_set(unordered_set *p_s)
{
    __private_unordered_set *p_private = (__private_unordered_set*)p_s->__obj_private;
    destory_hashtable(&p_private->ht);
}

unordered_set creat_unordered_set(size_t memb_size, Compare equal, HashFunc hash)
{
    unordered_set s;
    init_unordered_set(&s, memb_size, equal, hash);
    return s;
}
