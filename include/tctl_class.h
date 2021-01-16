//
// Created by xjs on 2020/12/30.
//

#ifndef TINY_CTL_TCTL_CLASS_H
#define TINY_CTL_TCTL_CLASS_H

#include "tctl_object.h"

#define Cast(obj, __T) __cast_aux(obj, #__T)
#define INHERIT_CLASS \
struct {                           \
    INHERIT_METACLASS;             \
    bool (*equal)(FormWO_t x);     \
    int (*cmp)(FormWO_t x);        \
    void *(*brackets)(FormWO_t x); \
    void (*inc)(void);             \
    void (*dec)(void);             \
    void (*self_add)(FormWO_t x);  \
    void (*self_sub)(FormWO_t x);  \
    void (*asign)(FormWO_t x);     \
    void *(*add)(FormWO_t x);      \
    void *(*sub)(FormWO_t x);      \
    void *(*mul)(FormWO_t x);      \
    void *(*div)(FormWO_t x);      \
    void *(*mod)(FormWO_t x);      \
    void *(*cast)(const char *c);  \
}

typedef struct {
    union {
        INHERIT_CLASS *_s;
        byte _pad[sizeof(*(Object)NULL)];
    };
} *Class;

void initClass(void) __attribute__((constructor));
void *__cast_aux(void *_this, const char *c);

Form_t _Class(void);
//extern const void *_Class;
#define CLASS Class : _Class()
#endif //TINY_CTL_TCTL_CLASS_H
