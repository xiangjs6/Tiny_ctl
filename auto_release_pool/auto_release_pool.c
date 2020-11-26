//
// Created by xjs on 2020/11/24.
//

#include "auto_release_pool.h"
#include <pthread.h>

#include <stddef.h>
#define container_of(ptr, type, member) ((type *) ((char *)(ptr) - offsetof(type, member)))

#define GUARD NULL
#define PAGE_SIZE 4096
union pair_node;
struct mem_node {
    union pair_node *p_point_self;
    size_t refCount;
    size_t size;
    ARP_ResId_t resident_id;
    dtorfunc_t dtor_func;
    char block[0];
};

struct Rel_pool {
    //tree;
    struct Rel_pool * const pre_pool;
    size_t node_size;
};

union pair_node {
    void *ptr;
    struct mem_node *node;
    struct Rel_pool *pool;
};
struct Rel_page {
    union {
        struct {
            struct Rel_page *nxt;
            struct Rel_page * const pre;
            union pair_node stack[0];
        };
        char pad[PAGE_SIZE];
    };
};

struct Rel_thread {
    union pair_node *next;
    struct Rel_page *cur_page;
    struct Rel_pool *cur_pool;
    size_t pool_size;
};

struct resident_node {
    ARP_ResId_t id;
    Res_ctorfunc_t ctor_func;
    struct mem_node *node;
};

static struct {
    size_t len;
    size_t cur;
    pthread_spinlock_t lock;
    unsigned *bit_flag;
} resident_memory = {0, 0, 0, NULL};

static struct Rel_page *creat_page(struct Rel_page *cur_page)
{
    struct Rel_page *new_page = malloc(sizeof(struct Rel_page));
    if (cur_page)
        cur_page->nxt = new_page;
    new_page->nxt = NULL;
    *(void**)&new_page->pre = cur_page;
    return new_page;
}

static struct Rel_page *destory_page(struct Rel_page *cur_page)
{
    struct Rel_page *pre = cur_page->pre;
    free(cur_page);
    return pre;
}

static struct Rel_pool *creat_pool(struct Rel_pool *cur_pool)
{
    struct Rel_pool *new_pool = malloc(sizeof(struct Rel_pool));
    //init tree
    new_pool->node_size = 0;
    *(void**)&new_pool->pre_pool = cur_pool;
    return new_pool;
}

static struct Rel_pool *destory_pool(struct Rel_pool *cur_pool)
{
    //destory tree
    struct Rel_pool *pre = cur_pool->pre_pool;
    free(cur_pool);
    return pre;
}

static union pair_node *push(void *p, struct Rel_page **cur_page, union pair_node **next)
{
    if (!*cur_page || !*next || (*cur_page)->pad + PAGE_SIZE == (void*)*next) {
        *cur_page = creat_page(*cur_page);
        *next = (*cur_page)->stack;
    }
    union pair_node *old_next = *next;
    (*next)->ptr = p;
    (*(next))++;
    return old_next;
}

static union pair_node *pop(struct Rel_page **cur_page, union pair_node **next)
{
    if (!*cur_page || !*next)
        return NULL;
    (*(next))--;
    union pair_node *p = *next;
    if ((*cur_page)->stack == *next) {
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
    free(p_pool_thread);
}
static void make_thread_key(void)
{
    pthread_key_create(&thread_key, destr_thread);
}

static struct Rel_thread *make_thread_pool(void)
{
    struct Rel_thread *p_pool_thread;
    p_pool_thread = malloc(sizeof(struct Rel_thread));
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
void ARP_CreateRelPool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    p_pool_thread->cur_pool = creat_pool(p_pool_thread->cur_pool);
    push(GUARD, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    push(p_pool_thread->cur_pool, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    p_pool_thread->pool_size++;
}

void ARP_FreePool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    union pair_node *pre = pop(&p_pool_thread->cur_page, &p_pool_thread->next);
    if (!pre->ptr)
        return;
    union pair_node *node;
    while ((node = pop(&p_pool_thread->cur_page, &p_pool_thread->next))->ptr)
    {
        ARP_Release(pre->node->block);
        pre = node;
    }
    p_pool_thread->cur_pool = destory_pool(p_pool_thread->cur_pool);
    p_pool_thread->pool_size--;
    //if (!--p_pool_thread->pool_size)
    //    ARP_CreateRelPool();
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
            node->dtor_func(node);
        free(node);
        return 0;
    }
    return node->refCount;
}

void *ARP_Retain(void *pMemLoc)
{
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    node->refCount++;
    return pMemLoc;
}

int ARP_JoinARel(void *pMemLoc)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    if (node->p_point_self)
        return -1;
    node->p_point_self = push(node, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
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
    struct mem_node *node = malloc(len + sizeof(struct mem_node));
    node->size = len + sizeof(struct mem_node);
    node->refCount = 1;
    node->p_point_self = NULL;
    node->resident_id = (ARP_ResId_t){0, 0};
    node->dtor_func = dtorFunc;
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
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    void *block = ARP_MallocDtor(len, dtorFunc);
    struct mem_node *node = container_of(block, struct mem_node, block);
    node->p_point_self = push(node, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    p_pool_thread->cur_pool->node_size++;
    return node->block;
}

void *ARP_Realloc(void *pMemLoc, size_t size)
{
    if (!pMemLoc)
        return ARP_Malloc(size);
    struct mem_node *node = container_of(pMemLoc, struct mem_node, block);
    node = realloc(node, size + sizeof(struct mem_node));
    node->size = size + sizeof(struct mem_node);
    if (node->p_point_self)
        node->p_point_self->node = node;
    return node->block;
}