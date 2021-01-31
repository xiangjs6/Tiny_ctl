//
// Created by xjs on 2020/9/29.
//

#include "../include/tctl_priority_queue.h"
#include "../include/tctl_heap.h"
#include "include/_tctl_priority_queue.h"
#include "../include/tctl_vector.h"
#include <memory.h>
#include <stdlib.h>
#include "../include/auto_release_pool.h"

#define Import CLASS, OBJECT, METACLASS, VECTOR

struct Priority_queueClass {
    void *(*top)(const void *_this);
    void (*push)(void *_this, FormWO_t x);
    void (*pop)(void *_this);
    bool (*empty)(void *_this);
    size_t (*size)(void *_this);
    void (*swap)(void *_this, Priority_queue);
};

struct Priority_queue {
    Vector c;
    Compare cmp;
};

//selector
static void *_top(void);
static void _push(FormWO_t x);
static void _pop(void);
static bool _empty(void);
static size_t _size(void);
static void _swap(Priority_queue que);
//Priority_queueclass
static void *_Priority_queueclass_ctor(void *_this, va_list *app);
//Priority_queue
static void *_Priority_queue_ctor(void *_this, va_list *app);
static void *_Priority_queue_dtor(void *_this);
static void *_Priority_queue_top(const void *_this);
static void _Priority_queue_push(void *_this, FormWO_t x);
static void _Priority_queue_pop(void *_this);
static bool _Priority_queue_empty(void *_this);
static size_t _Priority_queue_size(void *_this);
static void _Priority_queue_swap(void *_this, Priority_queue _que);

//init
static const void *__Priority_queue = NULL;
static const void *__Priority_queueClass = NULL;
volatile static struct Priority_queueSelector Priority_queueS = {
    {},
    _top,
    _push,
    _pop,
    _empty,
    _size,
    _swap
};
const struct Priority_queueSelector *_Priority_queueS = NULL;

void initPriority_queue(void)
{
    initClass();
    initIterator();
    initVector();
    if (!_Priority_queueS) {
        _Priority_queueS = (void*)&Priority_queueS;
        memcpy((void*)&Priority_queueS, _ClassS, sizeof(*_ClassS));
    }
    if (!__Priority_queueClass) {
        __Priority_queueClass = new(T(MetaClass), "Priority_queueClass",
                           T(Class), sizeof(struct Priority_queueClass) + classSz(_Class().class),
                           _MetaClassS->ctor, _Priority_queueclass_ctor, NULL);
    }
    if (!__Priority_queue) {
        __Priority_queue = new(_Priority_queueClass(), "Priority_queue",
                     T(Object), sizeof(struct Priority_queue) + classSz(_Object().class),
                     _MetaClassS->ctor, _Priority_queue_ctor,
                     _MetaClassS->dtor, _Priority_queue_dtor,
                     _Priority_queueS->top, _Priority_queue_top,
                     _Priority_queueS->push, _Priority_queue_push,
                     _Priority_queueS->pop, _Priority_queue_pop,
                     _Priority_queueS->empty, _Priority_queue_empty,
                     _Priority_queueS->size, _Priority_queue_size,
                     _Priority_queueS->swap, _Priority_queue_swap,
                     Selector, _Priority_queueS, NULL);
    }
}

Form_t _Priority_queue(void)
{
    Form_t t = {OBJ, .class = __Priority_queue};
    return t;
}

Form_t _Priority_queueClass(void)
{
    Form_t t = {OBJ, .class = __Priority_queueClass};
    return t;
}

//Priority_queueClass
static void *_Priority_queueclass_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Priority_queueClass, _this, app);
    struct VectorClass *this = offsetOf(_this, __Priority_queueClass);
    voidf selector;
    va_list ap;
    va_copy(ap, *app);
    voidf *begin = (void*)&Priority_queueS + sizeof(Priority_queueS._);
    voidf *end = (void*)&Priority_queueS + sizeof(Priority_queueS);
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

//Priority_queue
static void *_Priority_queue_ctor(void *_this, va_list *app)
{
    _this = super_ctor(__Priority_queue, _this, app);
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    this->c = malloc(classSz(_Vector().class));
    construct_v(_Vector(), this->c, app);
    FormWO_t t = va_arg(*app, FormWO_t);
    this->cmp = t.mem;
    return _this;
}

static void *_Priority_queue_dtor(void *_this)
{
    _this = super_dtor(__Priority_queue, _this);
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    destroy(this->c);
    free(this->c);
    return _this;
}

static void *_Priority_queue_top(const void *_this)
{
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    return THIS(this->c).front();
}

static void _Priority_queue_push(void *_this, FormWO_t x)
{
    ARP_CreatePool();
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    THIS(this->c).push_back(x);
    push_heap(THIS(this->c).begin(), THIS(this->c).end(), this->cmp);
    ARP_FreePool();
}

static void _Priority_queue_pop(void *_this)
{
    ARP_CreatePool();
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    pop_heap(THIS(this->c).begin(), THIS(this->c).end(), this->cmp);
    THIS(this->c).pop_back();
    ARP_FreePool();
}

static bool _Priority_queue_empty(void *_this)
{
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    return THIS(this->c).empty();
}

static size_t _Priority_queue_size(void *_this)
{
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    return THIS(this->c).size();
}

static void _Priority_queue_swap(void *_this, Priority_queue _que)
{
    struct Priority_queue *this = offsetOf(_this, __Priority_queue);
    struct Priority_queue *que = offsetOf(_que, __Priority_queue);
    THIS(this->c).swap(que->c);
    Compare tmp = this->cmp;
    this->cmp = que->cmp;
    que->cmp = tmp;
}

static void *_top(void)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->top);
    return class->top(_this);
}

static void _push(FormWO_t x)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->push);
    class->push(_this, x);
}

static void _pop(void)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->push);
    class->pop(_this);
}

static bool _empty(void)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->empty);
    return class->empty(_this);
}

static size_t _size(void)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->size);
    return class->size(_this);
}

static void _swap(Priority_queue que)
{
    void *_this = pop_this();
    const struct Priority_queueClass *class = offsetOf(classOf(_this), __Priority_queueClass);
    assert(class->swap);
    class->swap(_this, que);
}
