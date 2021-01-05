//
// Created by xjs on 2021/1/5.
//

#ifndef TINY_CTL__TCTL_ITERATOR_H
#define TINY_CTL__TCTL_ITERATOR_H
#include "_tctl_class.h"

struct IteratorClass {
    struct Class _;
    void *(*derefer)(void);
};

struct IteratorSelector {
    char _[sizeof(struct ClassSelector)];
    void *(*derefer)(void);
};
extern const struct IteratorSelector *_IteratorS;
#endif //TINY_CTL__TCTL_ITERATOR_H
