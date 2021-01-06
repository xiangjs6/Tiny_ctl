//
// Created by xjs on 2021/1/3.
//

#include <stdlib.h>
#include "include/_tctl_double.h"
#define Import CLASS, DOUBLE, OBJECT

struct Double {
    //struct Object _;
    double val;
};

static const void *__Double = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct Double *this = super_ctor(__Double, _this, app);
    this->val = va_arg(*app, double);
    return (void*)this + sizeof(struct Double);
}

static bool _equal(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return this->val == p->val;
}

static int _cmp(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return this->val - p->val;
}

static void _inc(void *_this)
{
    struct Double *this = _this + Object_size;
    this->val++;
}

static void _dec(void *_this)
{
    struct Double *this = _this + Object_size;
    this->val--;
}

static void _self_add(void *_this, const void *x)
{
    struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    this->val += p->val;
}

static void _self_sub(void *_this, const void *x)
{
    struct Double *this = _this + Object_size;
    const struct Double *p = x;
    this->val -= p->val;
}

static void _asign(void *_this, const void *x)
{
    struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    this->val = p->val;
}

static void *_add(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return new(Double, this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return new(Double, this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return new(Double, this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Double *this = _this + Object_size;
    const struct Double *p = x + Object_size;
    return new(Double, this->val / p->val);
}

void initDouble(void)
{
    if (!__Double)
        __Double = new(Class, "Double", T(Object), sizeof(struct Double) + Object_size,
                     _MetaClassS->ctor, _ctor,
                     _ClassS->equal, _equal,
                     _ClassS->cmp, _cmp,
                     _ClassS->inc, _inc,
                     _ClassS->dec, _dec,
                     _ClassS->self_add, _self_add,
                     _ClassS->self_sub, _self_sub,
                     _ClassS->asign, _asign,
                     _ClassS->add, _add,
                     _ClassS->sub, _sub,
                     _ClassS->mul, _mul,
                     _ClassS->div, _div,
                     Selector, _ClassS);
}

const void *_Double(void)
{
    return __Double;
}
