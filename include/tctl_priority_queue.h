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
    void (*push)(FormWO_t x);     \
    void (*pop)(void);            \
    bool (*empty)(void);          \
    size_t (*size)(void);         \
    void (*swap)(Priority_queue); \
}

typedef struct _Priority_queue *Priority_queue;
struct _Priority_queue {
    METAOBJECT_HEAD(PRIORITY_QUEUE_FUNC);
};

void initPriority_queue(void) __attribute__((constructor));
Form_t _Priority_queue(void);
#define PRIORITY_QUEUE Priority_queue : _Priority_queue()
#endif //TINY_CTL_TCTL_PRIORITY_QUEUE_H
