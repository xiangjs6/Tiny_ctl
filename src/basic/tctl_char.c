//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_char.h"
#include <stdlib.h>
#define Import CLASS, CHAR, OBJECT

struct Char {
    char val;
};

static const void *__Char = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct Char *this = super_ctor(__Char, _this, app);
    this->val = va_arg(*app, int);//va_arg会将低于int转为int，所以最低的类型长度及为int
    return _this + sizeof(struct Char);
}

static bool _equal(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return this->val == p->val;
}

static int _cmp(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return this->val - p->val;
}

static void _inc(void *_this)
{
    struct Char *this = offsetOf(_this, __Char);
    this->val++;
}

static void _dec(void *_this)
{
    struct Char *this = offsetOf(_this, __Char);
    this->val--;
}

static void _self_add(void *_this, const void *x)
{
    struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    this->val += p->val;
}

static void _self_sub(void *_this, const void *x)
{
    struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    this->val -= p->val;
}

static void _asign(void *_this, const void *x)
{
    struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    this->val = p->val;
}

static void *_add(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(Char, this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(Char, this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(Char, this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(Char, this->val / p->val);
}

static void *_mod(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(Char, this->val % p->val);
}
void initChar(void)
{
    if (!__Char)
        __Char = new(Class, "Char", T(Object), sizeof(struct Char) + classSz(_Object()),
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

const void *_Char(void)
{
    return __Char;
}
