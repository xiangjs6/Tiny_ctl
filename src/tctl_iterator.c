//
// Created by xjs on 2020/9/1.
//

#include "include/_tctl_iterator.h"
//#include "tctl_allocator.h"
//#include "../include/auto_release_pool.h"
#include <memory.h>
#include <assert.h>

#define Import METACLASS, CLASS, OBJECT, ITERATOR

struct IteratorClass {
    void *(*derefer)(const void *_this);
    Form_t (*type)(const void *_this);
    long long (*dist)(const void *_this, struct _Iterator *it);
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
static long long _dist(struct _Iterator *it);
//init
volatile static struct IteratorSelector IteratorS = {
        {},
        _derefer,
        _type,
        _dist
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
                              _MetaClassS->ctor, _class_ctor, NULL);
    }
    if (!__Iterator) {
        __Iterator = new(_IteratorClass(), "Iterator", T(Object),
                         sizeof(struct Iterator) + classSz(_Object().class),
                         _MetaClassS->ctor, _object_ctor,
                         IteratorS.derefer, _object_derefer,
                         IteratorS.type, _object_type,
                         Selector, _IteratorS, NULL);
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
    const struct IteratorClass *class = offsetOf(classOf(_this), __IteratorClass);
    assert(class->derefer);
    return class->derefer(_this);
}

static Form_t _type(void)
{
    void *_this = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_this), __IteratorClass);
    assert(class->type);
    return class->type(_this);
}

static long long _dist(struct _Iterator *it)
{
    void *_this = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_this), __IteratorClass);
    assert(class->dist);
    return class->dist(_this, it);
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
        else if (selector == (voidf)IteratorS.dist)
            *(voidf *) &this->dist = method;
    }
    va_end(ap);
    return this;
}

static void *_object_ctor(void *_this, va_list *app)
{
    struct Iterator *this = super_ctor(__Iterator, _this, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == ADDR);
    this->_t = *(Form_t*)t.mem;
    return (void*)this + sizeof(struct Iterator);
}

static void *_object_derefer(const void *_this)
{
    const struct Iterator *this = offsetOf(_this, __Iterator);
    return (void*)this->_v;
}

static Form_t _object_type(const void *_this)
{
    const struct Iterator *this = offsetOf(_this, __Iterator);
    return this->_t;
}

long long distance(Iterator _a, Iterator _b)
{
    struct IteratorClass *a_class = offsetOf(classOf(_a), __IteratorClass);
    struct IteratorClass *b_class = offsetOf(classOf(_b), __IteratorClass);
    assert(a_class == b_class);
    if (a_class->dist)
        return a_class->dist(_a, _b);
    long long dis = 0;
    char tmp[sizeOf(_a)];
    Form_t t = THIS(_a).type();
    Iterator it = THIS(_a).ctor(tmp, VA(VA_ADDR(t), _a), VAEND);
    for (; !THIS(it).equal(VA(_b)); THIS(it).inc())
        dis++;
    destroy(it);
    return dis;
}
