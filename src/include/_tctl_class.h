//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL__TCTL_CLASS_H
#define TINY_CTL__TCTL_CLASS_H
#include "_tctl_object.h"
#include "../../include/tctl_class.h"

struct ClassSelector {
    byte _[sizeof(struct MetaClassSelector)];
    bool (*equal)(const void *x);
    int (*cmp)(const void *x);
    void *(*brackets)(const void *x);
    void (*inc)(void);
    void (*dec)(void);
    void (*self_add)(const void *x);
    void (*self_sub)(const void *x);
    void (*asign)(const void *x);
    void *(*add)(const void *x);
    void *(*sub)(const void *x);
    void *(*mul)(const void *x);
    void *(*div)(const void *x);
    void *(*mod)(const void *x);
    void *(*cast)(const char *c);
};
extern const struct ClassSelector *_ClassS;

#endif //TINY_CTL__TCTL_CLASS_H
