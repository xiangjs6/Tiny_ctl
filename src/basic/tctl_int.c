//
// Created by xjs on 2020/12/31.
//

#include <stdarg.h>
#include "../../include/auto_release_pool.h"
#include "../../include/tctl_any.h"
#include "../../include/tctl_arg.h"
#include "../../include/tctl_int.h"
#include "../../include/tctl_char.h"
#include "../../include/tctl_double.h"
#include "../include/_tctl_metaclass.h"
#include "../include/_tctl_class.h"
#define Import CLASS, INT, OBJECT, ANY, CHAR, DOUBLE

struct Int {
    long long val;
};

static const void *__Int = NULL;

static void *_int_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Int, _self, app);
    struct Int *self = offsetOf(_self, __Int);
    MetaObject val = va_arg(*app, MetaObject);
    Int p;
    if (val == VAEND) {
        self->val = 0;
        return _self;
    } else if (classOf(val) == __Int) {
        p = (Int)val;
    } else if (classOf(val) == T(Any)) {
        Any any = (Any)val;
        p = THIS(any).cast(__Int);
    } else {
        p = THIS(val).cast(__Int);
    }
    self->val = p->val;
    return _self;
}

static bool _int_equal(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? x : THIS(x).cast(__Int);
    return self->val == x->val;
}

static int _int_cmp(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? x : THIS(x).cast(__Int);
    return self->val - x->val;
}

static void _int_inc(void *_self)
{
    struct Int *self = offsetOf(_self, __Int);
    self->val++;
}

static void _int_dec(void *_self)
{
    struct Int *self = offsetOf(_self, __Int);
    self->val--;
}

static void _int_self_add(void *_self, Int x)
{
    struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? x : THIS(x).cast(__Int);
    self->val += x->val;
}

static void _int_self_sub(void *_self, Int x)
{
    struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? x : THIS(x).cast(__Int);
    self->val -= x->val;
}

static void _int_assign(void *_self, Int x)
{
    struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? x : THIS(x).cast(__Int);
    self->val = x->val;
}

static void *_int_add(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? VA(x->val) : THIS(x).cast(__Int);
    x->val += self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return construct(_Int(), mem, x, VAEND);
}

static void *_int_sub(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? VA(x->val) : THIS(x).cast(__Int);
    x->val += self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return construct(_Int(), mem, x, VAEND);
}

static void *_int_mul(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? VA(x->val) : THIS(x).cast(__Int);
    x->val *= self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return construct(_Int(), mem, x, VAEND);
}

static void *_int_div(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? VA(x->val) : THIS(x).cast(__Int);
    x->val = self->val / x->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return construct(_Int(), mem, x, VAEND);
}

static void *_int_mod(const void *_self, Int x)
{
    const struct Int *self = offsetOf(_self, __Int);
    x = classOf(x) == __Int ? VA(x->val) : THIS(x).cast(__Int);
    x->val = self->val % x->val;
    void *mem = ARP_MallocARelDtor(classSz(__Int), destroy);
    return construct(_Int(), mem, x, VAEND);
}

static void *_int_cast(const void *_self, const void *class)
{
    const struct Int *self = offsetOf(_self, __Int);
    void *ret;
    if (class == __Int) {
        ret = ARP_MallocARel(classSz(class));
        construct(__Int, ret, _self, VAEND);
    } else if (class == T(Char)) {
        ret = ARP_MallocARel(classSz(class));
        Char v = construct(class, ret, VAEND);
        v->val = self->val;
    } else if (class == T(Double)) {
        ret = ARP_MallocARel(classSz(class));
        Double v = construct(class, ret, VAEND);
        v->val = self->val;
    } else if (class == T(Any)) {
        ret = ARP_MallocARel(classSz(class));
        construct(class, ret, _self, VAEND);
    } else {
        MetaObject m_obj = offsetOf(_self, T(MetaObject));
        ret = THIS(m_obj).cast(class);
    }
    return ret;
}

static void initInt(void)
{
    T(Class); //初始化Class选择器
    if (!__Int)
        __Int = new(T(Class), "Int", T(Object), sizeof(struct Int) + classSz(_Object().class),
                   _MetaClassS->ctor, _int_ctor,
                   _MetaClassS->cast, _int_cast,
                   _ClassS->equal, _int_equal,
                   _ClassS->cmp, _int_cmp,
                   _ClassS->inc, _int_inc,
                   _ClassS->dec, _int_dec,
                   _ClassS->self_add, _int_self_add,
                   _ClassS->self_sub, _int_self_sub,
                   _ClassS->assign, _int_assign,
                   _ClassS->add, _int_add,
                   _ClassS->sub, _int_sub,
                   _ClassS->mul, _int_mul,
                   _ClassS->div, _int_div,
                   _ClassS->mod, _int_mod,
                   Selector, _ClassS, NULL);
}

const void *_Int(void)
{
    if (!__Int)
        initInt();
    return __Int;
}
