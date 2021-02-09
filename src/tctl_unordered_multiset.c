//
// Created by xjs on 2020/12/20.
//

#include "include/_tctl_unordered_multiset.h"
#include "../include/tctl_hashtable.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR

struct Unordered_MultiSetClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, FormWO_t x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    size_t (*bucket_count)(void *_this);
    size_t (*max_bucket_count)(void *_this);
    void (*reserve)(void *_this, size_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Unordered_MultiSet _s);
};

struct Unordered_MultiSet {
    Hashtable c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(FormWO_t x);
static size_t _count(FormWO_t x);
static Iterator _find(FormWO_t x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_MultiSet _s);

//Unordered_MultiSetclass
static void *_unordered_multisetclass_ctor(void *_this, va_list *app);

//Unordered_MultiSet
static void *_unordered_multiset_ctor(void *_this, va_list *app);
static void *_unordered_multiset_dtor(void *_this);
static Iterator _unordered_multiset_begin(const void *_this);
static Iterator _unordered_multiset_end(const void *_this);
static size_t _unordered_multiset_size(const void *_this);
static bool _unordered_multiset_empty(const void *_this);
static void _unordered_multiset_erase(void *_this, Iterator iter);
static Iterator _unordered_multiset_insert(void *_this, FormWO_t x);
static size_t _unordered_multiset_count(void *_this, FormWO_t x);
static Iterator _unordered_multiset_find(void *_this, FormWO_t x);
static size_t _unordered_multiset_bucket_count(void *_this);
static size_t _unordered_max_bucket_count(void *_this);
void _unordered_multiset_reserve(void *_this, size_t x);
static void _unordered_multiset_clear(void *_this);
static void _unordered_multiset_swap(void *_this, Unordered_MultiSet _s);

//init
static const void *__Unordered_MultiSet = NULL;
static const void *__Unordered_MultiSetClass = NULL;
volatile static struct Unordered_MultiSetSelector Unordered_MultiSetS = {
    {},
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
const struct Unordered_MultiSetSelector *_Unordered_MultiSetS = NULL;

static void initUnordered_MultiSet(void)
{
    T(Class); //初始化_ClassS选择器
    if (!_Unordered_MultiSetS) {
        _Unordered_MultiSetS = (void*)&Unordered_MultiSetS;
        memcpy((void*)&Unordered_MultiSetS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Unordered_MultiSetClass) {
        __Unordered_MultiSetClass = new(T(MetaClass), "Unordered_MultiSetClass",
                           T(Class), sizeof(struct Unordered_MultiSetClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _unordered_multisetclass_ctor, NULL);
    }
    if (!__Unordered_MultiSet) {
        __Unordered_MultiSet = new(_Unordered_MultiSetClass(), "Unordered_MultiSet",
                     T(Object), sizeof(struct Unordered_MultiSet) + classSz(_Object().class),
                     _MetaClassS->ctor, _unordered_multiset_ctor,
                     _MetaClassS->dtor, _unordered_multiset_dtor,
                     _Unordered_MultiSetS->begin, _unordered_multiset_begin,
                     _Unordered_MultiSetS->end, _unordered_multiset_end,
                     _Unordered_MultiSetS->size, _unordered_multiset_size,
                     _Unordered_MultiSetS->empty, _unordered_multiset_empty,
                     _Unordered_MultiSetS->erase, _unordered_multiset_erase,
                     _Unordered_MultiSetS->insert, _unordered_multiset_insert,
                     _Unordered_MultiSetS->count, _unordered_multiset_count,
                     _Unordered_MultiSetS->find, _unordered_multiset_find,
                     _Unordered_MultiSetS->bucket_count, _unordered_multiset_bucket_count,
                     _Unordered_MultiSetS->max_bucket_count, _unordered_max_bucket_count,
                     _Unordered_MultiSetS->reserve, _unordered_multiset_reserve,
                     _Unordered_MultiSetS->clear, _unordered_multiset_clear,
                     _Unordered_MultiSetS->swap, _unordered_multiset_swap,
                     Selector, _Unordered_MultiSetS, NULL);
    }
}

Form_t _Unordered_MultiSet(void)
{
    if (!__Unordered_MultiSet)
        initUnordered_MultiSet();
    return (Form_t){OBJ, .class = __Unordered_MultiSet};
}

Form_t _Unordered_MultiSetClass(void)
{
    if (!__Unordered_MultiSetClass)
        initUnordered_MultiSet();
    return (Form_t){OBJ, .class = __Unordered_MultiSetClass};
}

//private
static FormWO_t _get_val(FormWO_t x)
{
    return x;
}

//public
//Unordered_MultiSetClass
static void *_unordered_multisetclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_MultiSetClass, _this, app);
    struct Unordered_MultiSetClass *this = offsetOf(_this, __Unordered_MultiSetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Unordered_MultiSetS + sizeof(Unordered_MultiSetS._);
    voidf *end = (void*)&Unordered_MultiSetS + sizeof(Unordered_MultiSetS);
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
    return _this;
}

//Unordered_MultiSet
static void *_unordered_multiset_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_MultiSet, _this, app);
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    this->c = malloc(classSz(_Hashtable().class));
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ) { //复制构造
        assert(t._.class == __Unordered_MultiSet);
        struct Unordered_MultiSet *s = offsetOf(t.mem, __Unordered_MultiSet);
        construct(_Hashtable(), this->c, f, VA(s->c), VAEND);
    } else { //迭代器构造和默认构造
        FormWO_t equal = t;
        FormWO_t hash = va_arg(*app, FormWO_t);
        assert(hash._.f == FUNC);
        t = va_arg(*app, FormWO_t);
        FormWO_t get_val = t._.f == FUNC ? t : VA_FUNC(_get_val);
        construct(_Hashtable(), this->c, f, equal, hash, get_val, VAEND);
        if (t._.f == END)
            return _this;

        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator first = t.mem;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, FormWO_t);
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator last = t.mem;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        Form_t it_t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            THIS(this->c).insert_unique(FORM_WITH_OBJ(it_t, THIS(first).derefer()));
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _this;
}

