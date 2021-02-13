//
// Created by xjs on 2020/9/8.
//

#include "include/_tctl_list.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "include/_tctl_metaclass.h"
#include "include/_tctl_iterator.h"
#include "../include/tctl_int.h"
#include "../include/tctl_uint.h"
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, LIST

struct ListNode {
    struct ListNode *pre;
    struct ListNode *nxt;
    byte data[0];
};
struct ListClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    void* (*front)(const void *_this);
    void* (*back)(const void *_this);
    size_t (*size)(const void *_this);
    bool (*empty)(const void *_this);
    void (*push_back)(void *_this, FormWO_t x);
    void (*push_front)(void *_this, FormWO_t x);
    void (*pop_back)(void *_this);
    void (*pop_front)(void *_this);
    Iterator (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Iterator iter, FormWO_t x);
    void (*remove)(void *_this, FormWO_t x);
    void (*unique)(void *_this);
    void (*splice)(void *_this, Iterator _position, List l, va_list *app);
    void (*merge)(void *_this, List l, Compare cmp);
    void (*reverse)(void *_this);
    void (*sort)(void *_this, Compare cmp);
    void (*clear)(void *_this);
    void (*swap)(void *_this, struct _List *_v);
};

struct List {
    Form_t _t;
    struct ListNode _end;
};

struct ListIter {
    struct ListNode *node;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static void *_front(void);
static void *_back(void);
static size_t _size(void);
static bool _empty(void);
static void _push_back(FormWO_t x);
static void _push_front(FormWO_t x);
static void _pop_back(void);
static void _pop_front(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, FormWO_t x);
static void _remove(FormWO_t x);
static void _unique(void);
static void _splice(Iterator position, List l, ...);
static void _merge(List l, Compare cmp);
static void _reverse(void);
static void _sort(Compare cmp);
static void _clear(void);
static void _swap(List _l);
//listclass
static void *_listclass_ctor(void *_this, va_list *app);
//list
static void *_list_ctor(void *_this, va_list *app);
static void *_list_dtor(void *_this);
static void *_list_brackets(const void *_this, FormWO_t x);
static Iterator _list_begin(const void *_this);
static Iterator _list_end(const void *_this);
static void *_list_front(const void *_this);
static void *_list_back(const void *_this);
static size_t _list_size(const void *_this);
static bool _list_empty(const void *_this);
static void _list_push_back(void *_this, FormWO_t x);
static void _list_push_front(void *_this, FormWO_t x);
static void _list_pop_back(void *_this);
static void _list_pop_front(void *_this);
static Iterator _list_erase(void *_this, Iterator iter);
static Iterator _list_insert(void *_this, Iterator iter, FormWO_t x);
static void _list_remove(void *_this, FormWO_t x);
static void _list_unique(void *_this);
static void _list_splice(void *_this, Iterator _position, List l, va_list *app);
static void _list_merge(void *_this, List l, Compare cmp);
static void _list_reverse(void *_this);
static void _list_sort(void *_this, Compare cmp);
static void _list_clear(void *_this);
static void _list_swap(void *_this, List _l);
//iterator
static void _iter_assign(void *_this, FormWO_t _x);
static void _iter_dec(void *_this);
static void _iter_inc(void *_this);
static bool _iter_equal(const void *_this, FormWO_t _x);
static void *_iter_ctor(void *_this, va_list *app);
static void *_iter_derefer(const void *_this);
//init
static const void *__ListIter = NULL;
static const void *__List = NULL;
static const void *__ListClass = NULL;
volatile static struct ListSelector ListS = {
    {},
    _begin,
    _end,
    _front,
    _back,
    _size,
    _empty,
    _push_back,
    _push_front,
    _pop_back,
    _pop_front,
    _erase,
    _insert,
    _remove,
    _unique,
    _splice,
    _merge,
    _reverse,
    _sort,
    _clear,
    _swap
};
const struct ListSelector *_ListS = NULL;

static void initList(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!_ListS) {
        _ListS = (void*)&ListS;
        memcpy((void*)&ListS, _ClassS, sizeof(*_ClassS));
    }
    if (!__ListIter) {
        __ListIter = new(_IteratorClass(), "ListIter",
                         T(Iterator), sizeof(struct ListIter) + classSz(_Iterator().class),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->dec, _iter_dec,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         Selector, _IteratorS, NULL);
    }
    if (!__ListClass) {
        __ListClass = new(T(MetaClass), "ListClass",
                            T(Class), sizeof(struct ListClass) + classSz(_Class().class),
                            _MetaClassS->ctor, _listclass_ctor, NULL);
    }
    if (!__List) {
        __List = new(_ListClass(), "List",
                       T(Object), sizeof(struct List) + classSz(_Object().class),
                       _MetaClassS->ctor, _list_ctor,
                       _MetaClassS->dtor, _list_dtor,
                       _ClassS->brackets, _list_brackets,
                       ListS.begin, _list_begin,
                       ListS.end, _list_end,
                       ListS.front, _list_front,
                       ListS.back, _list_back,
                       ListS.size, _list_size,
                       ListS.empty, _list_empty,
                       ListS.push_back, _list_push_back,
                       ListS.push_front, _list_push_front,
                       ListS.pop_back, _list_pop_back,
                       ListS.pop_front, _list_pop_front,
                       ListS.erase, _list_erase,
                       ListS.insert, _list_insert,
                       ListS.remove, _list_remove,
                       ListS.unique, _list_unique,
                       ListS.splice, _list_splice,
                       ListS.merge, _list_merge,
                       ListS.reverse, _list_reverse,
                       ListS.sort, _list_sort,
                       ListS.clear, _list_clear,
                       ListS.swap, _list_swap,
                       Selector, _ListS, NULL);
    }
}

