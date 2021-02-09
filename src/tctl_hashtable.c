//
// Created by xjs on 2020/12/14.
//

#include "../include/auto_release_pool.h"
#include "../include/tctl_allocator.h"
#include "../include/tctl_vector.h"
#include "include/_tctl_hashtable.h"
#include "include/_tctl_iterator.h"
#include "include/_tctl_metaclass.h"
#include <stdarg.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, HASHTABLE, VECTOR

struct Bucket_node {
    struct Bucket_node *next;
    byte data[0];
};

struct HashtableClass {
    Iterator (*begin)(void *_this);
    Iterator (*end)(void *_this);
    size_t (*size)(void *_this);
    bool (*empty)(void *_this);
    Iterator (*insert_unique)(void *_this, FormWO_t _x);
    Iterator (*insert_equal)(void *_this, FormWO_t _x);
    void (*erase)(void *_this, Iterator iter);
    Iterator (*find)(void *_this, FormWO_t _x);
    size_t (*count)(void *_this, FormWO_t _x);
    size_t (*bucket_count)(void *_this);
    size_t (*max_bucket_count)(void *_this);
    void (*resize)(void *_this, size_t n);
    void (*copy_from)(void *_this, const Hashtable _h);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Hashtable _h);
};

struct Hashtable{
    Form_t _t;
    size_t nmemb;
    Compare equal;
    HashFunc hash;
    ExtractKey get_key;
    size_t start;
    size_t finish;
    //上面成员会在copy函数中复制，所以要和下面成员分开
    Vector buckets;
};

