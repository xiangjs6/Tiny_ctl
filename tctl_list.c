//
// Created by xjs on 2020/9/8.
//

#include "tctl_list.h"
#include "tctl_common.h"
#include "tctl_allocator.h"
#include <memory.h>

//private
static void transfer(struct __list_node *pos, struct __list_node *first, struct __list_node *last)
{
    if (pos == first)
        return;
    struct __list_node *pos_node = pos;
    struct __list_node *pos_pre_node = pos_node->pre;
    struct __list_node *first_node = first;
    struct __list_node *last_node = last;
    struct __list_node *last_pre_node = last_node->pre;

    first_node->pre->next = last_node;
    last_node->pre = first_node->pre;
    pos_pre_node->next = first_node;
    last_pre_node->next = pos_node;
    pos_node->pre = last_pre_node;
    first_node->pre = pos_pre_node;
}

static void __splice(list *this, struct __list_node *_position, list *l, struct __list_node *_first, struct __list_node *_last)
{
    if (_last == NULL)
        _last = _first->next;
    if(_first != _last) {
        transfer(_position, _first, _last);
        __private_list *p_private = (__private_list*)this->__obj_private;
        __private_list *l_private = (__private_list*)l->__obj_private;
        //一定要先p_private再置l_private，因为指针可能关联着l链表的strart_iter或finish_iter
        if (p_private->start_ptr == _position->data) {
            p_private->start_ptr = _first->data;
        }
        if (l_private->start_ptr == _first->data) {
            l_private->start_ptr = _last->data;
        }
    }
}

static __list_iter __erase(list *this, __list_iter iter)
{
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = container_of(iter, struct __list_node, data);
    if (node == p_private->node)
        return NULL;

    if (node == p_private->node->next)
        p_private->start_ptr = node->next->data;
    node->pre->next = node->next;
    node->next->pre = node->pre;
    //iter->val = node->next->data;
    //obj_iter next = (void*)node->next;
    deallocate(node, sizeof(struct __list_node) + p_private->memb_size);
    return node->next->data;
}

static __list_iter __insert(list *this, __list_iter iter, void *x)
{
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = container_of(iter, struct __list_node, data);
    struct __list_node *new_node = allocate(sizeof(struct __list_node) + p_private->memb_size);
    memcpy(new_node->data, x, p_private->memb_size);

    new_node->next = node;
    node->pre->next = new_node;
    new_node->pre = node->pre;
    node->pre = new_node;
    if (new_node == p_private->node->next)
        p_private->start_ptr = new_node->data;
    return iter;
}