Form_t _List(void)
{
    if (!__List)
        initList();
    return (Form_t){OBJ, .class = __List};
}

Form_t _ListClass(void)
{
    if (!__ListClass)
        initList();
    return (Form_t){OBJ, .class = __ListClass};
}

static Form_t _ListIter(void)
{
    if (!__ListIter)
        initList();
    return (Form_t){OBJ, .class = __ListIter};
}
//private
static struct ListNode *_insert_aux(struct List *this, struct ListNode *node, FormWO_t _x)
{
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    struct ListNode *new_node = allocate(sizeof(struct ListNode) + memb_size);
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(new_node->data, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(new_node->data, &_x.mem, memb_size);
        else if (_x._.f == END)
            memset(new_node->data, 0, memb_size);
    } else {
        construct(this->_t, new_node->data, _x, VAEND);
    }
    new_node->nxt = node;
    node->pre->nxt = new_node;
    new_node->pre = node->pre;
    node->pre = new_node;
    return new_node;
}

static struct ListNode *_erase_aux(struct List *this, struct ListNode *node)
{
    assert(node != &this->_end);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (this->_t.f == OBJ)
        destroy(node->data);
    node->pre->nxt = node->nxt;
    node->nxt->pre = node->pre;
    struct ListNode *next_node = node->nxt;
    deallocate(node, memb_size + sizeof(struct ListNode));
    return next_node;
}

static void _transfer(struct ListNode *pos, struct ListNode *first, struct ListNode *last)
{
    if (pos == first)
        return;
    struct ListNode *pos_node = pos;
    struct ListNode *pos_pre_node = pos_node->pre;
    struct ListNode *first_node = first;
    struct ListNode *last_node = last;
    struct ListNode *last_pre_node = last_node->pre;

    first_node->pre->nxt = last_node;
    last_node->pre = first_node->pre;
    pos_pre_node->nxt = first_node;
    last_pre_node->nxt = pos_node;
    pos_node->pre = last_pre_node;
    first_node->pre = pos_pre_node;
}

static void _dealListArgs(void *_this, FormWO_t *args, int n)
{
    if (args->_.class == __List) { //复制一个List
        struct List *L = offsetOf(args->mem, __List);
        struct ListNode *node = L->_end.nxt;
        Form_t t = L->_t;
        while (node != &L->_end)
        {
            void *obj = node->data;
            if (t.f == POD) {
                char (*p)[t.size] = obj;
                _list_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _list_push_back(_this, FORM_WITH_OBJ(t, obj));
            }
            node = node->nxt;
        }
    } else if (args->_.f == POD || args->_.f == ADDR || args->_.class != _Iterator().class) { //size_type n, T value = T() 构造方法
        unsigned long long nmemb = toUInt(*args);
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _list_push_back(_this, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _list_push_back(_this, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(args[1]._.class == _Iterator().class); //因为上面的if已经检测过args[0]
        Iterator first = args[0].mem;
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, VA(first));
        Iterator last = args[1].mem;
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, VA(last));
        Form_t t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            void *obj = THIS(first).derefer();
            if (t.f == POD) {
                char (*p)[t.size] = obj;
                _list_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _list_push_back(_this, FORM_WITH_OBJ(t, obj));
            }
            THIS(first).inc();
        }
        destroy(first);
        destroy(last);
    }
}
//public
//Iterator
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__ListIter, _this, app);
    struct ListIter *this = offsetOf(_this, __ListIter);
    FormWO_t t = va_arg(*app, FormWO_t);
    if (t._.f == OBJ) {
        if (t._.class == _Iterator().class) {
            assert(classOf(t.mem) == __ListIter);
            struct ListIter *it = offsetOf(t.mem, __ListIter);
            *this = *it;
        }
    } else if (t._.f == POD) {
        this->node = t.mem;
    } else {
        this->node = *(struct ListNode**)t.mem;
    }
    return _this;
}

