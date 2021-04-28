//
// Created by xjs on 2020/9/4.
//

#include "include/_tctl_metaclass.h"
#include "include/_tctl_vector.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_algobase.h"
#include "include/_tctl_iterator.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#include <stdarg.h>
#include <string.h>

#define Import CLASS, ITERATOR, OBJECT, METACLASS, ANY, INT

struct VectorClass {
    Iterator (*begin)(const void *_self);
    Iterator (*end)(const void *_self);
    void *(*front)(const void *_self);
    void *(*back)(const void *_self);
    size_t (*size)(const void *_self);
    size_t (*capacity)(const void *_self);
    bool (*empty)(const void *_self);
    void (*push_back)(void *_self, const void *_x);
    void (*pop_back)(void *_self);
    Iterator (*erase)(void *_self, Iterator iter);
    Iterator (*insert)(void *_self, Iterator iter, const void *_x);
    void (*resize)(void *_self, size_t new_size);
    void (*clear)(void *_self);
    void (*swap)(void *_self, Vector _v);
};

struct Vector {
    void *class;
    size_t nmemb;
    size_t total_storage_memb;
    void *start_ptr;
    void *finish_ptr;
};

struct VectorIter {
    size_t cur;
    void *ptr;
};

//selector
static Iterator _begin(void);
static Iterator _end(void);
static void* _front(void);
static void* _back(void);
static size_t _size(void);
static size_t _capacity(void);
static bool _empty(void);
static void _push_back(const void *x);
static void _pop_back(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, const void *x);
static void _resize(size_t new_size);
static void _clear(void);
static void _swap(Vector _v);
//vectorclass
static void *_vectorclass_ctor(void *_self, va_list *app);
//vector
static void *_vector_ctor(void *_self, va_list *app);
static void *_vector_dtor(void *_self);
static void _vector_swap(void *_self, Vector _v);
static void _vector_clear(void *_self);
static void _vector_resize(void *_self, size_t new_size);
static Iterator _vector_insert(void *_self, Iterator _iter, const void *x);
static Iterator _vector_erase(void *_self, Iterator _iter);
static void _vector_pop_back(void *_self);
static void _vector_push_back(void *_self, const void *x);
static bool _vector_empty(const void *_self);
static size_t _vector_capacity(const void *_self);
static size_t _vector_size(const void *_self);
static const void *_vector_back(const void *_self);
static const void *_vector_front(const void *_self);
static Iterator _vector_end(const void *_self);
static Iterator _vector_begin(const void *_self);
static void *_vector_brackets(const void *_self, const void *_x);
//iterator
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
//init
static const void *__VectorIter = NULL;
static const void *__RVectorIter = NULL;
static const void *__Vector = NULL;
static const void *__VectorClass = NULL;
volatile static struct VectorSelector VectorS = {
        {},
        _begin,
        _end,
        _front,
        _back,
        _size,
        _capacity,
        _empty,
        _push_back,
        _pop_back,
        _erase,
        _insert,
        _resize,
        _clear,
        _swap
};
const struct VectorSelector *_VectorS = NULL;

