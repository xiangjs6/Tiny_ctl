//
// Created by xjs on 2020/11/24.
//

#include "auto_release_pool.h"
#include <pthread.h>
#define GUARD NULL
#define PAGE_SIZE 4096
struct mem_node {
    size_t refCount;
    size_t size;
    ARP_ResId_t resident_id;
    dtorfunc_t dtor_func;
    char block[0];
};

struct release_pool {
    tree;
    struct release_pool * const pre_pool;
};

union pair_node {
    struct mem_node *node;
    struct release_pool *pool;
};
struct release_pool_page {
    union {
        struct {
            struct release_pool_page *nxt;
            struct release_pool_page * const pre;
            union pair_node stack[0];
        };
        char pad[PAGE_SIZE];
    };
};

struct release_pool_thread {
    union pair_node *next;
    struct release_pool_page *cur_page;
    struct release_pool *cur_pool;
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

static struct release_pool_page *creat_page(struct release_pool_page *cur_page)
{
    struct release_pool_page *new_page = malloc(sizeof(struct release_pool_page));
    if (cur_page)
        cur_page->nxt = new_page;
    new_page->nxt = NULL;
    *(void**)&new_page->pre = cur_page;
    return new_page;
}

static struct release_pool_page *destory_page(struct release_pool_page *cur_page)
{
    struct release_pool_page *pre = cur_page->pre;
    free(cur_page);
    return pre;
}

static struct release_pool *creat_pool(struct release_pool *cur_pool)
{
    struct release_pool *new_pool = malloc(sizeof(struct release_pool));
    //init tree
    *(void**)&new_pool->pre_pool = cur_pool;
    return new_pool;
}

static struct release_pool *destory_pool(struct release_pool *cur_pool);

static void push(void *p, struct release_pool_page **cur_page, void **next)
{
    if ((*cur_page)->pad + PAGE_SIZE == *next) {
        *cur_page = creat_page(*cur_page);
        *next = (*cur_page)->stack;
    }
    *next = p;
    (*(next))++;
}

static void *pop(struct release_pool_page **cur_page, void **next)
{
    if ((*cur_page)->stack == *next) {
        *cur_page = destory_page(*cur_page);
        *next = (*cur_page)->stack;
    }
    (*(next))--;
    void *p = *next;
    return p;
}

static pthread_once_t thread_once = PTHREAD_ONCE_INIT;
static pthread_key_t thread_key;

static void destr_thread(void*);
static void make_thread_key(void)
{
    pthread_key_create(&thread_key, destr_thread);
}

static struct release_pool_thread *make_thread_pool(void)
{
    struct release_pool_thread *p_pool_thread;
    p_pool_thread = malloc(sizeof(struct release_pool_thread));
    p_pool_thread->cur_page = creat_page(NULL);
    p_pool_thread->cur_pool = creat_pool(NULL);
    p_pool_thread->next = p_pool_thread->cur_page->stack;
    push(NULL, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    push(p_pool_thread->cur_pool, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    return p_pool_thread;
}

static struct release_pool_thread *get_thread_pool(void)
{
    struct release_pool_thread *p_pool_thread;
    if (!(p_pool_thread = pthread_getspecific(thread_key)))
        p_pool_thread = make_thread_pool();
    return p_pool_thread;
}
//public
int ARP_CreateRelPool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct release_pool_thread *p_pool_thread = get_thread_pool();
    p_pool_thread->cur_pool = creat_pool(p_pool_thread->cur_pool);
    push(NULL, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    push(p_pool_thread->cur_pool, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
}

void ARP_FreePool(void)
{
    pthread_once(&thread_once, make_thread_key);
    struct release_pool_thread *p_pool_thread = get_thread_pool();
    union pair_node *pre = pop(&p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    union pair_node *node;
    while ((node = pop(&p_pool_thread->cur_page, (void*)&p_pool_thread->next)))
    {
        pre->node->dtor_func(pre);
        free(pre);
        pre = node;
    }
    p_pool_thread->cur_pool = destory_pool(p_pool_thread->cur_pool);
}