struct HashtableIter{
    struct Bucket_node *cur;
    struct Hashtable *ht;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static Iterator _insert_unique(FormWO_t _x);
static Iterator _insert_equal(FormWO_t _x);
static void _erase(Iterator);
static Iterator _find(FormWO_t _x);
static size_t _count(FormWO_t _x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _resize(size_t);
static void _copy_from(const Hashtable);
static void _clear(void);
static void _swap(Hashtable h);
//Hashtableclass
static void *_hashtableclass_ctor(void *_this, va_list *app);
//Hashtable
static void *_hashtable_ctor(void *_this, va_list *app);
static void *_hashtable_dtor(void *_this);
static Iterator _hashtable_begin(const void *_this);
static Iterator _hashtable_end(const void *_this);
static size_t _hashtable_size(const void *_this);
static bool _hashtable_empty(const void *_this);
static Iterator _hashtable_insert_unique(void *_this, FormWO_t _x);
static Iterator _hashtable_insert_equal(void *_this, FormWO_t _x);
static void _hashtable_erase(void *_this, Iterator iter);
static Iterator _hashtable_find(void *_this, FormWO_t _x);
static size_t _hashtable_count(void *_this, FormWO_t _x);
static size_t _hashtable_bucket_count(void *_this);
static size_t _hashtable_max_bucket_count(void *_this);
static void _hashtable_resize(void *_this, size_t n);
static void _hashtable_copy_from(void *_this, const Hashtable _h);
static void _hashtable_clear(void *_this);
static void _hashtable_swap(void *_this, Hashtable _h);
//iterator
static void _iter_assign(void *_this, FormWO_t _x);
static void _iter_inc(void *_this);
static bool _iter_equal(const void *_this, FormWO_t _x);
static void *_iter_ctor(void *_this, va_list *app);
static void *_iter_derefer(const void *_this);
//init
static const void *__HashtableIter = NULL;
static const void *__Hashtable = NULL;
static const void *__HashtableClass = NULL;
volatile static struct HashtableSelector HashtableS = {
    {},
    _begin,
    _end,
    _size,
    _empty,
    _insert_unique,
    _insert_equal,
    _erase,
    _find,
    _count,
    _bucket_count,
    _max_bucket_count,
    _resize,
    _copy_from,
    _clear,
    _swap
};
const struct HashtableSelector *_HashtableS = NULL;

static void initHashtable(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!_HashtableS) {
        _HashtableS = (void*)&HashtableS;
        memcpy((void*)&HashtableS, _ClassS, sizeof(*_ClassS));
    }
    if (!__HashtableIter) {
        __HashtableIter = new(_IteratorClass(), "HashtableIter",
                         T(Iterator), sizeof(struct HashtableIter) + classSz(_Iterator().class),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         Selector, _IteratorS, NULL);
    }
    if (!__HashtableClass) {
        __HashtableClass = new(T(MetaClass), "HashtableClass",
                           T(Class), sizeof(struct HashtableClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _hashtableclass_ctor, NULL);
    }
    if (!__Hashtable) {
        __Hashtable = new(_HashtableClass(), "Hashtable",
                       T(Object), sizeof(struct Hashtable) + classSz(_Object().class),
                       _MetaClassS->ctor, _hashtable_ctor,
                       _MetaClassS->dtor, _hashtable_dtor,
                       HashtableS.begin, _hashtable_begin,
                       HashtableS.end, _hashtable_end,
                       HashtableS.size, _hashtable_size,
                       HashtableS.empty, _hashtable_empty,
                       HashtableS.insert_unique, _hashtable_insert_unique,
                       HashtableS.insert_equal, _hashtable_insert_equal,
                       HashtableS.erase, _hashtable_erase,
                       HashtableS.find, _hashtable_find,
                       HashtableS.count, _hashtable_count,
                       HashtableS.bucket_count, _hashtable_bucket_count,
                       HashtableS.max_bucket_count, _hashtable_max_bucket_count,
                       HashtableS.resize, _hashtable_resize,
                       HashtableS.copy_from, _hashtable_copy_from,
                       HashtableS.clear, _hashtable_clear,
                       HashtableS.swap, _hashtable_swap,
                       Selector, _HashtableS, NULL);
    }
}

Form_t _Hashtable(void)
{
    if (!__Hashtable)
        initHashtable();
    Form_t t = {OBJ, .class = __Hashtable};
    return t;
}

Form_t _HashtableClass(void)
{
    if (!__HashtableClass)
        initHashtable();
    Form_t t = {OBJ, .class = __HashtableClass};
    return t;
}

static Form_t _HashtableIter(void)
{
    if (!__HashtableIter)
        initHashtable();
    Form_t t = {OBJ, .class = __HashtableIter};
    return t;
}

//private
static const unsigned long _prime_list[] = {
        53, 97, 193, 389, 769,
        1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433,
        1572869, 3145739, 6291469, 12582917, 25165843,
        50331653, 100663319, 201326611, 402653189, 805306457,
        1610612741, 3221225473ul, 4294967291ul
};
static const int _num_primes = sizeof(_prime_list) / sizeof(*_prime_list);

static unsigned long _next_prime(unsigned long n)
{
    for (int i = 0; i < _num_primes; i++)
        if (_prime_list[i] >= n)
            return _prime_list[i];
    return _prime_list[_num_primes - 1];
}

static struct Bucket_node *_new_node(size_t size)
{
    struct Bucket_node *n = allocate(sizeof(struct Bucket_node) + size);
    n->next = NULL;
    return n;
}

static void _delete_node(struct Bucket_node *n, size_t size)
{
    deallocate(n, sizeof(struct Bucket_node) + size);
}

static void _insert_aux(struct Bucket_node **pos, FormWO_t x, Form_t t)
{
    size_t memb_size = t.f == POD ? t.size : classSz(t.class);
    struct Bucket_node *node = _new_node(memb_size);
    if (t.f == POD) {
        assert(x._.f != OBJ);
        if (x._.f == POD)
            memcpy(node->data, &x.mem, memb_size);
        else if (x._.f == ADDR)
            memcpy(node->data, x.mem, memb_size);
        else if (x._.f == END)
            memset(node->data, 0, memb_size);
    } else if (t.f == OBJ)
        construct(t, node->data, x, VAEND);
    node->next = *pos;
    *pos = node;
}

static size_t _bkt_num(FormWO_t data, size_t n, HashFunc hash, ExtractKey get_key)
{
    return hash(get_key(data)) % n;
}

//public
//HashIter
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__HashtableIter, _this, app);
    struct HashtableIter *this = offsetOf(_this, __HashtableIter);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == OBJ) {
        if (t._.class == _Iterator().class) {
            assert(classOf(t.mem) == __HashtableIter);
            struct HashtableIter *it = offsetOf(t.mem, __HashtableIter);
            *this = *it;
        }
    } else if (t._.f == POD) {
        this->cur = t.mem;
        t = va_arg(*app, FormWO_t);
        assert(t._.f == POD);
        this->ht = t.mem;
    }
    return _this;
}

static bool _iter_equal(const void *_this, FormWO_t _x)
{
    const struct HashtableIter *this = offsetOf(_this, __HashtableIter);
    const struct HashtableIter *x = offsetOf(_x.mem, __HashtableIter);
    return this->cur == x->cur;
}

