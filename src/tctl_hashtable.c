//
// Created by xjs on 2020/12/14.
//

#include "../include/auto_release_pool.h"
#include "../include/tctl_allocator.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"
#include "include/_tctl_hashtable.h"
#include "include/_tctl_iterator.h"
#include <stdarg.h>
#include <memory.h>

#define Import CLASS, OBJECT, METACLASS, METAOBJECT, ITERATOR, HASHTABLE, VECTOR, ANY

#define DATA(node) ((void*)((char*)&(node) + sizeof(node)))

struct Bucket_node {
    struct Bucket_node *next;
};

struct HashtableClass {
    Iterator (*begin)(void *_self);
    Iterator (*end)(void *_self);
    size_t (*size)(void *_self);
    bool (*empty)(void *_self);
    Iterator (*insert_unique)(void *_self, const void *_x);
    Iterator (*insert_equal)(void *_self, const void *_x);
    void (*erase)(void *_self, Iterator iter);
    Iterator (*find)(void *_self, const void *_x);
    size_t (*count)(void *_self, const void *_x);
    size_t (*bucket_count)(void *_self);
    size_t (*max_bucket_count)(void *_self);
    void (*resize)(void *_self, size_t n);
    void (*copy_from)(void *_self, Hashtable _ht);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Hashtable _ht);
};

struct Hashtable{
    void *class;
    size_t nmemb;
    Compare equal;
    HashFunc hash;
    ExtractKey get_key;
    size_t start;
    size_t finish;
    //上面成员会在copy函数中复制，所以要和下面成员分开
    Vector buckets;
};

struct HashNode {
    struct Bucket_node *node;
};

typedef struct {
    METAOBJECT_HEAD(METACLASS_FUNC);
    struct Bucket_node *node;
} *HashNode;