static void initVector(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!_VectorS) {
        _VectorS = (void*)&VectorS;
        memcpy((void*)&VectorS, _ClassS, sizeof(*_ClassS));
    }
    if (!__VectorIter) {
        __VectorIter = new(_IteratorClass(), "VectorIter",
                           T(Iterator), sizeof(struct VectorIter) + classSz(T(Iterator)),
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
    if (!__RVectorIter) {
        __RVectorIter = new(_IteratorClass(), "RVectorIter",
                           T(Iterator), sizeof(struct VectorIter) + classSz(T(Iterator)),
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
    if (!__VectorClass) {
        __VectorClass = new(T(MetaClass), "VectorClass",
                            T(Class), sizeof(struct VectorClass) + classSz(T(Class)),
                            _MetaClassS->ctor, _vectorclass_ctor, NULL);
    }
    if (!__Vector) {
        __Vector = new(_VectorClass(), "Vector",
                       T(Object), sizeof(struct Vector) + classSz(T(Object)),
                       _MetaClassS->ctor, _vector_ctor,
                       _MetaClassS->dtor, _vector_dtor,
                       _ClassS->brackets, _vector_brackets,
                       VectorS.begin, _vector_begin,
                       VectorS.end, _vector_end,
                       VectorS.front, _vector_front,
                       VectorS.back, _vector_back,
                       VectorS.size, _vector_size,
                       VectorS.capacity, _vector_capacity,
                       VectorS.empty, _vector_empty,
                       VectorS.push_back, _vector_push_back,
                       VectorS.pop_back, _vector_pop_back,
                       VectorS.erase, _vector_erase,
                       VectorS.insert, _vector_insert,
                       VectorS.resize, _vector_resize,
                       VectorS.clear, _vector_clear,
                       VectorS.swap, _vector_swap,
                       Selector, _VectorS, NULL);
    }
}

const void *_Vector(void)
{
    if (!__Vector)
        initVector();
    return __Vector;
}

const void *_VectorClass(void)
{
    if (!__VectorClass)
        initVector();
    return __VectorClass;
}

static const void *_VectorIter(void)
{
    if (!__VectorIter)
        initVector();
    return __VectorIter;
}

static const void *_RVectorIter(void)
{
    if (!__RVectorIter)
        initVector();
    return __RVectorIter;
}

//private
static void _deal_Vector_Args(void *_self, MetaObject *args, int n)
{
    if (classOf(args) == __Vector) { //复制一个Vector
        struct Vector *v = offsetOf(args, __Vector);
        size_t v_memb_size = classSz(v->class);
        for (char (*ptr)[v_memb_size] = v->start_ptr; (void*)ptr < v->finish_ptr; ptr++) {
            _vector_push_back(_self, ptr);
        }
    } else if (classOf(args) != T(Int)) { //size_type n, T value = T() 构造方法
        Int ii = (Int)args;
        unsigned long long nmemb = (unsigned long long)ii->val;
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _vector_push_back(_self, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _vector_push_back(_self, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(class_check(args[0], T(Iterator)) && class_check(args[1], T(Iterator)));
        Iterator first = new(classOf(args[0]), args[0], VAEND);
        Iterator last = new(classOf(args[1]), args[1], VAEND);
        while (!THIS(first).equal(last))
        {
            void *obj = THIS(first).derefer();
            _vector_push_back(_self, obj);
            THIS(first).inc();
        }
        delete(first);
        delete(last);
    }
}

static void fill_allocate(struct Vector *self)
{
    size_t old_size = self->total_storage_memb;
    size_t memb_size = classSz(self->class);
    self->total_storage_memb *= 2;
    self->total_storage_memb = self->total_storage_memb ? self->total_storage_memb : 1;
    void *new_block = allocate(self->total_storage_memb * memb_size);
    Iterator new_it = new(_VectorIter(), VA(SequenceIter, 0, new_block));
    Iterator first = new(_VectorIter(), VA(SequenceIter, 0, self->start_ptr));
    Iterator last = new(_VectorIter(), VA(SequenceIter, self->nmemb, self->start_ptr));
    copy(first, last, new_it);
    for (; !THIS(first).equal(VA(last)); THIS(first).inc()) {
            Object obj = THIS(first).derefer();
            THIS(obj).dtor();
        }

    delete(new_it);
    delete(first);
    delete(last);
    deallocate(self->start_ptr, old_size * memb_size);
    self->start_ptr = new_block;
    self->finish_ptr = self->start_ptr + memb_size * self->nmemb;
}

//public
//Iterator
static void *_iter_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__VectorIter, _self, app);
    struct VectorIter *self = offsetOf(_self, __VectorIter);
    MetaObject m_obj = va_arg(*app, MetaObject);
    if (m_obj == VAEND) {
        self->cur = 0;
        self->ptr = NULL;
        return _self;
    }
    if (classOf(m_obj) == T(Any)) {
        Any any = (Any)m_obj;
        assert(THIS(any).size() == sizeof(void*));
        self->ptr = THIS(any).value();
    } else {
        assert(classOf(m_obj) == __VectorIter || classOf(m_obj) == __RVectorIter);
        struct VectorIter *it;
        if (classOf(m_obj) == __VectorIter)
            it = offsetOf(m_obj, __VectorIter);
        else
            it = offsetOf(m_obj, __RVectorIter);
        *self = *it;
        return _self;
    }

    m_obj = va_arg(*app, MetaObject);
    if (m_obj == VAEND) {
        self->cur = 0;
        return _self;
    }
    assert(classOf(m_obj) == T(Int));
    Int i = (Int)m_obj;
    self->cur = (size_t)i->val;
    return _self;
}

static bool _iter_equal(const void *_self, const void *_x)
{
    assert(classOf(_x) == __VectorIter || classOf(_x) == __RVectorIter);
    const struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    const struct VectorIter *x;
    if (classOf(_x) == __VectorIter)
        x = offsetOf(_x, __VectorIter);
    else
        x = offsetOf(_x, __RVectorIter);
    return self->cur == x->cur;
}

static int _iter_cmp(const void *_self, const void *_x)
{
    assert(classOf(_x) == __VectorIter || classOf(_x) == __RVectorIter);
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    const struct VectorIter *x;
    if (classOf(_x) == __VectorIter)
        x = offsetOf(_x, __VectorIter);
    else
        x = offsetOf(_x, __RVectorIter);
    if (self->cur > x->cur)
        return 1;
    else if (self->cur < x->cur)
        return -1;
    return 0;
}

static void *_iter_brackets(const void *_self, const void *_x)
{
    const struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    Iterator it = (void*)_self;
    MetaObject m_obj = (void*)_x;
    Int x;
    if (classOf(_x) == T(Int))
        x = (void*)_x;
    else
        x = THIS(m_obj).cast(T(Int));
    size_t size = classSz(it->class);
    void *res = self->ptr + size * ((size_t)x->val + self->cur);
    return res;
}

static void _iter_inc(void *_self)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    self->cur++;
}

static void _iter_dec(void *_self)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    self->cur--;
}

static void _iter_self_add(void *_self, const void *_x)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    Int x;
    MetaObject m_obj = (void*)_x;
    if (classOf(_x) == T(Int))
        x = (void*)_x;
    else
        x = THIS(m_obj).cast(T(Int));
    self->cur += (size_t)x->val;
}