static void _iter_inc(void *_this)
{
    struct HashtableIter *this = offsetOf(_this, __HashtableIter);
    struct Hashtable *ht = this->ht;
    if (!this->cur)
        return;
    if (!this->cur->next) {
        FormWO_t t = FORM_WITH_OBJ(this->ht->_t, this->cur->data);
        if (t._.f == POD)
            t._.f = ADDR;
        size_t n = _bkt_num(t, THIS(ht->buckets).size(), ht->hash, ht->get_key);
        struct Bucket_node **node = (struct Bucket_node**)THIS(ht->buckets).brackets(VA(n + 1));
        struct Bucket_node **end = (struct Bucket_node**)THIS(ht->buckets).brackets(VA(ht->finish));
        while (node != end)
        {
            if (*node)
                break;
            node++;
        }
        this->cur = *node;
    } else {
        this->cur = this->cur->next;
    }
}

static void _iter_assign(void *_this, FormWO_t _x)
{
    struct HashtableIter *this = offsetOf(_this, __HashtableIter);
    struct HashtableIter *it = offsetOf(_x.mem, __HashtableIter);
    *this = *it;
}

static void *_iter_derefer(const void *_this)
{
    struct HashtableIter *this = offsetOf(_this, __HashtableIter);
    return this->cur->data;
}

//HashtableClass
static void *_hashtableclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__HashtableClass, _this, app);
    struct HashtableClass *this = offsetOf(_this, __HashtableClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&HashtableS + sizeof(HashtableS._);
    voidf *end = (void*)&HashtableS + sizeof(HashtableS);
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

//Hashtable
static void *_hashtable_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Hashtable, _this, app);
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    //初始化成员
    this->buckets = new(T(Vector), VA(T(struct Bucket_node*)));
    this->nmemb = 0;
    this->start = this->finish = 0;
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f >= FORM);
    t._.f -= FORM;
    this->_t = t._;
    t = va_arg(*app, FormWO_t);
    if (t._.class == __Hashtable) { //处理复制构造
        _hashtable_copy_from(_this, t.mem);
        return _this;
    }
    //equal函数
    assert(t._.f == FUNC);
    this->equal = t.mem;
    assert(this->equal);
    t = va_arg(*app, FormWO_t); //hash函数
    assert(t._.f == FUNC);
    this->hash = t.mem;
    assert(this->hash);
    t = va_arg(*app, FormWO_t); //get_key函数
    assert(t._.f == FUNC);
    this->get_key = t.mem;
    assert(this->get_key);
    return _this;
}

static void *_hashtable_dtor(void *_this)
{
    _this = super_dtor(__Hashtable, _this);
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    _hashtable_clear(_this);
    delete(this->buckets);
    return _this;
}

static Iterator _hashtable_begin(const void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    struct Bucket_node **bucket = (struct Bucket_node**)THIS(this->buckets).brackets(VA(this->start));
    return new(compose(_HashtableIter(), mem), VA(t, ForwardIter, *bucket, this));
}

static Iterator _hashtable_end(const void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    struct Bucket_node **bucket = (struct Bucket_node**)THIS(this->buckets).brackets(VA(this->finish));
    return new(compose(_HashtableIter(), mem), VA(t, ForwardIter, *bucket, this));
}

static size_t _hashtable_size(const void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    return this->nmemb;
}

static bool _hashtable_empty(const void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    return this->nmemb;
}

static Iterator _hashtable_insert_unique(void *_this, FormWO_t _x)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    FormWO_t x = _x;
    if (x._.f == POD) {
        x._.f = ADDR;
        x.mem = &_x.mem;
    }
    _hashtable_resize(_this, this->nmemb + 1);
    const size_t n = _bkt_num(x, THIS(this->buckets).size(), this->hash, this->get_key);
    FormWO_t t = FORM_WITH_OBJ(this->_t);
    if (t._.f == POD)
        t._.f = ADDR;
    struct Bucket_node **first = THIS(this->buckets).brackets(VA(n));
    for (struct Bucket_node *cur = *first; cur; cur = cur->next) {
        t.mem = cur->data;
        if (!this->equal(this->get_key(t), this->get_key(x)))
            return _hashtable_end(_this);
    }
    _insert_aux(first, _x, this->_t);
    if (n < this->start || !this->nmemb)
        this->start = n;
    if (n + 1 > this->finish || !this->nmemb)
        this->finish = n + 1;
    this->nmemb++;

    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    return construct(_HashtableIter(), mem, VA(t._, ForwardIter, *first, this), VAEND);
}

