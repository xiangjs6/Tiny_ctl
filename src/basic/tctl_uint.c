//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_uint.h"
#define Import CLASS, UINT, OBJECT

struct UInt {
    unsigned long long val;
};

static const void *__UInt = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct UInt *this = super_ctor(__UInt, _this, app);
    this->val = va_arg(*app, unsigned long long);
    return _this + sizeof(struct UInt);
}

static bool _equal(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return this->val == p->val;
}

static int _cmp(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return this->val - p->val;
}

static void _inc(void *_this)
{
    struct UInt *this = offsetOf(_this, __UInt);
    this->val++;
}

static void _dec(void *_this)
{
    struct UInt *this = offsetOf(_this, __UInt);
    this->val--;
}

static void _self_add(void *_this, const void *x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    this->val += p->val;
}

static void _self_sub(void *_this, const void *x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    this->val -= p->val;
}

static void _asign(void *_this, const void *x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    this->val = p->val;
}

static void *_add(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return new(T(UInt), this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return new(T(UInt), this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return new(T(UInt), this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return new(T(UInt), this->val / p->val);
}

static void *_mod(const void *_this, const void *x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    const struct UInt *p = offsetOf(x, __UInt);
    return new(T(UInt), this->val % p->val);
}
void initUInt(void)
{
    if (!__UInt)
        __UInt = new(T(Class), "UInt", T(Object), sizeof(struct UInt) + classSz(_Object().class),
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
                    _ClassS->mod, _mod,
                    Selector, _ClassS);
}

Form_t _UInt(void)
{
    return (Form_t){OBJ, {.class = __UInt}};
}
