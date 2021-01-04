//
// Created by xjs on 2020/12/30.
//

#include "include/_tctl_class.h"
#include <memory.h>
#include <assert.h>
#include <stdlib.h>
#define Import METACLASS

static bool _equal(const void *x);
static int _cmp(const void *x);
static void *_at(int x);
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
volatile static typeof(*_ClassS) ClassS = {
        {},
        _equal,
        _cmp,
        _at,
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
typeof(_ClassS) _ClassS = NULL;
static const void *__Class = NULL;

const void *_Class(void)
{
    return __Class;
}

//selector
static bool _equal(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->equal);
    return class->equal(_this, x);
}
static int _cmp(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->cmp);
    return class->cmp(_this, x);
}
static void *_at(int x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->at);
    return class->at(_this, x);
}
static void _inc(void)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->inc);
    return class->inc(_this);
}
static void _dec(void)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->dec);
    return class->dec(_this);
}
static void _self_add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->self_add);
    class->self_add(_this, x);
}
static void _self_sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->self_sub);
    class->self_sub(_this, x);
}
static void _asign(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->asign);
    class->asign(_this, x);
}
static void *_add(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->add);
    return class->add(_this, x);
}
static void *_sub(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->sub);
    return class->sub(_this, x);
}
static void *_mul(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->mul);
    return class->mul(_this, x);
}
static void *_div(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
    assert(class->div);
    return class->div(_this, x);
}
static void *_mod(const void *x)
{
    void *_this = pop_this();
    const struct Class *class = classOf(_this);
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
    voidf *this_begin = (void*)this + sizeof(this->_);
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
    return this;
}


void initClass(void)
{
    if (!_ClassS) {
        _ClassS = (void*)&ClassS;
        memcpy((void*)&ClassS, _MetaClassS, sizeof(*_MetaClassS));
    }
    if (!__Class) {
        __Class = new(MetaClass, "Class",
                      T(MetaClass), sizeof(struct Class),
                     _MetaClassS->ctor, _ctor);
    }
}