//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL_TCTL_CLASS_H
#define TINY_CTL_TCTL_CLASS_H

#include "tctl_metaclass.h"

#define CLASS_FUNC                 \
struct {                           \
    METACLASS_FUNC;                \
    bool (*equal)(FormWO_t x);     \
    int (*cmp)(FormWO_t x);        \
    void *(*brackets)(FormWO_t x); \
    void (*inc)(void);             \
    void (*dec)(void);             \
    void (*self_add)(FormWO_t x);  \
    void (*self_sub)(FormWO_t x);  \
    void (*assign)(FormWO_t x);    \
    void *(*add)(FormWO_t x);      \
    void *(*sub)(FormWO_t x);      \
    void *(*mul)(FormWO_t x);      \
    void *(*div)(FormWO_t x);      \
    void *(*mod)(FormWO_t x);      \
    void *(*cast)(const char *c);  \
}

typedef struct {
    METAOBJECT_HEAD(CLASS_FUNC);
} *Class;

typedef struct {
    METAOBJECT_HEAD(CLASS_FUNC);
} *Object;

void initClass(void) __attribute__((constructor));

#define Cast(obj, __T) __cast_aux(obj, #__T)
void *__cast_aux(void *_this, const char *c);

Form_t _Class(void);
Form_t _Object(void);
#define CLASS Class : _Class()
#define OBJECT Object : _Object()
#endif //TINY_CTL_TCTL_CLASS_H
