//
// Created by xjs on 2020/9/22.
//

#ifndef TINY_CTL_TCTL_QUEUE_H
#define TINY_CTL_TCTL_QUEUE_H
#include "tctl_class.h"
#include "tctl_def.h"
#include "tctl_deque.h"
#include "tctl_metaclass.h"

#define QUEUE_FUNC            \
struct {                      \
    CLASS_FUNC;               \
    void *(*front)(void);     \
    void *(*back)(void);      \
    void (*push)(FormWO_t x); \
    void (*pop)(void);        \
    bool (*empty)(void);      \
    size_t (*size)(void);     \
}

typedef struct _Queue *Queue;
struct _Queue {
    METAOBJECT_HEAD(QUEUE_FUNC);
};

Form_t _Queue(void);
#define QUEUE Queue : _Queue()
#endif //TINY_CTL_TCTL_QUEUE_H
