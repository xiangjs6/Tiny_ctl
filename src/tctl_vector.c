//
// Created by xjs on 2020/9/4.
//

#include "include/_tctl_metaclass.h"
#include "include/_tctl_vector.h"
#include "../include/tctl_allocator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_uint.h"
#include "../include/tctl_algobase.h"
#include "include/_tctl_iterator.h"
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

#define Import CLASS, ITERATOR, OBJECT, METACLASS

struct VectorClass {
    Iterator (*begin)(const void *_this);
    Iterator (*end)(const void *_this);
    void *(*front)(const void *_this);
    void *(*back)(const void *_this);
    size_t (*size)(const void *_this);
    size_t (*capacity)(const void *_this);
    bool (*empty)(const void *_this);
    void (*push_back)(void *_this, FormWO_t _x);
    void (*pop_back)(void *_this);
    Iterator (*erase)(void *_this, Iterator iter);
    Iterator (*insert)(void *_this, Iterator iter, FormWO_t _x);
    void (*resize)(void *_this, size_t new_size);
    void (*clear)(void *_this);
    void (*swap)(void *_this, Vector _v);
};

struct Vector {
    Form_t _t;
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
static void _push_back(FormWO_t x);
static void _pop_back(void);
static Iterator _erase(Iterator iter);
static Iterator _insert(Iterator iter, FormWO_t x);
static void _resize(size_t new_size);
static void _clear(void);
static void _swap(Vector _v);
//vectorclass
static void *_vectorclass_ctor(void *_this, va_list *app);
//vector
static void *_vector_ctor(void *_this, va_list *app);
static void *_vector_dtor(void *_this);
static void _vector_swap(void *_this, Vector _v);
static void _vector_clear(void *_this);
static void _vector_resize(void *_this, size_t new_size);
static Iterator _vector_insert(void *_this, Iterator _iter, FormWO_t x);
static Iterator _vector_erase(void *_this, Iterator _iter);
static void _vector_pop_back(void *_this);
static void _vector_push_back(void *_this, FormWO_t x);
static bool _vector_empty(const void *_this);
static size_t _vector_capacity(const void *_this);
static size_t _vector_size(const void *_this);
static const void *_vector_back(const void *_this);
static const void *_vector_front(const void *_this);
static Iterator _vector_end(const void *_this);
static Iterator _vector_begin(const void *_this);
static void *_vector_brackets(const void *_this, FormWO_t _x);
//iterator
static void *_iter_sub(const void *_this, FormWO_t _x);
static void *_iter_add(const void *_this, FormWO_t _x);
static void _iter_asign(void *_this, FormWO_t _x);
static void _iter_self_sub(void *_this, FormWO_t _x);
static void _iter_self_add(void *_this, FormWO_t _x);
static void _iter_dec(void *_this);
static void _iter_inc(void *_this);
static void *_iter_brackets(const void *_this, FormWO_t _x);
static int _iter_cmp(const void *_this, FormWO_t _x);
static bool _iter_equal(const void *_this, FormWO_t _x);
static void *_iter_ctor(void *_this, va_list *app);
static void *_iter_derefer(const void *_this);
static long long _iter_dist(const void *_this, Iterator _it);
//init
static const void *__VectorIter = NULL;
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
const struct VectorSelector *_VectorS= NULL;

void initVector(void)
{
    initClass();
    initIterator();
    if (!_VectorS) {
        _VectorS = (void*)&VectorS;
        memcpy((void*)&VectorS, _ClassS, sizeof(*_ClassS));
    }
    if (!__VectorIter) {
        __VectorIter = new(_IteratorClass(), "VectorIter",
                           T(Iterator), sizeof(struct VectorIter) + classSz(_Iterator().class),
                           _MetaClassS->ctor, _iter_ctor,
                           _ClassS->equal, _iter_equal,
                           _ClassS->cmp, _iter_cmp,
                           _ClassS->brackets, _iter_brackets,
                           _ClassS->inc, _iter_inc,
                           _ClassS->dec, _iter_dec,
                           _ClassS->self_add, _iter_self_add,
                           _ClassS->self_sub, _iter_self_sub,
                           _ClassS->asign, _iter_asign,
                           _ClassS->add, _iter_add,
                           _ClassS->sub, _iter_sub,
                           _IteratorS->derefer, _iter_derefer,
                           _IteratorS->dist, _iter_dist,
                           Selector, _IteratorS, NULL);
    }
    if (!__VectorClass) {
        __VectorClass = new(T(MetaClass), "VectorClass",
                            T(Class), sizeof(struct VectorClass) + classSz(_Class().class),
                            _MetaClassS->ctor, _vectorclass_ctor, NULL);
    }
    if (!__Vector) {
        __Vector = new(_VectorClass(), "Vector",
                       T(Object), sizeof(struct Vector) + classSz(_Object().class),
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

Form_t _Vector(void)
{
    Form_t t = {OBJ, .class = __Vector};
    return t;
}

Form_t _VectorClass(void)
{
    Form_t t = {OBJ, .class = __VectorClass};
    return t;
}

static Form_t _VectorIter(void)
{
    Form_t t = {OBJ, .class = __VectorIter};
    return t;
}

//private
static size_t _dealIterVarySize(void *src, size_t size)
{
    if (size == 1) {
        char c;
        memcpy(&c, src, size);
        size = c;
    } else if (size == 2) {
        short s;
        memcpy(&s, src, size);
        size = s;
    } else if (size == 4) {
        int i;
        memcpy(&i, src, size);
        size = i;
    } else if (size == 8) {
        long long l;
        memcpy(&l, src, size);
        size = l;
    }
    return size;
}

static void *_dealIterVaryArg(FormWO_t t, char *type)
{
    if (t._.f == OBJ) {
        if (t._.class == _Iterator().class) {
            *type = 'I';
            return t.mem;
        }
        if (*type == 'p')
            return Cast(t.mem, "point");
        if (*type == 'S')
            return Cast(t.mem, "size");
    } else if (t._.f == POD) {
        if (*type == 'P') {
            assert(t._.size == sizeof(void*));
            return t.mem;
        }
        if (*type == 'S') {
            size_t size = _dealIterVarySize(&t.mem, t._.size);
            return (void*)size;
        }
    } else if (t._.f == ADDR) {
        if (*type == 'P')
            return *(void**)t.mem;
        if (*type == 'S') {
            size_t size = _dealIterVarySize(t.mem, t._.size);
            return (void*)size;
        }
    }
    assert(0);
    return NULL;
}

static void _dealVectorArgs(void *_this, FormWO_t *args, int n)
{
    struct Vector *this = offsetOf(_this, __Vector);
    if (args->_.class == _Vector().class) { //复制一个Vector
        struct Vector *v = offsetOf(args->mem, __Vector);
        size_t v_memb_size = v->_t.f == POD ? v->_t.size : classSz(v->_t.class);
        Form_t t = v->_t;
        for (char (*ptr)[v_memb_size] = v->start_ptr; (void*)ptr < v->finish_ptr; ptr++) {
            if (t.f == POD) {
                char (*p)[t.size] = ptr;
                _vector_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _vector_push_back(_this, FORM_WITH_OBJ(t, ptr));
            }
        }
    } else if (args->_.f == POD || args->_.f == ADDR || args->_.class != _Iterator().class) { //size_type n, T value = T() 构造方法
        unsigned long long nmemb = toUInt(*args);
        if (n == 1) {
            for (size_t i = 0; i < nmemb; i++)
                _vector_push_back(_this, VAEND);
        } else {
            for (size_t i = 0; i < nmemb; i++)
                _vector_push_back(_this, args[1]);
        }
    } else { //Iterator first, Iterator last 迭代器构造方法
        assert(n == 2);
        assert(args[1]._.class == _Iterator().class);
        Iterator first = args[0].mem;
        char first_mem[sizeOf(first)];
        first = THIS(first).ctor(first_mem, VA(THIS(first).type(), first));
        Iterator last = args[1].mem;
        char last_mem[sizeOf(last)];
        last = THIS(last).ctor(last_mem, VA(THIS(last).type(), last));
        Form_t t = THIS(first).type();
        while (!THIS(first).equal(VA(last)))
        {
            void *obj = THIS(first).derefer();
            if (t.f == POD) {
                char (*p)[t.size] = obj;
                _vector_push_back(_this, VA(VA_ADDR(*p)));
            } else if (t.f == OBJ) {
                _vector_push_back(_this, FORM_WITH_OBJ(t, obj));
            }
            THIS(first).inc();
        }
        destroy(first);
        destroy(last);
    }
}

static void fill_allocate(struct Vector *this)
{
    size_t old_size = this->total_storage_memb;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    this->total_storage_memb *= 2;
    this->total_storage_memb = this->total_storage_memb ? this->total_storage_memb : 1;
    void *new_block = allocate(this->total_storage_memb * memb_size);
    Iterator new_it = new(_VectorIter(), VA(this->_t, 0, new_block));
    Iterator first = new(_VectorIter(), VA(this->_t, 0, this->start_ptr));
    Iterator last = new(_VectorIter(), VA(this->_t, this->nmemb, this->start_ptr));
    copy(first, last, new_it);
    if (this->_t.f != POD) {
        for (; !THIS(first).equal(VA(last)); THIS(first).inc()) {
            Object obj = THIS(first).derefer();
            THIS(obj).dtor();
        }
    }
    delete(new_it);
    delete(first);
    delete(last);
    deallocate(this->start_ptr, old_size * memb_size);
    this->start_ptr = new_block;
    this->finish_ptr = this->start_ptr + memb_size * this->nmemb;
}

//public
//Iterator
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__VectorIter, _this, app);
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    FormWO_t t;
    const static char s[2] = {'S', 'P'};
    const char *s_p = s;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        char c = *s_p++;
        void *res = _dealIterVaryArg(t, &c);
        if (c == 'I') {
            assert(classOf(res) == __VectorIter);
            struct VectorIter *it = offsetOf(res, __VectorIter);
            *this = *it;
            break;
        } else if (c == 'S') {
            this->cur = (size_t)res;
        } else if (c == 'P') {
            this->ptr = res;
        }
    }
    return _this;
}
static bool _iter_equal(const void *_this, FormWO_t _x)
{
    const struct VectorIter *this = offsetOf(_this, __VectorIter);
    const struct VectorIter *x = offsetOf(_x.mem, __VectorIter);
    return this->cur == x->cur;
}

static int _iter_cmp(const void *_this, FormWO_t _x)
{
    const struct VectorIter *this = offsetOf(_this, __VectorIter);
    const struct VectorIter *x = offsetOf(_x.mem, __VectorIter);
    return this->cur - x->cur;
}

static void *_iter_brackets(const void *_this, FormWO_t _x)
{
    const struct VectorIter *this = offsetOf(_this, __VectorIter);
    const Iterator it = (void*)_this;
    long long x =   toInt(_x);
    Form_t t = THIS(it).type();
    size_t size = t.f == POD ? t.size : classSz(t.class);
    void *res = this->ptr + size * (x + this->cur);
    return res;
}

static void _iter_inc(void *_this)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    this->cur++;
}

