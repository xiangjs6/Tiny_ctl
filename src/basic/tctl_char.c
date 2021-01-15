//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_char.h"
#include "../../include/auto_release_pool.h"
#include <string.h>
#define Import CLASS, CHAR, OBJECT

struct Char {
    char val;
};

static const void *__Char = NULL;
static void *_ctor(void *_this, va_list *app)
{
    struct Char *this = super_ctor(__Char, _this, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == POD)
        memcpy(&this->val, &t.mem, sizeof(char));
    else if (t._.f == ADDR)
        memcpy(&this->val, t.mem, sizeof(char));
    else
        this->val = *(char *) Cast(t.mem, char);
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
    return new(T(Char), this->val + p->val);
}

static void *_sub(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(T(Char), this->val - p->val);
}

static void *_mul(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(T(Char), this->val * p->val);
}

static void *_div(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(T(Char), this->val / p->val);
}

static void *_mod(const void *_this, const void *x)
{
    const struct Char *this = offsetOf(_this, __Char);
    const struct Char *p = offsetOf(x, __Char);
    return new(T(Char), this->val % p->val);
}

static void *_cast(const void *_this, const char *c)
{
    const struct Char *this = offsetOf(_this, __Char);
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

void initChar(void)
{
    if (!__Char)
        __Char = new(T(Class), "Char", T(Object), sizeof(struct Char) + classSz(_Object().class),
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

Form_t _Char(void)
{
    return (Form_t){OBJ, {.class = __Char}};
}