static Iterator _hashtable_insert_equal(void *_this, FormWO_t _x)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    FormWO_t x = _x;
    if (x._.f == POD) {
        x._.f = ADDR;
        x.mem = &_x.mem;
    }
    _hashtable_resize(_this, this->nmemb + 1);
    const size_t n = _bkt_num(x, THIS(this->buckets).size(), this->hash, this->get_key);
    struct Bucket_node **first = THIS(this->buckets).brackets(VA(n));
    _insert_aux(first, _x, this->_t);
    if (n < this->start || !this->nmemb)
        this->start = n;
    if (n + 1 > this->finish || !this->nmemb)
        this->finish = n + 1;
    this->nmemb++;

    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    FormWO_t t = FORM_WITH_OBJ(this->_t);
    if (t._.f == POD)
        t._.f = ADDR;
    return construct(_HashtableIter(), mem, VA(t._, ForwardIter, *first, this), VAEND);
}

static void _hashtable_erase(void *_this, Iterator iter)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    struct HashtableIter *it = offsetOf(iter, __HashtableIter);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (it->ht != this)
        return;
    struct Bucket_node *cur = it->cur;
    _iter_inc(iter); //迭代器向后走一个
    FormWO_t x = FORM_WITH_OBJ(this->_t, cur->data);
    if (x._.f == POD)
        x._.f = ADDR;
    const size_t n = _bkt_num(x, THIS(this->buckets).size(), this->hash, this->get_key);
    struct Bucket_node **p = THIS(this->buckets).brackets(VA(n));
    if (*p == cur) {
        *p = cur->next;
    } else {
        struct Bucket_node *i = (*p)->next;
        while (i)
        {
            if (i->next == cur) {
                i->next = cur->next;
                break;
            }
        }
    }
    if (this->_t.f == OBJ)
        destroy(cur->data);
    _delete_node(cur, memb_size);
    this->nmemb--;
    //判断是否需要更新start和finish
    if (!this->nmemb) {
        this->start = this->finish = 0;
        return;
    }
    if (n == this->start) {
        int i;
        for (i = n + 1; !*(struct Bucket_node**)THIS(this->buckets).brackets(VA(i)); i++);
        this->start = i;
    } else if (n == this->finish - 1) {
        int i;
        for (i = n - 1; !*(struct Bucket_node**)THIS(this->buckets).brackets(VA(i)); i--);
        this->finish = i + 1;
    }
}

static Iterator _hashtable_find(void *_this, FormWO_t _x)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    FormWO_t x = _x;
    if (x._.f == POD) {
        x._.f = ADDR;
        x.mem = &_x.mem;
    }
    size_t n = _bkt_num(x, THIS(this->buckets).size(), this->hash, this->get_key);
    struct Bucket_node **first = THIS(this->buckets).brackets(VA(n));
    FormWO_t t = FORM_WITH_OBJ(this->_t);
    if (t._.f == POD)
        t._.f = ADDR;
    for (struct Bucket_node *cur = *first; cur; cur = cur->next) {
        t.mem = cur->data;
        if (!this->equal(this->get_key(t), this->get_key(x))) {
            void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
            return new(compose(_HashtableIter(), mem), VA(t._, ForwardIter, cur, this));
        }
    }
    return _hashtable_end(_this);
}

static size_t _hashtable_count(void *_this, FormWO_t _x)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    FormWO_t addr_t = _x;
    if (addr_t._.f == POD) {
        addr_t._.f = ADDR;
        addr_t.mem = &_x.mem;
    }
    size_t res = 0;
    size_t n = _bkt_num(addr_t, THIS(this->buckets).size(), this->hash, this->get_key);
    struct Bucket_node **first = THIS(this->buckets).brackets(VA(n));
    FormWO_t t = FORM_WITH_OBJ(this->_t);
    if (t._.f == POD)
        t._.f = ADDR;
    for (struct Bucket_node *cur = *first; cur; cur = cur->next) {
        t.mem = cur->data;
        if (!this->equal(this->get_key(t), this->get_key(addr_t)))
            res++;
    }
    return res;
}

static size_t _hashtable_bucket_count(void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    return THIS(this->buckets).size();
}

static size_t _hashtable_max_bucket_count(void *_this)
{
    return _prime_list[_num_primes - 1];
}