struct HashtableIter{
    struct Bucket_node *cur;
    struct Hashtable *ht;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static size_t _size(void);
static bool _empty(void);
static Iterator _insert_unique(const void *_x);
static Iterator _insert_equal(const void *_x);
static void _erase(Iterator);
static Iterator _find(const void *_x);
static size_t _count(const void *_x);
static size_t _bucket_count(void);
static size_t _max_bucket_count(void);
static void _resize(size_t);
static void _copy_from(Hashtable _ht);
static void _clear(void);
static void _swap(Hashtable ht);
//Hashtableclass
static void *_hashtableclass_ctor(void *_self, va_list *app);
//Hashtable
static void *_hashtable_ctor(void *_self, va_list *app);
static void *_hashtable_dtor(void *_self);
static Iterator _hashtable_begin(const void *_self);
static Iterator _hashtable_end(const void *_self);
static size_t _hashtable_size(const void *_self);
static bool _hashtable_empty(const void *_self);
static Iterator _hashtable_insert_unique(void *_self, const void *_x);
static Iterator _hashtable_insert_equal(void *_self, const void *_x);
static void _hashtable_erase(void *_self, Iterator iter);
static Iterator _hashtable_find(void *_self, const void *_x);
static size_t _hashtable_count(void *_self, const void *_x);
static size_t _hashtable_bucket_count(void *_self);
static size_t _hashtable_max_bucket_count(void *_self);
static void _hashtable_resize(void *_self, size_t n);
static void _hashtable_copy_from(void *_self, Hashtable _ht);
static void _hashtable_clear(void *_self);
static void _hashtable_swap(void *_self, Hashtable _h);
//HashNode
static void *_hashnode_ctor(void *_self, va_list *app);
//iterator
static void _iter_assign(void *_self, const void *_x);
static void _iter_inc(void *_self);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
//init
static const void *__HashtableIter = NULL;
static const void *__Hashtable = NULL;
static const void *__HashtableClass = NULL;
static const void *__HashNode = NULL;

volatile static struct HashtableSelector HashtableS = {
    {0},
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
    if (!__HashNode) {
        __HashNode = new(T(MetaClass), "HashNode",
                         T(MetaObject), sizeof(struct HashNode) + classSz(T(MetaObject)),
                         _MetaClassS->ctor, _hashnode_ctor, NULL);
    }
    if (!__HashtableIter) {
        __HashtableIter = new(_IteratorClass(), "HashtableIter",
                         T(Iterator), sizeof(struct HashtableIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         Selector, _IteratorS, NULL);
    }
    if (!__HashtableClass) {
        __HashtableClass = new(T(MetaClass), "HashtableClass",
                           T(Class), sizeof(struct HashtableClass) + classSz(T(Class)),
                           _MetaClassS->ctor, _hashtableclass_ctor, NULL);
    }
    if (!__Hashtable) {
        __Hashtable = new(_HashtableClass(), "Hashtable",
                       T(Object), sizeof(struct Hashtable) + classSz(T(Object)),
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

const void *_Hashtable(void)
{
    if (!__Hashtable)
        initHashtable();
    return __Hashtable;
}

const void *_HashtableClass(void)
{
    if (!__HashtableClass)
        initHashtable();
    return  __HashtableClass;
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
    size_t len = _num_primes, half;
    const unsigned long *middle;
    const unsigned long *first = _prime_list;
    while (len > 0)
    {
        half = len >> 1;
        middle = first + half;
        if (*middle < n) {
            first = middle + 1;
            len = len - half - 1;
        } else
            len = half;
    }
    return *first;
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

static void _insert_aux(struct Bucket_node **pos, const void *x, const void *class)
{
    size_t memb_size = classSz(class);
    struct Bucket_node *node = _new_node(memb_size);
    construct(class, DATA(*node), x, VAEND);
    node->next = *pos;
    *pos = node;
}

static size_t _bkt_num(const void *data, size_t n, HashFunc hash, ExtractKey get_key)
{
    return hash(get_key(data)) % n;
}

//public
//HashNode
static void *_hashnode_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__HashNode, _self, app);
    struct HashNode *self = offsetOf(_self, __HashNode);
    void *t = va_arg(*app, void*);
    assert(t != VAEND);
    if (classOf(t) == __HashNode) {
        struct HashNode *node = offsetOf(t, __HashNode);
        self->node = node->node;
    } else if (classOf(t) == T(Any)) {
        Any any = t;
        self->node = *(void**)THIS(any).value();
    }
    assert(va_arg(*app, void*) == VAEND);
    return _self;
}

//HashIter
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__HashtableIter, _self, app);
    struct HashtableIter *self = offsetOf(_self, __HashtableIter);
    void *t = va_arg(*app, void*);
    if (t == VAEND) {
        self->cur = NULL;
        self->ht = NULL;
    } else if (classOf(t) == __HashtableIter) {
        struct HashtableIter *it = offsetOf(t, __HashtableIter);
        *self = *it;
        assert(va_arg(*app, void*) == VAEND);
    } else {
        assert(classOf(t) == __HashNode);
        self->cur = ((HashNode)t)->node;
        t = va_arg(*app, void*);
        assert(classOf(t) == __Hashtable);
        self->ht = offsetOf(t, __Hashtable);
        assert(va_arg(*app, void*) == VAEND);
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    const struct HashtableIter *self = offsetOf(_self, __HashtableIter);
    const struct HashtableIter *x = offsetOf(_x, __HashtableIter);
    return self->cur == x->cur;
}

static void _iter_inc(void *_self)
{
    struct HashtableIter *self = offsetOf(_self, __HashtableIter);
    struct Hashtable *ht = self->ht;
    if (!self->cur)
        return;
    if (!self->cur->next) {
        void *t = DATA(*self->cur);
        size_t n = _bkt_num(t, THIS(ht->buckets).size(), ht->hash, ht->get_key);
        char *it = THIS(ht->buckets).brackets(VA(n + 1));
        char *end = THIS(ht->buckets).brackets(VA(ht->finish));
        size_t memb_size = classSz(__HashNode);
        struct Bucket_node *node;
        while (it != end)
        {
            node = ((HashNode)it)->node;
            if (node)
                break;
            it += memb_size;
        }
        if (it != end)
            self->cur = node;
        else
            self->cur = NULL; //发现为end的时候都置为NULL
    } else {
        self->cur = self->cur->next;
    }
}

static void _iter_assign(void *_self, const void *_x)
{
    struct HashtableIter *self = offsetOf(_self, __HashtableIter);
    struct HashtableIter *it = offsetOf(_x, __HashtableIter);
    *self = *it;
}

static void *_iter_derefer(const void *_self)
{
    struct HashtableIter *self = offsetOf(_self, __HashtableIter);
    return DATA(*self->cur);
}

//HashtableClass
static void *_hashtableclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__HashtableClass, _self, app);
    struct HashtableClass *self = offsetOf(_self, __HashtableClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&HashtableS + sizeof(HashtableS._));
    voidf *end = (voidf*)((char*)&HashtableS + sizeof(HashtableS));
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

//Hashtable
static void *_hashtable_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Hashtable, _self, app);
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    //初始化成员
    self->buckets = new(T(Vector), __HashNode, VAEND);
    self->nmemb = 0;
    self->start = self->finish = 0;
    void *t = va_arg(*app, void*);
    self->class = t;
    t = va_arg(*app, void*);
    if (classOf(t) == __Hashtable) { //处理复制构造
        _hashtable_copy_from(_self, t);
        return _self;
    }
    //equal函数
    Any any = t;
    assert(classOf(t) == T(Any));
    assert(THIS(any).type() == FUNC);
    self->equal = (Compare)THIS(any).value();
    assert(self->equal);

