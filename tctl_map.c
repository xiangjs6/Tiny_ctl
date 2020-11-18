//
// Created by xjs on 2020/11/18.
//

#include "tctl_map.h"

static const IterType begin(void)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).begin();
}

static const IterType end(void)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).end();
}

static size_t size(void)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).size();
}

static bool empty(void)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).empty();
}

static void erase(IterType iter)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    THIS(&p_private->t).erase(iter);
}

static const IterType insert(void *x)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).insert_unique(x);
}

static size_t count(void *x)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).count(x);
}

static const IterType find(void *x)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    return THIS(&p_private->t).find(x);
}

static void clear(void)
{
    map *this = pop_this();
    __private_map *p_private = (__private_map*)this->__obj_private;
    THIS(&p_private->t).clear();
}

static const map __def_map = {
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

void init_map(map *p_map, size_t key_size, size_t val_size, Compare cmp)
{
    *p_map = __def_map;
    __private_map *p_private = (__private_map*)p_map->__obj_private;
    init_rb_tree(&p_private->t, key_size + val_size, cmp);
}

void destory_map(map *p_map)
{
    __private_map *p_private = (__private_map*)p_map->__obj_private;
    destory_rb_tree(&p_private->t);
}

map creat_map(size_t key_size, size_t val_size, Compare cmp)
{
    map m;
    init_map(&m, key_size, val_size, cmp);
    return m;
}
