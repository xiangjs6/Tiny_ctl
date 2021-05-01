//
// Created by xjs on 2020/9/1.
//

#include "include/_tctl_iterator.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"

#define Import METACLASS, CLASS, OBJECT, ITERATOR, ANY

struct IteratorClass {
    void *(*derefer)(const void *_self);
    long long (*dist)(const void *_self, Iterator it);
    Iterator (*reverse_iterator)(void *_self);
};

struct Iterator {
    enum IterRank rank;
    void *class;
};

static void *_class_ctor(void *_self, va_list *app);
static void *_object_ctor(void *_self, va_list *app);
static void *_object_derefer(const void *_self);
static void *_derefer(void);
static long long _dist(Iterator it);
static Iterator _reverse_iterator(void);
//init
volatile static struct IteratorSelector IteratorS = {
        {},
        _derefer,
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
                              sizeof(struct IteratorClass) + classSz(T(Class)),
                              _MetaClassS->ctor, _class_ctor, NULL);
    }
    if (!__Iterator) {
        __Iterator = new(_IteratorClass(), "Iterator", T(Object),
                         sizeof(struct Iterator) + classSz(T(Object)),
                         _MetaClassS->ctor, _object_ctor,
                         IteratorS.derefer, _object_derefer,
                         Selector, _IteratorS, NULL);
    }
}

const void *_IteratorClass(void)
{
    if (!__IteratorClass)
        initIterator();
    return __IteratorClass;
}

const void *_Iterator(void)
{
    if (!__Iterator)
        initIterator();
    return __Iterator;
}

static void *_derefer(void)
{
    void *_self = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_self), __IteratorClass);
    assert(class->derefer);
    return class->derefer(_self);
}

static long long _dist(Iterator it)
{
    void *_self = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_self), __IteratorClass);
    assert(class->dist);
    return class->dist(_self, it);
}

static Iterator _reverse_iterator(void)
{
    void *_self = pop_this();
    const struct IteratorClass *class = offsetOf(classOf(_self), __IteratorClass);
    assert(class->reverse_iterator);
    return class->reverse_iterator(_self);
}

static void *_class_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__IteratorClass, _self, app);
    struct IteratorClass *self = offsetOf(_self, __IteratorClass);
    va_list ap;
    va_copy(ap, *app);
    voidf selector;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        if (selector == (voidf)IteratorS.derefer)
            *(void**)&self->derefer = method;
        else if (selector == (voidf)IteratorS.dist)
            *(void**)&self->dist = method;
        else if (selector == (voidf)IteratorS.reverse_iterator)
            *(void**)&self->reverse_iterator = method;
    }
    va_end(ap);
    return _self;
}

static void *_object_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Iterator, _self, app);
    struct Iterator *self = offsetOf(_self, __Iterator);
    va_list ap;
    va_copy(ap, *app);
    MetaObject m_obj = va_arg(*app, MetaObject);
    if (classOf(m_obj) == T(Any)) {
        Any any = (Any)m_obj;
        assert(THIS(any).size() == sizeof(unsigned long long));
        self->rank = *(unsigned long long*)THIS(any).value();
        assert(self->rank <= SequenceIter);
        void *class = va_arg(*app, void*);
        assert(class != VAEND);
        self->class = class;
    } else {
        assert(class_check(m_obj, __Iterator));
        struct Iterator *it = offsetOf(m_obj, __Iterator);
        *self = *it;
        va_copy(*app, ap);
    }
    va_end(ap);
    return _self;
}

static void *_object_derefer(const void *_self)
{
    const struct Iterator *self = offsetOf(_self, __Iterator);
    return (char*)self + sizeof(struct Iterator);
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
    } else if (n < 0) {
        if (it->rank >= RandomAccessIter)
            THIS(it).self_sub(VA(-n));
        else
            while (n++)
                THIS(it).dec();
    }
}
