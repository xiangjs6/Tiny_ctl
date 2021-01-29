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
    void (*push)(FormWO_t x);
    void (*pop)(void);
    bool (*empty)(void);
    size_t (*size)(void);
};

Form_t _StackClass(void);
extern const struct StackSelector *_QueueS;
#endif //TINY_CTL__TCTL_STACK_H
