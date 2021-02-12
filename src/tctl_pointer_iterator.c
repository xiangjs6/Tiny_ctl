//
// Created by xjs on 2020/11/20.
//
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_int.h"
#include "../include/tctl_uint.h"
#include "include/_tctl_metaclass.h"
#include "include/_tctl_iterator.h"
#include "../include/auto_release_pool.h"

#define Import ITERATOR, CLASS, INT
struct OriPointIter {
    size_t cur;
    void *ptr;
};

//Iterator函数
static void *_iter_sub(const void *_this, FormWO_t _x);
static void *_iter_add(const void *_this, FormWO_t _x);
static void _iter_assign(void *_this, FormWO_t _x);
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
//OriPointIter对象
static const void *__OriPointIter = NULL;
//init
static void initOriPointIter(void)
{
    T(Class); //初始化_ClassS选择器
    T(Iterator); //初始化Iterator选择器
    if (!__OriPointIter) {
        __OriPointIter = new(_IteratorClass(), "OriPointIter",
                           T(Iterator), sizeof(struct OriPointIter) + classSz(_Iterator().class),
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
                           Selector, _IteratorS, NULL);
    }
}

static Form_t _OriPointIter(void)
{
    if (!__OriPointIter)
        initOriPointIter();
    return (Form_t){OBJ, .class = __OriPointIter};
}
//Iterator
static void *_iter_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__OriPointIter, _this, app);
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    FormWO_t t = va_arg(*app, FormWO_t);
    switch (t._.f) {
        case POD:
            this->ptr = t.mem;
            break;
        case ADDR:
            assert(t._.size != sizeof(void*));
            this->ptr = *(void**)t.mem;
            break;
        case OBJ:
            assert(t._.class == T(Iterator).class);
            assert(classOf(t.mem) == __OriPointIter);
            struct OriPointIter *p = offsetOf(t.mem, __OriPointIter);
            *this = *p;
            break;
        default:
            this->ptr = NULL;
    }
    t = va_arg(*app, FormWO_t);
    this->cur = toUInt(t);
    while (va_arg(*app, FormWO_t)._.f != END);
    return _this;
}

static bool _iter_equal(const void *_this, FormWO_t _x)
{
    const struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    const struct OriPointIter *x = offsetOf(_x.mem, __OriPointIter);
    return this->cur == x->cur;
}

static int _iter_cmp(const void *_this, FormWO_t _x)
{
    const struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    const struct OriPointIter *x = offsetOf(_x.mem, __OriPointIter);
    return this->cur - x->cur;
}

static void *_iter_brackets(const void *_this, FormWO_t _x)
{
    const struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    const Iterator it = (void*)_this;
    long long x =   toInt(_x);
    Form_t t = THIS(it).type();
    size_t size = t.f == ADDR ? t.size : classSz(t.class);
    void *res = this->ptr + size * (x + this->cur);
    return res;
}

static void _iter_inc(void *_this)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    this->cur++;
}

static void _iter_dec(void *_this)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    this->cur--;
}

static void _iter_self_add(void *_this, FormWO_t _x)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    char tmp[classSz(_Int().class)];
    Int x = (void*)tmp;
    construct(_Int(), tmp, _x, VAEND);
    this->cur += x->val;
    destroy(x);
}

static void _iter_self_sub(void *_this, FormWO_t _x)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    char tmp[classSz(_Int().class)];
    Int x = (void*)tmp;
    construct(_Int(), tmp, _x, VAEND);
    this->cur -= x->val;
    destroy(x);
}

static void _iter_assign(void *_this, FormWO_t _x)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    struct OriPointIter *x = offsetOf(_x.mem, __OriPointIter);
    *this = *x;
}

static void *_iter_add(const void *_this, FormWO_t _x)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    Iterator it = (void*)_this;
    char i_mem[classSz(_Int().class)];
    Int x = (void*)i_mem;
    construct(T(Int), i_mem, _x, VAEND);
    void *mem = ARP_MallocARelDtor(classSz(__OriPointIter), destroy);
    Form_t t = THIS(it).type();
    void *res = new(compose(_OriPointIter(), mem), VA(t, SequenceIter, this->cur + x->val, this->ptr));
    destroy(x);
    return res;
}

static void *_iter_sub(const void *_this, FormWO_t _x)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    Iterator it = (void*)_this;
    char i_mem[classSz(_Int().class)];
    Int x = (void*)i_mem;
    construct(T(Int), i_mem, _x, VAEND);
    void *mem = ARP_MallocARelDtor(classSz(__OriPointIter), destroy);
    Form_t t = THIS(it).type();
    void *res = new(compose(_OriPointIter(), mem), VA(t, SequenceIter, this->cur - x->val, this->ptr));
    destroy(x);
    return res;
}

static void *_iter_derefer(const void *_this)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    Iterator it = (void*)_this;
    Form_t t = THIS(it).type();
    size_t memb_size = t.f == ADDR ? t.size : classSz(t.class);
    return this->ptr + this->cur * memb_size;
}

static long long _iter_dist(const void *_this, Iterator _it)
{
    struct OriPointIter *this = offsetOf(_this, __OriPointIter);
    assert(classOf(_it) == __OriPointIter);
    struct OriPointIter *it = offsetOf(_it, __OriPointIter);
    return it->cur - this->cur;
}

//对外接口
Iterator _oriPointIter_aux(Form_t t, void *p, size_t x, ...)
{
    Form_t f = _OriPointIter(); //用于调用初始化initxxx函数
    if (t.f == POD)
        t.f = ADDR;
    void *mem = ARP_MallocARelDtor(classSz(__OriPointIter), destroy);
    return construct(f, mem, VA(t, SequenceIter, p, x), VAEND);
}