    t = va_arg(*app, void*); //hash函数
    any = t;
    assert(classOf(t) == T(Any));
    assert(THIS(any).type() == FUNC);
    self->hash = (HashFunc)THIS(any).value();
    assert(self->hash);

    t = va_arg(*app, void*); //get_key函数
    any = t;
    assert(classOf(t) == T(Any));
    assert(THIS(any).type() == FUNC);
    self->get_key = (ExtractKey)THIS(any).value();
    assert(self->get_key);

    return _self;
}

static void *_hashtable_dtor(void *_self)
{
    _self = super_dtor(__Hashtable, _self);
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    _hashtable_clear(_self);
    delete(self->buckets);
    return _self;
}

static Iterator _hashtable_begin(const void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    HashNode node = THIS(self->buckets).brackets(VA(self->start));
    return construct(__HashtableIter, mem, VA(ForwardIter), self->class, node, _self, VAEND);
}

static Iterator _hashtable_end(const void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    if (self->finish == THIS(self->buckets).size()) {
        void *ret = construct(__HashtableIter, mem, VA(ForwardIter), self->class, VAEND);
        struct HashtableIter *h_iter = offsetOf(ret, __HashtableIter);
        h_iter->ht = self;
        return ret;
    }
    HashNode node = THIS(self->buckets).brackets(VA(self->finish));
    return construct(__HashtableIter, mem, VA(ForwardIter), self->class, node, _self, VAEND);
}

static size_t _hashtable_size(const void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    return self->nmemb;
}

static bool _hashtable_empty(const void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    return self->nmemb;
}

static Iterator _hashtable_insert_unique(void *_self, const void *_x)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    _hashtable_resize(_self, self->nmemb + 1);
    const size_t n = _bkt_num(_x, THIS(self->buckets).size(), self->hash, self->get_key);
    HashNode node = THIS(self->buckets).brackets(VA(n));
    struct Bucket_node **first = &node->node;
    for (struct Bucket_node *cur = *first; cur; cur = cur->next) {
        void *t = DATA(*cur);
        if (!self->equal(self->get_key(t), self->get_key(_x)))
            return _hashtable_end(_self);
    }
    _insert_aux(first, _x, self->class);
    if (n < self->start || !self->nmemb)
        self->start = n;
    if (n + 1 > self->finish || !self->nmemb)
        self->finish = n + 1;
    self->nmemb++;

    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    return construct(__HashtableIter, mem, VA(ForwardIter), self->class, node, _self, VAEND);
}