static void _iter_self_sub(void *_self, const void *_x)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    Int x;
    MetaObject m_obj = (void*)_x;
    if (classOf(_x) == T(Int))
        x = (void*)_x;
    else
        x = THIS(m_obj).cast(T(Int));
    self->cur -= (size_t)x->val;
}

static void _iter_assign(void *_self, const void *_x)
{
    assert(classOf(_x) == __VectorIter || classOf(_x) == __RVectorIter);
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    struct VectorIter *x;
    if (classOf(_x) == __VectorIter)
        x = offsetOf(_x, __VectorIter);
    else
        x = offsetOf(_x, __RVectorIter);
    *self = *x;
}

static void *_iter_add(const void *_self, const void *_x)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    Iterator res = construct(classOf(self), mem, _self, VAEND);
    THIS(res).self_add((void*)_x);
    return res;
}

static void *_iter_sub(const void *_self, const void *_x)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    Iterator res = construct(classOf(self), mem, _self, VAEND);
    THIS(res).self_sub((void*)_x);
    return res;
}

static void *_iter_derefer(const void *_self)
{
    struct VectorIter *self;
    if (classOf(self) == __VectorIter)
        self = offsetOf(_self, __VectorIter);
    else
        self = offsetOf(_self, __RVectorIter);
    Iterator it = (void*)_self;
    size_t memb_size = classSz(it->class);
    return self->ptr + self->cur * memb_size;
}

static long long _iter_dist(const void *_self, Iterator _it)
{
    struct VectorIter *self = offsetOf(_self, __VectorIter);
    assert(classOf(_it) == __VectorIter);
    struct VectorIter *it = offsetOf(_it, __VectorIter);
    return it->cur - self->cur;
}

static long long _riter_dist(const void *_self, Iterator _it)
{
    struct VectorIter *self = offsetOf(_self, __RVectorIter);
    assert(classOf(_it) == __RVectorIter);
    struct VectorIter *it = offsetOf(_it, __VectorIter);
    return self->cur - it->cur;
}

static Iterator _iter_reverse_iterator(const void *_self)
{
    Iterator it = (void*)_self;
    if (classOf(_self) == __VectorIter) {
        void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
        return construct(_RVectorIter(), mem, it, VAEND);
    } else {
        void *mem = ARP_MallocARelDtor(classSz(__RVectorIter), destroy);
        return construct(_VectorIter(), mem, it, VAEND);
    }
}

