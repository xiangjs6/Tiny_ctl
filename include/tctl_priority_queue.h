//
// Created by xjs on 2020/9/29.
//

#ifndef TINY_CTL_TCTL_PRIORITY_QUEUE_H
#define TINY_CTL_TCTL_PRIORITY_QUEUE_H

#include "tctl_vector.h"
#include "tctl_def.h"
typedef struct {
    vector c;
    Compare cmp;
} __private_priority_queue;

typedef struct {
    bool (*empty)(void);
    size_t (*size)(void);
    void const *(*top)(void);
    void (*push)(void*);
    void (*pop)(void);
    byte __obj_private[sizeof(__private_priority_queue)];
} priority_queue;

void init_priority_queue(priority_queue *p, size_t memb_size, Compare cmp);
void destory_priority_queue(priority_queue *p);
priority_queue creat_priority_queue(size_t memb_size, Compare cmp);
#endif //TINY_CTL_TCTL_PRIORITY_QUEUE_H
