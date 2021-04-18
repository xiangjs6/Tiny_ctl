//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_METACLASS_H
#define TINY_CTL_TCTL_METACLASS_H

#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include "map_macro.h"

typedef struct {
    enum {OBJ, POD, ADDR, FUNC, END, FORM} f;
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

void *_valueAux(int t, ...); //用于在_Generic中欺骗编译器，将变量转为合适的类型
Form_t _formAux(int t, ...); //用于将Form_t和FormWO_t转化成Form_t Form_t会改变f FormWO_t不变
//FormWO_t的初始化宏
#define FORM_WITH_OBJ(_t, ...) (FormWO_t){_t, __VA_ARGS__}
//VA的结尾描述变量
#define VAEND (FormWO_t){{END}}
//获取变量的地址
#define VA_ADDR(arg) (FORM_WITH_OBJ((Form_t){ADDR, {sizeof(arg)}}, _valueAux('p', &(arg))))
//遇到需要传入函数指针时，VA_FUNC()创造FormWO_t
#define VA_FUNC(func) (FORM_WITH_OBJ((Form_t){FUNC, {sizeof(&(func))}}, _valueAux('p', &(func))))
//自定义类型变量
#define VA_CT(v) (FORM_WITH_OBJ((Form_t){POD, {sizeof(v)}}, memcpy(_valueAux('S', sizeof(v)), &v, sizeof(v))))
//为变量生成正确的指针放入FormWO_t中的mem成员变量
#define V(_t) _Generic(_t,                                           \
                            float              : _valueAux('f', _t), \
                            double             : _valueAux('F', _t), \
                            char               : _valueAux('c', _t), \
                            short              : _valueAux('s', _t), \
                            int                : _valueAux('i', _t), \
                            long               : _valueAux('l', _t), \
                            long long          : _valueAux('L', _t), \
                            unsigned char      : _valueAux('c', _t), \
                            unsigned short     : _valueAux('s', _t), \
                            unsigned int       : _valueAux('i', _t), \
                            unsigned long      : _valueAux('l', _t), \
                            unsigned long long : _valueAux('L', _t), \
                            default            : _valueAux('p', _t), \
                            Form_t             : NULL,               \
                            FormWO_t           : _valueAux('t', _t))
//为每个变量生成对应的FormWO_t变量
#define _VA_AUX(_t) FORM_WITH_OBJ(_T(_t), V(_t))
//用于各个函数调用时的参数列表中，对每一个放入该宏的参数，都会计算它的Form_t并生成FormWO_t变量
#define VA(...) MAP_LIST(_VA_AUX, ##__VA_ARGS__)

//对变量本身去生成对应的Form_t
#define _T(__T) _Generic(__T, Import,                                    \
                         Form_t : _formAux(0, __T),                      \
                         FormWO_t : _formAux(1, __T),                    \
                         default : (Form_t){POD, {.size = sizeof(__T)}})
//由类型名生成Form_t变量，Form_t变量名不能放入宏中
#define T(__T) _Generic(*(__T volatile *)0, Form_t : assert(0), default : _T(*(__T volatile *)0))
//当创建数组类型时，使用ARRAY_T()替代T()
#define ARRAY_T(__T, __N) _T(*(__T(*)[__N])0)

//创建对象时应该使用VA()，创建类时不能使用VA()，应按照MetaClass创建类
#define new(__T, ...) _new(FORM_WITH_OBJ(__T), ##__VA_ARGS__, VAEND)
#define delete(this) _delete(VA(this))
void *_new(FormWO_t t, ...);
void _delete(FormWO_t t);

//只能传对象
void *construct_v(Form_t t, void *mem, va_list *app);
void *construct(Form_t t, void *mem, ...);
void destroy(void *obj);
const void *classOf(const void *this);
size_t sizeOf(const void *this);
size_t classSz(const void *this);
void *offsetOf(const void *this, const void *class);

#define METACLASS_FUNC             \
struct {                           \
    void *(*ctor)(void *mem, ...); \
    void *(*dtor)(void);           \
    int (*differ)(const void *b);  \
    int (*puto)(FILE *fp);         \
}

typedef struct {
    METACLASS_FUNC *_s;
    void *_c;
} *MetaObject;

#define METAOBJECT_HEAD(Func) union { Func *_s; char __[sizeof(*(MetaObject)NULL)]; }

typedef struct {
    METACLASS_FUNC *_s;
} *MetaClass;

const void *super(const void *this);	/* class' superclass */

void push_this(const void *);
void *pop_this(void);
#define THIS(p) (push_this(p), *((p)->_s))

Form_t _MetaObject(void);		/* new(Object); */
Form_t _MetaClass(void);	/* new(MetaClass, "name", super, size, sel, meth, ... 0); */

#define METACLASS MetaClass : _MetaClass()
#define METAOBJECT MetaObject : _MetaObject()

#endif //TINY_CTL_TCTL_METACLASS_H
