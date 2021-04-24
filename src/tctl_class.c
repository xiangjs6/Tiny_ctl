//
// Created by xjs on 2020/12/30.
//

#include "include/_tctl_class.h"
#include "include/_tctl_metaclass.h"
#include <memory.h>
#include <assert.h>
#define Import METACLASS, METAOBJECT

struct Class {
    bool (*equal)(const void *_self, void *x);
    int (*cmp)(const void *_self, void *x);
    void *(*brackets)(const void *_self, void *x);
    void (*inc)(void *_self);
    void (*dec)(void *_self);
    void (*self_add)(void *_self, void *x);
    void (*self_sub)(void *_self, void *x);
    void (*assign)(void *_self, void *x);
    void *(*add)(const void *_self, void *x);
    void *(*sub)(const void *_self, void *x);
    void *(*mul)(const void *_self, void *x);
    void *(*div)(const void *_self, void *x);
    void *(*mod)(const void *_self, void *x);
};

struct Object {};

static bool _equal(void *x);
static int _cmp(void *x);
static void *_brackets(void *x);
static void _inc(void);
static void _dec(void);
static void _self_add(void *x);
static void _self_sub(void *x);
static void _assign(void *x);
static void *_add(void *x);
static void *_sub(void *x);
static void *_mul(void *x);
static void *_div(void *x);
static void *_mod(void *x);
//init
volatile static struct ClassSelector ClassS = {
        {},
        _equal,
        _cmp,
        _brackets,
        _inc,
        _dec,
        _self_add,
        _self_sub,
        _assign,
        _add,
        _sub,
        _mul,
        _div,
        _mod,
};
const struct ClassSelector *_ClassS = NULL;
static const void *__Class = NULL;
static const void *__Object = NULL;

//selector
static bool _equal(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->equal);
    return class->equal(_self, x);
}

static int _cmp(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->cmp);
    return class->cmp(_self, x);
}

static void *_brackets(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->brackets);
    return class->brackets(_self, x);
}

static void _inc(void)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->inc);
    return class->inc(_self);
}

static void _dec(void)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->dec);
    return class->dec(_self);
}

static void _self_add(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->self_add);
    class->self_add(_self, x);
}

static void _self_sub(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->self_sub);
    class->self_sub(_self, x);
}

static void _assign(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->assign);
    class->assign(_self, x);
}

static void *_add(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->add);
    return class->add(_self, x);
}

static void *_sub(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->sub);
    return class->sub(_self, x);
}

static void *_mul(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->mul);
    return class->mul(_self, x);
}

static void *_div(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->div);
    return class->div(_self, x);
}

static void *_mod(void *x)
{
    void *_self = pop_this();
    const struct Class *class = offsetOf(classOf(_self), __Class);
    assert(class->mod);
    return class->mod(_self, x);
}


static void *_class_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Class, _self, app);
    struct Class *self = offsetOf(_self, __Class);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&ClassS + sizeof(ClassS._);
    voidf *end = (void*)&ClassS + sizeof(ClassS);
    voidf *self_begin = (void*)self;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(self_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _self;
}

static void initClass(void)
{
    if (!_ClassS) {
        _ClassS = (void*)&ClassS;
        memcpy((void*)&ClassS, _MetaClassS, sizeof(*_MetaClassS));
    }
    if (!__Class) {
        __Class = new(T(MetaClass), "Class",
                      T(MetaClass), sizeof(struct Class) + classSz(T(MetaClass)),
                      _MetaClassS->ctor, _class_ctor, NULL);
    }
    if (!__Object) {
        __Object = new(T(MetaClass), "Object",
                       T(MetaObject), sizeof(struct Object) + classSz(T(MetaObject)), NULL);
    }
}

const void *_Class(void)
{
    if (!__Class)
        initClass();
    return __Class;
}

const void *_Object(void)
{
    if (!__Object)
        initClass();
    return __Object;
}