static bool _iter_equal(const void *_this, FormWO_t _x)
{
    const struct ListIter *this = offsetOf(_this, __ListIter);
    const struct ListIter *x = offsetOf(_x.mem, __ListIter);
    return this->node == x->node;
}

static void _iter_inc(void *_this)
{
    struct ListIter *this = offsetOf(_this, __ListIter);
    this->node = this->node->nxt;
}

static void _iter_dec(void *_this)
{
    struct ListIter *this = offsetOf(_this, __ListIter);
    this->node = this->node->pre;
}

static void _iter_assign(void *_this, FormWO_t _x)
{
    struct ListIter *this = offsetOf(_this, __ListIter);
    struct ListIter *x = offsetOf(_x.mem, __ListIter);
    this->node = x->node;
}

static void *_iter_derefer(const void *_this)
{
    struct ListIter *this = offsetOf(_this, __ListIter);
    return this->node->data;
}

//ListClass
static void *_listclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__ListClass, _this, app);
    struct ListClass *this = offsetOf(_this, __ListClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&ListS + sizeof(ListS._);
    voidf *end = (void*)&ListS + sizeof(ListS);
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

//List
static void *_list_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__List, _this, app);
    struct List *this = offsetOf(_this, __List);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f >= FORM);
    t._.f -=FORM;
    this->_t = t._;
    this->_end.nxt = &this->_end;
    this->_end.pre = &this->_end;
    FormWO_t args[2];
    int i = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _dealListArgs(_this, args, i);
    return _this;
}

static void *_list_dtor(void *_this)
{
    _this = super_dtor(__List, _this);
    _list_clear(_this);
    return _this;
}

static void *_list_brackets(const void *_this, FormWO_t x)
{
    struct List *this = offsetOf(_this, __List);
    struct ListNode *node = this->_end.nxt;
    long long n = toInt(x);
    while (n > 0 && node != &this->_end)
    {
        node = node->nxt;
        n--;
    }
    assert(!n);
    return node->data;
}
static Iterator _list_begin(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    void *mem = ARP_MallocARelDtor(classSz(__ListIter), destroy);
    return new(compose(_ListIter(), mem), VA(t, BidirectionalIter, this->_end.nxt));
}

static Iterator _list_end(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    Form_t t = this->_t;
    if (t.f == POD)
        t.f = ADDR;
    void *mem = ARP_MallocARelDtor(classSz(__ListIter), destroy);
    return new(compose(_ListIter(), mem), VA(t, BidirectionalIter, &this->_end));
}

static void *_list_front(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    return this->_end.nxt->data;
}

static void *_list_back(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    return this->_end.pre->data;
}

static size_t _list_size(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    struct ListNode *node = this->_end.nxt;
    size_t size = 0;
    while (node != &this->_end)
    {
        size++;
        node = node->nxt;
    }
    return size;
}

static bool _list_empty(const void *_this)
{
    struct List *this = offsetOf(_this, __List);
    return this->_end.nxt == &this->_end;
}

static void _list_push_back(void *_this, FormWO_t x)
{
    struct List *this = offsetOf(_this, __List);
    _insert_aux(this, &this->_end, x);
}

static void _list_push_front(void *_this, FormWO_t x)
{
    struct List *this = offsetOf(_this, __List);
    _insert_aux(this, this->_end.nxt, x);
}

static void _list_pop_back(void *_this)
{
    struct List *this = offsetOf(_this, __List);
    _erase_aux(this, this->_end.pre);
}

static void _list_pop_front(void *_this)
{
    struct List *this = offsetOf(_this, __List);
    _erase_aux(this, this->_end.nxt);
}

static Iterator _list_erase(void *_this, Iterator iter)
{
    struct List *this = offsetOf(_this, __List);
    assert(classOf(iter) == __ListIter);
    struct ListIter *it = offsetOf(iter, __ListIter);
    it->node = _erase_aux(this, it->node);
    return iter;
}

