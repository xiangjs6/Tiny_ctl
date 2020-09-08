//
// Created by xjs on 2020/9/8.
//

#include "tctl_list.h"
#include "tctl_common.h"
#include "tctl_allocator.h"

static iter_ptr erase(iter_ptr iter)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->OBJECT_PRIVATE;
    struct __list_node *node = iter - sizeof(struct __list_node);

    if (iter == p_private->node->next)
        p_private->start.ptr = node->next;
    node->pre->next = node->next;
    node->next->pre = node->pre;
    iter_ptr next = (void*)node->next + sizeof(struct __list_node);
    deallocate(node, sizeof(struct __list_node) + p_private->memb_size);
    return next;
}
static iter_ptr insert(iter_ptr iter, void *x)
{
    list *this = pop_this();
    __private_list *p_private = (__private_list *)this->OBJECT_PRIVATE;
    struct __list_node *node = iter - sizeof(struct __list_node);
    void *ptr = allocate(sizeof(struct __list_node) + p_private->memb_size);
    struct __list_node *new_node = ptr;
    new_node->data = ptr + sizeof(struct __list_node);

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
    THIS(this).erase(*THIS(this).begin());
}
static void clear(void)
{
    list *this = pop_this();
    while (!this->empty())
        THIS(this).pop_back();
}
static void remove(void *value);
static void unique(void);
static void splice(iter_ptr position, list *l, iter_ptr first, iter_ptr last);
static void merge(list *l);
static void reverse(void);
static void sort(void);
static iter_ptr const *begin(void);
static iter_ptr const *end(void);
static size_t size(void);
static bool empty(void);
static void *front(void);
static void *back(void);