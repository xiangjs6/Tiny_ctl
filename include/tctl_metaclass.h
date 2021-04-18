//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_METACLASS_H
#define TINY_CTL_TCTL_METACLASS_H

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "map_macro.h"

//由类型名生成对应的class变量
#define T(__T) _Generic(*(__T volatile *)NULL, Import)

//只能传对象
void *construct_v(void *class, void *mem, va_list *app);
void *construct(void *class, void *mem, ...);
void destroy(void *obj);
const void *classOf(const void *self);
size_t sizeOf(const void *self);
size_t classSz(const void *self);
void *offsetOf(const void *self, const void *class);

#define METACLASS_FUNC             \
struct {                           \
    void *(*ctor)(void *mem, ...); \
    void *(*dtor)(void);           \
    int (*differ)(const void *b);  \
    int (*puto)(FILE *fp);         \
    void *(*cast)(void *class);    \
}

typedef struct {
    METACLASS_FUNC *_s;
    void *_c;
} *MetaObject;

#define METAOBJECT_HEAD(Func) union { Func *_s; char __[sizeof(*(MetaObject)NULL)]; }

typedef struct {
    METACLASS_FUNC *_s;
} *MetaClass;

const void *super(const void *self);	/* class' superclass */

void push_this(const void *);
void *pop_this(void);
#define THIS(p) (push_this(p), *((p)->_s))

const void *_MetaObject(void);		/* new(Object); */
const void *_MetaClass(void);	/* new(MetaClass, "name", super, size, sel, meth, ... 0); */

#define METACLASS MetaClass : _MetaClass()
#define METAOBJECT MetaObject : _MetaObject()

void *new(const void *class, ...);
void delete(void *obj);
#endif //TINY_CTL_TCTL_METACLASS_H
