//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stdio.h>
#include <stdarg.h>
#include "tctl_def.h"

/*
 * tctl的对象规则
 * 1、使用对象前调用THIS或者push_this传递对象地址，并且在对象方法中使用pop_this获取地址（关键）
 * 2、建议将私有变量重新封装到单独结构体中，并将其大小填充到对象结构体
 * 3、建议私有成员函数使用同一结构体指针，指向一个不变常量上
 * 4、遵守各个泛类对象指定的规则，比如迭代器
 * */

#define VAEND NULL

typedef struct {
    enum {OBJ, POD} f;
    union {
        size_t size;
        const void *class;
    };
} Form_t;

struct _Form_t {
    Form_t _;
    void *mem;
};

#define _FORMINIT(_t, ...) (struct _Form_t){_t, ##__VA_ARGS__}
#define _T(__T) _Generic(__T, Import,\
                              default : (Form_t){POD, {.size = sizeof(__T)}})
#define T(__T, ...) _T(*(__T*)0), ##__VA_ARGS__
#define new(__T, ...) _new(_FORMINIT(__T), ##__VA_ARGS__, VAEND)
#define delete(this) _delete(_T(this), this)
void *_new(struct _Form_t t, ...);
void _delete(Form_t t, void *this);

const void *classOf(const void *this);
size_t sizeOf(const void *this);
size_t classSz(const void *this);
void *offsetOf(const void *this, const void *class);

#define INHERIT_METACLASS \
struct {                  \
    void *(*ctor)(void *mem, ...); \
    void *(*dtor)(void);        \
    int (*differ)(const void *b); \
    int (*puto)(FILE *fp);  \
}

typedef struct {
    INHERIT_METACLASS *_s;
    void *_c;
} *Object;

typedef struct {
    INHERIT_METACLASS *_s;
} *MetaClass;
const void *super(const void * this);	/* class' superclass */

void push_this(const void *);
void *pop_this(void);
#define THIS(p) (push_this(p), *(p->_s))

Form_t _Object(void);		/* new(Object); */
Form_t _MetaClass(void);	/* new(MetaClass, "name", super, size, sel, meth, ... 0); */

#define METACLASS MetaClass : _MetaClass()
#define OBJECT Object : _Object()

#endif //TINY_CTL_TCTL_OBJECT_H
