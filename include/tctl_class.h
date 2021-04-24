//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL_TCTL_CLASS_H
#define TINY_CTL_TCTL_CLASS_H

#include "tctl_metaclass.h"

#define CLASS_FUNC              \
struct {                        \
    METACLASS_FUNC;             \
    bool (*equal)(void *x);     \
    int (*cmp)(void *x);        \
    void *(*brackets)(void *x); \
    void (*inc)(void);          \
    void (*dec)(void);          \
    void (*self_add)(void *x);  \
    void (*self_sub)(void *x);  \
    void (*assign)(void *x);    \
    void *(*add)(void *x);      \
    void *(*sub)(void *x);      \
    void *(*mul)(void *x);      \
    void *(*div)(void *x);      \
    void *(*mod)(void *x);      \
}

typedef struct {
    METAOBJECT_HEAD(CLASS_FUNC);
} *Class;

typedef struct {
    METAOBJECT_HEAD(CLASS_FUNC);
} *Object;

const void *_Class(void);
const void *_Object(void);
#define CLASS Class : _Class()
#define OBJECT Object : _Object()
#endif //TINY_CTL_TCTL_CLASS_H
