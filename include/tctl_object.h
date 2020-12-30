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

#define new(T, ...) _new(#T, ##__VA_ARGS__)
void *_new(const char *class_name, ...);
void delete(void *this);

const void *classOf(const void *this);
size_t sizeOf(const void *this);

#define INHERIT_CLASS \
struct {       \
    void *(*ctor)(va_list *app); \
    void *(*dtor) (void);        \
    int (*differ) (const void *b); \
    int (*puto) (FILE *fp);  \
}

typedef struct {
    INHERIT_CLASS *_s;
    void *_c;
    char _o[0];
} *Object;

typedef struct {
    INHERIT_CLASS *_s;
    char _o[0];
} *MetaClass;
const void *super(const void * this);	/* class' superclass */

void *push_this(void *);
void *pop_this(void);
#define THIS(p) (*(typeof(p))(push_this(p)))

#endif //TINY_CTL_TCTL_OBJECT_H
