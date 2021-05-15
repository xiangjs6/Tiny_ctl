//
// Created by xjs on 2020/12/20.
//
#include "include/_tctl_unordered_multimap.h"
#include "../include/tctl_hashtable.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR, PAIR, ANY

struct Unordered_MultiMapClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Pair x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    size_t (*bucket_count)(void *_self);
    size_t (*max_bucket_count)(void *_self);
    void (*reserve)(void *_self, size_t x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Unordered_MultiMap _s);
};

struct Unordered_MultiMap {
    void *key_class;
    void *val_class;
    Hashtable c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(Pair x);
static size_t _count(const void *x);
static Iterator _find(const void *x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_MultiMap _s);

//Unordered_MultiMapclass
static void *_unordered_multimapclass_ctor(void *_self, va_list *app);

//Unordered_MultiMap
static void *_unordered_multimap_ctor(void *_self, va_list *app);
static void *_unordered_multimap_dtor(void *_self);
static Iterator _unordered_multimap_begin(const void *_self);
static Iterator _unordered_multimap_end(const void *_self);
static size_t _unordered_multimap_size(const void *_self);
static bool _unordered_multimap_empty(const void *_self);
static void _unordered_multimap_erase(void *_self, Iterator iter);
static Iterator _unordered_multimap_insert(void *_self, Pair x);
static size_t _unordered_multimap_count(void *_self, const void *x);
static Iterator _unordered_multimap_find(void *_self, const void *x);
static size_t _unordered_multimap_bucket_count(void *_self);
static size_t _unordered_max_bucket_count(void *_self);
void _unordered_multimap_reserve(void *_self, size_t x);
static void _unordered_multimap_clear(void *_self);
static void _unordered_multimap_swap(void *_self, Unordered_MultiMap _s);

//init
static const void *__Unordered_MultiMap = NULL;
static const void *__Unordered_MultiMapClass = NULL;
volatile static struct Unordered_MultiMapSelector Unordered_MultiMapS = {
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
const struct Unordered_MultiMapSelector *_Unordered_MultiMapS = NULL;

static void initUnordered_MultiMap(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Unordered_MultiMapS) {
        _Unordered_MultiMapS = (void*)&Unordered_MultiMapS;
        memcpy((void*)&Unordered_MultiMapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Unordered_MultiMapClass) {
        __Unordered_MultiMapClass = new(T(MetaClass), "Unordered_MultiMapClass",
                           T(Class), sizeof(struct Unordered_MultiMapClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _unordered_multimapclass_ctor, NULL);
    }
    if (!__Unordered_MultiMap) {
        __Unordered_MultiMap = new(_Unordered_MultiMapClass(), "Unordered_MultiMap",
                     T(Object), sizeof(struct Unordered_MultiMap) + classSz(T(Object)),
                     _MetaClassS->ctor, _unordered_multimap_ctor,
                     _MetaClassS->dtor, _unordered_multimap_dtor,
                     _Unordered_MultiMapS->begin, _unordered_multimap_begin,
                     _Unordered_MultiMapS->end, _unordered_multimap_end,
                     _Unordered_MultiMapS->size, _unordered_multimap_size,
                     _Unordered_MultiMapS->empty, _unordered_multimap_empty,
                     _Unordered_MultiMapS->erase, _unordered_multimap_erase,
                     _Unordered_MultiMapS->insert, _unordered_multimap_insert,
                     _Unordered_MultiMapS->count, _unordered_multimap_count,
                     _Unordered_MultiMapS->find, _unordered_multimap_find,
                     _Unordered_MultiMapS->bucket_count, _unordered_multimap_bucket_count,
                     _Unordered_MultiMapS->max_bucket_count, _unordered_max_bucket_count,
                     _Unordered_MultiMapS->reserve, _unordered_multimap_reserve,
                     _Unordered_MultiMapS->clear, _unordered_multimap_clear,
                     _Unordered_MultiMapS->swap, _unordered_multimap_swap,
                     Selector, _Unordered_MultiMapS, NULL);
    }
}

const void *_Unordered_MultiMap(void)
{
    if (!__Unordered_MultiMap)
        initUnordered_MultiMap();
    return __Unordered_MultiMap;
}

const void *_Unordered_MultiMapClass(void)
{
    if (!__Unordered_MultiMapClass)
        initUnordered_MultiMap();
    return __Unordered_MultiMapClass;
}

//private
static void *_get_val(void *x)
{
    assert(classOf(x) == T(Pair));
    Pair p = x;
    return p->first;
}

//public
//Unordered_MultiMapClass
static void *_unordered_multimapclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_MultiMapClass, _self, app);
    struct Unordered_MultiMapClass *self = offsetOf(_self, __Unordered_MultiMapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)((char*)&Unordered_MultiMapS + sizeof(Unordered_MultiMapS._));
    voidf *end = (void*)((char*)&Unordered_MultiMapS + sizeof(Unordered_MultiMapS));
    voidf *self_begin = (void*)self;
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

//Unordered_MultiMap
static void *_unordered_multimap_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Unordered_MultiMap, _self, app);
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    self->c = malloc(classSz(T(Hashtable)));
    //获取key和val的类型
    void *class = va_arg(*app, void*);
    self->key_class = class;
    class = va_arg(*app, void*);
    self->val_class = class;
    //构建Hashtable
    void *t = va_arg(*app, void*);
    if (classOf(t) == __Unordered_MultiMap) { //复制构造
        struct Unordered_MultiMap *s = offsetOf(t, __Unordered_MultiMap);
        construct(_Hashtable(), self->c, T(Pair), s->c, VAEND);
    } else { //迭代器构造和默认构造
        // assert(classOf(t) == T(Any));
        Any equal = t;
        t = va_arg(*app, void*);
        assert(classOf(t) == T(Any));
        Any hash = t;
        t = va_arg(*app, void*);
        Any get_val;
        if (t == VAEND && classOf(t) == T(Any))
            get_val = t;
        else
            get_val = VA_ANY(TEMP_VAR(voidf, &_get_val), NULL);
        construct(T(Hashtable), self->c, T(Pair), equal, hash, get_val, VAEND);
        if (t == VAEND)
            return _self;
        //迭代器
        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, first, VAEND);
        t = va_arg(*app, void*);
        assert(class_check(t, T(Iterator)));
        Iterator last = t;
        last = THIS(first).ctor(NULL, last, VAEND);
        while (!THIS(first).equal(last))
        {
            THIS(self->c).insert_equal(THIS(first).derefer());
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _self;
}

static void *_unordered_multimap_dtor(void *_self)
{
    _self = super_dtor(__Unordered_MultiMap, _self);
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _unordered_multimap_begin(const void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).begin();
}

static Iterator _unordered_multimap_end(const void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).end();
}

