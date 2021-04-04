//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_uint.h"
#include <string.h>
#include <assert.h>
#include "../../include/auto_release_pool.h"
#define Import CLASS, UINT, OBJECT

struct UInt {
    unsigned long long val;
};

static const void *__UInt = NULL;

inline unsigned long long toUInt(FormWO_t t)
{
    unsigned long long *l = 0;
    unsigned char *c;
    unsigned short *s;
    unsigned int *i;
    unsigned long long res;
    switch (t._.f) {
        case POD:
            switch (t._.size) {
                case 1:
                    c = t.mem;
                    res = *c;
                    break;
                case 2:
                    s = t.mem;
                    res = *s;
                    break;
                case 4:
                    i = t.mem;
                    res = *i;
                    break;
                case 8:
                    l = t.mem;
                    res = *l;
                    break;
            }
            break;
        case ADDR:
            memcpy(&res, *(void**)t.mem, sizeof(res));
            break;
        case OBJ:
            res = *(long long *) Cast(*(Object*)t.mem, long long);
            break;
        default:
            assert(0);
    }
    return res;
}

static void *_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__UInt, _this, app);
    struct UInt *this = offsetOf(_this, __UInt);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == END)
        this->val = 0;
    else
        this->val = toUInt(t);
    return _this;
}

static bool _equal(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x);
    return this->val == val;
}

static int _cmp(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x);
    return this->val - val;
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

static void _self_add(void *_this, FormWO_t x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x);
    this->val += val;
}

static void _self_sub(void *_this, FormWO_t x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x);
    this->val -= val;
}

static void _assign(void *_this, FormWO_t x)
{
    struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x);
    this->val = val;
}

static void *_add(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x) + this->val;
    void *mem = ARP_MallocARelDtor(classSz(__UInt), destroy);
    return new(compose(_UInt(), mem), VA(val));
}

static void *_sub(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x) - this->val;
    void *mem = ARP_MallocARelDtor(classSz(__UInt), destroy);
    return new(compose(_UInt(), mem), VA(val));
}

static void *_mul(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x) * this->val;
    void *mem = ARP_MallocARelDtor(classSz(__UInt), destroy);
    return new(compose(_UInt(), mem), VA(val));
}

static void *_div(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x) / this->val;
    void *mem = ARP_MallocARelDtor(classSz(__UInt), destroy);
    return new(compose(_UInt(), mem), VA(val));
}

static void *_mod(const void *_this, FormWO_t x)
{
    const struct UInt *this = offsetOf(_this, __UInt);
    unsigned long long val = toUInt(x) % this->val;
    void *mem = ARP_MallocARelDtor(classSz(__UInt), destroy);
    return new(compose(_UInt(), mem), VA(val));
}

static void *_cast(const void *_this, const char *c)
{
    const struct UInt *this = offsetOf(_this, __UInt);
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
    if (!strcmp(c, "float")) {
        float *v = ARP_MallocARel(sizeof(float));
        *v = (float)this->val;
        return v;
    }
    if (!strcmp(c, "double")) {
        double *v = ARP_MallocARel(sizeof(double));
        *v = (double)this->val;
        return v;
    }
    assert(0);
    return NULL;
}

static void initUInt(void)
{
    T(Class); //初始化Class选择器
    if (!__UInt)
        __UInt = new(T(Class), "UInt", T(Object), sizeof(struct UInt) + classSz(_Object().class),
                    _MetaClassS->ctor, _ctor,
                    _ClassS->equal, _equal,
                    _ClassS->cmp, _cmp,
                    _ClassS->inc, _inc,
                    _ClassS->dec, _dec,
                    _ClassS->self_add, _self_add,
                    _ClassS->self_sub, _self_sub,
                    _ClassS->assign, _assign,
                    _ClassS->add, _add,
                    _ClassS->sub, _sub,
                    _ClassS->mul, _mul,
                    _ClassS->div, _div,
                    _ClassS->mod, _mod,
                    _ClassS->cast, _cast,
                    Selector, _ClassS, NULL);
}

Form_t _UInt(void)
{
    if (!__UInt)
        initUInt();
    return (Form_t){OBJ, {.class = __UInt}};
}
