//
// Created by xjs on 2020/9/22.
//

#include "../include/tctl_queue.h"
static bool empty(void)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    return THIS(&p_private->c).empty();
}
static size_t size(void)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    return THIS(&p_private->c).size();
}
static void const *front(void)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    return THIS(&p_private->c).front();
}
static void const *back(void)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    return THIS(&p_private->c).back();
}
static void push(void* x)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    THIS(&p_private->c).push_back(x);
}
static void pop(void)
{
    queue *this = pop_this();
    __private_queue *p_private = (__private_queue*)this->__obj_private;
    THIS(&p_private->c).pop_front();
}

static const queue __def_queue = {
        empty,
        size,
        front,
        back,
        push,
        pop
};

void init_queue(queue *p_queue, size_t memb_size)
{
    *p_queue = __def_queue;
    __private_queue *p_private = (__private_queue*)p_queue->__obj_private;
    init_deque(&p_private->c, memb_size, QUEUE_BLOCK_SIZE);
}

void destory_queue(queue *p_queue)
{
    __private_queue *p_private = (__private_queue*)p_queue->__obj_private;
    destory_deque(&p_private->c);
}

queue creat_queue(size_t memb_size)
{
    queue que = __def_queue;
    __private_queue *p_private = (__private_queue*)que.__obj_private;
    p_private->c = creat_deque(memb_size, QUEUE_BLOCK_SIZE);
    return que;
}