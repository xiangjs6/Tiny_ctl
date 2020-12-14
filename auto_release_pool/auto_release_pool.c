//
// Created by xjs on 2020/11/24.
//

#include "auto_release_pool.h"
#include "../tctl_debug.h"
#include "../tctl_common.h"
#include "../tctl_allocator.h"
#include <pthread.h>
#include <stddef.h>
//#include <stdlib.h>

#define GUARD NULL
#define PAGE_SIZE 4096
struct mem_node {
    size_t refCount;
    size_t size;
    dtorfunc_t dtor_func;
    struct mem_node **p_pool_node;
    char block[0];
};

struct Rel_pool {
    struct Rel_pool * const pre_pool;
    size_t node_size;
};

struct Rel_page {
    union {
        struct {
            struct Rel_page * const pre;
            struct mem_node *stack[0];
        };
        char pad[PAGE_SIZE];
    };
};

struct Rel_thread {
    struct mem_node **next;
    struct Rel_page *cur_page;
    struct Rel_pool *cur_pool;
    size_t pool_size;
};

static struct Rel_page *creat_page(struct Rel_page *cur_page)
{
    struct Rel_page *new_page = allocate(sizeof(struct Rel_page));
    //struct Rel_page *new_page = malloc(sizeof(struct Rel_page));
    *(void**)&new_page->pre = cur_page;
    return new_page;
}

static struct Rel_page *destory_page(struct Rel_page *cur_page)
{
    struct Rel_page *pre = cur_page->pre;
    deallocate(cur_page, sizeof(struct Rel_page));
    //free(cur_page);
    return pre;
}

static struct Rel_pool *creat_pool(struct Rel_pool *cur_pool)
{
    struct Rel_pool *new_pool = allocate(sizeof(struct Rel_pool));
    //struct Rel_pool *new_pool = malloc(sizeof(struct Rel_pool));
    new_pool->node_size = 0;
    *(void**)&new_pool->pre_pool = cur_pool;
    return new_pool;
}

static struct Rel_pool *destory_pool(struct Rel_pool *cur_pool)
{
    struct Rel_pool *pre = cur_pool->pre_pool;
    deallocate(cur_pool, sizeof(struct Rel_pool));
    //free(cur_pool);
    return pre;
}

static struct mem_node **push(void *p, struct Rel_page **cur_page, struct mem_node ***next)
{
    if (!*cur_page || !*next || (*cur_page)->pad + PAGE_SIZE == (void*)*next) {
        *cur_page = creat_page(*cur_page);
        *next = (void*)(*cur_page)->stack;
    }
    struct mem_node **old_next = *next;
    **next = p;
    (*(next))++;
    return old_next;
}

static struct mem_node *pop(struct Rel_page **cur_page, struct mem_node ***next)
{
    if (!*cur_page || !*next)
        return NULL;
    (*(next))--;
    struct mem_node *p = **next;
    if ((*cur_page)->stack == (void*)*next) {
        *cur_page = destory_page(*cur_page);
        *next = *cur_page ? (void*)(*cur_page)->pad + PAGE_SIZE : NULL;
    }
    return p;
}

static pthread_once_t thread_once = PTHREAD_ONCE_INIT;
static pthread_key_t thread_key;

static void destr_thread(void *x)
{
    struct Rel_thread *p_pool_thread = x;
    while (p_pool_thread->pool_size)
        ARP_FreePool();
    //pthread_key_delete(thread_key);
    deallocate(p_pool_thread, sizeof(struct Rel_thread));
    //free(p_pool_thread);
}
static void make_thread_key(void)
{
    pthread_key_create(&thread_key, destr_thread);
}
static struct Rel_thread *make_thread_pool(void)
{
    struct Rel_thread *p_pool_thread;
    p_pool_thread = allocate(sizeof(struct Rel_thread));
    //p_pool_thread = malloc(sizeof(struct Rel_thread));
    p_pool_thread->cur_page = NULL;
    p_pool_thread->cur_pool = NULL;
    p_pool_thread->next = NULL;
    //push(GUARD, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    //push(p_pool_thread->cur_pool, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    p_pool_thread->pool_size = 0;
    return p_pool_thread;
}