static void _iter_dec(void *_this)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    this->cur--;
}

static void _iter_self_add(void *_this, FormWO_t _x)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    char tmp[classSz(_Int().class)];
    Int x = (void*)tmp;
    construct(_Int(), tmp, _x);
    this->cur += x->val;
    destroy(x);
}

static void _iter_self_sub(void *_this, FormWO_t _x)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    char tmp[classSz(_Int().class)];
    Int x = (void*)tmp;
    construct(_Int(), tmp, _x);
    this->cur -= x->val;
    destroy(x);
}

static void _iter_asign(void *_this, FormWO_t _x)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    struct VectorIter *x = offsetOf(_x.mem, __VectorIter);
    *this = *x;
}

static void *_iter_add(const void *_this, FormWO_t _x)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    Iterator it = (void*)_this;
    char i_mem[classSz(_Int().class)];
    Int x = (void*)i_mem;
    construct(_Int(), i_mem, _x);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    Form_t t = THIS(it).type();
    void *res = new(compose(_VectorIter(), mem), VA(t, this->cur + x->val, this->ptr));
    destroy(x);
    return res;
}

static void *_iter_sub(const void *_this, FormWO_t _x)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    Iterator it = (void*)_this;
    char i_mem[classSz(_Int().class)];
    Int x = (void*)i_mem;
    construct(_Int(), i_mem, _x);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    Form_t t = THIS(it).type();
    void *res = new(compose(_VectorIter(), mem), VA(t, this->cur - x->val, this->ptr));
    destroy(x);
    return res;
}

