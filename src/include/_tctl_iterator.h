//
// Created by xjs on 2021/1/5.
//

#ifndef TINY_CTL__TCTL_ITERATOR_H
#define TINY_CTL__TCTL_ITERATOR_H
#include "_tctl_class.h"
#include "../../include/tctl_iterator.h"

struct IteratorClass {
    struct Class _;
    void *(*derefer)(const void *_this);
};

struct Iterator {
    struct Object _;
    byte _v[0];
};

struct IteratorSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*derefer)(void);
};

extern const struct IteratorSelector *_IteratorS;
#endif //TINY_CTL__TCTL_ITERATOR_H