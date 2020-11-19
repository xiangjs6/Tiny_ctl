//
// Created by xjs on 2020/11/19.
//

#include "tctl_multimap.h"
static const IterType begin(void)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).begin();
}

static const IterType end(void)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).end();
}

static size_t size(void)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).size();
}

static bool empty(void)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).empty();
}

static void erase(IterType iter)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    THIS(&p_private->t).erase(iter);
}

static const IterType insert(void *x)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).insert_unique(x);
}

static size_t count(void *x)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).count(x);
}

static const IterType find(void *x)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    return THIS(&p_private->t).find(x);
}

static void clear(void)
{
    multimap *this = pop_this();
    __private_multimap *p_private = (__private_multimap*)this->__obj_private;
    THIS(&p_private->t).clear();
}

static const multimap __def_multimap = {
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

void init_multimap(multimap *p_multimap, size_t key_size, size_t val_size, Compare cmp)
{
    *p_multimap = __def_multimap;
    __private_multimap *p_private = (__private_multimap*)p_multimap->__obj_private;
    init_rb_tree(&p_private->t, key_size + val_size, cmp);
}

void destory_multimap(multimap *p_multimap)
{
    __private_multimap *p_private = (__private_multimap*)p_multimap->__obj_private;
    destory_rb_tree(&p_private->t);
}

multimap creat_multimap(size_t key_size, size_t val_size, Compare cmp)
{
    multimap m;
    init_multimap(&m, key_size, val_size, cmp);
    return m;
}