static void *_iter_derefer(const void *_this)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    Iterator it = (void*)_this;
    Form_t t = THIS(it).type();
    size_t memb_size = t.f == POD ? t.size : classSz(t.class);
    return this->ptr + this->cur * memb_size;
}

static long long _iter_dist(const void *_this, Iterator _it)
{
    struct VectorIter *this = offsetOf(_this, __VectorIter);
    assert(classOf(_it) == __VectorIter);
    struct VectorIter *it = offsetOf(_it, __VectorIter);
    return it->cur - this->cur;
}

//VectorClass
static void *_vectorclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__VectorClass, _this, app);
    struct VectorClass *this = offsetOf(_this, __VectorClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&VectorS + sizeof(VectorS._);
    voidf *end = (void*)&VectorS + sizeof(VectorS);
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

//Vector
static void *_vector_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Vector, _this, app);
    struct Vector *this = offsetOf(_this, __Vector);
    FormWO_t t = va_arg(*app, FormWO_t);
    assert(t._.f >= FORM);
    t._.f -= FORM;
    this->_t = t._;
    this->nmemb = 0;
    this->start_ptr = NULL;
    this->finish_ptr = NULL;
    this->total_storage_memb = 0;
    FormWO_t args[2];
    int i = 0;
    while ((t = va_arg(*app, FormWO_t))._.f != END)
    {
        assert(i < 2);
        args[i++] = t;
    }
    if (i)
        _dealVectorArgs(_this, args, i);
    return _this;
}