static Iterator _list_insert(void *_this, Iterator iter, FormWO_t x)
{
    struct List *this = offsetOf(_this, __List);
    assert(classOf(iter) == __ListIter);
    struct ListIter *it = offsetOf(iter, __ListIter);
    it->node = _insert_aux(this, it->node, x);
    return iter;
}

static void _list_remove(void *_this, FormWO_t x)
{
    struct List *this = offsetOf(_this, __List);
    struct ListNode *node = this->_end.nxt;
    if (this->_t.f == POD) {
        size_t memb_size = this->_t.size;
        assert(x._.class != OBJ);
        void *mem = x._.f == POD ? &x.mem : x.mem;
        while (node != &this->_end)
        {
            if (!memcmp(node->data, mem, memb_size)) {
                _erase_aux(this, node);
                break;
            }
        }
    } else {
        while (node != &this->_end)
        {
            Object obj = (Object)node->data;
            if (THIS(obj).equal(x)) {
                _erase_aux(this, node);
                break;
            }
            node = node->nxt;
        }
    }
}

static void _list_unique(void *_this)
{
    struct List *this = offsetOf(_this, __List);
    struct ListNode *node = this->_end.nxt;
    struct ListNode *next_node = node->nxt;
    if (this->_t.f == POD) {
        size_t memb_size = this->_t.size;
        while (next_node != &this->_end)
        {
            if (!memcmp(next_node->data, node->data, memb_size))
                _erase_aux(this, next_node);
            else
                node = next_node;
            next_node = next_node->nxt;
        }
    } else {
        FormWO_t x = FORM_WITH_OBJ(this->_t);
        while (next_node != &this->_end)
        {
            Object obj = (Object)node->data;
            x.mem = next_node->data;
            if (THIS(obj).equal(x))
                _erase_aux(this, next_node);
            else
                node = next_node;
            next_node = next_node->nxt;
        }
    }
}

static void _list_splice(void *_this, Iterator _position, List l, va_list *app)
{
    struct List *this = offsetOf(_this, __List);
    struct List *L = offsetOf(l, __List);
    assert(L->_t.f == this->_t.f && L->_t.class == this->_t.class);
    FormWO_t t;
    FormWO_t args[2];
    int n = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(n < 2);
        args[n++] = t;
    }
    assert(classOf(_position) == __ListIter);
    struct ListNode *pos_node = ((struct ListIter*)offsetOf(_position, __ListIter))->node;
    struct ListNode *first_node = NULL;
    struct ListNode *last_node = NULL;
    if (n == 0) {
        if (_list_empty(L))
            return;
        first_node = L->_end.nxt;
        last_node = &L->_end;
    } else if (n == 1) {
        assert(args[0]._.f == OBJ);
        first_node = ((struct ListIter*)offsetOf(args[0].mem, __ListIter))->node;
        last_node = first_node->nxt;
        if (pos_node == first_node || pos_node == last_node)
            return;
    } else {
        assert(args[0]._.f == OBJ);
        assert(args[1]._.f == OBJ);
        first_node = ((struct ListIter*)offsetOf(args[0].mem, __ListIter))->node;
        last_node = ((struct ListIter*)offsetOf(args[1].mem, __ListIter))->node;
        if (first_node == last_node)
            return;
    }
    _transfer(pos_node, first_node, last_node);
}

static void _list_merge(void *_this, List l, Compare cmp)
{
    struct List *this = offsetOf(_this, __List);
    struct List *L = offsetOf(l, __List);
    assert(L->_t.f == this->_t.f && L->_t.class == this->_t.class);
    struct ListNode *first1 = this->_end.nxt;
    struct ListNode *last1 = &this->_end;
    struct ListNode *first2 = L->_end.nxt;
    struct ListNode *last2 = &L->_end;
    Form_t t1 = this->_t;
    if (t1.f == POD)
        t1.f = ADDR;
    Form_t t2 = L->_t;
    if (t2.f == POD)
        t2.f = ADDR;
    while (first1 != last1 && first2 != last2)
    {
        FormWO_t v1 = FORM_WITH_OBJ(t1, first1->data);
        FormWO_t v2 = FORM_WITH_OBJ(t2, first2->data);
        if (cmp(v1, v2) > 0) {
            struct ListNode *next = first2->nxt;
            _transfer(first1, first2, next);
            first2 = next;
        } else {
            first1 = first1->nxt;
        }
    }
    if (first2 != last2)
        _transfer(last1, first2, last2);
}
static void _list_reverse(void *_this)
{
    struct List *this = offsetOf(_this, __List);
    if (this->_end.nxt == &this->_end || this->_end.nxt->nxt == &this->_end)
        return;
    struct ListNode *first = this->_end.nxt;
    while (first != &this->_end)
    {
        struct ListNode *next = first->nxt;
        _transfer(this->_end.nxt, first, next);
        first = next;
    }
}

