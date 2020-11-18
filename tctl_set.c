//
// Created by xjs on 2020/11/16.
//

#include "tctl_set.h"
//public
static const IterType begin(void)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).begin();
}

static const IterType end(void)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).end();
}

static size_t size(void)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).size();
}

static bool empty(void)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).empty();
}

static void erase(IterType iter)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    THIS(&p_private->t).erase(iter);
}

static const IterType insert(void *x)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).insert_unique(x);
}

static size_t count(void *x)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).count(x);
}

static const IterType find(void *x)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    return THIS(&p_private->t).find(x);
}

static void clear(void)
{
    set *this = pop_this();
    __private_set *p_private = (__private_set*)this->__obj_private;
    THIS(&p_private->t).clear();
}

static const set __def_set = {
        begin,
        end,
        size,
        empty,
        erase,
        insert,
        count,
        find,
        clear
};

void init_set(set *p_set, size_t memb_size, Compare cmp)
{
    *p_set = __def_set;
    __private_set *p_private = (__private_set*)p_set->__obj_private;
    p_private->t = creat_rb_tree(memb_size, cmp);
}

void destory_set(set *p_set)
{
    __private_set *p_private = (__private_set*)p_set->__obj_private;
    destory_rb_tree(&p_private->t);
}

set creat_set(size_t memb_size, Compare cmp)
{
    set s;
    init_set(&s, memb_size, cmp);
    return s;
}