static void *_unordered_multiset_dtor(void *_this)
{
    _this = super_dtor(__Unordered_MultiSet, _this);
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _unordered_multiset_begin(const void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).begin();
}

static Iterator _unordered_multiset_end(const void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).end();
}

static size_t _unordered_multiset_size(const void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).size();
}

static bool _unordered_multiset_empty(const void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).empty();
}

static void _unordered_multiset_erase(void *_this, Iterator iter)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    THIS(this->c).erase(iter);
}

static Iterator _unordered_multiset_insert(void *_this, FormWO_t x)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).insert_equal(x);
}

static size_t _unordered_multiset_count(void *_this, FormWO_t x)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).count(x);
}

static Iterator _unordered_multiset_find(void *_this, FormWO_t x)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).find(x);
}

static size_t _unordered_multiset_bucket_count(void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    return THIS(this->c).max_bucket_count();
}

void _unordered_multiset_reserve(void *_this, size_t x)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    THIS(this->c).resize(x);
}

static void _unordered_multiset_clear(void *_this)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    THIS(this->c).clear();
}

static void _unordered_multiset_swap(void *_this, Unordered_MultiSet _s)
{
    struct Unordered_MultiSet *this = offsetOf(_this, __Unordered_MultiSet);
    struct Unordered_MultiSet *s = offsetOf(_s, __Unordered_MultiSet);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->find);
    return class->find(_this, x);
}

static size_t _bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->bucket_count);
    return class->bucket_count(_this);
}

static size_t _max_bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_this);
}

static void _reserve(size_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->reserve);
    return class->reserve(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Unordered_MultiSet _s)
{
    void *_this = pop_this();
    const struct Unordered_MultiSetClass *class = offsetOf(classOf(_this), __Unordered_MultiSetClass);
    assert(class->swap);
    class->swap(_this, _s);
}
