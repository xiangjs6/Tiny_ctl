//
// Created by xjs on 2021/1/28.
//

#ifndef TINY_CTL__TCTL_QUEUE_H
#define TINY_CTL__TCTL_QUEUE_H

#include "../../include/tctl_queue.h"
#include "_tctl_class.h"
struct QueueSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*front)(void);
    void *(*back)(void);
    void (*push)(FormWO_t x);
    void (*pop)(void);
    bool (*empty)(void);
    size_t (*size)(void);
};          

Form_t _QueueClass(void);
extern const struct QueueSelector *_QueueS;
#endif //TINY_CTL__TCTL_QUEUE_H
