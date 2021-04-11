//
// Created by xjs on 2020/12/20.
//
#include "include/_tctl_unordered_multimap.h"
#include "../include/tctl_hashtable.h"
#include <stdlib.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, HASHTABLE , ITERATOR, PAIR

struct Unordered_MultiMapClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Pair x);
    size_t (*count)(void *_this, FormWO_t x);
    Iterator (*find)(void *_this, FormWO_t x);
    size_t (*bucket_count)(void *_this);
    size_t (*max_bucket_count)(void *_this);
    void (*reserve)(void *_this, size_t x);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Unordered_MultiMap _s);
};

struct Unordered_MultiMap {
    Form_t key_f;
    Form_t val_f;
    Hashtable c;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static void _erase(Iterator iter);
static Iterator _insert(Pair x);
static size_t _count(FormWO_t x);
static Iterator _find(FormWO_t x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _reserve(size_t);
static void _clear(void);
static void _swap(Unordered_MultiMap _s);

//Unordered_MultiMapclass
static void *_unordered_multimapclass_ctor(void *_this, va_list *app);

//Unordered_MultiMap
static void *_unordered_multimap_ctor(void *_this, va_list *app);
static void *_unordered_multimap_dtor(void *_this);
static Iterator _unordered_multimap_begin(const void *_this);
static Iterator _unordered_multimap_end(const void *_this);
static size_t _unordered_multimap_size(const void *_this);
static bool _unordered_multimap_empty(const void *_this);
static void _unordered_multimap_erase(void *_this, Iterator iter);
static Iterator _unordered_multimap_insert(void *_this, Pair x);
static size_t _unordered_multimap_count(void *_this, FormWO_t x);
static Iterator _unordered_multimap_find(void *_this, FormWO_t x);
static size_t _unordered_multimap_bucket_count(void *_this);
static size_t _unordered_max_bucket_count(void *_this);
void _unordered_multimap_reserve(void *_this, size_t x);
static void _unordered_multimap_clear(void *_this);
static void _unordered_multimap_swap(void *_this, Unordered_MultiMap _s);

//init
static const void *__Unordered_MultiMap = NULL;
static const void *__Unordered_MultiMapClass = NULL;
volatile static struct Unordered_MultiMapSelector Unordered_MultiMapS = {
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
                           T(Class), sizeof(struct Unordered_MultiMapClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _unordered_multimapclass_ctor, NULL);
    }
    if (!__Unordered_MultiMap) {
        __Unordered_MultiMap = new(_Unordered_MultiMapClass(), "Unordered_MultiMap",
                     T(Object), sizeof(struct Unordered_MultiMap) + classSz(_Object().class),
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

Form_t _Unordered_MultiMap(void)
{
    if (!__Unordered_MultiMap)
        initUnordered_MultiMap();
    return (Form_t){OBJ, .class = __Unordered_MultiMap};
}

Form_t _Unordered_MultiMapClass(void)
{
    if (!__Unordered_MultiMapClass)
        initUnordered_MultiMap();
    return (Form_t){OBJ, .class = __Unordered_MultiMapClass};
}

//private
static FormWO_t _get_val(FormWO_t x)
{
    assert(x._.f == OBJ && x._.class == T(Pair).class);
    Pair p = *(Pair*)x.mem;
    if (p->f_t.f == OBJ) //OBJ则要加一层指针
        return FORM_WITH_OBJ(p->f_t, V(p->first));
    return FORM_WITH_OBJ(p->f_t, p->first);
}

//public
//Unordered_MultiMapClass
static void *_unordered_multimapclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_MultiMapClass, _this, app);
    struct Unordered_MultiMapClass *this = offsetOf(_this, __Unordered_MultiMapClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Unordered_MultiMapS + sizeof(Unordered_MultiMapS._);
    voidf *end = (void*)&Unordered_MultiMapS + sizeof(Unordered_MultiMapS);
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

//Unordered_MultiMap
static void *_unordered_multimap_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Unordered_MultiMap, _this, app);
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    this->c = malloc(classSz(_Hashtable().class));
    //获取key和val的类型
    FormWO_t f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    f._.f -= FORM;
    this->key_f = f._;
    f = va_arg(*app, FormWO_t);
    assert(f._.f >= FORM);
    f._.f -= FORM;
    this->val_f = f._;
    //构建Hashtable
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f == OBJ || t._.f == FUNC);
    if (t._.f == OBJ) { //复制构造
        assert(t._.class == __Unordered_MultiMap);
        struct Unordered_MultiMap *s = offsetOf(*(Object*)t.mem, __Unordered_MultiMap);
        construct(_Hashtable(), this->c, f, VA(s->c), VAEND);
    } else { //迭代器构造和默认构造
        FormWO_t equal = t;
        FormWO_t hash = va_arg(*app, FormWO_t);
        assert(hash._.f == FUNC);
        t = va_arg(*app, FormWO_t);
        FormWO_t get_val = t._.f == FUNC ? t : VA_FUNC(_get_val);
        construct(_Hashtable(), this->c, VA(T(Pair)), equal, hash, get_val, VAEND);
        if (t._.f == END)
            return _this;
        //迭代器
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator first = *(Iterator*)t.mem;
        first = THIS(first).ctor(NULL, VA(first), VAEND);
        t = va_arg(*app, FormWO_t);
        assert(t._.f == OBJ && t._.class == _Iterator().class);
        Iterator last = t.mem;
        last = THIS(first).ctor(NULL, VA(last), VAEND);
        Form_t it_t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            THIS(this->c).insert_unique(FORM_WITH_OBJ(it_t, V(THIS(first).derefer())));
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
    return _this;
}

static void *_unordered_multimap_dtor(void *_this)
{
    _this = super_dtor(__Unordered_MultiMap, _this);
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    destroy(this->c);
    free(this->c);
    return _this;
}

static Iterator _unordered_multimap_begin(const void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).begin();
}

static Iterator _unordered_multimap_end(const void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).end();
}

