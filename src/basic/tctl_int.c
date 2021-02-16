//
// Created by xjs on 2020/12/31.
//

#include "include/_tctl_int.h"
#include "../../include/auto_release_pool.h"
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#define Import CLASS, INT, OBJECT

struct Int {
    long long val;
};

static const void *__Int = NULL;

inline long long toInt(FormWO_t t)
{
    long long val = 0;
    char c;
    short s;
    int i;
    switch (t._.f) {
        case POD:
            switch (t._.size) {
                case 1:
                    memcpy(&c, &t.mem, t._.size);
                    val = c;
                    break;
                case 2:
                    memcpy(&s, &t.mem, t._.size);
                    val = s;
                    break;
                case 4:
                    memcpy(&i, &t.mem, t._.size);
                    val = i;
                    break;
                case 8:
                    memcpy(&val, &t.mem, t._.size);
                    break;
            }
            break;
        case ADDR:
            memcpy(&val, t.mem, t._.size);
            break;
        case OBJ:
            val = *(long long *) Cast(t.mem, long long);
            break;
        default:
            assert(0);
    }
    return val;
}

static void *_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Int, _this, app);
    struct Int *this = offsetOf(_this, __Int);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == END)
        this->val = 0;
    else
        this->val = toInt(t);
    return _this;
}

static bool _equal(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x);
    return this->val == val;
}

static int _cmp(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x);
    return this->val - val;
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

static void _self_add(void *_this, FormWO_t x)
{
    struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x);
    this->val += val;
}

static void _self_sub(void *_this, FormWO_t x)
{
    struct Int *this = offsetOf(_this, __Int);
    long long val;
    switch (x._.f) {
        case POD:
            memcpy(&val, &x.mem, x._.size);
            break;
        case ADDR:
            memcpy(&val, x.mem, x._.size);
            break;
        case OBJ:
            val = *(long long*)Cast(x.mem, long long);
            break;
        default:
            assert(0);
    }
    this->val -= val;
}

static void _assign(void *_this, FormWO_t x)
{
    struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x);
    this->val = val;
}

static void *_add(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x) + this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return new(compose(_Int(), mem), VA(val));
}

static void *_sub(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x) - this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return new(compose(_Int(), mem), VA(val));
}

static void *_mul(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x) * this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return new(compose(_Int(), mem), VA(val));
}

static void *_div(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x) / this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return new(compose(_Int(), mem), VA(val));
}

static void *_mod(const void *_this, FormWO_t x)
{
    const struct Int *this = offsetOf(_this, __Int);
    long long val = toInt(x) % this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return new(compose(_Int(), mem), VA(val));
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

static void initInt(void)
{
    T(Class); //初始化Class选择器
    if (!__Int)
        __Int = new(T(Class), "Int", T(Object), sizeof(struct Int) + classSz(_Object().class),
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

Form_t _Int(void)
{
    if (!__Int)
        initInt();
    return (Form_t){OBJ, {.class = __Int}};
}
