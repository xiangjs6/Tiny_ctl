//
// Created by xjs on 2021/2/6.
//

#include "../include/tctl_utility.h"
#include "include/_tctl_metaclass.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include <stdarg.h>
#define Import METACLASS, METAOBJECT
//Pair
struct Pair {
    void *f_t;
    void *first;
    void *s_t;
    void *second;
};
static const void *__Pair = NULL;

static void *_pair_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Pair, _self, app);
    struct Pair *self = offsetOf(_self, __Pair);

    void *t = va_arg(*app, void*);
    if (classOf(t) == __Pair) { //复制构造函数
        struct Pair *p = offsetOf(t, __Pair);
        self->f_t = p->f_t;
        self->first = new(self->f_t, p->first, VAEND);
        self->s_t = p->s_t;
        self->second = new(self->s_t, p->second, VAEND);
    } else {
        self->f_t = t;
        assert(self->f_t != VAEND);
        self->s_t = va_arg(*app, void*);
        assert(self->s_t != VAEND);

        t = va_arg(*app, void*);
        self->first = new(self->f_t, t, VAEND);
        if (t != VAEND)
            t = va_arg(*app, void*);
        self->second = new(self->s_t, t, VAEND);
    }

    if (t != VAEND)
        while (va_arg(*app, void*) != VAEND); //吃掉VAEND
    return _self;
}

static void *_pair_dtor(void *_self)
{
    _self = super_dtor(__Pair, _self);
    struct Pair *self = offsetOf(_self, __Pair);
    delete(self->first);
    delete(self->second);
    return _self;
}

static void initPair(void)
{
    if (!__Pair) {
        __Pair = new(T(MetaClass), "Pair", T(MetaObject), sizeof(struct Pair) + classSz(T(MetaObject)),
                     _MetaClassS->ctor, _pair_ctor,
                     _MetaClassS->dtor, _pair_dtor, NULL);
    }
}

const void *_Pair(void)
{
    if (!__Pair)
        initPair();
    return __Pair;
}

Pair tmpPair(const void *first_t, const void *second_t, ...)
{
    if (!__Pair)
        initPair();
    va_list ap;
    va_start(ap, second_t);
    void *args[2] = {VAEND, VAEND};
    void *t;
    int n = 0;
    while ((t = va_arg(ap, void*)) != VAEND)
    {
        assert(n < 2);
        args[n++] = t;
    }
    void *mem = ARP_MallocARelDtor(classSz(__Pair), destroy);
    return construct(_Pair(), mem, first_t, second_t, args[0], args[1], VAEND);
}
