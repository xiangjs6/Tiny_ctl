//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL_TCTL_CLASS_H
#define TINY_CTL_TCTL_CLASS_H

#include "../include/tctl_object.h"

#define INHERIT_CLASS \
struct {              \
    INHERIT_METACLASS;\
    bool (*equal)(const void *x); \
    int (*cmp)(const void *x);    \
    void *(*at)(int x);           \
    void (*inc)(void);            \
    void (*dec)(void);            \
    void (*self_add)(const void *x);    \
    void (*self_sub)(const void *x);    \
    void (*asign)(const void *x);       \
    void *(*add)(const void *x);        \
    void *(*sub)(const void *x);        \
    void *(*mul)(const void *x);        \
    void *(*div)(const void *x);        \
    void *(*mod)(const void *x);        \
}

typedef struct {
    union {
        INHERIT_CLASS *_x;
        byte _pad[sizeof(*(Object)NULL)];
    };
    byte _o[0];
} *Class;

void initClass(void) __attribute__((constructor));

const void *_Class(void);
//extern const void *_Class;
#define CLASS Class : _Class()
#endif //TINY_CTL_TCTL_CLASS_H