static void *_vector_dtor(void *_this)
{
    _this = super_dtor(__Vector, _this);
    struct Vector *this = offsetOf(_this, __Vector);
    _vector_clear(_this);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    deallocate(this->start_ptr, this->total_storage_memb * memb_size);
    return _this;
}

static void *_vector_brackets(const void *_this, FormWO_t _x)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    long long x = toInt(_x);
    assert(x >= 0 && x < this->nmemb);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    void *res = this->start_ptr + memb_size * x;
    return res;
}

static Iterator _vector_begin(const void *_this)
{
    struct Vector *this = offsetOf(_this, __Vector);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    return new(compose(_VectorIter(), mem), VA(this->_t, 0, this->start_ptr));
}

static Iterator _vector_end(const void *_this)
{
    struct Vector *this = offsetOf(_this, __Vector);
    void *mem = ARP_MallocARelDtor(classSz(__VectorIter), destroy);
    return new(compose(_VectorIter(), mem), VA(this->_t, this->nmemb, this->start_ptr));
}

static const void *_vector_front(const void *_this)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    return this->start_ptr;
}

static const void *_vector_back(const void *_this)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    return this->finish_ptr - memb_size;
}

static size_t _vector_size(const void *_this)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    return this->nmemb;
}

static size_t _vector_capacity(const void *_this)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    return this->total_storage_memb - this->nmemb;
}

static bool _vector_empty(const void *_this)
{
    const struct Vector *this = offsetOf(_this, __Vector);
    return !this->nmemb;
}

static void _vector_push_back(void *_this, FormWO_t _x)
{
    struct Vector *this = offsetOf(_this, __Vector);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    if (!_vector_capacity(_this))
        fill_allocate((void*)this);
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(this->finish_ptr, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(this->finish_ptr, &_x.mem, memb_size);
    } else {
        construct(this->_t, this->finish_ptr, _x);
    }
    this->nmemb++;
    this->finish_ptr += memb_size;
}

static void _vector_pop_back(void *_this)
{
    struct Vector *this = offsetOf(_this, __Vector);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    this->nmemb--;
    this->finish_ptr -= memb_size;
    if (this->_t.f == OBJ) {
        Object o = this->finish_ptr;
        destroy(o);
    }
}

static Iterator _vector_erase(void *_this, Iterator _iter)
{
    struct VectorIter *iter = offsetOf(_iter, __VectorIter);
    assert(classOf(_iter) == __VectorIter);
    struct Vector *this = offsetOf(_this, __Vector);
    assert(iter->cur >= 0 && iter->cur < this->nmemb);
    if (iter->cur == this->nmemb) {
        _vector_pop_back(_this);
        return _iter;
    }
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    Iterator target_it = new(_VectorIter(), VA(this->_t, iter->cur, iter->ptr));
    Iterator first = new(_VectorIter(), VA(this->_t, iter->cur + 1, this->start_ptr));
    Iterator last = new(_VectorIter(), VA(this->_t, this->nmemb, this->start_ptr));
    copy(first, last, target_it);
    delete(target_it);
    delete(first);
    delete(last);
    _vector_pop_back(_this);
    return _iter;
}

