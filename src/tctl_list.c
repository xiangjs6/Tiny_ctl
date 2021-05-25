//
// Created by xjs on 2020/9/8.
//

#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_def.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include "include/_tctl_list.h"
#include "include/_tctl_iterator.h"
#include <assert.h>
#include <stdarg.h>
#include <string.h>

#define Import CLASS, OBJECT, METACLASS, ITERATOR, LIST, ANY, INT

#define DATA(node) ((void*)((char*)&(node) + sizeof(node)))

struct ListNode {
    struct ListNode *pre;
    struct ListNode *nxt;
};
struct ListClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    void* (*front)(const void *_self);
    void* (*back)(const void *_self);
    size_t (*size)(const void *_self);
    bool (*empty)(const void *_self);
    void (*push_back)(void *_self, const void *x);
    void (*push_front)(void *_self, const void *x);
    void (*pop_back)(void *_self);
    void (*pop_front)(void *_self);
    Iterator (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Iterator iter, const void *x);
    void (*remove)(void *_self, const void *x);
    void (*unique)(void *_self);
    void (*splice)(void *_self, Iterator _position, List l, va_list *app);
    void (*merge)(void *_self, List l, Compare cmp);
    void (*reverse)(void *_self);
    void (*sort)(void *_self, Compare cmp);
    void (*clear)(void *_self);
    void (*swap)(void *_self, struct _List *_v);
};

