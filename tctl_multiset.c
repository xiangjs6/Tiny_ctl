//
// Created by xjs on 2020/11/19.
//

#include "tctl_multiset.h"
//public
static IterType begin(void)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).begin();
}

static IterType end(void)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).end();
}

static size_t size(void)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).size();
}

static bool empty(void)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).empty();
}

static void erase(IterType iter)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    THIS(&p_private->t).erase(iter);
}

static IterType insert(void *x)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).insert_equal(x);
}

static size_t count(void *x)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).count(x);
}

static IterType find(void *x)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    return THIS(&p_private->t).find(x);
}

static void clear(void)
{
    multiset *this = pop_this();
    __private_multiset *p_private = (__private_multiset*)this->__obj_private;
    THIS(&p_private->t).clear();
}

static const multiset __def_multiset = {
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

void init_multiset(multiset *p_multiset, size_t memb_size, Compare cmp)
{
    *p_multiset = __def_multiset;
    __private_multiset *p_private = (__private_multiset*)p_multiset->__obj_private;
    p_private->t = creat_rb_tree(memb_size, cmp);
}

void destory_multiset(multiset *p_multiset)
{
    __private_multiset *p_private = (__private_multiset*)p_multiset->__obj_private;
    destory_rb_tree(&p_private->t);
}

multiset creat_multiset(size_t memb_size, Compare cmp)
{
    multiset s;
    init_multiset(&s, memb_size, cmp);
    return s;
}
