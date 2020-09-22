//
// Created by xjs on 2020/9/22.
//

#ifndef TINY_CTL_TCTL_QUEUE_H
#define TINY_CTL_TCTL_QUEUE_H
#include "tctl_def.h"
#include "tctl_deque.h"

#define QUEUE_BLOCK_SIZE 512
typedef struct {
    deque c;
} __private_queue;

typedef struct {
    bool (*empty)(void);
    size_t (*size)(void);
    void const *(*front)(void);
    void const *(*back)(void);
    void (*push)(void*);
    void (*pop)(void);
    byte __obj_private[sizeof(__private_queue)];
} queue;

void init_queue(queue *p_queue, size_t memb_size);
void destory_queue(queue *p_queue);
queue creat_queue(size_t memb_size);
#endif //TINY_CTL_TCTL_QUEUE_H
