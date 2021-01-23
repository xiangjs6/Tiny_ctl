//
// Created by xjs on 2021/1/21.
//

#ifndef TINY_CTL__TCTL_DEQUE_H
#define TINY_CTL__TCTL_DEQUE_H

#include "../../include/tctl_deque.h"
#include "_tctl_class.h"
struct DequeSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    void* (*front)(void);
    void* (*back)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*push_back)(FormWO_t x);
    void (*push_front)(FormWO_t x);
    void (*pop_back)(void);
    void (*pop_front)(void);
    Iterator (*erase)(Iterator iter);
    Iterator (*insert)(Iterator iter, FormWO_t x);
    void (*resize)(size_t new_size);
    void (*clear)(void);
    void (*swap)(Deque _d);
};
Form_t _DequeClass(void);
extern const struct DequeSelector *_DequeS;
#endif //TINY_CTL__TCTL_DEQUE_H
