//
// Created by xjs on 2020/12/31.
//

#include "include/_tctl_int.h"
#define Import CLASS, INT, OBJECT

struct Int {
    long long val;
};

static const void *__Int = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct Int *this = super_ctor(__Int, _this, app);
    this->val = va_arg(*app, long long);
    return _this + sizeof(struct Int);
}

static bool _equal(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return this->val == p->val;
}

static int _cmp(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return this->val - p->val;
}

static void _inc(void *_this)
{
    struct Int *this = offsetOf(_this, __Int);
    this->val++;
}

static void _dec(void *_this)
{
    struct Int *this = offsetOf(_this, __Int);
    this->val--;
}

static void _self_add(void *_this, const void *x)
{
    struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    this->val += p->val;
}

static void _self_sub(void *_this, const void *x)
{
    struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    this->val -= p->val;
}

static void _asign(void *_this, const void *x)
{
    struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    this->val = p->val;
}

static void *_add(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(Int, this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(Int, this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(Int, this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(Int, this->val / p->val);
}

static void *_mod(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(Int, this->val % p->val);
}
void initInt(void)
{
    if (!__Int)
        __Int = new(Class, "Int", T(Object), sizeof(struct Int) + classSz(_Object().class),
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

Form_t _Int(void)
{
    return (Form_t){OBJ, {.class = __Int}};
}
