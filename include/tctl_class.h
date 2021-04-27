//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL_TCTL_CLASS_H
#define TINY_CTL_TCTL_CLASS_H

#include "tctl_metaclass.h"
#include <stdbool.h>

#define CLASS_FUNC                    \
struct {                              \
    METACLASS_FUNC;                   \
    bool (*equal)(const void *x);     \
    int (*cmp)(const void *x);        \
    void *(*brackets)(const void *x); \
    void (*inc)(void);                \
    void (*dec)(void);                \
    void (*self_add)(const void *x);  \
    void (*self_sub)(const void *x);  \
    void (*assign)(const void *x);    \
    void *(*add)(const void *x);      \
    void *(*sub)(const void *x);      \
    void *(*mul)(const void *x);      \
    void *(*div)(const void *x);      \
    void *(*mod)(const void *x);      \
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
