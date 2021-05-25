//
// Created by xjs on 2020/11/20.
//
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_int.h"
#include "include/_tctl_metaclass.h"
#include "include/_tctl_iterator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"

#define Import ITERATOR, CLASS, INT, ANY
struct OriPointerIter {
    size_t cur;
    char *ptr;
};

//Iterator函数
static void *_iter_sub(const void *_self, const void *_x);
static void *_iter_add(const void *_self, const void *_x);
static void _iter_assign(void *_self, const void *_x);
static void _iter_self_sub(void *_self, const void *_x);
static void _iter_self_add(void *_self, const void *_x);
static void _iter_dec(void *_self);
static void _iter_inc(void *_self);
static void *_iter_brackets(const void *_self, const void *_x);
static int _iter_cmp(const void *_self, const void *_x);
static bool _iter_equal(const void *_self, const void *_x);
static void *_iter_ctor(void *_self, va_list *app);
static void *_iter_derefer(const void *_self);
static long long _iter_dist(const void *_self, Iterator _it);
static long long _riter_dist(const void *_self, Iterator _it);
static Iterator _iter_reverse_iterator(const void *_self);
//OriPointerIter对象
static const void *__OriPointerIter = NULL;
static const void *__ROriPointerIter = NULL;
//init
static void initOriPointIter(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!__OriPointerIter) {
        __OriPointerIter = new(_IteratorClass(), "OriPointerIter",
                           T(Iterator), sizeof(struct OriPointerIter) + classSz(T(Iterator)),
                           _MetaClassS->ctor, _iter_ctor,
                           _ClassS->equal, _iter_equal,
                           _ClassS->cmp, _iter_cmp,
                           _ClassS->brackets, _iter_brackets,
                           _ClassS->inc, _iter_inc,
                           _ClassS->dec, _iter_dec,
                           _ClassS->self_add, _iter_self_add,
                           _ClassS->self_sub, _iter_self_sub,
                           _ClassS->assign, _iter_assign,
                           _ClassS->add, _iter_add,
                           _ClassS->sub, _iter_sub,
                           _IteratorS->derefer, _iter_derefer,
                           _IteratorS->dist, _iter_dist,
                           _IteratorS->reverse_iterator, _iter_reverse_iterator,
                           Selector, _IteratorS, NULL);
    }
    if (!__ROriPointerIter) {
        __ROriPointerIter = new(_IteratorClass(), "ROriPointerIter",
                           T(Iterator), sizeof(struct OriPointerIter) + classSz(T(Iterator)),
                           _MetaClassS->ctor, _iter_ctor,
                           _ClassS->equal, _iter_equal,
                           _ClassS->cmp, _iter_cmp,
                           _ClassS->brackets, _iter_brackets,
                           _ClassS->inc, _iter_dec,
                           _ClassS->dec, _iter_inc,
                           _ClassS->self_add, _iter_self_sub,
                           _ClassS->self_sub, _iter_self_add,
                           _ClassS->assign, _iter_assign,
                           _ClassS->add, _iter_sub,
                           _ClassS->sub, _iter_add,
                           _IteratorS->derefer, _iter_derefer,
                           _IteratorS->dist, _riter_dist,
                           _IteratorS->reverse_iterator, _iter_reverse_iterator,
                           Selector, _IteratorS, NULL);
    }
}

//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__OriPointerIter, _self, app);
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    self->cur = 0;
    void *t = va_arg(*app, void*);
    if (classOf(t) == T(Any)) {
        Any any = t;
        self->ptr = *(void**)THIS(any).value();
    } else {
        assert(class_check(t, T(Iterator)));
        assert(classOf(t) == __OriPointerIter || classOf(t) == __ROriPointerIter);
        struct OriPointerIter *p;
        if (classOf(t) == __OriPointerIter)
            p = offsetOf(t, __OriPointerIter);
        else
            p = offsetOf(t, __ROriPointerIter);
        *self = *p;
    }
    t = va_arg(*app, void*);
    if (t != VAEND) {
        MetaObject obj = t;
        Int I = classOf(t) == T(Int) ? t : THIS(obj).cast(T(Int));
        self->cur = I->val;
        while (va_arg(*app, void*) != VAEND);
    }
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    const struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    const struct OriPointerIter *x = offsetOf(_x, __OriPointerIter);
    return self->cur == x->cur && self->ptr == x->ptr;
}

