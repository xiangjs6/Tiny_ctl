//
// Created by xjs on 2020/11/24.
//

#include "auto_release_pool.h"
#include "rb_tree.h"
#include <pthread.h>
#include <memory.h>

#include <stddef.h>
#define container_of(ptr, type, member) ((type *) ((char *)(ptr) - offsetof(type, member)))

#define GUARD NULL
#define PAGE_SIZE 4096
union pair_node;
struct mem_node;

struct mem_block {
    struct mem_node *p_point_node;
    size_t size;
    ARP_ResId_t resident_id;
    dtorfunc_t dtor_func;
    char block[0];
};

struct mem_node {
    size_t refCount;
    struct mem_block *p_block;
};

struct Rel_pool {
    rb_tree tree;
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
    Res_ctorfunc_t ctor_func;
    struct mem_block *p_block;
};

static struct {
    size_t len;
    size_t cur;
    pthread_spinlock_t lock;
    union {
        unsigned *bit_flag;
        unsigned bit_flag_self[sizeof(unsigned*) / sizeof(unsigned)];
    };
} resident_memory = {sizeof(unsigned*) / sizeof(unsigned), 0, 0, (unsigned*)~0llu};

static unsigned *fullfill_flag(unsigned *bit_flag)
{
    size_t new_len = resident_memory.len * 2;
    if (bit_flag == resident_memory.bit_flag_self) {
        bit_flag = realloc(NULL, new_len * sizeof(unsigned));
        memcpy(bit_flag, resident_memory.bit_flag_self, resident_memory.len * sizeof(unsigned));
        resident_memory.bit_flag = bit_flag;
    } else {
        bit_flag = resident_memory.bit_flag = realloc(bit_flag, new_len * sizeof(unsigned));
    }
    memset(bit_flag + resident_memory.len, -1, sizeof(unsigned) * resident_memory.len);
    resident_memory.len = new_len;
    return bit_flag;
}

static unsigned long long cal_pos(unsigned mask, size_t i)
{
    unsigned long long pos = 0;
    while (mask) {
        mask >>= 1u;
        pos++;
    }
    return i * sizeof(unsigned) * 8 + pos;
}

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
    init_rb_tree(&new_pool->tree);
    new_pool->node_size = 0;
    *(void**)&new_pool->pre_pool = cur_pool;
    return new_pool;
}

static struct Rel_pool *destory_pool(struct Rel_pool *cur_pool)
{
    destory_rb_tree(&cur_pool->tree);
    struct Rel_pool *pre = cur_pool->pre_pool;
    free(cur_pool);
    return pre;
}

static void push(void *p, struct Rel_page **cur_page, union pair_node **next)
{
    if (!*cur_page || !*next || (*cur_page)->pad + PAGE_SIZE == (void*)*next) {
        *cur_page = creat_page(*cur_page);
        *next = (*cur_page)->stack;
    }
    //union pair_node *old_next = *next;
    (*next)->ptr = p;
    (*(next))++;
    //return old_next;
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
    pthread_spin_destroy(&resident_memory.lock);
    pthread_key_delete(thread_key);
    free(p_pool_thread);
}
static void make_thread_key(void)
{
    pthread_key_create(&thread_key, destr_thread);
    pthread_spin_init(&resident_memory.lock, PTHREAD_PROCESS_PRIVATE);
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
        ARP_Release(pre->node->p_block->block);
        pre = node;
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
    struct mem_block *block = container_of(pMemLoc, struct mem_block, block);
    if (!block->p_point_node) {
        free(block);
        return 0;
    }
    struct mem_node *node = block->p_point_node;
    if(!--node->refCount) {
        if (block->dtor_func)
            block->dtor_func(block->block);
        free(node);
        free(block);
        return 0;
    }
    return node->refCount;
}

void *ARP_Retain(void *pMemLoc)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_block *block = container_of(pMemLoc, struct mem_block, block);
    struct mem_node *node = block->p_point_node;
    node->refCount++;
    if (block->resident_id.major != 0) {
        struct rb_tree_node *rb_node = insert_unique(&p_pool_thread->cur_pool->tree, block->resident_id);
        struct resident_node *res_node = rb_node->p_node;
        struct mem_block *new_block = malloc(block->size);
        *new_block = *block;
        if (res_node->ctor_func)
            res_node->ctor_func(new_block->block, block->block);
        else
            memcpy(new_block, block, block->size);
        res_node->p_block = new_block;
        block->resident_id.major = 0;
    }
    return pMemLoc;
}

