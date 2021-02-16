//
// Created by xjs on 2021/1/3.
//

#include "include/_tctl_double.h"
#include "../../include/auto_release_pool.h"
#include <string.h>
#include <assert.h>
#define Import CLASS, DOUBLE, OBJECT

struct Double {
    double val;
};

static const void *__Double = NULL;

inline double toDouble(FormWO_t t)
{
    double val = 0;
    float f;
    switch (t._.f) {
        case POD:
            if (t._.size == 4) {
                memcpy(&f, &t.mem, t._.size);
                val = f;
            } else {
                memcpy(&val, &t.mem, t._.size);
            }
            break;
        case ADDR:
            memcpy(&val, t.mem, t._.size);
            break;
        case OBJ:
            val = *(double*)Cast(t.mem, double);
            break;
        default:
            assert(0);
    }
    return val;
}

static void *_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Double, _this, app);
    struct Double *this = offsetOf(_this, __Double);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == END)
        this->val = 0;
    else
        this->val = toDouble(t);
    return _this;
}

static bool _equal(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x);
    return this->val == val;
}

static int _cmp(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x);
    return this->val - val;
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

static void _self_add(void *_this, FormWO_t x)
{
    struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x);
    this->val += val;
}

static void _self_sub(void *_this, FormWO_t x)
{
    struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x);
    this->val -= val;
}

static void _assign(void *_this, FormWO_t x)
{
    struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x);
    this->val = val;
}

static void *_add(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x) + this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return new(compose(_Double(), mem), VA(val));
}

static void *_sub(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x) - this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return new(compose(_Double(), mem), VA(val));
}

static void *_mul(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x) * this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return new(compose(_Double(), mem), VA(val));
}

static void *_div(const void *_this, FormWO_t x)
{
    const struct Double *this = offsetOf(_this, __Double);
    double val = toDouble(x) / this->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return new(compose(_Double(), mem), VA(val));
}

static void *_cast(const void *_this, const char *c)
{
    const struct Double *this = offsetOf(_this, __Double);
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

static void initDouble(void)
{
    T(Class); //初始化Class选择器
    if (!__Double)
        __Double = new(T(Class), "Double", T(Object), sizeof(struct Double) + classSz(_Object().class),
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
                     _ClassS->cast, _cast,
                     Selector, _ClassS, NULL);
}

Form_t _Double(void)
{
    if (!__Double)
        initDouble();
    return (Form_t){OBJ, {.class = __Double}};
}