static int _iter_cmp(const void *_self, const void *_x)
{
    const struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    const struct OriPointerIter *x = offsetOf(_x, __OriPointerIter);
    assert(x->ptr == self->ptr);
    return (int)(self->cur - x->cur);
}

static void *_iter_brackets(const void *_self, const void *_x)
{
    const struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    MetaObject m_obj = (void*)_x;
    Int x = classOf(_x) == T(Int) ? (Int)_x : THIS(m_obj).cast(T(Int));
    return *(MetaObject*)(self->ptr + sizeof(MetaObject) * (x->val + self->cur));
}

static void _iter_inc(void *_self)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    self->cur++;
}

static void _iter_dec(void *_self)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    self->cur--;
}

static void _iter_self_add(void *_self, const void *_x)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    MetaObject m_obj = (void*)_x;
    Int x = classOf(_x) == T(Int) ? (void*)_x : THIS(m_obj).cast(T(Int));
    self->cur += x->val;
}

static void _iter_self_sub(void *_self, const void *_x)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    MetaObject m_obj = (void*)_x;
    Int x = classOf(_x) == T(Int) ? (void*)_x : THIS(m_obj).cast(T(Int));
    self->cur -= x->val;
}

static void _iter_assign(void *_self, const void *_x)
{
    assert(classOf(_x) == __OriPointerIter || classOf(_x) == __ROriPointerIter);
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    struct OriPointerIter *x = offsetOf(_x, __OriPointerIter);
    *self = *x;
}

static void *_iter_add(const void *_self, const void *_x)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    Iterator it = (void*)_self;
    MetaObject m_obj = (void*)_x;
    Int x = classOf(_x) == T(Int) ? (void*)_x : THIS(m_obj).cast(T(Int));
    void *mem = ARP_MallocARelDtor(classSz(__OriPointerIter), destroy);
    void *res = construct(classOf(self), mem, SequenceIter, it->class, VA(self->cur + x->val), VA(self->ptr, ANYONE), VAEND);
    return res;
}

static void *_iter_sub(const void *_self, const void *_x)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    Iterator it = (void*)_self;
    MetaObject m_obj = (void*)_x;
    Int x = classOf(_x) == T(Int) ? (void*)_x : THIS(m_obj).cast(T(Int));
    void *mem = ARP_MallocARelDtor(classSz(__OriPointerIter), destroy);
    void *res = construct(classOf(self), mem, SequenceIter, it->class, VA(self->cur - x->val), VA(self->ptr, ANYONE), VAEND);
    return res;
}

static void *_iter_derefer(const void *_self)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    return *(MetaObject*)(self->ptr + sizeof(MetaObject) * self->cur);
}

static long long _iter_dist(const void *_self, Iterator _it)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    assert(classOf(_it) == __OriPointerIter);
    struct OriPointerIter *it = offsetOf(_it, __OriPointerIter);
    return it->cur - self->cur;
}

static long long _riter_dist(const void *_self, Iterator _it)
{
    struct OriPointerIter *self = offsetOf(_self, __OriPointerIter);
    assert(classOf(_it) == __OriPointerIter);
    struct OriPointerIter *it = offsetOf(_it, __OriPointerIter);
    return self->cur - it->cur;
}

static Iterator _iter_reverse_iterator(const void *_self)
{
    Iterator it = (void*)_self;
    if (classOf(_self) == __OriPointerIter) {
        void *mem = ARP_MallocARelDtor(classSz(__OriPointerIter), destroy);
        return construct(__ROriPointerIter, mem, VA(it), VAEND);
    } else {
        void *mem = ARP_MallocARelDtor(classSz(__ROriPointerIter), destroy);
        return construct(__OriPointerIter, mem, VA(it), VAEND);
    }
}

//对外接口
Iterator _oriPointerIter_aux(const void *class, void *p, size_t x, ...)
{
    if (!__OriPointerIter)
        initOriPointIter();
    void *mem = ARP_MallocARelDtor(classSz(__OriPointerIter), destroy);
    return construct(__OriPointerIter, mem, VA(SequenceIter), class, VA(p, ANYONE), VA(x), VAEND);
}
