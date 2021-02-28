//
// Created by xjs on 2020/9/1.
//

#include "include/_tctl_iterator.h"
//#include "tctl_allocator.h"
//#include "../include/auto_release_pool.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define Import METACLASS, CLASS, OBJECT, ITERATOR

struct IteratorClass {
    void *(*derefer)(const void *_this);
    Form_t (*type)(const void *_this);
    long long (*dist)(const void *_this, Iterator it);
    Iterator (*reverse_iterator)(void *_this);
};

struct Iterator {
    enum IterRank rank;
    Form_t _t;
    byte _v[0];
};

static void *_class_ctor(void *_this, va_list *app);
static void *_object_ctor(void *_this, va_list *app);
static void *_object_derefer(const void *_this);
static Form_t _object_type(const void *_this);
static void *_derefer(void);
static Form_t _type(void);
static long long _dist(Iterator it);
static Iterator _reverse_iterator(void);
//init
volatile static struct IteratorSelector IteratorS = {
        {},
        _derefer,
        _type,
        _dist,
        _reverse_iterator
};
const struct IteratorSelector *_IteratorS = NULL;

static struct IteratorClass *__IteratorClass = NULL;
static struct Iterator *__Iterator = NULL;

static void initIterator(void)
{
    T(Class); //初始化Class选择器
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
    if (!__IteratorClass)
        initIterator();
    return (Form_t){OBJ, {.class = __IteratorClass}};
}

Form_t _Iterator(void)
{
    if (!__Iterator)
        initIterator();
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

static long long _dist(Iterator it)
{
    void *_this = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_this), __IteratorClass);
    assert(class->dist);
    return class->dist(_this, it);
}

static Iterator _reverse_iterator(void)
{
    void *_this = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_this), __IteratorClass);
    assert(class->reverse_iterator);
    return class->reverse_iterator(_this);
}

static void *_class_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__IteratorClass, _this, app);
    struct IteratorClass *this = offsetOf(_this, __IteratorClass);
    va_list ap;
    va_copy(ap, *app);
    voidf selector;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf)IteratorS.derefer)
            *(void**)&this->derefer = method;
        else if (selector == (voidf)IteratorS.type)
            *(void**)&this->type = method;
        else if (selector == (voidf)IteratorS.dist)
            *(void**)&this->dist = method;
        else if (selector == (voidf)IteratorS.reverse_iterator)
            *(void**)&this->reverse_iterator = method;
    }
    va_end(ap);
    return _this;
}

static void *_object_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Iterator, _this, app);
    struct Iterator *this = offsetOf(_this, __Iterator);
    va_list ap;
    va_copy(ap, *app);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == OBJ) {
        assert(t._.class == __Iterator);
        struct Iterator *it = offsetOf(t.mem, __Iterator);
        *this = *it;
        va_copy(*app, ap);
        va_end(ap);
        return _this;
    }
    assert(t._.f >= FORM);
    t._.f -= FORM;
    assert(t._.f == OBJ || t._.f == ADDR);
    this->_t = t._;
    t  = va_arg(*app, FormWO_t);
    assert(t._.f == POD);
    memcpy(&this->rank, &t.mem, t._.size);
    va_end(ap);
    return _this;
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
    Iterator it = THIS(_a).ctor(tmp, VA(_a), VAEND);
    for (; !THIS(it).equal(VA(_b)); THIS(it).inc())
        dis++;
    destroy(it);
    return dis;
}

void advance(Iterator it, long long n)
{
    if (n > 0) {
        if (it->rank >= RandomAccessIter)
            THIS(it).self_add(VA(n));
        else
            while (n--)
                THIS(it).inc();
    } else {
        if (it->rank >= RandomAccessIter)
            THIS(it).self_sub(VA(-n));
        else
            while (n++)
                THIS(it).dec();
    }
}
