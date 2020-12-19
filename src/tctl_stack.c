//
// Created by xjs on 2020/9/22.
//

#include "../include/tctl_stack.h"
static bool empty(void)
{
    stack *this = pop_this();
    __private_stack *p_private = (__private_stack*)this->__obj_private;
    return THIS(&p_private->c).empty();
}
static size_t size(void)
{
    stack *this = pop_this();
    __private_stack *p_private = (__private_stack*)this->__obj_private;
    return THIS(&p_private->c).size();
}
static void *top(void)
{
    stack *this = pop_this();
    __private_stack *p_private = (__private_stack*)this->__obj_private;
    return (void*)THIS(&p_private->c).back();
}
static void push(void *x)
{
    stack *this = pop_this();
    __private_stack *p_private = (__private_stack*)this->__obj_private;
    THIS(&p_private->c).push_back(x);
}
static void pop(void)
{
    stack *this = pop_this();
    __private_stack *p_private = (__private_stack*)this->__obj_private;
    THIS(&p_private->c).pop_back();
}

static const stack __def_stack = {
        empty,
        size,
        top,
        push,
        pop
};

void init_stack(stack *p_stack, size_t memb_size)
{
    *p_stack = __def_stack;
    __private_stack *p_private = (__private_stack*)p_stack->__obj_private;
    init_deque(&p_private->c, memb_size, STACK_BLOCK_SIZE);
}

void destory_stack(stack *p_stack)
{
    __private_stack *p_private = (__private_stack*)p_stack->__obj_private;
    destory_deque(&p_private->c);
}

stack creat_stack(size_t memb_size)
{
    stack st = __def_stack;
    __private_stack *p_private = (__private_stack*)st.__obj_private;
    p_private->c = creat_deque(memb_size, STACK_BLOCK_SIZE);
    return st;
}