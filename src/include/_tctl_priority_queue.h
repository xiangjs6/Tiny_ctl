//
// Created by xjs on 2021/1/31.
//

#ifndef TINY_CTL__TCTL_PRIORITY_QUEUE_H
#define TINY_CTL__TCTL_PRIORITY_QUEUE_H

#include "../../include/tctl_priority_queue.h"
#include "_tctl_class.h"
struct Priority_queueSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*top)(void);
    void (*push)(FormWO_t x);
    void (*pop)(void);
    bool (*empty)(void);
    size_t (*size)(void);
    void (*swap)(Priority_queue);
};          

Form_t _Priority_queueClass(void);
extern const struct Priority_queueSelector *_Priority_queueS;
#endif //TINY_CTL__TCTL_PRIORITY_QUEUE_H
