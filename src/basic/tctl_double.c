//
// Created by xjs on 2021/1/3.
//

#include "../../include/tctl_any.h"
#include "../../include/tctl_arg.h"
#include "../../include/tctl_double.h"
#include "../../include/tctl_int.h"
#include "../../include/tctl_char.h"
#include "../include/_tctl_metaclass.h"
#include "../include/_tctl_class.h"
#include "../../include/auto_release_pool.h"
#define Import CLASS, INT, OBJECT, ANY, CHAR, DOUBLE, METAOBJECT

struct Double {
    double val;
};

static const void *__Double = NULL;

static void *_double_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Double, _self, app);
    struct Double *self = offsetOf(_self, __Double);
    MetaObject val = va_arg(*app, MetaObject);
    Double p;
    if (val == VAEND) {
        self->val = 0;
        return _self;
    } else if (classOf(val) == __Double) {
        p = (Double)val;
    } else {
        p = THIS(val).cast(__Double);
    }
    self->val = p->val;
    return _self;
}

static bool _double_equal(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    return self->val == x->val;
}

static int _double_cmp(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    return (int)(self->val - x->val);
}

static void _double_inc(void *_self)
{
    struct Double *self = offsetOf(_self, __Double);
    self->val++;
}

static void _double_dec(void *_self)
{
    struct Double *self = offsetOf(_self, __Double);
    self->val--;
}

static void _double_self_add(void *_self, Double x)
{
    struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    self->val += x->val;
}

static void _double_self_sub(void *_self, Double x)
{
    struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    self->val -= x->val;
}

static void _double_assign(void *_self, Double x)
{
    struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    self->val = x->val;
}

static void *_double_add(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    x->val += self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return construct(_Double(), mem, x, VAEND);
}

static void *_double_sub(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    x->val -= self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return construct(_Double(), mem, x, VAEND);
}

static void *_double_mul(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    x->val *= self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return construct(_Double(), mem, x, VAEND);
}

static void *_double_div(const void *_self, Double x)
{
    const struct Double *self = offsetOf(_self, __Double);
    x = THIS(x).cast(__Double);
    x->val = self->val / x->val;
    void *mem = ARP_MallocARelDtor(classSz(__Double), destroy);
    return construct(_Double(), mem, x, VAEND);
}

static void *_double_cast(const void *_self, const void *class)
{
    const struct Double *self = offsetOf(_self, __Double);
    void *ret;
    if (class == __Double) {
        ret = ARP_MallocARel(classSz(class));
        construct(__Double, ret, _self, VAEND);
    } else if (class == T(Char)) {
        ret = ARP_MallocARel(classSz(class));
        Char v = construct(class, ret, VAEND);
        v->val = (char)self->val;
    } else if (class == T(Int)) {
        ret = ARP_MallocARel(classSz(class));
        Int v = construct(class, ret, VAEND);
        v->val = (int)self->val;
    } else if (class == T(Any)) {
        ret = ARP_MallocARel(classSz(class));
        construct(class, ret, _self, VAEND);
    } else {
        MetaObject m_obj = offsetOf(_self, T(MetaObject));
        ret = THIS(m_obj).cast(class);
    }
    return ret;
}

static void initDouble(void)
{
    T(Class); //初始化Class选择器
    if (!__Double)
        __Double = new(T(Class), "Double", T(Object), sizeof(struct Double) + classSz(T(Object)),
                     _MetaClassS->ctor, _double_ctor,
                     _MetaClassS->cast, _double_cast,
                     _ClassS->equal, _double_equal,
                     _ClassS->cmp, _double_cmp,
                     _ClassS->inc, _double_inc,
                     _ClassS->dec, _double_dec,
                     _ClassS->self_add, _double_self_add,
                     _ClassS->self_sub, _double_self_sub,
                     _ClassS->assign, _double_assign,
                     _ClassS->add, _double_add,
                     _ClassS->sub, _double_sub,
                     _ClassS->mul, _double_mul,
                     _ClassS->div, _double_div,
                     Selector, _ClassS, NULL);
}

const void *_Double(void)
{
    if (!__Double)
        initDouble();
    return __Double;
}

void *to_Double(const double *p, const void *class)
{
    void *mem = ARP_MallocARelDtor(classSz(T(Double)), delete);
    Double d = construct(T(Double), mem, VAEND);
    d->val = *p;
    if (class != T(Double))
        return THIS(d).cast(class);
    return d;
}
