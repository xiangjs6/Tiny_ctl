//
// Created by xjs on 2021/1/3.
//

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
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return this->val == p->val;
}

static int _cmp(const void *_this, const void *x)
{
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return this->val - p->val;
}

static void _inc(void *_this)
{
    struct Double *this = offsetOf(_this, __Double);
    this->val++;
}

static void _dec(void *_this)
{
    struct Double *this = offsetOf(_this, __Double);
    this->val--;
}

static void _self_add(void *_this, const void *x)
{
    struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    this->val += p->val;
}

static void _self_sub(void *_this, const void *x)
{
    struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    this->val -= p->val;
}

static void _asign(void *_this, const void *x)
{
    struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    this->val = p->val;
}

static void *_add(const void *_this, const void *x)
{
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return new(T(Double), this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return new(T(Double), this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return new(T(Double), this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Double *this = offsetOf(_this, __Double);
    const struct Double *p = offsetOf(x, __Double);
    return new(T(Double), this->val / p->val);
}

void initDouble(void)
{
    if (!__Double)
        __Double = new(T(Class), "Double", T(Object), sizeof(struct Double) + classSz(_Object().class),
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

Form_t _Double(void)
{
    Form_t t = {OBJ, {.class = __Double}};
    return t;
}