static size_t _unordered_multimap_size(const void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).size();
}

static bool _unordered_multimap_empty(const void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).empty();
}

static void _unordered_multimap_erase(void *_self, Iterator iter)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    THIS(self->c).erase(iter);
}

static Iterator _unordered_multimap_insert(void *_self, Pair x)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).insert_equal(x);
}

static size_t _unordered_multimap_count(void *_self, const void *x)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    size_t res = THIS(self->c).count(p);
    delete(p);
    return res;
}

static Iterator _unordered_multimap_find(void *_self, const void *x)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    Iterator res = THIS(self->c).find(p);
    delete(p);
    return res;
}

static size_t _unordered_multimap_bucket_count(void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    return THIS(self->c).max_bucket_count();
}

void _unordered_multimap_reserve(void *_self, size_t x)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    THIS(self->c).resize(x);
}

static void _unordered_multimap_clear(void *_self)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    THIS(self->c).clear();
}

static void _unordered_multimap_swap(void *_self, Unordered_MultiMap _s)
{
    struct Unordered_MultiMap *self = offsetOf(_self, __Unordered_MultiMap);
    struct Unordered_MultiMap *s = offsetOf(_s, __Unordered_MultiMap);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(Pair x)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->find);
    return class->find(_self, x);
}

static size_t _bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->bucket_count);
    return class->bucket_count(_self);
}

static size_t _max_bucket_count(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_self);
}

static void _reserve(size_t x)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->reserve);
    class->reserve(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Unordered_MultiMap _s)
{
    void *_self = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_self), __Unordered_MultiMapClass);
    assert(class->swap);
    class->swap(_self, _s);
}
