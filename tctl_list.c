//
// Created by xjs on 2020/9/8.
//

#include "tctl_list.h"
#include "tctl_common.h"
#include "tctl_allocator.h"
#include <memory.h>

//private
static void transfer(iter_ptr pos, iter_ptr first, iter_ptr last)
{
    if (pos == first)
        return;
    struct __list_node *pos_node = pos - sizeof(struct __list_node);
    struct __list_node *pos_pre_node = pos_node->pre;
    struct __list_node *first_node = first - sizeof(struct __list_node);
    struct __list_node *last_node = last - sizeof(struct __list_node);
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
static iter_ptr erase(iter_ptr iter)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = iter - sizeof(struct __list_node);
    if (node == p_private->node)
        return NULL;

    if (node == p_private->node->next)
        p_private->start.ptr = node->next->data;
    node->pre->next = node->next;
    node->next->pre = node->pre;
    iter_ptr next = (void*)node->next + sizeof(struct __list_node);
    deallocate(node, sizeof(struct __list_node) + p_private->memb_size);
    return next;
}
static iter_ptr insert(iter_ptr iter, void *x)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    struct __list_node *node = iter - sizeof(struct __list_node);
    void *ptr = allocate(sizeof(struct __list_node) + p_private->memb_size);
    struct __list_node *new_node = ptr;
    new_node->data = ptr + sizeof(struct __list_node);
    memcpy(new_node->data, x, p_private->memb_size);

    new_node->next = node;
    node->pre->next = new_node;
    new_node->pre = node->pre;
    node->pre = new_node;
    if (new_node == p_private->node->next)
        p_private->start.ptr = new_node->data;
    return iter;
}
static void push_front(void *x)
{
    list *this = pop_this();
    THIS(this).insert(*THIS(this).begin(), x);
}
static void push_back(void *x)
{
    list *this = pop_this();
    THIS(this).insert(*THIS(this).end(), x);
}
static void pop_front(void)
{
    list *this = pop_this();
    THIS(this).erase(*THIS(this).begin());
}
static void pop_back(void)
{
    list *this = pop_this();
    void *ptr = *THIS(this).end();
    struct __list_node *node = ptr - sizeof(struct __list_node);
    THIS(this).erase(node->pre->data);
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
            THIS(this).erase(node->data);
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
            THIS(this).erase(node->data);
        else
            node = next_node;
        next_node = next_node->next;
    }
}
static iter_ptr const *begin(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return &p_private->start.ptr;
}
static iter_ptr const *end(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return &p_private->finish.ptr;
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
static void *front(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->start.ptr;
}
static void *back(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    return p_private->node->pre->data;
}
static void splice(iter_ptr position, list *l, iter_ptr first, iter_ptr last)
{
    list *this = pop_this();
    if (last == NULL) {
        struct __list_node *node = first - sizeof(struct __list_node);
        last = node->next->data;
    }
    if(first != last) {
        transfer(position, first, last);
        __private_list *p_private = (__private_list*)this->__obj_private;
        __private_list *l_private = (__private_list*)l->__obj_private;
        if (l_private->start.ptr == first)
            l_private->start.ptr = last;
        if (p_private->start.ptr == position)
            p_private->start.ptr = first;
    }
}
static void merge(list *l, bool (*cmp)(iter_ptr, iter_ptr))
{
    list *this = pop_this();
    iter_ptr first1 = *THIS(this).begin();
    iter_ptr last1 = *THIS(this).end();
    iter_ptr first2 = *THIS(l).begin();
    iter_ptr last2 = *THIS(l).end();
    while (first1 != last1 && first2 != last2)
    {
        if (cmp(first1, first2)) {
            struct __list_node *next = first2 - sizeof(struct __list_node);
            next = next->next;
            THIS(this).splice(first1, l, first2, next->data);
            //transfer(first1, first2, next->data);
            first2 = next->data;
        } else {
            struct __list_node *next = first1 - sizeof(struct __list_node);
            next = next->next;
            first1 = next->data;
        }
    }
    if (first2 != last2)
        THIS(this).splice(first1, l, first2, last2);
}
static void reverse(void)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->__obj_private;
    if (p_private->node == p_private->node->next || p_private->node->next->next == p_private->node)
        return;
    iter_ptr first = *THIS(this).begin();
    while (first != *THIS(this).end())
    {
        struct __list_node *next = first - sizeof(struct __list_node);
        next = next->next;
        transfer(p_private->node->next->data, first, next->data);
        first = next->data;
    }
    p_private->start.ptr = p_private->node->next->data;
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
static void sort(bool (*cmp)(iter_ptr, iter_ptr))
{
    list *this = pop_this();
    __private_list *p_prvate = (__private_list*)this->__obj_private;
    if (p_prvate->node->next == p_prvate->node || p_prvate->node->next->next == p_prvate->node)
        return;
    list carry = creat_list(p_prvate->memb_size);
    list counter[64];
    int fill = 0;
    while (!THIS(this).empty())
    {
        THIS(&carry).splice(*THIS(&carry).begin(), this, *(THIS(this)).begin(), NULL);
        int i = 0;
        while (i < fill && !THIS(&counter[i]).empty())
        {
            THIS(&counter[i]).merge(&carry, cmp);
            THIS(&carry).swap(&counter[i++]);
        }
        if (i == fill) {
            init_list(&counter[fill], p_prvate->memb_size);
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

static void *iter_increment(void *p)
{
    pop_this();
    struct __list_node *node = p - sizeof(struct __list_node);
    node = node->next;
    return node->data;
}
static void *iter_decrement(void *p)
{
    pop_this();
    struct __list_node *node = p - sizeof(struct __list_node);
    node = node->pre;
    return node->data;
}

static __iterator_obj_func  __def_list_iter = {
        iter_increment,
        iter_decrement
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
    private->start = private->finish = init_iter(p_list, private->node->data, memb_size, &__def_list_iter);
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