//VectorClass
static void *_vectorclass_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__VectorClass, _self, app);
    struct VectorClass *self = offsetOf(_self, __VectorClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&VectorS + sizeof(VectorS._);
    voidf *end = (void*)&VectorS + sizeof(VectorS);
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

//Vector
static void *_vector_ctor(void *_self, va_list *app)
{
    _self = super_ctor(__Vector, _self, app);
    struct Vector *self = offsetOf(_self, __Vector);
    void *class = va_arg(*app, void*);
    self->class = class;
    self->nmemb = 0;
    self->start_ptr = NULL;
    self->finish_ptr = NULL;
    self->total_storage_memb = 0;
    MetaObject args[2];
    MetaObject arg;
    int i = 0;
    while ((arg = va_arg(*app, MetaObject)) != VAEND)
    {
        assert(i < 2);
        args[i++] = arg;
    }
    if (i)
        _deal_Vector_Args(_self, args, i);
    return _self;
}

static void *_vector_dtor(void *_self)
{
    _self = super_dtor(__Vector, _self);
    struct Vector *self = offsetOf(_self, __Vector);
    _vector_clear(_self);
    size_t memb_size = classSz(self->class);
    deallocate(self->start_ptr, self->total_storage_memb * memb_size);
    return _self;
}

static void *_vector_brackets(const void *_self, const void *_x)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    Int i;
    if (classOf(_x) == T(Int))
        i = (void*)_x;
    else {
        MetaObject m_obj = (void*)_x;
        i = THIS(m_obj).cast(T(Int));
    }
    long long x = (long long)i->val;
    assert(x >= 0 && x <= self->nmemb);
    size_t memb_size = classSz(self->class);
    void *res = self->start_ptr + memb_size * x;
    return res;
}

static Iterator _vector_begin(const void *_self)
{
    struct Vector *self = offsetOf(_self, __Vector);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    char any_mem[classSz(T(Any))];
    Any any = construct(T(Any), any_mem, POD, &self->start_ptr, sizeof(void*), VAEND);
    Iterator res = construct(_VectorIter(), mem, VA(SequenceIter), self->class, VA(0), any, VAEND);
    destroy(any);
    return res;
}

static Iterator _vector_end(const void *_self)
{
    struct Vector *self = offsetOf(_self, __Vector);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    char any_mem[classSz(T(Any))];
    Any any = construct(T(Any), any_mem, POD, &self->start_ptr, sizeof(void*), VAEND);
    Iterator res = construct(_VectorIter(), mem, VA(SequenceIter), self->class, VA(self->nmemb), any, VAEND);
    destroy(any);
    return res;
}

static const void *_vector_front(const void *_self)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    return self->start_ptr;
}

static const void *_vector_back(const void *_self)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    size_t memb_size = classSz(self->class);
    return self->finish_ptr - memb_size;
}

static size_t _vector_size(const void *_self)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    return self->nmemb;
}

static size_t _vector_capacity(const void *_self)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    return self->total_storage_memb - self->nmemb;
}

static bool _vector_empty(const void *_self)
{
    const struct Vector *self = offsetOf(_self, __Vector);
    return !self->nmemb;
}

static void _vector_push_back(void *_self, const void *_x)
{
    struct Vector *self = offsetOf(_self, __Vector);
    size_t memb_size = classSz(self->class);
    if (!_vector_capacity(_self))
        fill_allocate((void*)self);
    construct(self->class, self->finish_ptr, _x, VAEND);
    self->nmemb++;
    self->finish_ptr += memb_size;
}

static void _vector_pop_back(void *_self)
{
    struct Vector *self = offsetOf(_self, __Vector);
    size_t memb_size = classSz(self->class);
    self->nmemb--;
    self->finish_ptr -= memb_size;
    destroy(self->finish_ptr);
}

