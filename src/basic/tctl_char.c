//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_char.h"
#include "../../include/auto_release_pool.h"
#include <string.h>
#include <assert.h>
#define Import CLASS, CHAR, OBJECT

struct Char {
    char val;
};

static const void *__Char = NULL;

static inline char toChar(FormWO_t t)
{
    char val;
    short s;
    int i;
    long long l;
    switch (t._.f) {
        case POD:
            switch (t._.size) {
                case 1:
                    memcpy(&val, &t.mem, t._.size);
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
                    memcpy(&l, &t.mem, t._.size);
                    val = l;
                    break;
            }
            break;
        case ADDR:
            memcpy(&val, t.mem, t._.size);
            break;
        case OBJ:
            val = *(char*)Cast(t.mem, char);
            break;
    }
    return val;
}

static void *_ctor(void *_this, va_list *app)
{
    struct Char *this = super_ctor(__Char, _this, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    this->val = toChar(t);
    return _this + sizeof(struct Char);
}

static bool _equal(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x);
    return this->val == val;
}

static int _cmp(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x);
    return this->val - val;
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

static void _self_add(void *_this, FormWO_t x)
{
    struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x);
    this->val += val;
}

static void _self_sub(void *_this, FormWO_t x)
{
    struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x);
    this->val -= val;
}

static void _asign(void *_this, FormWO_t x)
{
    struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x);
    this->val = val;
}

static void *_add(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x) + this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return new(compose(_Char(), mem), VA(val));
}

static void *_sub(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x) - this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return new(compose(_Char(), mem), VA(val));
}

static void *_mul(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x) * this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return new(compose(_Char(), mem), VA(val));
}

static void *_div(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x) / this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return new(compose(_Char(), mem), VA(val));
}

static void *_mod(const void *_this, FormWO_t x)
{
    const struct Char *this = offsetOf(_this, __Char);
    char val = toChar(x) % this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return new(compose(_Char(), mem), VA(val));
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
