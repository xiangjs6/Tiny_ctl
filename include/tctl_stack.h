//
// Created by xjs on 2020/9/22.
//

#ifndef TINY_CTL_TCTL_STACK_H
#define TINY_CTL_TCTL_STACK_H
#include "tctl_deque.h"
#define STACK_BLOCK_SIZE 512

typedef struct {
    deque c;
} __private_stack;

typedef struct {
    bool (*empty)(void);
    size_t (*size)(void);
    void *(*top)(void);
    void (*push)(void*);
    void (*pop)(void);
    byte __obj_private[sizeof(__private_stack)];
} stack;

void init_stack(stack *p_stack, size_t memb_size);
void destory_stack(stack *p_stack);
stack creat_stack(size_t memb_size);
#endif //TINY_CTL_TCTL_STACK_H
