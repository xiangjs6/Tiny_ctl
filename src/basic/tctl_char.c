//
// Created by xjs on 2021/1/3.
//

#include <assert.h>
#include "../../include/auto_release_pool.h"
#include "../../include/tctl_any.h"
#include "../../include/tctl_arg.h"
#include "../../include/tctl_char.h"
#include "../../include/tctl_int.h"
#include "../../include/tctl_double.h"
#include "../include/_tctl_metaclass.h"
#include "../include/_tctl_class.h"
#define Import CLASS, CHAR, OBJECT， ANY, INT, DOUBLE

struct Char {
    char val;
};

static const void *__Char = NULL;

static void *_char_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Char, _self, app);
    struct Char *self = offsetOf(_self, __Char);
    MetaObject val = va_arg(*app, MetaObject);
    Char p;
    if (val == VAEND) {
        self->val = 0;
        return _self;
    } else if (classOf(val) == __Char) {
        p = (Char)val;
    } else if (classOf(val) == T(Any)) {
        Any any = (Any)val;
        p = THIS(any).cast(__Char);
    } else {
        p = THIS(val).cast(__Char);
    }
    
    return _self;
}

static bool _char_equal(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? x : THIS(x).cast(__Char);
    return self->val == x->val;
}

static int _char_cmp(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? x : THIS(x).cast(__Char);
    return self->val - x->val;
}

static void _char_inc(void *_self)
{
    struct Char *self = offsetOf(_self, __Char);
    self->val++;
}

static void _char_dec(void *_self)
{
    struct Char *self = offsetOf(_self, __Char);
    self->val--;
}

static void _char_self_add(void *_self, Char x)
{
    struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? x : THIS(x).cast(__Char);
    self->val += x->val;
}

static void _char_self_sub(void *_self, Char x)
{
    struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? x : THIS(x).cast(__Char);
    self->val -= x->val;
}

static void _char_assign(void *_self, Char x)
{
    struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? x : THIS(x).cast(__Char);
    self->val = x->val;
}

static void *_char_add(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? VA(x->val) : THIS(x).cast(__Char);
    x->val += self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return construct(__Char, mem, x, VAEND);
}

static void *_char_sub(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? VA(x->val) : THIS(x).cast(__Char);
    x->val -= self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return construct(__Char, mem, x, VAEND);
}

static void *_char_mul(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? VA(x->val) : THIS(x).cast(__Char);
    x->val *= self->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return construct(__Char, mem, x, VAEND);
}

static void *_char_div(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? VA(x->val) : THIS(x).cast(__Char);
    x->val = self->val / x->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return construct(__Char, mem, x, VAEND);
}

static void *_char_mod(const void *_self, Char x)
{
    const struct Char *self = offsetOf(_self, __Char);
    x = classOf(x) == __Char ? VA(x->val) : THIS(x).cast(__Char);
    x->val = self->val % x->val;
    void *mem = ARP_MallocARelDtor(classSz(__Char), destroy);
    return construct(__Char, mem, x, VAEND);
}

static void *_char_cast(const void *_self, const void *class)
{
    const struct Char *self = offsetOf(_self, __Char);
    void *ret;
    if (class == __Char) {
        ret = ARP_MallocARel(classSz(class));
        construct(__Char, ret, _self, VAEND);
    } else if (class == T(Int)) {
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

static void initChar(void)
{
    T(Class); //初始化Class选择器
    if (!__Char)
        __Char = new(T(Class), "Char", T(Object), sizeof(struct Char) + classSz(_Object().class),
                    _MetaClassS->ctor, _char_ctor,
                    _MetaClassS->cast, _char_cast,
                    _ClassS->equal, _char_equal,
                    _ClassS->cmp, _char_cmp,
                    _ClassS->inc, _char_inc,
                    _ClassS->dec, _char_dec,
                    _ClassS->self_add, _char_self_add,
                    _ClassS->self_sub, _char_self_sub,
                    _ClassS->assign, _char_assign,
                    _ClassS->add, _char_add,
                    _ClassS->sub, _char_sub,
                    _ClassS->mul, _char_mul,
                    _ClassS->div, _char_div,
                    _ClassS->mod, _char_mod,
                    Selector, _ClassS, NULL);
}

const void *_Char(void)
{
    if (!__Char)
        initChar();
    return __Char;
}