static struct Rel_thread *get_thread_pool(void)
{
    struct Rel_thread *p_pool_thread;
    if (!(p_pool_thread = pthread_getspecific(thread_key))) {
        p_pool_thread = make_thread_pool();
        pthread_setspecific(thread_key, p_pool_thread);
    }
    return p_pool_thread;
}

//public
void ARP_CreatePool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    p_pool_thread->cur_pool = creat_pool(p_pool_thread->cur_pool);
    push(GUARD, &p_pool_thread->cur_page, &p_pool_thread->next);
    //push(p_pool_thread->cur_pool, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    p_pool_thread->pool_size++;
}

void ARP_FreePool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_node *node;
    struct mem_node **old_next = p_pool_thread->next;
    while ((node = pop(&p_pool_thread->cur_page, &p_pool_thread->next)))
    {
        //if (node->p_pool_node && *node->p_pool_node == node)
	if (node->p_pool_node == old_next - 1)
            node->p_pool_node = NULL;
        ARP_Release(node->block);
	old_next = p_pool_thread->next;
    }
    p_pool_thread->cur_pool = destory_pool(p_pool_thread->cur_pool);
    p_pool_thread->pool_size--;
}

int ARP_GetPoolNodesCount(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    return p_pool_thread->cur_pool->node_size;
}

int ARP_GetPoolsCount(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    return p_pool_thread->pool_size;
}

int ARP_Release(void *pMemLoc)
{
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    if(!--node->refCount) {
        if (node->dtor_func)
            node->dtor_func(node->block);
        deallocate(node, node->size);
        //free(node);
        return 0;
    }
    return node->refCount;
}

void *ARP_Retain(void *pMemLoc)
{
    pthread_once(&thread_once, make_thread_key);
    //struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    node->refCount++;
    return pMemLoc;
}

int ARP_JoinARel(void *pMemLoc)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    if (!p_pool_thread->pool_size)
        exit_log("There is no memory pool available\n");
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    if (node->p_pool_node)
        return -1;
    //node->refCount++;
    node->p_pool_node = push(node, &p_pool_thread->cur_page, &p_pool_thread->next);
    p_pool_thread->cur_pool->node_size++;
    return 0;
}

int ARP_ExitARel(void *pMemLoc)
{
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    if (!node->p_pool_node)
        return -1;
    node->p_pool_node = NULL;
    node->refCount++;
    return 0;
}

void *ARP_Calloc(size_t num, size_t size)
{
    return ARP_CallocDtor(num, size, NULL);
}

void * ARP_CallocDtor(size_t num, size_t size, dtorfunc_t dtorFunc)
{
    return ARP_MallocDtor(num * size, dtorFunc);
}

void *ARP_Malloc(size_t len)
{
    return ARP_MallocDtor(len, NULL);
}

void *ARP_MallocDtor(size_t len, dtorfunc_t dtorFunc)
{
    struct mem_node *node = allocate(len + sizeof(struct mem_node));
    //struct mem_node *node = malloc(len + sizeof(struct mem_node));
    node->size = len + sizeof(struct mem_node);
    node->dtor_func = dtorFunc;
    node->p_pool_node = NULL;
    node->refCount = 1;
    return node->block;
}

void *ARP_CallocARel(size_t num, size_t size)
{
    return ARP_CallocDtor(num, size, NULL);
}

void * ARP_CallocARelDtor(size_t num, size_t size, dtorfunc_t dtorFunc)
{
    return ARP_MallocARelDtor(num * size, dtorFunc);
}

void *ARP_MallocARel(size_t len)
{
    return ARP_MallocARelDtor(len, NULL);
}

void *ARP_MallocARelDtor(size_t len, dtorfunc_t dtorFunc)
{
    //pthread_once(&thread_once, make_thread_key);
    //struct Rel_thread *p_pool_thread = get_thread_pool();
    void *block = ARP_MallocDtor(len, dtorFunc);
    ARP_JoinARel(block);
    //struct mem_node *node = container_of(block, struct mem_node, block);
    //node->p_point_self = push(node, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    return block;
}

void *ARP_Realloc(void *pMemLoc, size_t size)
{
    if (!pMemLoc)
        return ARP_Malloc(size);
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    node = reallocate(node, node->size, size + sizeof(struct mem_node));
    //node = realloc(node, size + sizeof(struct mem_node));
    node->size = size + sizeof(struct mem_node);
    if (node->p_pool_node)
        *node->p_pool_node = node;
    return node->block;
}