//public
void *at(int pos)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list*)this->__obj_private;
    struct __list_node *node = p_private->node->next;
    while (pos-- && node != p_private->node)
        node = node->next;
    if (node == p_private->node)
        return NULL;
    return node->data;
}
static IterType erase(IterType iter)
{
    list *this = pop_this();
    __iterator *__iter = iter;
    __iter->val = __erase(this, __iter->val);
    return iter;
}
static IterType insert(IterType iter, void *x)
{
    list *this = pop_this();
    __iterator *__iter = iter;
    __iter->val = __insert(this, __iter->val, x);
    return iter;
}
static void push_front(void *x)
{
    list *this = pop_this();
    __insert(this, ((__iterator*)THIS(this).begin())->val, x);
}
static void push_back(void *x)
{
    list *this = pop_this();
    __insert(this, ((__iterator*)THIS(this).end())->val, x);
}
static void pop_front(void)
{
    list *this = pop_this();
    __erase(this, ((__iterator*)THIS(this).begin())->val);
}
static void pop_back(void)
{
    list *this = pop_this();
    struct __list_node *node = container_of(((__iterator*)THIS(this).end())->val, struct __list_node, data);
    __erase(this, node->pre->data);
}
static void clear(void)
{
    list *this = pop_this();
    while (!THIS(this).empty())
        THIS(this).pop_front();
}
static void remove(void *value)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = p_private->node->next;
    while (node != p_private->node)
    {
        if (!memcmp(value, node->data, p_private->memb_size)) {
            __erase(this, node->data);
            return;
        }
        node = node->next;
    }
}
static void unique(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = p_private->node->next;
    struct __list_node *next_node = node->next;
    while (next_node != p_private->node)
    {
        if (!memcmp(next_node->data, node->data, p_private->memb_size))
            __erase(this, next_node->data);
        else
            node = next_node;
        next_node = next_node->next;
    }
}
static const IterType begin(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    if (p_private->start_iter.obj_this != this)
        p_private->start_iter.obj_this = this;
    return &p_private->start_iter;
}
static const IterType end(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    if (p_private->finish_iter.obj_this != this)
        p_private->finish_iter.obj_this = this;
    return &p_private->finish_iter;
}
static size_t size(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = p_private->node->next;
    size_t count = 0;
    while (node != p_private->node)
    {
        count++;
        node = node->next;
    }
    return count;
}
static bool empty(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->node == p_private->node->next;
}
static void const *front(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->start_ptr;
}
static void const *back(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->finish_ptr;
}
static void splice(const IterType position, list *l, const IterType first, const IterType last)
{
    list *this = pop_this();
    const __iterator *__first = first;
    const __iterator *__last = last;
    const __iterator *__position = position;
    struct __list_node *first_node = container_of(__first->val, struct __list_node, data);
    struct __list_node *position_node = container_of(__position->val, struct __list_node, data);
    struct __list_node *last_node = last ? container_of(__last->val, struct __list_node, data) : NULL;
    __splice(this, position_node, l, first_node, last_node);
}
static void merge(list *l, Compare cmp)
{
    list *this = pop_this();
    struct __list_node *first1 = container_of(((__iterator*)THIS(this).begin())->val, struct __list_node, data);
    struct __list_node *last1 = container_of(((__iterator*)THIS(this).end())->val, struct __list_node, data);
    struct __list_node *first2 = container_of(((__iterator*)THIS(l).begin())->val, struct __list_node, data);
    struct __list_node *last2 = container_of(((__iterator*)THIS(l).end())->val, struct __list_node, data);
    while (first1 != last1 && first2 != last2)
    {
        if (cmp(first1->data, first2->data) > 0) {
            struct __list_node *next = first2->next;
            __splice(this, first1, l, first2, next);
            //transfer(first1, first2, next->data);
            first2 = next;
        } else {
            first1 = first1->next;
        }
    }
    if (first2 != last2)
        __splice(this, first1, l, first2, last2);
}
static void reverse(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    if (p_private->node == p_private->node->next || p_private->node->next->next == p_private->node)
        return;
    struct __list_node *first = container_of(((__iterator*)THIS(this).begin())->val, struct __list_node, data);
    while (first != p_private->node)
    {
        struct __list_node *next = first;
        next = next->next;
        transfer(p_private->node->next, first, next);
        first = next;
    }
    p_private->start_ptr = p_private->node->next->data;
}
static void swap(list *l)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    __private_list *p_l_private = (__private_list*)l->__obj_private;
    if (p_private->memb_size != p_l_private->memb_size)
        return;
    __private_list temp = *p_private;
    memcpy(p_private, p_l_private, sizeof(__private_list));
    memcpy(p_l_private, &temp, sizeof(__private_list));
}
static void sort(Compare cmp)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list*)this->__obj_private;
    if (p_private->node->next == p_private->node || p_private->node->next->next == p_private->node)
        return;
    list carry;
    init_list(&carry, p_private->memb_size);
    list counter[64];
    int fill = 0;
    while (!THIS(this).empty())
    {
        struct __list_node *carry_first = container_of(((__iterator*)THIS(&carry).begin())->val, struct __list_node, data);
        struct __list_node *this_first = container_of(((__iterator*)THIS(this).begin())->val, struct __list_node, data);
        __splice(&carry, carry_first, this, this_first, NULL);
        //THIS(&carry).splice((list_iter*)THIS(&carry).begin(), this, (list_iter*)(THIS(this)).begin(), NULL);
        int i = 0;
        while (i < fill && !THIS(&counter[i]).empty())
        {
            THIS(&counter[i]).merge(&carry, cmp);
            THIS(&carry).swap(&counter[i++]);
        }
        if (i == fill) {
            init_list(&counter[fill], p_private->memb_size);
            fill++;
        }
        THIS(&counter[i]).swap(&carry);
    }
    for (int i = 1; i < fill; i++) {
        THIS(&counter[i]).merge(&counter[i - 1], cmp);
        destory_list(&counter[i - 1]);
    }
    THIS(this).swap(&counter[fill - 1]);
    destory_list(&counter[fill - 1]);
    destory_list(&carry);
}

static void iter_increment(__iterator *p)
{
    pop_this();
    struct __list_node *node = container_of(p->val, struct __list_node, data);
    p->val = node->next->data;
}
static void iter_decrement(__iterator *p)
{
    pop_this();
    struct __list_node *node = container_of(p->val, struct __list_node, data);
    p->val = node->pre->data;
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    pop_this();
    return it1->val == it2->val;
}

static __iterator_obj_func  __def_list_iter = {
        NULL,
        iter_increment,
        iter_decrement,
        NULL,
        NULL,
        NULL,
        iter_equal
};

static const iterator_func __def_list_iter_func = INIT_ITER_FUNC(&__def_list_iter);

static const list def_list = {
        at,
        push_front,
        push_back,
        erase,
        insert,
        pop_front,
        pop_back,
        clear,
        remove,
        unique,
        splice,
        merge,
        reverse,
        swap,
        sort,
        begin,
        end,
        size,
        empty,
        front,
        back
};

void init_list(list *p_list, size_t memb_size)
{
    *p_list = def_list;
    __private_list *private = (__private_list*)p_list->__obj_private;
    *(size_t*)&private->memb_size = memb_size;
    private->node = allocate(sizeof(struct __list_node));
    private->node->next = private->node;
    private->node->pre = private->node;
    private->start_iter = __creat_iter(sizeof(__list_iter), p_list, memb_size, &__def_list_iter_func);
    private->finish_iter = __creat_iter(sizeof(__list_iter), p_list, memb_size, &__def_list_iter_func);
    private->start_ptr = private->finish_ptr = private->node->data;
    //memcpy(p_list->__obj_private, &private, sizeof(__private_list));
}

void destory_list(list *p_list)
{
    THIS(p_list).clear();
    __private_list *p_private = (__private_list*)p_list->__obj_private;
    deallocate(p_private->node, sizeof(struct __list_node));
}

list creat_list(size_t memb_size)
{
    list l;
    init_list(&l, memb_size);
    return l;
}
