//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL__TCTL_CLASS_H
#define TINY_CTL__TCTL_CLASS_H
#include "_tctl_metaclass.h"
#include "../../include/tctl_class.h"

struct ClassSelector {
    char _[sizeof(struct MetaClassSelector)];
    bool (*equal)(void *x);
    int (*cmp)(void *x);
    void *(*brackets)(void *x);
    void (*inc)(void);
    void (*dec)(void);
    void (*self_add)(void *x);
    void (*self_sub)(void *x);
    void (*assign)(void *x);
    void *(*add)(void *x);
    void *(*sub)(void *x);
    void *(*mul)(void *x);
    void *(*div)(void *x);
    void *(*mod)(void *x);
};
extern const struct ClassSelector *_ClassS;

#endif //TINY_CTL__TCTL_CLASS_H