int ARP_JoinARel(void *pMemLoc)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_block *block = container_of(pMemLoc, struct mem_block, block);
    if (block->p_point_node)
        return -1;
    struct mem_node *node = block->p_point_node = malloc(sizeof(struct mem_node));
    node->p_block = block;
    node->refCount = 1;
    push(node, &p_pool_thread->cur_page, (void*)&p_pool_thread->next);
    p_pool_thread->cur_pool->node_size++;
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
    struct mem_block *block = malloc(len + sizeof(struct mem_block));
    block->size = len + sizeof(struct mem_block);
    block->dtor_func = dtorFunc;
    block->resident_id = (ARP_ResId_t) {0, 0};
    block->p_point_node = NULL;
    return block->block;
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
    struct mem_block *block = container_of(pMemLoc, struct mem_block, block);
    block = realloc(block, size + sizeof(struct mem_block));
    block->size = size + sizeof(struct mem_block);
    if (block->p_point_node)
        block->p_point_node->p_block = block;
    return block->block;
}

ARP_ResId_t ARP_AssignResId(unsigned long long minor)
{
    pthread_once(&thread_once, make_thread_key);
    ARP_ResId_t res_id = {0, minor};
    pthread_spin_lock(&resident_memory.lock);
    unsigned *bit_flag = resident_memory.len == (sizeof(unsigned *) / sizeof(unsigned)) ? resident_memory.bit_flag_self
                                                                                        : resident_memory.bit_flag;
    size_t old_cur = resident_memory.cur;
    size_t i = old_cur;
    do
    {
        if (bit_flag[i])
            break;
        i = (i + 1) % resident_memory.len;
    } while (i != old_cur);
    if (!bit_flag[i]) {
        i = resident_memory.len;
        bit_flag = fullfill_flag(bit_flag);
    }
    resident_memory.cur = i;
    unsigned mask = bit_flag[i] & ~(bit_flag[i] - 1);
    bit_flag[i] &= ~mask;
    pthread_spin_unlock(&resident_memory.lock);
    res_id.major = cal_pos(mask, i);
    return res_id;
}

void ARP_FreeResId(ARP_ResId_t id)
{
    pthread_once(&thread_once, make_thread_key);
    size_t i = (id.major - 1) / (sizeof(unsigned) * 8);
    size_t pos = (id.major - 1) % (sizeof(unsigned) * 8);
    pthread_spin_lock(&resident_memory.lock);
    unsigned *bit_flag = resident_memory.len == (sizeof(unsigned *) / sizeof(unsigned)) ? resident_memory.bit_flag_self
                                                                                        : resident_memory.bit_flag;
    bit_flag[i] |= 1u << pos;
    pthread_spin_unlock(&resident_memory.lock);
}

int ARP_SetResId(void *pMemLoc, ARP_ResId_t id, Res_ctorfunc_t ctorFunc)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct mem_block *block = container_of(pMemLoc, struct mem_block, block);
    if (block->resident_id.major != 0)
        return -1;
    block->resident_id = id;
    struct rb_tree_node *rb_node = insert_unique(&p_pool_thread->cur_pool->tree, id);
    if (!rb_node->p_node)
        rb_node->p_node = malloc(sizeof(struct resident_node));
    struct resident_node *res_node = rb_node->p_node;
    res_node->p_block = block;
    res_node->ctor_func = ctorFunc;
    return 0;
}

void *ARP_AllocWithResId(ARP_ResId_t id)
{
    pthread_once(&thread_once, make_thread_key);
    struct Rel_thread *p_pool_thread = get_thread_pool();
    struct rb_tree_node *rb_node = insert_unique(&p_pool_thread->cur_pool->tree, id);
    struct resident_node *res_node = rb_node->p_node;
    if (!rb_node->p_node) {
        struct rb_tree_node *old_node;
        struct Rel_pool *pool = p_pool_thread->cur_pool->pre_pool;
        while (pool && !(old_node = find(&pool->tree, id)))
            pool = pool->pre_pool;
        if (!pool)
            return NULL;
        struct resident_node *old_res_node = old_node->p_node;
        res_node = rb_node->p_node = malloc(sizeof(struct resident_node));
        res_node->ctor_func = old_res_node->ctor_func;
        res_node->p_block = malloc(old_res_node->p_block->size);
        *res_node->p_block = *old_res_node->p_block;
        if (res_node->ctor_func)
            res_node->ctor_func(res_node->p_block, old_res_node->p_block);
        else
            memcpy(res_node->p_block, old_res_node->p_block, old_res_node->p_block->size);
        if (res_node->p_block->p_point_node) {
            res_node->p_block->p_point_node = NULL;
            ARP_JoinARel(res_node->p_block->block);
        }
    }
    return res_node->p_block->block;
}