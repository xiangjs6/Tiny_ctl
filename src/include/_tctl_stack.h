//
// Created by xjs on 2021/1/29.
//

#ifndef TINY_CTL__TCTL_STACK_H
#define TINY_CTL__TCTL_STACK_H
#include "../../include/tctl_stack.h"
#include "_tctl_class.h"
struct StackSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*top)(void);
    void (*push)(const void *x);
    void (*pop)(void);
    bool (*empty)(void);
    size_t (*size)(void);
};

const void *_StackClass(void);
extern const struct StackSelector *_QueueS;
#endif //TINY_CTL__TCTL_STACK_H
