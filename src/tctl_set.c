//
// Created by xjs on 2020/11/16.
//

#include "include/_tctl_set.h"
#include "../include/tctl_rb_tree.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, ANY

struct SetClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, const void *x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Set _s);
};

struct Set {
    RB_tree c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(const void *x);
static size_t _count(const void *x);
static Iterator _find(const void *x);
static void _clear(void);
static void _swap(Set _s);

//Setclass
static void *_setclass_ctor(void *_self, va_list *app);

//Set
static void *_set_ctor(void *_self, va_list *app);
static void *_set_dtor(void *_self);
static Iterator _set_begin(const void *_self);
static Iterator _set_end(const void *_self);
static size_t _set_size(const void *_self);
static bool _set_empty(const void *_self);
static void _set_erase(void *_self, Iterator iter);
static Iterator _set_insert(void *_self, const void *x);
static size_t _set_count(void *_self, const void *x);
static Iterator _set_find(void *_self, const void *x);
static void _set_clear(void *_self);
static void _set_swap(void *_self, Set _s);

//init
static const void *__Set = NULL;
static const void *__SetClass = NULL;
volatile static struct SetSelector SetS = {
    {0},
    _begin,
    _end,
    _size,
    _empty,
    _erase,
    _insert,
    _count,
    _find,
    _clear,
    _swap
};
const struct SetSelector *_SetS = NULL;

static void initSet(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_SetS) {
        _SetS = (void*)&SetS;
        memcpy((void*)&SetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__SetClass) {
        __SetClass = new(T(MetaClass), "SetClass",
                           T(Class), sizeof(struct SetClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _setclass_ctor, NULL);
    }
    if (!__Set) {
        __Set = new(_SetClass(), "Set",
                     T(Object), sizeof(struct Set) + classSz(T(Object)),
                     _MetaClassS->ctor, _set_ctor,
                     _MetaClassS->dtor, _set_dtor,
                     _SetS->begin, _set_begin,
                     _SetS->end, _set_end,
                     _SetS->size, _set_size,
                     _SetS->empty, _set_empty,
                     _SetS->erase, _set_erase,
                     _SetS->insert, _set_insert,
                     _SetS->count, _set_count,
                     _SetS->find, _set_find,
                     _SetS->clear, _set_clear,
                     _SetS->swap, _set_swap,
                     Selector, _SetS, NULL);
    }
}

const void *_Set(void)
{
    if (!__Set)
        initSet();
    return __Set;
}

const void *_SetClass(void)
{
    if (!__SetClass)
        initSet();
    return __SetClass;
}

//SetClass
static void *_setclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__SetClass, _self, app);
    struct SetClass *self = offsetOf(_self, __SetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&SetS + sizeof(SetS._));
    voidf *end = (voidf*)((char*)&SetS + sizeof(SetS));
    voidf *self_begin = (voidf*)self;
    while ((selector = va_arg(ap, voidf)))
    {
        voidf method = va_arg(ap, voidf);
        for (voidf *p = begin; p != end; p++) {
            if (*p == selector) {
                size_t n = p - begin;
                *(self_begin + n) = method;
                break;
            }
        }
    }
    va_end(ap);
    return _self;
}

//Set
static void *_set_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Set, _self, app);
    struct Set *self = offsetOf(_self, __Set);
    self->c = malloc(classSz(T(RB_tree)));
    const void *f = va_arg(*app, void*);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Set) { //复制构造
        struct Set *s = offsetOf(t, __Set);
        construct(T(RB_tree), self->c, f, s->c, VAEND);
    } else { //迭代器构造和默认构造
        assert(classOf(t) == T(Any));
        construct(T(RB_tree), self->c, f, t, VAEND);
        t = va_arg(*app, void*);
        if (t == VAEND)
            return _self;

        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        while (!THIS(first).equal(VA(last)))
        {
            THIS(self->c).insert_unique(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_set_dtor(void *_self)
{
    _self = super_dtor(__Set, _self);
    struct Set *self = offsetOf(_self, __Set);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _set_begin(const void *_self)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).begin();
}

static Iterator _set_end(const void *_self)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).end();
}

static size_t _set_size(const void *_self)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).size();
}

static bool _set_empty(const void *_self)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).empty();
}

static void _set_erase(void *_self, Iterator iter)
{
    struct Set *self = offsetOf(_self, __Set);
    THIS(self->c).erase(iter);
}

static Iterator _set_insert(void *_self, const void *x)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).insert_unique(x);
}

static size_t _set_count(void *_self, const void *x)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).count(x);
}

static Iterator _set_find(void *_self, const void *x)
{
    struct Set *self = offsetOf(_self, __Set);
    return THIS(self->c).find(x);
}

static void _set_clear(void *_self)
{
    struct Set *self = offsetOf(_self, __Set);
    THIS(self->c).clear();
}

static void _set_swap(void *_self, Set _s)
{
    struct Set *self = offsetOf(_self, __Set);
    struct Set *s = offsetOf(_s, __Set);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(const void *x)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->find);
    return class->find(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Set _s)
{
    void *_self = pop_this();
    const struct SetClass *class = offsetOf(classOf(_self), __SetClass);
    assert(class->swap);
    class->swap(_self, _s);
}
