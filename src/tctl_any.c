#include <memory.h>
#include <stdarg.h>
#include <stdlib.h>
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include "include/_tctl_metaclass.h"

#define Import METAOBJECT, METACLASS

struct AnySelector {
    char _[sizeof(struct MetaClassSelector)];
    void *(*value)(void);
    size_t (*size)(void);
};

struct AnyClass {
    void *(*value)(void *self);
    size_t (*size)(void *self);
};

struct Any {
    size_t size;
    enum TypeFlag type_flag;
    void *mem;
};

//Selector
static void *_value(void);
static size_t _size(void);
//AnyClass
static void *_AnyClass_ctor(void *_self, va_list *app);
//Any
static void *_Any_ctor(void *_self, va_list *app);
static void *_Any_dtor(void *_self);
static size_t _Any_size(void *_self);
static void *_Any_value(void *_self);

static const void *__AnyClass = NULL;
static const void *__Any = NULL;

volatile static struct AnySelector AnyS = {
        ._ = {},
        .value = _value,
        .size = _size
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
                               _MetaClassS->ctor, _AnyClass_ctor, NULL);

    if (!__Any)
        __Any = new(__AnyClass, "Any",
                          T(MetaObject), sizeof(struct Any) + classSz(T(MetaObject)),
                          _MetaClassS->ctor, _Any_ctor,
                          _MetaClassS->dtor, _Any_dtor,
                          _AnyS->value, _Any_value,
                          _AnyS->size, _Any_size, NULL);
}

const void *_Any(void)
{
    if (!__Any)
        initAny();
    return __Any;
}

//AnyClass
static void *_AnyClass_ctor(void *_self, va_list *app)
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
static void *_Any_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Any, _self, app);
    struct Any *self = offsetOf(_self, __Any);
    void *value = va_arg(*app, void*);
    va_list ap;
    va_copy(ap, *app);

    if (va_arg(ap, void*) == VAEND) {
        if (classOf(value) == __Any) {
            struct Any *p = offsetOf(value, __Any);
            self->size = p->size;
            if (self->type_flag == POD)
                self->mem = new(classOf(p->mem), p->mem, VAEND);
            else {
                self->mem = malloc(self->size);
                memcpy(self->mem, p->mem, self->size);
            }
        } else {
            self->size = classSz(value);
            self->mem = new(classOf(value), value, VAEND);
        }
    } else {
        self->size = va_arg(*app, size_t);
        self->mem = malloc(self->size);
        memcpy(self->mem, value, self->size);
    }
    va_end(ap);
    return _self;
}

static void *_Any_dtor(void *_self)
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

static size_t _Any_size(void *_self)
{
    struct Any *self = offsetOf(_self, __Any);
    return self->size;
}

static void *_Any_value(void *_self)
{
    struct Any *self = offsetOf(_self, __Any);
    return self->mem;
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
