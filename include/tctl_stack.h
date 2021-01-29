//
// Created by xjs on 2020/9/22.
//

#ifndef TINY_CTL_TCTL_STACK_H
#define TINY_CTL_TCTL_STACK_H
#include "tctl_class.h"
#include "tctl_def.h"
#include "tctl_deque.h"
#include "tctl_metaclass.h"

#define STACK_FUNC            \
struct {                      \
    CLASS_FUNC;               \
    void *(*top)(void);       \
    void (*push)(FormWO_t x); \
    void (*pop)(void);        \
    bool (*empty)(void);      \
    size_t (*size)(void);     \
}

typedef struct _Stack *Stack;
struct _Stack {
    METAOBJECT_HEAD(STACK_FUNC);
};

void initStack(void) __attribute__((constructor));
Form_t _Stack(void);
#define STACK Stack : _Stack()
#endif //TINY_CTL_TCTL_STACK_H
