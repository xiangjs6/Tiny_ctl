//
// Created by xjs on 2021/2/6.
//

#include "../include/tctl_utility.h"
#include "include/_tctl_metaclass.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_def.h"
#include <stdarg.h>
#include <memory.h>
#define Import METACLASS, METAOBJECT
//Pair
struct Pair {
    Form_t f_t;
    void *first;
    Form_t s_t;
    void *second;
};
static const void *__Pair = NULL;

static void *_pair_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Pair, _this, app);
    struct Pair *this = offsetOf(_this, __Pair);

    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == OBJ && t._.class == __Pair) { //复制构造函数
        struct Pair *p = offsetOf(t.mem, __Pair);
        this->f_t = p->f_t;
        if (this->f_t.f == OBJ)
            this->first = new(this->f_t, FORM_WITH_OBJ(p->f_t, p->first));
        else {
            this->first = new(this->f_t);
            memcpy(this->first, p->first, this->f_t.size);
        }
        this->s_t = p->s_t;
        if (this->s_t.f == OBJ)
            this->second = new(this->s_t, FORM_WITH_OBJ(p->s_t, p->second));
        else {
            this->second = new(this->s_t);
            memcpy(this->second, p->second, this->s_t.size);
        }
    } else {
        FormWO_t f[2];
        f[0] = t;
        assert(f[0]._.f >= FORM);
        f[0]._.f -= FORM;
        f[1] = va_arg(*app, FormWO_t);
        assert(f[1]._.f >= FORM);
        f[1]._.f -= FORM;
        this->f_t = f[0]._;
        this->s_t = f[1]._;

        t = va_arg(*app, FormWO_t);
        this->first = new(this->f_t, t);
        if (t._.f != END)
            t = va_arg(*app, FormWO_t);
        this->second = new(this->s_t, t);
    }

    if (t._.f != END)
        while (va_arg(*app, FormWO_t)._.f != END); //吃掉VAEND
    return _this;
}

static void *_pair_dtor(void *_this)
{
    _this = super_dtor(__Pair, _this);
    struct Pair *this = offsetOf(_this, __Pair);
    FormWO_t t = FORM_WITH_OBJ(this->f_t, this->first);
    delete(t);
    t = FORM_WITH_OBJ(this->s_t, this->second);
    delete(t);
    return _this;
}

void initPair(void)
{
    if (!__Pair) {
        __Pair = new(T(MetaClass), "Pair", T(MetaObject), sizeof(struct Pair) + classSz(_MetaObject().class),
                     _MetaClassS->ctor, _pair_ctor,
                     _MetaClassS->dtor, _pair_dtor, NULL);
    }
}

Form_t _Pair(void)
{
    return (Form_t){OBJ, .class = __Pair};
}

Pair tmpPair(Form_t key_t, Form_t val_t, ...)
{
    va_list ap;
    va_start(ap, val_t);
    FormWO_t args[2] = {VAEND, VAEND};
    FormWO_t t;
    int n = 0;
    while ((t = va_arg(ap, FormWO_t))._.f != END)
    {
        assert(n < 2);
        args[n++] = t;
    }
    void *mem = ARP_MallocARelDtor(classSz(__Pair), destroy);
    return construct(_Pair(), mem, VA(key_t, val_t), args[0], args[1], VAEND);
}
