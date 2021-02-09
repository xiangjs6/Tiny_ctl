//
// Created by xjs on 2020/12/19.
//

#include "include/_tctl_unordered_set.h"
#include "../include/tctl_hashtable.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR

struct Unordered_SetClass {
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
    void (*swap)(void *_this, Unordered_Set _s);
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
static Iterator _insert(FormWO_t x);
static size_t _count(FormWO_t x);
static Iterator _find(FormWO_t x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_Set _s);

//Unordered_Setclass
static void *_unordered_setclass_ctor(void *_this, va_list *app);

//Unordered_Set
static void *_unordered_set_ctor(void *_this, va_list *app);
static void *_unordered_set_dtor(void *_this);
static Iterator _unordered_set_begin(const void *_this);
static Iterator _unordered_set_end(const void *_this);
static size_t _unordered_set_size(const void *_this);
static bool _unordered_set_empty(const void *_this);
static void _unordered_set_erase(void *_this, Iterator iter);
static Iterator _unordered_set_insert(void *_this, FormWO_t x);
static size_t _unordered_set_count(void *_this, FormWO_t x);
static Iterator _unordered_set_find(void *_this, FormWO_t x);
static size_t _unordered_set_bucket_count(void *_this);
static size_t _unordered_max_bucket_count(void *_this);
void _unordered_set_reserve(void *_this, size_t x);
static void _unordered_set_clear(void *_this);
static void _unordered_set_swap(void *_this, Unordered_Set _s);

//init
static const void *__Unordered_Set = NULL;
static const void *__Unordered_SetClass = NULL;
volatile static struct Unordered_SetSelector Unordered_SetS = {
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
                           T(Class), sizeof(struct Unordered_SetClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _unordered_setclass_ctor, NULL);
    }
    if (!__Unordered_Set) {
        __Unordered_Set = new(_Unordered_SetClass(), "Unordered_Set",
                     T(Object), sizeof(struct Unordered_Set) + classSz(_Object().class),
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

Form_t _Unordered_Set(void)
{
    if (!__Unordered_Set)
        initUnordered_Set();
    return (Form_t){OBJ, .class = __Unordered_Set};
}

Form_t _Unordered_SetClass(void)
{
    if (!__Unordered_SetClass)
        initUnordered_Set();
    return (Form_t){OBJ, .class = __Unordered_SetClass};
}

//private
static FormWO_t _get_val(FormWO_t x)
{
    return x;
}

//public
//Unordered_SetClass
static void *_unordered_setclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_SetClass, _this, app);
    struct Unordered_SetClass *this = offsetOf(_this, __Unordered_SetClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Unordered_SetS + sizeof(Unordered_SetS._);
    voidf *end = (void*)&Unordered_SetS + sizeof(Unordered_SetS);
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

//Unordered_Set
static void *_unordered_set_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_Set, _this, app);
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    this->c = malloc(classSz(_Hashtable().class));
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ) { //复制构造
        assert(t._.class == __Unordered_Set);
        struct Unordered_Set *s = offsetOf(t.mem, __Unordered_Set);
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

static void *_unordered_set_dtor(void *_this)
{
    _this = super_dtor(__Unordered_Set, _this);
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _unordered_set_begin(const void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).begin();
}

static Iterator _unordered_set_end(const void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).end();
}

static size_t _unordered_set_size(const void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).size();
}

static bool _unordered_set_empty(const void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).empty();
}

static void _unordered_set_erase(void *_this, Iterator iter)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    THIS(this->c).erase(iter);
}

static Iterator _unordered_set_insert(void *_this, FormWO_t x)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).insert_unique(x);
}

static size_t _unordered_set_count(void *_this, FormWO_t x)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).count(x);
}

static Iterator _unordered_set_find(void *_this, FormWO_t x)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).find(x);
}

static size_t _unordered_set_bucket_count(void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    return THIS(this->c).max_bucket_count();
}

void _unordered_set_reserve(void *_this, size_t x)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    THIS(this->c).resize(x);
}

static void _unordered_set_clear(void *_this)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    THIS(this->c).clear();
}

static void _unordered_set_swap(void *_this, Unordered_Set _s)
{
    struct Unordered_Set *this = offsetOf(_this, __Unordered_Set);
    struct Unordered_Set *s = offsetOf(_s, __Unordered_Set);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->find);
    return class->find(_this, x);
}

static size_t _bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->bucket_count);
    return class->bucket_count(_this);
}

static size_t _max_bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_this);
}

static void _reserve(size_t x)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->reserve);
    return class->reserve(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Unordered_Set _s)
{
    void *_this = pop_this();
    const struct Unordered_SetClass *class = offsetOf(classOf(_this), __Unordered_SetClass);
    assert(class->swap);
    class->swap(_this, _s);
}
