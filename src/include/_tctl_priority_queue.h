//
// Created by xjs on 2021/1/31.
//

#ifndef TINY_CTL__TCTL_PRIORITY_QUEUE_H
#define TINY_CTL__TCTL_PRIORITY_QUEUE_H

#include "../../include/tctl_priority_queue.h"
#include "_tctl_class.h"
struct Priority_QueueSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*top)(void);
    void (*push)(const void *x);
    void (*pop)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    void (*swap)(Priority_Queue);
};          

const void *_Priority_QueueClass(void);
extern const struct Priority_QueueSelector *_Priority_QueueS;
#endif //TINY_CTL__TCTL_PRIORITY_QUEUE_H
