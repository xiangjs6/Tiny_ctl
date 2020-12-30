//
// Created by xjs on 2020/12/29.
//

#ifndef TINY_CTL__TCTL_OBJECT_H
#define TINY_CTL__TCTL_OBJECT_H
#include "../include/tctl_object.h"
typedef void (*voidf)();	/* generic function pointer */
struct Object {
    void *s;
    const struct Class *class;	/* object's description */
};

struct Class {
    const struct Object _;			/* class' description */
    const char *name;				/* class' name */
    const struct Class *super;		/* class' super class */
    size_t size;					/* class' object's size */
    void *(*ctor)(void *this, va_list *app);
    void *(*dtor)(void *this);
    int (*differ)(const void *this, const void *b);
    int (*puto)(const void *this, FILE *fp);
};

void *super_ctor(const void *class, void *this, va_list *app);
void *super_dtor(const void *class, void *this);
int super_differ(const void *class, const void *this, const void *b);
int super_puto(const void *class, const void *this, FILE *fp);
extern INHERIT_CLASS *ClassS;
extern const void *SelectorF;
extern const void *_Object;		/* new(Object); */
extern const void *_Class;	/* new(Class, "name", super, size, sel, meth, ... 0); */
#endif //TINY_CTL__TCTL_OBJECT_H