static void _list_sort(void *_this, Compare cmp)
{
    struct List *this = offsetOf(_this, __List);
    if (this->_end.nxt == &this->_end || this->_end.nxt->nxt == &this->_end)
        return;
    List _carry = new(T(List), VA(this->_t));
    struct List *carry = offsetOf(_carry, __List);
    List counter[64];
    int fill = 0;
    while (this->_end.nxt != &this->_end)
    {
        struct ListNode *carry_first = carry->_end.nxt;
        struct ListNode *this_first = this->_end.nxt;
        _transfer(carry_first, this_first, this_first->nxt);
        int i = 0;
        while (i < fill  && !THIS(counter[i]).empty())
        {
            THIS(counter[i]).merge(_carry, cmp);
            THIS(_carry).swap(counter[i++]);
        }
        if (i == fill)
            counter[fill++] = new(T(List), VA(this->_t));
        THIS(counter[i]).swap(_carry);
    }
    for (int i = 1; i < fill; i++) {
        THIS(counter[i]).merge(counter[i - 1], cmp);
        delete(counter[i - 1]);
    }
    _list_swap(_this, counter[fill - 1]);
    delete(counter[fill - 1]);
    delete(_carry);
}

static void _list_clear(void *_this)
{
    struct List *this = offsetOf(_this, __List);
    while (this->_end.nxt != &this->_end)
        _list_pop_back(_this);
}

static void _list_swap(void *_this, List _l)
{
    struct List *this = offsetOf(_this, __List);
    struct List *L = offsetOf(_l, __List);
    assert(L->_t.f == this->_t.f && L->_t.class == this->_t.class);
    if (this->_end.nxt == &this->_end && L->_end.nxt == &L->_end) //都是空的
        return;
    else if (this->_end.nxt == &this->_end) { //this是空的
        this->_end = L->_end;
        L->_end.pre->nxt = &this->_end;
        L->_end.nxt->pre = &this->_end;
        L->_end.nxt = &L->_end;
        L->_end.pre = &L->_end;
    } else if (L->_end.nxt == &L->_end) { //L是空的
        L->_end = this->_end;
        this->_end.pre->nxt = &L->_end;
        this->_end.nxt->pre = &L->_end;
        this->_end.nxt = &this->_end;
        this->_end.pre = &this->_end;
    } else { //都不是空的
        this->_end.pre->nxt = &L->_end;
        this->_end.nxt->pre = &L->_end;
        L->_end.pre->nxt = &this->_end;
        L->_end.nxt->pre = &this->_end;
        struct ListNode tmp = this->_end;
        this->_end = L->_end;
        L->_end = tmp;
    }
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->end);
    return class->end(_this);
}

static void *_front(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->front);
    return class->front(_this);
}

static void *_back(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->back);
    return class->back(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->size);
    return class->size(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _push_back(FormWO_t x)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->push_back);
    class->push_back(_this, x);
}

static void _push_front(FormWO_t x)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->push_front);
    class->push_front(_this, x);
}

static void _pop_back(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->pop_back);
    class->pop_back(_this);
}

static void _pop_front(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->pop_front);
    class->pop_front(_this);
}

static Iterator _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->erase);
    return class->erase(_this, iter);
}

static Iterator _insert(Iterator iter, FormWO_t x)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->insert);
    return class->insert(_this, iter, x);
}

static void _remove(FormWO_t x)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->remove);
    class->remove(_this, x);
}

static void _unique(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->unique);
    class->unique(_this);
}

static void _splice(Iterator position, List l, ...)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->splice);
    va_list ap;
    va_start(ap, l);
    class->splice(_this, position, l, &ap);
    va_end(ap);
}

static void _merge(List l, Compare cmp)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->merge);
    class->merge(_this, l, cmp);
}

static void _reverse(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->reverse);
    class->reverse(_this);
}

static void _sort(Compare cmp)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->sort);
    class->sort(_this, cmp);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->clear);
    class->clear(_this);
}

static void _swap(List _l)
{
    void *_this = pop_this();
    const struct ListClass *class = offsetOf(classOf(_this), __ListClass);
    assert(class->swap);
    class->swap(_this, _l);
}