static void _hashtable_resize(void *_this, size_t new_size)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    const size_t old_size = THIS(this->buckets).size();
    if (new_size > old_size) {
        size_t min_bkt = new_size, max_bkt = 0;
        size_t n = _next_prime(new_size);
        Vector tmp = new(T(Vector), VA(T(struct Bucket_node*), n, NULL));
        if (this->nmemb) {
            struct Bucket_node **node = THIS(this->buckets).brackets(VA(this->start));
            struct Bucket_node **end = THIS(this->buckets).brackets(VA(this->finish));
            FormWO_t x = FORM_WITH_OBJ(this->_t);
            while (node != end)
            {
                if (*node) {
                    struct Bucket_node *first = *node;
                    while (first)
                    {
                        x.mem = first->data;
                        size_t new_bucket = _bkt_num(x, n, this->hash, this->get_key);
                        max_bkt = max_bkt > new_bucket ? max_bkt : new_bucket;
                        min_bkt = min_bkt < new_bucket ? min_bkt : new_bucket;
                        *node = first->next;
                        first->next = *(struct Bucket_node **) THIS(tmp).brackets(VA(new_bucket));
                        *(struct Bucket_node **) THIS(tmp).brackets(VA(new_bucket)) = first;
                        first = *node;
                    }
                }
                node++;
            }
            this->start = min_bkt;
            this->finish = max_bkt + 1;
        }
        THIS(this->buckets).swap(tmp);
        delete(tmp);
    }
}

static void _hashtable_copy_from(void *_this, const Hashtable _h)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    struct Hashtable *h = offsetOf(_h, __Hashtable);
    _hashtable_clear(_this);
    delete(this->buckets);
    size_t n = THIS(h->buckets).size();
    this->buckets = new(T(Vector), VA(T(struct Bucket_node*), n, NULL));
    memcpy(this, h, offsetof(struct Hashtable, buckets)); //将成员参数复制过来
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    FormWO_t x = FORM_WITH_OBJ(this->_t);
    if (x._.f == POD)
        x._.f = ADDR;
    struct Bucket_node *tmpNode = _new_node(0);
    for (size_t i = h->start; i != h->finish; i++) {
        struct Bucket_node *cur = *(struct Bucket_node**)THIS(h->buckets).brackets(VA(i));
        if (cur) {
            x.mem = cur->data;
            _insert_aux(&tmpNode, x, this->_t);
            struct Bucket_node *copy = tmpNode;
            tmpNode = copy->next;
            copy->next = NULL;
            *(struct Bucket_node**)THIS(this->buckets).brackets(VA(i)) = copy;
            for (struct Bucket_node *next = cur->next; next; next = next->next) {
                x.mem = next->data;
                _insert_aux(&tmpNode, x, this->_t);
                copy->next = tmpNode;
                tmpNode = tmpNode->next;
                copy->next->next = NULL;
                copy = copy->next;
            }
        }
    }
    _delete_node(tmpNode, 0);
}

static void _hashtable_clear(void *_this)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    for (size_t i = this->start; i != this->finish; i++) {
        struct Bucket_node **node = THIS(this->buckets).brackets(VA(i));
        struct Bucket_node *cur = *node;
        while (cur)
        {
            *node = cur->next;
            if (this->_t.f == OBJ)
                destroy(cur->data);
            _delete_node(cur, memb_size);
            cur = *node;
        }
    }
    this->nmemb = 0;
    this->start = this->finish = 0;
}

static void _hashtable_swap(void *_this, Hashtable _h)
{
    struct Hashtable *this = offsetOf(_this, __Hashtable);
    struct Hashtable *h = offsetOf(_h, __Hashtable);
    THIS(this->buckets).swap(h->buckets);
    struct Hashtable tmp = *this;
    memcpy(this, h, offsetof(struct Hashtable, buckets));
    memcpy(h, &tmp, offsetof(struct Hashtable, buckets));
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->end);
    return class->end(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->empty);
    return class->empty(_this);
}

static Iterator _insert_unique(FormWO_t _x)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->insert_unique);
    return class->insert_unique(_this, _x);
}

static Iterator _insert_equal(FormWO_t _x)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->insert_equal);
    return class->insert_equal(_this, _x);
}

static void _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->erase);
    return class->erase(_this, iter);
}

static Iterator _find(FormWO_t _x)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->find);
    return class->find(_this, _x);
}

static size_t _count(FormWO_t _x)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->count);
    return class->count(_this, _x);
}

static size_t _bucket_count(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->bucket_count);
    return class->bucket_count(_this);
}

static size_t _max_bucket_count(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_this);
}

static void _resize(size_t x)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->resize);
    class->resize(_this, x);
}

static void _copy_from(const Hashtable h)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->copy_from);
    class->copy_from(_this, h);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(Hashtable t)
{
    void *_this = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_this), __HashtableClass);
    assert(class->swap);
    class->swap(_this, t);
}
