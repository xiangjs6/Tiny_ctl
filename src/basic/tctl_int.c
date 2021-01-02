//
// Created by xjs on 2020/12/31.
//

#include "_tctl_int.h"
#include "../_tctl_class.h"
//#include "../../include/auto_release_pool.h"
#define Import CLASS, INT

static void *_ctor(void *_this, va_list *app)
{
    int *this = _this;
    *this = va_arg(*app, int);
    return _this;
}

static bool _equal(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return *this == *p;
}

static int _cmp(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return *this - *p;
}

static void _inc(void *_this)
{
    int *this = _this;
    (*this)++;
}

static void _dec(void *_this)
{
    int *this = _this;
    (*this)--;
}

static void _self_add(void *_this, const void *x)
{
    int *this = _this;
    const int *p = x;
    (*this) += *p;
}

static void _self_sub(void *_this, const void *x)
{
    int *this = _this;
    const int *p = x;
    (*this) -= *p;
}

static void _asign(void *_this, const void *x)
{
    int *this = _this;
    const int *p = x;
    *this = *p;
}

static void *_add(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return new(int, *this + *p);
}

static void *_sub(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return new(int, *this - *p);
}

static void *_mul(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return new(int, *this * *p);
}

static void *_div(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return new(int, *this / *p);
}

static void *_mod(const void *_this, const void *x)
{
    const int *this = _this;
    const int *p = x;
    return new(int, *this % *p);
}
const void *_Int = NULL;
void initInt(void)
{
    if (!_Int)
        _Int = new(Class, "int", _Object, sizeof(int),
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
