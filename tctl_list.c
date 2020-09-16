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
static list_iter *erase(list_iter *iter)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = iter->node;
    if (node == p_private->node)
        return NULL;

    if (node == p_private->node->next) {
        p_private->start_iter.val = node->next->data;
        p_private->start_iter.node = node->next;
    }
    node->pre->next = node->next;
    node->next->pre = node->pre;
    iter->node = node->next;
    iter->val = node->data;
    //obj_iter next = (void*)node->next;
    deallocate(node, sizeof(struct __list_node) + p_private->memb_size);
    return iter;
}
static list_iter *insert(list_iter *iter, void *x)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = iter->node;
    void *ptr = allocate(sizeof(struct __list_node) + p_private->memb_size);
    struct __list_node *new_node = ptr;
    new_node->data = ptr + sizeof(struct __list_node);
    memcpy(new_node->data, x, p_private->memb_size);

    new_node->next = node;
    node->pre->next = new_node;
    new_node->pre = node->pre;
    node->pre = new_node;
    if (new_node == p_private->node->next) {
        p_private->start_iter.val = new_node->data;
        p_private->start_iter.node = new_node;
    }
    return iter;
}
static void push_front(void *x)
{
    list *this = pop_this();
    THIS(this).insert((list_iter*)THIS(this).begin(), x);
}
static void push_back(void *x)
{
    list *this = pop_this();
    THIS(this).insert((list_iter*)THIS(this).end(), x);
}
static void pop_front(void)
{
    list *this = pop_this();
    THIS(this).erase((list_iter*)THIS(this).begin());
}
static void pop_back(void)
{
    list *this = pop_this();
    list_iter ptr = *(list_iter*)THIS(this).end();
    ptr.node = ptr.node->next;
    ptr.val = ptr.node->next->data;
    THIS(this).erase(&ptr);
}
static void clear(void)
{
    list *this = pop_this();
    while (!THIS(this).empty())
        THIS(this).pop_back();
}
static void remove(void *value)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = p_private->node->next;
    while (node != p_private->node)
    {
        if (!memcmp(value, node->data, p_private->memb_size)) {
            list_iter iter = {node->data, node};
            THIS(this).erase(&iter);
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
        if (!memcmp(next_node->data, node->data, p_private->memb_size)) {
            list_iter iter = {node->data, node};
            THIS(this).erase(&iter);
        }
        else
            node = next_node;
        next_node = next_node->next;
    }
}
static void const * const * begin(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return (void*)&p_private->start_iter;
}
static void const * const * end(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return (void*)&p_private->finish_iter;
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
    return p_private->start_iter.val;
}
static void const *back(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->finish_iter.val;
}
static void splice(list_iter *position, list *l, list_iter *first, list_iter *last)
{
    list *this = pop_this();
    list_iter _last_is_null;
    if (last == NULL) {
        _last_is_null.node = first->node->next;
        _last_is_null.val = first->node->next->data;
        last = &_last_is_null;
    }
    if(first->node != last->node) {
        transfer(position->node, first->node, last->node);
        __private_list *p_private = (__private_list*)this->__obj_private;
        __private_list *l_private = (__private_list*)l->__obj_private;
        //一定要先p_private再置l_private，因为指针可能关联着l链表的strart_iter或finish_iter
        if (p_private->start_iter.node == position->node) {
            p_private->start_iter.node = first->node;
            p_private->start_iter.val = first->node->data;
        }
        if (l_private->start_iter.node == first->node) {
            l_private->start_iter.node = last->node;
            l_private->start_iter.val = last->val;
        }
    }
}
static void merge(list *l, bool (*cmp)(void*, void*))
{
    list *this = pop_this();
    list_iter first1 = *(list_iter*)THIS(this).begin();
    list_iter last1 = *(list_iter*)THIS(this).end();
    list_iter first2 = *(list_iter*)THIS(l).begin();
    list_iter last2 = *(list_iter*)THIS(l).end();
    while (first1.node != last1.node && first2.node != last2.node)
    {
        if (cmp(first1.val, first2.val)) {
            list_iter next = {first2.node->next->data, first2.node->next};
            THIS(this).splice(&first1, l, &first2, &next);
            //transfer(first1, first2, next->data);
            first2 = next;
        } else {
            first1.node = first1.node->next;
            first1.val = first1.node->data;
        }
    }
    if (first2.node != last2.node)
        THIS(this).splice(&first1, l, &first2, &last2);
}
static void reverse(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    if (p_private->node == p_private->node->next || p_private->node->next->next == p_private->node)
        return;
    list_iter first = *(list_iter*)THIS(this).begin();
    while (first.node != (*(list_iter*)THIS(this).end()).node)
    {
        struct __list_node *next = first.node;
        next = next->next;
        transfer(p_private->node->next, first.node, next);
        first.node = next;
        first.val = first.node->data;
    }
    p_private->start_iter.val = p_private->node->next->data;
    p_private->start_iter.node = p_private->node->next;
}
void swap(list *l)
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
static void sort(bool (*cmp)(void*, void*))
{
    list *this = pop_this();
    __private_list *p_private = (__private_list*)this->__obj_private;
    if (p_private->node->next == p_private->node || p_private->node->next->next == p_private->node)
        return;
    list carry = creat_list(p_private->memb_size);
    list counter[64];
    int fill = 0;
    while (!THIS(this).empty())
    {
        THIS(&carry).splice((list_iter*)THIS(&carry).begin(), this, (list_iter*)(THIS(this)).begin(), NULL);
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

static void iter_increment(void *p)
{
    pop_this();
    list_iter *_p = p;
    _p->node = _p->node->next;
    _p->val = _p->node->data;
}
static void iter_decrement(void *p)
{
    pop_this();
    list_iter *_p = p;
    _p->node = _p->node->pre;
    _p->val = _p->node->data;
}

static __iterator_obj_func  __def_list_iter = {
        NULL,
        iter_increment,
        iter_decrement,
        NULL,
        NULL
};

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
    void *ptr = allocate(sizeof(struct __list_node) + memb_size);
    private->node = ptr;
    private->node->data = ptr + sizeof(struct __list_node);
    private->node->next = private->node;
    private->node->pre = private->node;
    __init_iter(&private->start, p_list, private->node->data, sizeof(list_iter), memb_size, &__def_list_iter);
    private->finish = private->start;
    private->start_iter.node = private->finish_iter.node = private->node;
    private->start_iter.val = private->finish_iter.val = private->node->data;
    //memcpy(p_list->__obj_private, &private, sizeof(__private_list));
}

list creat_list(size_t memb_size)
{
    list l;
    init_list(&l, memb_size);
    return l;
}

void destory_list(list *p_list)
{
    THIS(p_list).clear();
    __private_list *p_private = (__private_list*)p_list->__obj_private;
    deallocate(p_private->node, sizeof(struct __list_node));
}