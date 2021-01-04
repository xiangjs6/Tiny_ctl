//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL__TCTL_CLASS_H
#define TINY_CTL__TCTL_CLASS_H
#include "_tctl_object.h"
#include "../../include/tctl_class.h"
struct Class {
    const struct MetaClass _;
    bool (*equal)(const void *_this, const void *x);
    int (*cmp)(const void *_this, const void *x);
    void *(*at)(const void *_this, int x);
    void (*inc)(void *_this);
    void (*dec)(void *_this);
    void (*self_add)(void *_this, const void *x);
    void (*self_sub)(void *_this, const void *x);
    void (*asign)(void *_this, const void *x);
    void *(*add)(const void *_this, const void *x);
    void *(*sub)(const void *_this, const void *x);
    void *(*mul)(const void *_this, const void *x);
    void *(*div)(const void *_this, const void *x);
    void *(*mod)(const void *_this, const void *x);
};

extern const struct {
    byte _[sizeof(*_MetaClassS)];
    bool (*equal)(const void *x);
    int (*cmp)(const void *x);
    void *(*at)(int x);
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
} *_ClassS;

#endif //TINY_CTL__TCTL_CLASS_H
