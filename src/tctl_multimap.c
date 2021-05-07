//
// Created by xjs on 2020/11/19.
//

#include "include/_tctl_multimap.h"
#include "../include/tctl_rb_tree.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, RB_TREE, ITERATOR, PAIR, ANY

struct MultiMapClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Pair x);
    size_t (*count)(void *_self, const void *x);
    Iterator (*find)(void *_self, const void *x);
    void (*clear)(void *_self);
    void (*swap)(void *_self, MultiMap _s);
};

struct MultiMap {
    void *key_class;
    void *val_class;
    RB_tree c;
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
static void _clear(void);
static void _swap(MultiMap _s);

//MultiMapclass
static void *_multimapclass_ctor(void *_self, va_list *app);

//MultiMap
static void *_multimap_ctor(void *_self, va_list *app);
static void *_multimap_dtor(void *_self);
static Iterator _multimap_begin(const void *_self);
static Iterator _multimap_end(const void *_self);
static size_t _multimap_size(const void *_self);
static bool _multimap_empty(const void *_self);
static void _multimap_erase(void *_self, Iterator iter);
static Iterator _multimap_insert(void *_self, Pair x);
static size_t _multimap_count(void *_self, const void *x);
static Iterator _multimap_find(void *_self, const void *x);
static void _multimap_clear(void *_self);
static void _multimap_swap(void *_self, MultiMap _s);

//init
static const void *__MultiMap = NULL;
static const void *__MultiMapClass = NULL;
volatile static struct MultiMapSelector MultiMapS = {
    {},
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
const struct MultiMapSelector *_MultiMapS = NULL;

static void initMultiMap(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_MultiMapS) {
        _MultiMapS = (void*)&MultiMapS;
        memcpy((void*)&MultiMapS, _ClassS, sizeof(*_ClassS));
    }
    if (!__MultiMapClass) {
        __MultiMapClass = new(T(MetaClass), "MultiMapClass",
                           T(Class), sizeof(struct MultiMapClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _multimapclass_ctor, NULL);
    }
    if (!__MultiMap) {
        __MultiMap = new(_MultiMapClass(), "MultiMap",
                     T(Object), sizeof(struct MultiMap) + classSz(T(Object)),
                     _MetaClassS->ctor, _multimap_ctor,
                     _MetaClassS->dtor, _multimap_dtor,
                     _MultiMapS->begin, _multimap_begin,
                     _MultiMapS->end, _multimap_end,
                     _MultiMapS->size, _multimap_size,
                     _MultiMapS->empty, _multimap_empty,
                     _MultiMapS->erase, _multimap_erase,
                     _MultiMapS->insert, _multimap_insert,
                     _MultiMapS->count, _multimap_count,
                     _MultiMapS->find, _multimap_find,
                     _MultiMapS->clear, _multimap_clear,
                     _MultiMapS->swap, _multimap_swap,
                     Selector, _MultiMapS, NULL);
    }
}

const void *_MultiMap(void)
{
    if (!__MultiMap)
        initMultiMap();
    return __MultiMap;
}

const void *_MultiMapClass(void)
{
    if (!__MultiMapClass)
        initMultiMap();
    return __MultiMapClass;
}

//MultiMapClass
static void *_multimapclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__MultiMapClass, _self, app);
    struct MultiMapClass *self = offsetOf(_self, __MultiMapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&MultiMapS + sizeof(MultiMapS._);
    voidf *end = (void*)&MultiMapS + sizeof(MultiMapS);
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

//MultiMap
static void *_multimap_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__MultiMap, _self, app);
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    self->c = malloc(classSz(T(RB_tree)));
    //获取key和val的类型
    void *f = va_arg(*app, void*);
    self->key_class = f;
    f = va_arg(*app, void*);
    self->val_class = f;
    //构建rb_tree
    void *t = va_arg(*app, void*);
    if (classOf(t) == __MultiMap) { //复制构造
        struct MultiMap *s = offsetOf(t, __MultiMap);
        construct(_RB_tree(), self->c, T(Pair), s->c, VAEND);
    } else { //迭代器构造和默认构造
        construct(_RB_tree(), self->c, T(Pair), t, VAEND);
        t = va_arg(*app, void*);
        if (t == VAEND)
            return _self;
        //迭代器
        assert(class_check(t, T(Iterator)));
        Iterator first = t;
        first = THIS(first).ctor(NULL, first, VAEND);
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

static void *_multimap_dtor(void *_self)
{
    _self = super_dtor(__MultiMap, _self);
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    destroy(self->c);
    free(self->c);
    return _self;
}

static Iterator _multimap_begin(const void *_self)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    return THIS(self->c).begin();
}

static Iterator _multimap_end(const void *_self)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    return THIS(self->c).end();
}

static size_t _multimap_size(const void *_self)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    return THIS(self->c).size();
}

static bool _multimap_empty(const void *_self)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    return THIS(self->c).empty();
}

static void _multimap_erase(void *_self, Iterator iter)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    THIS(self->c).erase(iter);
}

static Iterator _multimap_insert(void *_self, Pair x)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    return THIS(self->c).insert_unique(x);
}

static size_t _multimap_count(void *_self, const void *x)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    size_t res = THIS(self->c).count(p);
    delete(p);
    return res;
}

static Iterator _multimap_find(void *_self, const void *x)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    Pair p = new(T(Pair), self->key_class, self->val_class, x, VAEND);
    Iterator res = THIS(self->c).find(p);
    delete(p);
    return res;
}

static void _multimap_clear(void *_self)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    THIS(self->c).clear();
}

static void _multimap_swap(void *_self, MultiMap _s)
{
    struct MultiMap *self = offsetOf(_self, __MultiMap);
    struct MultiMap *s = offsetOf(_s, __MultiMap);
    THIS(self->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _insert(Pair x)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->insert);
    return class->insert(_self, x);
}

static size_t _count(const void *x)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->count);
    return class->count(_self, x);
}

static Iterator _find(const void *x)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->find);
    return class->find(_self, x);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(MultiMap _s)
{
    void *_self = pop_this();
    const struct MultiMapClass *class = offsetOf(classOf(_self), __MultiMapClass);
    assert(class->swap);
    class->swap(_self, _s);
}