static Iterator _vector_insert(void *_this, Iterator _iter, FormWO_t _x)
{
    struct VectorIter *iter = offsetOf(_iter, __VectorIter);
    assert(classOf(_iter) == __VectorIter);
    struct Vector *this = offsetOf(_this, __Vector);
    assert(iter->cur >= 0 && iter->cur <= this->nmemb);
    if (iter->cur == this->nmemb) {
        _vector_push_back(_this, _x);
        return _iter;
    }
    if (!_vector_capacity(_this))
        fill_allocate(_this);
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    Iterator target_it = new(_VectorIter(), VA(this->_t, iter->cur + 1, iter->ptr));
    Iterator first = new(_VectorIter(), VA(this->_t, iter->cur, this->start_ptr));
    Iterator last = new(_VectorIter(), VA(this->_t, this->nmemb, this->start_ptr));
    copy(first, last, target_it);
    delete(target_it);
    delete(first);
    delete(last);
    void *p_target = iter->ptr + iter->cur * memb_size;
    if (this->_t.f == POD) {
        assert(_x._.f != OBJ);
        if (_x._.f == ADDR)
            memcpy(p_target, _x.mem, memb_size);
        else if (_x._.f == POD)
            memcpy(p_target, &_x.mem, memb_size);
    } else {
        Object obj = p_target;
        THIS(obj).asign(_x);
    }
    this->nmemb++;
    this->finish_ptr += memb_size;
    return _iter;
}

static void _vector_resize(void *_this, size_t new_size)
{
    struct Vector *this = offsetOf(_this, __Vector);
    while (new_size > this->total_storage_memb)
        fill_allocate(this);
    this->nmemb = new_size;
    size_t memb_size = this->_t.f == POD ? this->_t.size : classSz(this->_t.class);
    char *old_finish_ptr = this->finish_ptr;
    this->finish_ptr = this->start_ptr + new_size * memb_size;
    if (this->_t.f == OBJ) {
        for (; old_finish_ptr < (char*)this->finish_ptr; old_finish_ptr += memb_size)
            construct(this->_t, old_finish_ptr, VAEND);
    }
}

static void _vector_clear(void *_this)
{
    struct Vector *this = offsetOf(_this, __Vector);
    if (this->_t.f == OBJ) {
        Iterator it = new(_VectorIter(), VA(this->_t, 0, this->start_ptr));
        Iterator end = new(_VectorIter(), VA(this->_t, this->nmemb, this->start_ptr));
        while (!THIS(it).equal(VA(end)))
        {
            Object obj = THIS(it).derefer();
            destroy(obj);
            THIS(it).inc();
        }
        delete(it);
        delete(end);
    }
    this->nmemb = 0;
    this->finish_ptr = this->start_ptr;
}

static void _vector_swap(void *_this, Vector _v)
{
    struct Vector *this = offsetOf(_this, __Vector);
    struct Vector *v = offsetOf((void*)_v, __Vector);
    struct Vector tmp = *v;
    *v = *this;
    *this = tmp;
}

//selector
static Iterator _begin(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->begin);
    return class->begin(_this);
}

static Iterator _end(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->end);
    return class->end(_this);
}

static void* _front(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->front);
    return class->front(_this);
}

static void* _back(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->back);
    return class->back(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->size);
    return class->size(_this);
}

static size_t _capacity(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->capacity);
    return class->capacity(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->empty);
    return class->empty(_this);
}

static void _push_back(FormWO_t x)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->push_back);
    return class->push_back(_this, x);
}

static void _pop_back(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->pop_back);
    return class->pop_back(_this);
}

static Iterator _erase(Iterator iter)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->erase);
    return class->erase(_this, iter);
}

static Iterator _insert(Iterator iter, FormWO_t x)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->insert);
    return class->insert(_this, iter, x);
}

static void _resize(size_t new_size)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->resize);
    return class->resize(_this, new_size);
}

static void _clear(void)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->clear);
    return class->clear(_this);
}

static void _swap(Vector _v)
{
    void *_this = pop_this();
    const struct VectorClass *class = offsetOf(classOf(_this), __VectorClass);
    assert(class->swap);
    return class->swap(_this, _v);
}