static Iterator _hashtable_insert_equal(void *_self, const void *_x)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    _hashtable_resize(_self, self->nmemb + 1);
    const size_t n = _bkt_num(_x, THIS(self->buckets).size(), self->hash, self->get_key);
    HashNode node = THIS(self->buckets).brackets(VA(n));
    struct Bucket_node **first = &node->node;
    _insert_aux(first, _x, self->class);
    if (n < self->start || !self->nmemb)
        self->start = n;
    if (n + 1 > self->finish || !self->nmemb)
        self->finish = n + 1;
    self->nmemb++;

    void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
    return construct(__HashtableIter, mem, VA(ForwardIter), self->class, node, _self, VAEND);
}

static void _hashtable_erase(void *_self, Iterator iter)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    struct HashtableIter *it = offsetOf(iter, __HashtableIter);
    size_t memb_size = classSz(self->class);
    if (it->ht != self)
        return;
    struct Bucket_node *cur = it->cur;
    _iter_inc(iter); //迭代器向后走一个
    void *x = DATA(*cur);
    const size_t n = _bkt_num(x, THIS(self->buckets).size(), self->hash, self->get_key);
    HashNode node = THIS(self->buckets).brackets(VA(n));
    struct Bucket_node **p = &node->node;
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
    destroy(DATA(*cur));
    _delete_node(cur, memb_size);
    self->nmemb--;
    //判断是否需要更新start和finish
    if (!self->nmemb) {
        self->start = self->finish = 0;
        return;
    }
    if (n == self->start) {
        unsigned i;
        for (i = n + 1; !((HashNode)THIS(self->buckets).brackets(VA(i)))->node; i++);
        self->start = i;
    } else if (n == self->finish - 1) {
        unsigned i;
        for (i = n - 1; !((HashNode)THIS(self->buckets).brackets(VA(i)))->node; i--);
        self->finish = i + 1;
    }
}

static Iterator _hashtable_find(void *_self, const void *_x)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    size_t n = _bkt_num(_x, THIS(self->buckets).size(), self->hash, self->get_key);
    HashNode node = THIS(self->buckets).brackets(VA(n));
    struct Bucket_node *first = node->node;
    for (struct Bucket_node *cur = first; cur; cur = cur->next) {
        void *t= DATA(*cur);
        if (!self->equal(self->get_key(t), self->get_key(_x))) {
            void *mem = ARP_MallocARelDtor(classSz(__HashtableIter), destroy);
            return construct(__HashtableIter, mem, VA(ForwardIter), self->class, node, _self, VAEND);
        }
    }
    return _hashtable_end(_self);
}

static size_t _hashtable_count(void *_self, const void *_x)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    size_t res = 0;
    size_t n = _bkt_num(_x, THIS(self->buckets).size(), self->hash, self->get_key);
    HashNode node = THIS(self->buckets).brackets(VA(n));
    struct Bucket_node *first = node->node;
    for (struct Bucket_node *cur = first; cur; cur = cur->next) {
        void *t = DATA(*cur);
        if (!self->equal(self->get_key(t), self->get_key(_x)))
            res++;
    }
    return res;
}

static size_t _hashtable_bucket_count(void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    return THIS(self->buckets).size();
}

static size_t _hashtable_max_bucket_count(void *_self)
{
    return _prime_list[_num_primes - 1];
}

static void _hashtable_resize(void *_self, size_t new_size)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    const size_t old_size = THIS(self->buckets).size();
    if (new_size > old_size) {
        size_t min_bkt = new_size, max_bkt = 0;
        size_t n = _next_prime(new_size);
        Any any = VA(TEMP_VAR(void*, NULL), ANYONE);
        Vector tmp = new(T(Vector), __HashNode, VA(n), any, VAEND);
        if (self->nmemb) {
            char *it = THIS(self->buckets).brackets(VA(self->start));
            char *end = THIS(self->buckets).brackets(VA(self->finish));
            size_t memb_size = classSz(__HashNode);
            struct Bucket_node *node;
            while (it != end)
            {
                node = ((HashNode)it)->node;
                if (node) {
                    struct Bucket_node *first = node;
                    while (first)
                    {
                        void *x= DATA(*first);
                        size_t new_bucket = _bkt_num(x, n, self->hash, self->get_key);
                        max_bkt = max_bkt > new_bucket ? max_bkt : new_bucket;
                        min_bkt = min_bkt < new_bucket ? min_bkt : new_bucket;
                        node = first->next;
                        first->next = ((HashNode)THIS(tmp).brackets(VA(new_bucket)))->node;
                        ((HashNode)THIS(tmp).brackets(VA(new_bucket)))->node = first;
                        first = node;
                    }
                }
                it += memb_size;
            }
            self->start = min_bkt;
            self->finish = max_bkt + 1;
        }
        THIS(self->buckets).swap(tmp);
        delete(tmp);
    }
}

