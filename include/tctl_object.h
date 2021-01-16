//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stdio.h>
#include <stdarg.h>
#include "tctl_def.h"
#include "map_macro.h"

/*
 * tctl的对象规则
 * 1、使用对象前调用THIS或者push_this传递对象地址，并且在对象方法中使用pop_this获取地址（关键）
 * 2、建议将私有变量重新封装到单独结构体中，并将其大小填充到对象结构体
 * 3、建议私有成员函数使用同一结构体指针，指向一个不变常量上
 * 4、遵守各个泛类对象指定的规则，比如迭代器
 * */

typedef struct {
    enum {OBJ, POD, ADDR} f;
    union {
        size_t size;
        const void *class;
    };
} Form_t;

//Form With Object
typedef struct {
    Form_t _;
    void *mem;
} FormWO_t;

typedef struct {void *p; size_t size;} __ARG_ADDR_t;
void *_ToPoint(char t, size_t size, ...);
void *_AddrAux(int t, ...);
#define FORM_WITH_OBJ(_t, ...) (FormWO_t){_t, ##__VA_ARGS__}
#define VAEND NULL
#define VA_ADDR(arg) ((__ARG_ADDR_t){&(arg), sizeof(arg)})
#define _VA_AUX(_t) FORM_WITH_OBJ(_T(_t), _Generic(_t, float : _ToPoint('f', sizeof(_t), _t),        \
                                                       double : _ToPoint('f', sizeof(_t), _t),       \
                                                       const float : _ToPoint('f', sizeof(_t), _t),  \
                                                       const double : _ToPoint('f', sizeof(_t), _t), \
                                                       __ARG_ADDR_t : _AddrAux('a', _t),             \
                                                       default : _ToPoint(0, sizeof(_t), _t)))
#define VA(...) MAP_LIST(_VA_AUX, ##__VA_ARGS__)

#define _T(__T) _Generic(__T, Import,                                                         \
                         __ARG_ADDR_t : (Form_t){ADDR, {.size = (size_t)_AddrAux('s', __T)}}, \
                         default : (Form_t){POD, {.size = sizeof(__T)}})
#define T(__T, ...) _T(*(__T volatile *)0), ##__VA_ARGS__
#define ARRAY_T(__T, __N) _T(*(__T(*)[__N])0)

#define new(__T, ...) _new(FORM_WITH_OBJ(__T), ##__VA_ARGS__, VAEND)
#define delete(this) _delete(_T(this), this)
void *_new(FormWO_t t, ...);
void _delete(Form_t t, void *this);

//只能传对象
void construct(Form_t t, void *mem, FormWO_t x);
void destroy(void *obj);
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