struct List {
    void *class;
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
static void _push_back(const void *x);
static void _push_front(const void *x);
static void _pop_back(void);
static void _pop_front(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, const void *x);
static void _remove(const void *x);
static void _unique(void);
static void _splice(Iterator position, List l, ...);
static void _merge(List l, Compare cmp);
static void _reverse(void);
static void _sort(Compare cmp);
static void _clear(void);
static void _swap(List _l);
//listclass
static void *_listclass_ctor(void *_self, va_list *app);
//list
static void *_list_ctor(void *_self, va_list *app);
static void *_list_dtor(void *_self);
static void *_list_brackets(const void *_self, const void *x);
static Iterator _list_begin(const void *_self);
static Iterator _list_end(const void *_self);
static void *_list_front(const void *_self);
static void *_list_back(const void *_self);
static size_t _list_size(const void *_self);
static bool _list_empty(const void *_self);
static void _list_push_back(void *_self, const void *x);
static void _list_push_front(void *_self, const void *x);
static void _list_pop_back(void *_self);
static void _list_pop_front(void *_self);
static Iterator _list_erase(void *_self, Iterator iter);
static Iterator _list_insert(void *_self, Iterator iter, const void *x);
static void _list_remove(void *_self, const void *x);
static void _list_unique(void *_self);
static void _list_splice(void *_self, Iterator _position, List l, va_list *app);
static void _list_merge(void *_self, List l, Compare cmp);
static void _list_reverse(void *_self);
static void _list_sort(void *_self, Compare cmp);
static void _list_clear(void *_self);
static void _list_swap(void *_self, List _l);
//iterator
static void _iter_assign(void *_self, const void *_x);
static void _iter_dec(void *_self);
static void _iter_inc(void *_self);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
static Iterator _iter_reverse_iterator(void *_self);
//init
static const void *__ListIter = NULL;
static const void *__RListIter = NULL;
static const void *__List = NULL;
static const void *__ListClass = NULL;
volatile static struct ListSelector ListS = {
    {0},
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
                         T(Iterator), sizeof(struct ListIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_inc,
                         _ClassS->dec, _iter_dec,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         _IteratorS->reverse_iterator, _iter_reverse_iterator,
                         Selector, _IteratorS, NULL);
    }
    if (!__RListIter) {
        __RListIter = new(_IteratorClass(), "RListIter",
                         T(Iterator), sizeof(struct ListIter) + classSz(T(Iterator)),
                         _MetaClassS->ctor, _iter_ctor,
                         _ClassS->equal, _iter_equal,
                         _ClassS->inc, _iter_dec,
                         _ClassS->dec, _iter_inc,
                         _ClassS->assign, _iter_assign,
                         _IteratorS->derefer, _iter_derefer,
                         _IteratorS->reverse_iterator, _iter_reverse_iterator,
                         Selector, _IteratorS, NULL);
    }
    if (!__ListClass) {
        __ListClass = new(T(MetaClass), "ListClass",
                            T(Class), sizeof(struct ListClass) + classSz(T(Class)),
                            _MetaClassS->ctor, _listclass_ctor, NULL);
    }
    if (!__List) {
        __List = new(_ListClass(), "List",
                       T(Object), sizeof(struct List) + classSz(T(Object)),
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

const void *_List(void)
{
    if (!__List)
        initList();
    return __List;
}

const void *_ListClass(void)
{
    if (!__ListClass)
        initList();
    return __ListClass;
}

static const void *_ListIter(void)
{
    if (!__ListIter)
        initList();
    return __ListIter;
}

static const void *_RListIter(void)
{
    if (!__RListIter)
        initList();
    return __RListIter;
}

//private
static struct ListNode *_insert_aux(struct List *self, struct ListNode *node, const void *_x)
{
    size_t memb_size = classSz(self->class);
    struct ListNode *new_node = allocate(sizeof(struct ListNode) + memb_size);
    construct(self->class, DATA(*new_node), _x, VAEND);
    new_node->nxt = node;
    node->pre->nxt = new_node;
    new_node->pre = node->pre;
    node->pre = new_node;
    return new_node;
}

static struct ListNode *_erase_aux(struct List *self, struct ListNode *node)
{
    assert(node != &self->_end);
    size_t memb_size = classSz(self->class);
    destroy(DATA(*node));
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

static void _deal_List_Args(void *_self, MetaObject *args, int n)
{
    if (classOf(*args) == __List) { //复制一个List
        struct List *L = offsetOf(*args, __List);
        struct ListNode *node = L->_end.nxt;
        while (node != &L->_end)
        {
            void *obj = DATA(*node);
            _list_push_back(_self, obj);
            node = node->nxt;
        }
    } else if (classOf(*args) == T(Any)) { //size_type n, T value = T() 构造方法
        Int ii = THIS(*args).cast(T(Int));
        unsigned long long nmemb = ii->val;
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _list_push_back(_self, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _list_push_back(_self, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(class_check(args[0], T(Iterator)));
        assert(class_check(args[1], T(Iterator)));
        Iterator first = (Iterator)args[0];
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, first, VAEND);
        Iterator last = (Iterator)args[1];
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, last, VAEND);
        while (!THIS(first).equal(last))
        {
            void *obj = THIS(first).derefer();
            _list_push_back(_self, obj);
            THIS(first).inc();
        }
        destroy(first);
        destroy(last);
    }
}
//public
//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__ListIter, _self, app);
    struct ListIter *self = offsetOf(_self, __ListIter);
    MetaObject t = va_arg(*app, MetaObject);
    if (classOf(t) == __ListIter || classOf(t) == __RListIter) {
        struct ListIter *it;
        if (classOf(t) == __ListIter)
            it = offsetOf(t, __ListIter);
        else
            it = offsetOf(t, __RListIter);
        *self = *it;
    } else if (classOf(t) == T(Any)) {
        Any any = (Any)t;
        assert(THIS(any).size() == sizeof(void*));
        memcpy(&self->node, THIS(any).value(), sizeof(void*));
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    assert(classOf(_self) == classOf(_x));
    const struct ListIter *self = offsetOf(_self, __ListIter);
    const struct ListIter *x = offsetOf(_x, __ListIter);
    return self->node == x->node;
}

static void _iter_inc(void *_self)
{
    struct ListIter *self = offsetOf(_self, __ListIter);
    self->node = self->node->nxt;
}

static void _iter_dec(void *_self)
{
    struct ListIter *self = offsetOf(_self, __ListIter);
    self->node = self->node->pre;
}

static void _iter_assign(void *_self, const void *_x)
{
    assert(classOf(_self) == classOf(_x));
    struct ListIter *self = offsetOf(_self, __ListIter);
    struct ListIter *x = offsetOf(_x, __ListIter);
    self->node = x->node;
}

static Iterator _iter_reverse_iterator(void *_self)
{
    Iterator it = (void*)_self;
    if (classOf(_self) == __ListIter) {
        void *mem = ARP_MallocARelDtor(classSz(__ListIter), destroy);
        return construct(_RListIter(), mem, VA(it), VAEND);
    } else {
        void *mem = ARP_MallocARelDtor(classSz(__RListIter), destroy);
        return construct(_ListIter(), mem, VA(it), VAEND);
    }
}

static void *_iter_derefer(const void *_self)
{
    struct ListIter *self = offsetOf(_self, __ListIter);
    return DATA(*self->node);
}

//ListClass
static void *_listclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__ListClass, _self, app);
    struct ListClass *self = offsetOf(_self, __ListClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (voidf*)((char*)&ListS + sizeof(ListS._));
    voidf *end = (voidf*)((char*)&ListS + sizeof(ListS));
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

//List
static void *_list_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__List, _self, app);
    struct List *self = offsetOf(_self, __List);
    void *t = va_arg(*app, void*);
    self->class = t;
    self->_end.nxt = &self->_end;
    self->_end.pre = &self->_end;
    MetaObject args[2] = {VAEND, VAEND};
    int i = 0;
    while ((t = va_arg(*app, void*)) != VAEND)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _deal_List_Args(_self, args, i);
    return _self;
}

static void *_list_dtor(void *_self)
{
    _self = super_dtor(__List, _self);
    _list_clear(_self);
    return _self;
}

static void *_list_brackets(const void *_self, const void *_x)
{
    struct List *self = offsetOf(_self, __List);
    struct ListNode *node = self->_end.nxt;
    MetaObject x = (void*)_x;
    Int n = THIS(x).cast(T(Int));
    while (n->val > 0 && node != &self->_end)
    {
        node = node->nxt;
        n->val--;
    }
    assert(!n->val);
    return DATA(*node);
}

static Iterator _list_begin(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    void *mem = ARP_MallocARelDtor(classSz(__ListIter), destroy);
    Any any = VA(self->_end.nxt, ANYONE);
    Iterator ret = construct(__ListIter, mem, VA(BidirectionalIter), self->class, any, VAEND);
    return ret;
}

static Iterator _list_end(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    void *mem = ARP_MallocARelDtor(classSz(__ListIter), destroy);
    char any_mem[classSz(T(Any))];
    Any any = VA(self->_end.nxt->pre, ANYONE);
    Iterator ret = construct(__ListIter, mem, VA(BidirectionalIter), self->class, any, VAEND);
    return ret;
}

static void *_list_front(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    return DATA(self->_end.nxt);
}

static void *_list_back(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    return DATA(self->_end.pre);
}

static size_t _list_size(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    struct ListNode *node = self->_end.nxt;
    size_t size = 0;
    while (node != &self->_end)
    {
        size++;
        node = node->nxt;
    }
    return size;
}

static bool _list_empty(const void *_self)
{
    struct List *self = offsetOf(_self, __List);
    return self->_end.nxt == &self->_end;
}

static void _list_push_back(void *_self, const void *x)
{
    struct List *self = offsetOf(_self, __List);
    _insert_aux(self, &self->_end, x);
}

static void _list_push_front(void *_self, const void *x)
{
    struct List *self = offsetOf(_self, __List);
    _insert_aux(self, self->_end.nxt, x);
}

static void _list_pop_back(void *_self)
{
    struct List *self = offsetOf(_self, __List);
    _erase_aux(self, self->_end.pre);
}

static void _list_pop_front(void *_self)
{
    struct List *self = offsetOf(_self, __List);
    _erase_aux(self, self->_end.nxt);
}

static Iterator _list_erase(void *_self, Iterator iter)
{
    struct List *self = offsetOf(_self, __List);
    assert(classOf(iter) == __ListIter);
    struct ListIter *it = offsetOf(iter, __ListIter);
    it->node = _erase_aux(self, it->node);
    return iter;
}

static Iterator _list_insert(void *_self, Iterator iter, const void *x)
{
    struct List *self = offsetOf(_self, __List);
    assert(classOf(iter) == __ListIter);
    struct ListIter *it = offsetOf(iter, __ListIter);
    it->node = _insert_aux(self, it->node, x);
    return iter;
}

static void _list_remove(void *_self, const void *x)
{
    struct List *self = offsetOf(_self, __List);
    struct ListNode *node = self->_end.nxt;
    while (node != &self->_end)
    {
        Object obj = DATA(*node);
        if (THIS(obj).equal(x)) {
            _erase_aux(self, node);
            break;
        }
        node = node->nxt;
    }
}

static void _list_unique(void *_self)
{
    struct List *self = offsetOf(_self, __List);
    struct ListNode *node = self->_end.nxt;
    struct ListNode *next_node = node->nxt;
    while (next_node != &self->_end)
    {
        Object obj = DATA(*node);
        void *x = DATA(*next_node);
        if (THIS(obj).equal(x))
            _erase_aux(self, next_node);
        else
            node = next_node;
        next_node = next_node->nxt;
    }
}

static void _list_splice(void *_self, Iterator _position, List l, va_list *app)
{
    struct List *self = offsetOf(_self, __List);
    struct List *L = offsetOf(l, __List);
    assert(L->class == self->class);
    MetaObject args[2];
    void *t;
    int n = 0;
    while ((t = va_arg(*app, MetaObject)) != VAEND)
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
        first_node = ((struct ListIter*)offsetOf(args[0], __ListIter))->node;
        last_node = first_node->nxt;
        if (pos_node == first_node || pos_node == last_node)
            return;
    } else {
        first_node = ((struct ListIter*)offsetOf(args[0], __ListIter))->node;
        last_node = ((struct ListIter*)offsetOf(args[1], __ListIter))->node;
        if (first_node == last_node)
            return;
    }
    _transfer(pos_node, first_node, last_node);
}

static void _list_merge(void *_self, List l, Compare cmp)
{
    struct List *self = offsetOf(_self, __List);
    struct List *L = offsetOf(l, __List);
    assert(L->class == self->class);
    struct ListNode *first1 = self->_end.nxt;
    struct ListNode *last1 = &self->_end;
    struct ListNode *first2 = L->_end.nxt;
    struct ListNode *last2 = &L->_end;
    while (first1 != last1 && first2 != last2)
    {
        if (cmp(DATA(*first1), DATA(*first2)) > 0) {
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

static void _list_reverse(void *_self)
{
    struct List *self = offsetOf(_self, __List);
    if (self->_end.nxt == &self->_end || self->_end.nxt->nxt == &self->_end)
        return;
    struct ListNode *first = self->_end.nxt;
    while (first != &self->_end)
    {
        struct ListNode *next = first->nxt;
        _transfer(self->_end.nxt, first, next);
        first = next;
    }
}

static void _list_sort(void *_self, Compare cmp)
{
    struct List *self = offsetOf(_self, __List);
    if (self->_end.nxt == &self->_end || self->_end.nxt->nxt == &self->_end)
        return;
    List _carry = new(T(List), self->class, VAEND);
    struct List *carry = offsetOf(_carry, __List);
    List counter[64];
    int fill = 0;
    while (self->_end.nxt != &self->_end)
    {
        struct ListNode *carry_first = carry->_end.nxt;
        struct ListNode *self_first = self->_end.nxt;
        _transfer(carry_first, self_first, self_first->nxt);
        int i = 0;
        while (i < fill  && !THIS(counter[i]).empty())
        {
            THIS(counter[i]).merge(_carry, cmp);
            THIS(_carry).swap(counter[i++]);
        }
        if (i == fill)
            counter[fill++] = new(T(List), self->class, VAEND);
        THIS(counter[i]).swap(_carry);
    }
    for (int i = 1; i < fill; i++) {
        THIS(counter[i]).merge(counter[i - 1], cmp);
        delete(counter[i - 1]);
    }
    _list_swap(_self, counter[fill - 1]);
    delete(counter[fill - 1]);
    delete(_carry);
}

static void _list_clear(void *_self)
{
    struct List *self = offsetOf(_self, __List);
    while (self->_end.nxt != &self->_end)
        _list_pop_back(_self);
}

static void _list_swap(void *_self, List _l)
{
    struct List *self = offsetOf(_self, __List);
    struct List *L = offsetOf(_l, __List);
    assert(L->class == self->class);
    if (self->_end.nxt == &self->_end && L->_end.nxt == &L->_end) //都是空的
        return;
    else if (self->_end.nxt == &self->_end) { //self是空的
        self->_end = L->_end;
        L->_end.pre->nxt = &self->_end;
        L->_end.nxt->pre = &self->_end;
        L->_end.nxt = &L->_end;
        L->_end.pre = &L->_end;
    } else if (L->_end.nxt == &L->_end) { //L是空的
        L->_end = self->_end;
        self->_end.pre->nxt = &L->_end;
        self->_end.nxt->pre = &L->_end;
        self->_end.nxt = &self->_end;
        self->_end.pre = &self->_end;
    } else { //都不是空的
        self->_end.pre->nxt = &L->_end;
        self->_end.nxt->pre = &L->_end;
        L->_end.pre->nxt = &self->_end;
        L->_end.nxt->pre = &self->_end;
        struct ListNode tmp = self->_end;
        self->_end = L->_end;
        L->_end = tmp;
    }
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->end);
    return class->end(_self);
}

static void *_front(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->front);
    return class->front(_self);
}

static void *_back(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->back);
    return class->back(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->size);
    return class->size(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _push_back(const void *x)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->push_back);
    class->push_back(_self, x);
}

static void _push_front(const void *x)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->push_front);
    class->push_front(_self, x);
}

static void _pop_back(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->pop_back);
    class->pop_back(_self);
}

static void _pop_front(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->pop_front);
    class->pop_front(_self);
}

static Iterator _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->erase);
    return class->erase(_self, iter);
}

static Iterator _insert(Iterator iter, const void *x)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->insert);
    return class->insert(_self, iter, x);
}

static void _remove(const void *x)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->remove);
    class->remove(_self, x);
}

static void _unique(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->unique);
    class->unique(_self);
}

static void _splice(Iterator position, List l, ...)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->splice);
    va_list ap;
    va_start(ap, l);
    class->splice(_self, position, l, &ap);
    va_end(ap);
}

static void _merge(List l, Compare cmp)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->merge);
    class->merge(_self, l, cmp);
}

static void _reverse(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->reverse);
    class->reverse(_self);
}

static void _sort(Compare cmp)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->sort);
    class->sort(_self, cmp);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->clear);
    class->clear(_self);
}

static void _swap(List _l)
{
    void *_self = pop_this();
    const struct ListClass *class = offsetOf(classOf(_self), __ListClass);
    assert(class->swap);
    class->swap(_self, _l);
}