static size_t _unordered_multimap_size(const void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).size();
}

static bool _unordered_multimap_empty(const void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).empty();
}

static void _unordered_multimap_erase(void *_this, Iterator iter)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    THIS(this->c).erase(iter);
}

static Iterator _unordered_multimap_insert(void *_this, Pair x)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).insert_equal(VA(x));
}

static size_t _unordered_multimap_count(void *_this, FormWO_t x)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    Pair p = new(T(Pair), VA(this->key_f, this->val_f, x));
    size_t res = THIS(this->c).count(VA(p));
    delete(p);
    return res;
}

static Iterator _unordered_multimap_find(void *_this, FormWO_t x)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    Pair p = new(T(Pair), VA(this->key_f, this->val_f, x));
    Iterator res = THIS(this->c).find(VA(p));
    delete(p);
    return res;
}

static size_t _unordered_multimap_bucket_count(void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).bucket_count();
}

static size_t _unordered_max_bucket_count(void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    return THIS(this->c).max_bucket_count();
}

void _unordered_multimap_reserve(void *_this, size_t x)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    THIS(this->c).resize(x);
}

static void _unordered_multimap_clear(void *_this)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    THIS(this->c).clear();
}

static void _unordered_multimap_swap(void *_this, Unordered_MultiMap _s)
{
    struct Unordered_MultiMap *this = offsetOf(_this, __Unordered_MultiMap);
    struct Unordered_MultiMap *s = offsetOf(_s, __Unordered_MultiMap);
    THIS(this->c).swap(s->c);
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->erase);
    class->erase(_this, iter);
}

static Iterator _insert(Pair x)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->insert);
    return class->insert(_this, x);
}

static size_t _count(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->count);
    return class->count(_this, x);
}

static Iterator _find(FormWO_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->find);
    return class->find(_this, x);
}

static size_t _bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->bucket_count);
    return class->bucket_count(_this);
}

static size_t _max_bucket_count(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_this);
}

static void _reserve(size_t x)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->reserve);
    return class->reserve(_this, x);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Unordered_MultiMap _s)
{
    void *_this = pop_this();
    const struct Unordered_MultiMapClass *class = offsetOf(classOf(_this), __Unordered_MultiMapClass);
    assert(class->swap);
    class->swap(_this, _s);
}