static Iterator _vector_erase(void *_self, Iterator _iter)
{
    struct VectorIter *iter = offsetOf(_iter, __VectorIter);
    assert(classOf(_iter) == __VectorIter);
    struct Vector *self = offsetOf(_self, __Vector);
    assert(iter->cur >= 0 && iter->cur < self->nmemb);
    if (iter->cur == self->nmemb) {
        _vector_pop_back(_self);
        return _iter;
    }
    Iterator target_it = new(_VectorIter(), self->class, VA(SequenceIter, iter->cur), iter->ptr);
    Iterator first = new(_VectorIter(), self->class, VA(SequenceIter, iter->cur + 1), self->start_ptr);
    Iterator last = new(_VectorIter(), self->class, VA(SequenceIter, self->nmemb), self->start_ptr);
    copy(first, last, target_it);
    delete(target_it);
    delete(first);
    delete(last);
    _vector_pop_back(_self);
    return _iter;
}

static Iterator _vector_insert(void *_self, Iterator _iter, const void *_x)
{
    struct VectorIter *iter = offsetOf(_iter, __VectorIter);
    assert(classOf(_iter) == __VectorIter);
    struct Vector *self = offsetOf(_self, __Vector);
    assert(iter->cur >= 0 && iter->cur <= self->nmemb);
    if (iter->cur == self->nmemb) {
        _vector_push_back(_self, _x);
        return _iter;
    }
    if (!_vector_capacity(_self))
        fill_allocate(_self);
    size_t memb_size = classSz(self->class);
    Iterator target_it = new(_VectorIter(), self->class, VA(SequenceIter, iter->cur + 1), iter->ptr, VAEND);
    Iterator first = new(_VectorIter(), self->class, VA(SequenceIter, iter->cur), self->start_ptr, VAEND);
    Iterator last = new(_VectorIter(), VA(self->class, SequenceIter, self->nmemb, self->start_ptr));
    copy(first, last, target_it);
    delete(target_it);
    delete(first);
    delete(last);
    void *p_target = iter->ptr + iter->cur * memb_size;
    Object obj = p_target;
    THIS(obj).assign(_x);
    self->nmemb++;
    self->finish_ptr += memb_size;
    return _iter;
}

static void _vector_resize(void *_self, size_t new_size)
{
    struct Vector *self = offsetOf(_self, __Vector);
    while (new_size > self->total_storage_memb)
        fill_allocate(self);
    self->nmemb = new_size;
    size_t memb_size = classSz(self->class);
    char *old_finish_ptr = self->finish_ptr;
    self->finish_ptr = self->start_ptr + new_size * memb_size;
    for (; old_finish_ptr < (char*)self->finish_ptr; old_finish_ptr += memb_size)
        construct(self->class, old_finish_ptr, VAEND);
}

static void _vector_clear(void *_self)
{
    struct Vector *self = offsetOf(_self, __Vector);
    void *it = self->start_ptr;
    size_t memb_size = classSz(self->class);
    while (it != self->finish_ptr)
    {
        destroy(it);
        it = (char*)it + memb_size;
    }
    self->nmemb = 0;
    self->finish_ptr = self->start_ptr;
}

static void _vector_swap(void *_self, Vector _v)
{
    struct Vector *self = offsetOf(_self, __Vector);
    struct Vector *v = offsetOf((void*)_v, __Vector);
    struct Vector tmp = *v;
    *v = *self;
    *self = tmp;
}

//selector
static Iterator _begin(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->begin);
    return class->begin(_self);
}

static Iterator _end(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->end);
    return class->end(_self);
}

static void* _front(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->front);
    return class->front(_self);
}

static void* _back(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->back);
    return class->back(_self);
}

static size_t _size(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->size);
    return class->size(_self);
}

static size_t _capacity(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->capacity);
    return class->capacity(_self);
}

static bool _empty(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->empty);
    return class->empty(_self);
}

static void _push_back(const void *x)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->push_back);
    return class->push_back(_self, x);
}

static void _pop_back(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->pop_back);
    return class->pop_back(_self);
}

static Iterator _erase(Iterator iter)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->erase);
    return class->erase(_self, iter);
}

static Iterator _insert(Iterator iter, const void *x)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->insert);
    return class->insert(_self, iter, x);
}

static void _resize(size_t new_size)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->resize);
    return class->resize(_self, new_size);
}

static void _clear(void)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->clear);
    return class->clear(_self);
}

static void _swap(Vector _v)
{
    void *_self = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_self), __VectorClass);
    assert(class->swap);
    return class->swap(_self, _v);
}
