//
// Created by xjs on 2020/9/1.
//

#include "include/_tctl_iterator.h"
//#include "tctl_allocator.h"
//#include "../include/auto_release_pool.h"
#include <memory.h>
#include <assert.h>

#define Import METACLASS, CLASS, OBJECT, ITERATORCLASS, ITERATOR

struct IteratorClass {
    void *(*derefer)(const void *_this);
    Form_t (*type)(const void *_this);
};

struct Iterator {
    Form_t _t;
    byte _v[0];
};

static void *_derefer(void);
static Form_t _type(void);
static void *_class_ctor(void *_this, va_list *app);
static void *_object_ctor(void *_this, va_list *app);
static void *_object_derefer(const void *_this);
static Form_t _object_type(const void *_this);
//init
volatile static struct IteratorSelector IteratorS = {
        {},
        _derefer,
        _type
};
const struct IteratorSelector *_IteratorS = NULL;

static struct IteratorClass *__IteratorClass = NULL;
static struct Iterator *__Iterator = NULL;

void initIterator(void)
{
    initClass();
    if (!_IteratorS) {
        memcpy((void*)&IteratorS, _ClassS, sizeof(struct ClassSelector));
        _IteratorS = (void*)&IteratorS;
    }
    if (!__IteratorClass) {
        __IteratorClass = new(T(MetaClass), "IteratorClass", T(Class),
                              sizeof(struct IteratorClass) + classSz(_Class().class),
                              _MetaClassS->ctor, _class_ctor);
    }
    if (!__Iterator) {
        __Iterator = new(T(IteratorClass), "Iterator", T(Object),
                         sizeof(struct Iterator) + classSz(_Object().class),
                         _MetaClassS->ctor, _object_ctor,
                         IteratorS.derefer, _object_derefer,
                         IteratorS.type, _object_type,
                         Selector, _IteratorS);
    }
}

Form_t _IteratorClass(void)
{
    return (Form_t){OBJ, {.class = __IteratorClass}};
}

Form_t _Iterator(void)
{
    return (Form_t){OBJ, {.class = __Iterator}};
}

static void *_derefer(void)
{
    void *_this = pop_this();
    const struct IteratorClass *class = classOf(_this);
    assert(class->derefer);
    return class->derefer(_this);
}

static Form_t _type(void)
{
    void *_this = pop_this();
    const struct IteratorClass *class = classOf(_this);
    assert(class->type);
    return class->type(_this);
}

static void *_class_ctor(void *_this, va_list *app)
{
    struct IteratorClass *this = super_ctor(__IteratorClass, _this, app);
    va_list ap;
    va_copy(ap, *app);
    voidf selector;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf)IteratorS.derefer)
            *(voidf *) &this->derefer = method;
        else if (selector == (voidf)IteratorS.type)
            *(voidf *) &this->type = method;
    }
    va_end(ap);
    return this;
}

static void *_object_ctor(void *_this, va_list *app)
{
    struct Iterator *this = super_ctor(__Iterator, _this, app);
    this->_t = va_arg(*app, Form_t);
    return this;
}

static void *_object_derefer(const void *_this)
{
    const struct Iterator *this = _this;
    return (void*)this->_v;
}

static Form_t _object_type(const void *_this)
{
    const struct Iterator *this = _this;
    return this->_t;
}
