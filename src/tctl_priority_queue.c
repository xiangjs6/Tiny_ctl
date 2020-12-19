//
// Created by xjs on 2020/9/29.
//

#include "../include/tctl_priority_queue.h"
#include "../include/tctl_heap.h"

static bool empty(void)
{
    priority_queue *this = pop_this();
    __private_priority_queue *p_private = (__private_priority_queue*)this->__obj_private;
    return THIS(&p_private->c).empty();
}

static size_t size(void)
{
    priority_queue *this = pop_this();
    __private_priority_queue *p_private = (__private_priority_queue*)this->__obj_private;
    return THIS(&p_private->c).size();
}

static void const *top(void)
{
    priority_queue *this = pop_this();
    __private_priority_queue *p_private = (__private_priority_queue*)this->__obj_private;
    return THIS(&p_private->c).front();
}

static void push(void *x)
{
    priority_queue *this = pop_this();
    __private_priority_queue *p_private = (__private_priority_queue*)this->__obj_private;
    THIS(&p_private->c).push_back(x);
    push_heap(THIS(&p_private->c).begin(), THIS(&p_private->c).end(), p_private->cmp);
}

static void pop(void)
{
    priority_queue *this = pop_this();
    __private_priority_queue *p_private = (__private_priority_queue*)this->__obj_private;
    pop_heap(THIS(&p_private->c).begin(), THIS(&p_private->c).end(), p_private->cmp);
    THIS(&p_private->c).pop_back();
}

static const priority_queue __def_priority_queue = {
        empty,
        size,
        top,
        push,
        pop
};

void init_priority_queue(priority_queue *p, size_t memb_size, Compare cmp)
{
    *p = __def_priority_queue;
    __private_priority_queue *p_private = (__private_priority_queue*)p->__obj_private;
    p_private->cmp = cmp;
    init_vector(&p_private->c, memb_size);
    make_heap(THIS(&p_private->c).begin(), THIS(&p_private->c).end(), cmp);
}

void destory_priority_queue(priority_queue *p)
{
    __private_priority_queue *p_private = (__private_priority_queue*)p->__obj_private;
    destory_vector(&p_private->c);
}

priority_queue creat_priority_queue(size_t memb_size, Compare cmp)
{
    priority_queue pri_que;
    init_priority_queue(&pri_que, memb_size, cmp);
    return pri_que;
}
