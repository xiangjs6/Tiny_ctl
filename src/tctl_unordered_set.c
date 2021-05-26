//
// Created by xjs on 2020/12/19.
//

#include "include/_tctl_unordered_set.h"
#include "../include/tctl_hashtable.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR, ANY

struct Unordered_SetClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, const void *x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    size_t (*bucket_count)(void *_self);
    size_t (*max_bucket_count)(void *_self);
    void (*reserve)(void *_self, size_t x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Unordered_Set _s);
};

struct Unordered_Set {
    Hashtable c;
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
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_Set _s);

//Unordered_Setclass
static void *_unordered_setclass_ctor(void *_self, va_list *app);

//Unordered_Set
static void *_unordered_set_ctor(void *_self, va_list *app);
static void *_unordered_set_dtor(void *_self);
static Iterator _unordered_set_begin(const void *_self);
static Iterator _unordered_set_end(const void *_self);
static size_t _unordered_set_size(const void *_self);
static bool _unordered_set_empty(const void *_self);
static void _unordered_set_erase(void *_self, Iterator iter);
static Iterator _unordered_set_insert(void *_self, const void *x);
static size_t _unordered_set_count(void *_self, const void *x);
static Iterator _unordered_set_find(void *_self, const void *x);
static size_t _unordered_set_bucket_count(void *_self);
static size_t _unordered_max_bucket_count(void *_self);
void _unordered_set_reserve(void *_self, size_t x);
static void _unordered_set_clear(void *_self);
static void _unordered_set_swap(void *_self, Unordered_Set _s);

//init
static const void *__Unordered_Set = NULL;
static const void *__Unordered_SetClass = NULL;
volatile static struct Unordered_SetSelector Unordered_SetS = {
    {0},
    _begin,
    _end,
    _size,
    _empty,
    _erase,
    _insert,
    _count,
    _find,
    _bucket_count,
    _max_bucket_count,
    _reserve,
    _clear,
    _swap
};
const struct Unordered_SetSelector *_Unordered_SetS = NULL;

static void initUnordered_Set(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Unordered_SetS) {
        _Unordered_SetS = (void*)&Unordered_SetS;
        memcpy((void*)&Unordered_SetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Unordered_SetClass) {
        __Unordered_SetClass = new(T(MetaClass), "Unordered_SetClass",
                           T(Class), sizeof(struct Unordered_SetClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _unordered_setclass_ctor, NULL);
    }
    if (!__Unordered_Set) {
        __Unordered_Set = new(_Unordered_SetClass(), "Unordered_Set",
                     T(Object), sizeof(struct Unordered_Set) + classSz(T(Object)),
                     _MetaClassS->ctor, _unordered_set_ctor,
                     _MetaClassS->dtor, _unordered_set_dtor,
                     _Unordered_SetS->begin, _unordered_set_begin,
                     _Unordered_SetS->end, _unordered_set_end,
                     _Unordered_SetS->size, _unordered_set_size,
                     _Unordered_SetS->empty, _unordered_set_empty,
                     _Unordered_SetS->erase, _unordered_set_erase,
                     _Unordered_SetS->insert, _unordered_set_insert,
                     _Unordered_SetS->count, _unordered_set_count,
                     _Unordered_SetS->find, _unordered_set_find,
                     _Unordered_SetS->bucket_count, _unordered_set_bucket_count,
                     _Unordered_SetS->max_bucket_count, _unordered_max_bucket_count,
                     _Unordered_SetS->reserve, _unordered_set_reserve,
                     _Unordered_SetS->clear, _unordered_set_clear,
                     _Unordered_SetS->swap, _unordered_set_swap,
                     Selector, _Unordered_SetS, NULL);
    }
}

const void *_Unordered_Set(void)
{
    if (!__Unordered_Set)
        initUnordered_Set();
    return __Unordered_Set;
}

const void *_Unordered_SetClass(void)
{
    if (!__Unordered_SetClass)
        initUnordered_Set();
    return __Unordered_SetClass;
}

//private
static void *_get_val(void *x)
{
    return x;
}

//public
//Unordered_SetClass
static void *_unordered_setclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_SetClass, _self, app);
    struct Unordered_SetClass *self = offsetOf(_self, __Unordered_SetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&Unordered_SetS + sizeof(Unordered_SetS._));
    voidf *end = (voidf*)((char*)&Unordered_SetS + sizeof(Unordered_SetS));
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

//Unordered_Set
static void *_unordered_set_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_Set, _self, app);
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    self->c = malloc(classSz(T(Hashtable)));
    void *class = va_arg(*app, void*);
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Unordered_Set) { //复制构造
        struct Unordered_Set *s = offsetOf(t, __Unordered_Set);
        construct(T(Hashtable), self->c, class, s->c, VAEND);
    } else { //迭代器构造和默认构造
        assert(classOf(t) == T(Any));
        Any equal = t;
        Any hash = va_arg(*app, void*);
        assert(classOf(hash) == T(Any));
        t = va_arg(*app, void*);
        Any get_val;
        if (t != VAEND && classOf(t) == T(Any))
            get_val = t;
        else
            get_val = VA(_get_val, FUNC);
        construct(T(Hashtable), self->c, class, equal, hash, get_val, VAEND);
        if (t == VAEND)
            return _self;
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, first, VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, last, VAEND);
        while (!THIS(first).equal(last))
        {
            THIS(self->c).insert_unique(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_unordered_set_dtor(void *_self)
{
    _self = super_dtor(__Unordered_Set, _self);
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _unordered_set_begin(const void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).begin();
}

static Iterator _unordered_set_end(const void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).end();
}

static size_t _unordered_set_size(const void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).size();
}

static bool _unordered_set_empty(const void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).empty();
}

static void _unordered_set_erase(void *_self, Iterator iter)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    THIS(self->c).erase(iter);
}

static Iterator _unordered_set_insert(void *_self, const void *x)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).insert_unique(x);
}

static size_t _unordered_set_count(void *_self, const void *x)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).count(x);
}

static Iterator _unordered_set_find(void *_self, const void *x)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).find(x);
}

static size_t _unordered_set_bucket_count(void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    return THIS(self->c).max_bucket_count();
}

void _unordered_set_reserve(void *_self, size_t x)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    THIS(self->c).resize(x);
}

static void _unordered_set_clear(void *_self)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    THIS(self->c).clear();
}

static void _unordered_set_swap(void *_self, Unordered_Set _s)
{
    struct Unordered_Set *self = offsetOf(_self, __Unordered_Set);
    struct Unordered_Set *s = offsetOf(_s, __Unordered_Set);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->find);
    return class->find(_self, x);
}

static size_t _bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->bucket_count);
    return class->bucket_count(_self);
}

static size_t _max_bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_self);
}

static void _reserve(size_t x)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->reserve);
    class->reserve(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Unordered_Set _s)
{
    void *_self = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_self), __Unordered_SetClass);
    assert(class->swap);
    class->swap(_self, _s);
}
