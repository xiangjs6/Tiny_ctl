//
// Created by xjs on 2020/12/30.
//

#include "include/_tctl_class.h"
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#define Import METACLASS

struct Class {
    //const struct MetaClass _;
    bool (*equal)(const void *_this, const void *x);
    int (*cmp)(const void *_this, const void *x);
    void *(*brackets)(const void *_this, const void *x);
    void (*inc)(void *_this);
    void (*dec)(void *_this);
    void (*self_add)(void *_this, const void *x);
    void (*self_sub)(void *_this, const void *x);
    void (*asign)(void *_this, const void *x);
    void *(*add)(const void *_this, const void *x);
    void *(*sub)(const void *_this, const void *x);
    void *(*mul)(const void *_this, const void *x);
    void *(*div)(const void *_this, const void *x);
    void *(*mod)(const void *_this, const void *x);
};

static bool _equal(const void *x);
static int _cmp(const void *x);
static void *_brackets(const void *x);
static void _inc(void);
static void _dec(void);
static void _self_add(const void *x);
static void _self_sub(const void *x);
static void _asign(const void *x);
static void *_add(const void *x);
static void *_sub(const void *x);
static void *_mul(const void *x);
static void *_div(const void *x);
static void *_mod(const void *x);
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
        _asign,
        _add,
        _sub,
        _mul,
        _div,
        _mod
};
const struct ClassSelector *_ClassS = NULL;
static const void *__Class = NULL;

const void *_Class(void)
{
    return __Class;
}

//selector
static bool _equal(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->equal);
    return class->equal(_this, x);
}
static int _cmp(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->cmp);
    return class->cmp(_this, x);
}
static void *_brackets(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->brackets);
    return class->brackets(_this, x);
}
static void _inc(void)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->inc);
    return class->inc(_this);
}
static void _dec(void)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->dec);
    return class->dec(_this);
}
static void _self_add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->self_add);
    class->self_add(_this, x);
}
static void _self_sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->self_sub);
    class->self_sub(_this, x);
}
static void _asign(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->asign);
    class->asign(_this, x);
}
static void *_add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->add);
    return class->add(_this, x);
}
static void *_sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->sub);
    return class->sub(_this, x);
}
static void *_mul(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->mul);
    return class->mul(_this, x);
}
static void *_div(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->div);
    return class->div(_this, x);
}
static void *_mod(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = sizeOf(_MetaClass()) + (void*)classOf(_this);
    assert(class->mod);
    return class->mod(_this, x);
}

static void *_ctor(void *_this, va_list *app)
{
    struct Class *this = super_ctor(__Class, _this, app);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&ClassS + sizeof(ClassS._);
    voidf *end = (void*)&ClassS + sizeof(ClassS);
    voidf *this_begin = (void*)this;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(this_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return (void*)this + sizeof(struct Class);
}


void initClass(void)
{
    if (!_ClassS) {
        _ClassS = (void*)&ClassS;
        memcpy((void*)&ClassS, _MetaClassS, sizeof(*_MetaClassS));
    }
    if (!__Class) {
        __Class = new(MetaClass, "Class",
                      T(MetaClass), sizeof(struct Class) + sizeOf(_MetaClass()),
                     _MetaClassS->ctor, _ctor);
    }
}