static void _hashtable_copy_from(void *_self, Hashtable _ht)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    struct Hashtable *h = offsetOf(_ht, __Hashtable);
    _hashtable_clear(_self);
    delete(self->buckets);
    size_t n = THIS(h->buckets).size();
    Any any = VA(TEMP_VAR(void*, NULL), ANYONE);
    self->buckets = new(T(Vector), __HashNode, VA(n), any, VAEND);
    memcpy(self, h, offsetof(struct Hashtable, buckets)); //将成员参数复制过来
    struct Bucket_node *tmpNode = _new_node(0);
    for (size_t i = h->start; i != h->finish; i++) {
        struct Bucket_node *cur = ((HashNode)THIS(h->buckets).brackets(VA(i)))->node;
        if (cur) {
            void *x= DATA(*cur);
            _insert_aux(&tmpNode, x, self->class);
            struct Bucket_node *copy = tmpNode;
            tmpNode = copy->next;
            copy->next = NULL;
            ((HashNode)THIS(self->buckets).brackets(VA(i)))->node = copy;
            for (struct Bucket_node *next = cur->next; next; next = next->next) {
                x = DATA(*next);
                _insert_aux(&tmpNode, x, self->class);
                copy->next = tmpNode;
                tmpNode = tmpNode->next;
                copy->next->next = NULL;
                copy = copy->next;
            }
        }
    }
    _delete_node(tmpNode, 0);
}

static void _hashtable_clear(void *_self)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    size_t memb_size = classSz(self->class);
    for (size_t i = self->start; i != self->finish; i++) {
        struct Bucket_node *node = ((HashNode)THIS(self->buckets).brackets(VA(i)))->node;
        struct Bucket_node *cur = node;
        while (cur)
        {
            node = cur->next;
            destroy(DATA(*cur));
            _delete_node(cur, memb_size);
            cur = node;
        }
    }
    self->nmemb = 0;
    self->start = self->finish = 0;
}

static void _hashtable_swap(void *_self, Hashtable _h)
{
    struct Hashtable *self = offsetOf(_self, __Hashtable);
    struct Hashtable *h = offsetOf(_h, __Hashtable);
    THIS(self->buckets).swap(h->buckets);
    struct Hashtable tmp = *self;
    memcpy(self, h, offsetof(struct Hashtable, buckets));
    memcpy(h, &tmp, offsetof(struct Hashtable, buckets));
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->end);
    return class->end(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->empty);
    return class->empty(_self);
}

static Iterator _insert_unique(const void *_x)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->insert_unique);
    return class->insert_unique(_self, _x);
}

static Iterator _insert_equal(const void *_x)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->insert_equal);
    return class->insert_equal(_self, _x);
}

static void _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->erase);
    class->erase(_self, iter);
}

static Iterator _find(const void *_x)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->find);
    return class->find(_self, _x);
}

static size_t _count(const void *_x)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->count);
    return class->count(_self, _x);
}

static size_t _bucket_count(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->bucket_count);
    return class->bucket_count(_self);
}

static size_t _max_bucket_count(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->max_bucket_count);
    return class->max_bucket_count(_self);
}

static void _resize(size_t x)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->resize);
    class->resize(_self, x);
}

static void _copy_from(Hashtable _ht)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->copy_from);
    class->copy_from(_self, _ht);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(Hashtable t)
{
    void *_self = pop_this();
    const struct HashtableClass *class = offsetOf(classOf(_self), __HashtableClass);
    assert(class->swap);
    class->swap(_self, t);
}
