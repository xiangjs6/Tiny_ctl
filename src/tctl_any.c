#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include "include/_tctl_metaclass.h"
#include "../include/auto_release_pool.h"

#define Import METAOBJECT, METACLASS, ANY

struct AnySelector {
    char _[sizeof(struct MetaClassSelector)];
    void *(*value)(void);
    size_t (*size)(void);
    enum TypeFlag (*type)(void);
};

struct AnyClass {
    void *(*value)(void *self);
    size_t (*size)(void *self);
    enum TypeFlag (*type)(void *self);
};

struct Any {
    size_t size;
    enum TypeFlag type_flag;
    void *(*cast)(void *self, const void *class);
    void *mem;
};

//Selector
static void *_value(void);
static size_t _size(void);
static enum TypeFlag _type(void);
//AnyClass
static void *_anyclass_ctor(void *_self, va_list *app);
//Any
static void *_any_ctor(void *_self, va_list *app);
static void *_any_dtor(void *_self);
static size_t _any_size(void *_self);
static void *_any_value(void *_self);
static enum TypeFlag _any_type(void *_self);
static void *_any_cast(void *_self, const void *class);

static const void *__AnyClass = NULL;
static const void *__Any = NULL;

volatile static struct AnySelector AnyS = {
        ._ = {},
        .value = _value,
        .size = _size,
        .type = _type
};

static const struct AnySelector *_AnyS = NULL;

static void initAny(void)
{
    if (!_AnyS) {
        _AnyS = (void*)&AnyS;
        memcpy((void*)&AnyS, _MetaClassS, sizeof(*_MetaClassS));
    }

    if (!__AnyClass)
        __AnyClass = new(T(MetaClass), "AnyClass",
                               T(MetaClass), sizeof(struct AnyClass) + classSz(T(MetaClass)),
                               _MetaClassS->ctor, _anyclass_ctor, NULL);

    if (!__Any)
        __Any = new(__AnyClass, "Any",
                          T(MetaObject), sizeof(struct Any) + classSz(T(MetaObject)),
                          _MetaClassS->ctor, _any_ctor,
                          _MetaClassS->dtor, _any_dtor,
                          _MetaClassS->cast, _any_cast,
                          _AnyS->value, _any_value,
                          _AnyS->size, _any_size,
                          _AnyS->type, _any_type,
                          Selector, _AnyS, NULL);
}

const void *_Any(void)
{
    if (!__Any)
        initAny();
    return __Any;
}

//AnyClass
static void *_anyclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__AnyClass, _self, app);
    struct AnyClass *this = offsetOf(_self, __AnyClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&AnyS + sizeof(AnyS._);
    voidf *end = (void*)&AnyS + sizeof(AnyS);
    voidf *this_begin = (void*)this;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(this_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _self;
}

//Any
static void *_any_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Any, _self, app);
    struct Any *self = offsetOf(_self, __Any);
    void *value = va_arg(*app, void*);
    va_list ap;
    va_copy(ap, *app);

    if (va_arg(ap, void*) == VAEND) {
        if (classOf(value) == __Any) {
            struct Any *p = offsetOf(value, __Any);
            self->type_flag = p->type_flag;
            self->size = p->size;
            self->cast = p->cast;
            if (self->type_flag == POD)
                self->mem = new(classOf(p->mem), p->mem, VAEND);
            else {
                self->mem = malloc(self->size);
                memcpy(self->mem, p->mem, self->size);
            }
        } else {
            self->type_flag = OBJ;
            self->size = classSz(value);
            self->mem = new(classOf(value), value, VAEND);
        }
    } else {
        self->type_flag = POD;
        self->size = va_arg(*app, size_t);
        self->cast = va_arg(*app, void*);
        if (self->cast == VAEND)
            self->cast = NULL;
        self->mem = malloc(self->size);
        assert(self->mem);
        memcpy(self->mem, value, self->size);
    }
    va_end(ap);
    return _self;
}

static void *_any_dtor(void *_self)
{
    _self = super_dtor(__Any, _self);
    struct Any *self = offsetOf(_self, __Any);
    switch (self->type_flag)
    {
        case OBJ:
            delete(self->mem);
            break;
        case POD:
            free(self->mem);
            break;
        default:
            assert(0);
    }
    return _self;
}

static size_t _any_size(void *_self)
{
    struct Any *self = offsetOf(_self, __Any);
    return self->size;
}

static void *_any_value(void *_self)
{
    struct Any *self = offsetOf(_self, __Any);
    return self->mem;
}

static enum TypeFlag _any_type(void *_self)
{
    struct Any *self = offsetOf(_self, __Any);
    return self->type_flag;
}

static void *_any_cast(void *_self, const void *class)
{
    struct Any *self = offsetOf(_self, __Any);
    if (self->type_flag == OBJ) {
        MetaObject m_obj = self->mem;
        return THIS(m_obj).cast(class);
    }
    assert(self->cast);
    return self->cast(self->mem, class);
}

//Selector
static void *_value(void)
{
    void *_self = pop_this();
    const struct AnyClass *class = offsetOf(classOf(_self), __AnyClass);
    assert(class->value);
    return class->value(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct AnyClass *class = offsetOf(classOf(_self), __AnyClass);
    assert(class->size);
    return class->size(_self);
}

static enum TypeFlag _type(void)
{
    void *_self = pop_this();
    const struct AnyClass *class = offsetOf(classOf(_self), __AnyClass);
    assert(class->type);
    return class->type(_self);
}

Any ctor_any(void *mem, void *ptr, size_t size)
{
    if (!__Any)
        T(Any);
    mem = mem ? mem : ARP_MallocARelDtor(classSz(__Any), destroy);
    return construct(__Any, mem, ptr, size, VAEND);
}
