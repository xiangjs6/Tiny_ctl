//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL__TCTL_CLASS_H
#define TINY_CTL__TCTL_CLASS_H
#include "_tctl_metaclass.h"
#include "../../include/tctl_class.h"

struct ClassSelector {
    byte _[sizeof(struct MetaClassSelector)];
    bool (*equal)(FormWO_t x);
    int (*cmp)(FormWO_t x);
    void *(*brackets)(FormWO_t x);
    void (*inc)(void);
    void (*dec)(void);
    void (*self_add)(FormWO_t x);
    void (*self_sub)(FormWO_t x);
    void (*asign)(FormWO_t x);
    void *(*add)(FormWO_t x);
    void *(*sub)(FormWO_t x);
    void *(*mul)(FormWO_t x);
    void *(*div)(FormWO_t x);
    void *(*mod)(FormWO_t x);
    void *(*cast)(const char *c);
};
extern const struct ClassSelector *_ClassS;

#endif //TINY_CTL__TCTL_CLASS_H
