//
// Created by xjs on 2020/12/31.
//

#include "include/_tctl_int.h"
#include "../../include/auto_release_pool.h"
#include <stdarg.h>
#include <string.h>
#define Import CLASS, INT, OBJECT

struct Int {
    long long val;
};

static const void *__Int = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct Int *this = super_ctor(__Int, _this, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == POD)
        this->val = *(long long*)t.mem;
    else
        this->val = *(long long*)Cast(t.mem, long long);
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
    return new(T(Int), this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(T(Int), this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(T(Int), this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(T(Int), this->val / p->val);
}

static void *_mod(const void *_this, const void *x)
{
    const struct Int *this = offsetOf(_this, __Int);
    const struct Int *p = offsetOf(x, __Int);
    return new(T(Int), this->val % p->val);
}

static void *_cast(const void *_this, const char *c)
{
    const struct Int *this = offsetOf(_this, __Int);
    if (!strcmp(c, "long long")) {
        long long *v = ARP_MallocARel(sizeof(long long));
        *v = (long long)this->val;
        return v;
    }
    if (!strcmp(c, "long")) {
        long *v = ARP_MallocARel(sizeof(long));
        *v = (long)this->val;
        return v;
    }
    if (!strcmp(c, "int")) {
        int *v = ARP_MallocARel(sizeof(int));
        *v = (int)this->val;
        return v;
    }
    if (!strcmp(c, "short")) {
        short *v = ARP_MallocARel(sizeof(short));
        *v = (short)this->val;
        return v;
    }
    if (!strcmp(c, "char")) {
        char *v = ARP_MallocARel(sizeof(char));
        *v = (char)this->val;
        return v;
    }
    if (!strcmp(c, "unsigned long long")) {
        unsigned long long *v = ARP_MallocARel(sizeof(unsigned long long));
        *v = (unsigned long long)this->val;
        return v;
    }
    if (!strcmp(c, "unsigned long")) {
        unsigned long *v = ARP_MallocARel(sizeof(unsigned long));
        *v = (unsigned long)this->val;
        return v;
    }
    if (!strcmp(c, "unsigned int")) {
        unsigned int *v = ARP_MallocARel(sizeof(unsigned int));
        *v = (unsigned int)this->val;
        return v;
    }
    if (!strcmp(c, "unsigned short")) {
        unsigned short *v = ARP_MallocARel(sizeof(unsigned short));
        *v = (unsigned short)this->val;
        return v;
    }
    if (!strcmp(c, "unsigned char")) {
        unsigned char *v = ARP_MallocARel(sizeof(unsigned char));
        *v = (unsigned char)this->val;
        return v;
    }
    return NULL;
}

void initInt(void)
{
    if (!__Int)
        __Int = new(T(Class), "Int", T(Object), sizeof(struct Int) + classSz(_Object().class),
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
                   _ClassS->cast, _cast,
                   Selector, _ClassS);
}

Form_t _Int(void)
{
    return (Form_t){OBJ, {.class = __Int}};
}
