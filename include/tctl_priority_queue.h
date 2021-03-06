//
// Created by xjs on 2020/9/29.
//

#ifndef TINY_CTL_TCTL_PRIORITY_QUEUE_H
#define TINY_CTL_TCTL_PRIORITY_QUEUE_H

#include "tctl_metaclass.h"
#include "tctl_class.h"
#include "tctl_def.h"

#define PRIORITY_QUEUE_FUNC       \
struct {                          \
    CLASS_FUNC;                   \
    void *(*top)(void);           \
    void (*push)(const void *x);  \
    void (*pop)(void);            \
    bool (*empty)(void);          \
    size_t (*size)(void);         \
    void (*swap)(Priority_Queue); \
}

typedef struct _Priority_Queue *Priority_Queue;
struct _Priority_Queue {
    METAOBJECT_HEAD(PRIORITY_QUEUE_FUNC);
};

const void *_Priority_Queue(void);
#define PRIORITY_QUEUE Priority_Queue : _Priority_Queue()
#endif //TINY_CTL_TCTL_PRIORITY_QUEUE_H
