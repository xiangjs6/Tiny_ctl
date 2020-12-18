//
// Created by xjs on 2020/12/14.
//

#include "tctl_hashtable.h"
#include "auto_release_pool/auto_release_pool.h"
#include "tctl_allocator.h"
#include "tctl_point_iterator.h"
#include <memory.h>
//private
static size_t bkt_num(const void *data, size_t n, HashFunc hash, ExtractKey get_key)
{
    return hash(get_key(data)) % n;
}

static void iter_increment(__iterator *iter)
{
    __hashtable_iter *_iter = (__hashtable_iter *)&iter->__inner.__address;
    hashtable *this = iter->__inner.obj_this;
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    if (!_iter->cur)
        return;
    if (!_iter->cur->next) {
        size_t n = bkt_num(_iter->cur->data, p_private->memb_size, p_private->hash, p_private->get_key);
        struct __bucket_node **node = (struct __bucket_node**)THIS(&p_private->buckets).at(n + 1);
        struct __bucket_node **end = (struct __bucket_node**)THIS(&p_private->buckets).at(p_private->finish);
        do
        {
            if (*node)
                break;
            node++;
        } while (node != end);
        _iter->cur = *node;
        _iter->val = _iter->cur ? _iter->cur->data : NULL;
    } else {
        _iter->cur = _iter->cur->next;
        _iter->val = _iter->cur->data;
    }
}

static bool iter_equal(const __iterator *it1, const __iterator *it2)
{
    __hashtable_iter *_iter1 = (__hashtable_iter *)&it1->__inner.__address;
    __hashtable_iter *_iter2 = (__hashtable_iter *)&it2->__inner.__address;
    return _iter1->cur == _iter2->cur;
}
static __iterator_obj_func  __def_hashtable_iter = {
        NULL,
        iter_increment,
        NULL,
        NULL,
        NULL,
        NULL,
        iter_equal
};

static const iterator_func __def_hashtable_iter_func = INIT_ITER_FUNC(&__def_hashtable_iter);
static const unsigned long __prime_list[] = {
        53, 97, 193, 389, 769,
        1543, 3079, 6151, 12289, 24593,
        49157, 98317, 196613, 393241, 786433,
        1572869, 3145739, 6291469, 12582917, 25165843,
        50331653, 100663319, 201326611, 402653189, 805306457,
        1610612741, 3221225473ul, 4294967291ul
};
static const int __num_primes = sizeof(__prime_list) / sizeof(unsigned long);

static unsigned long __next_prime(unsigned long n)
{
    for (int i = 0; i < __num_primes; i++)
        if (__prime_list[i] >= n)
            return __prime_list[i];
    return __prime_list[__num_primes - 1];
}

static struct __bucket_node *__new_node(void *x, size_t size)
{
    struct __bucket_node *n = allocate(sizeof(struct __bucket_node) + size);
    memcpy(n->data, x, size);
    n->next = NULL;
    return n;
}

static void __delete_node(struct __bucket_node *n, size_t size)
{
    deallocate(n, sizeof(struct __bucket_node) + size);
}

static void __insert(struct __bucket_node **pos, void *x, size_t size)
{
    struct __bucket_node *node = __new_node(x, size);
    node->next = *pos;
    *pos = node;
}

//public
static IterType begin(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    struct __inner_iterator *start = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
    init_iter(start, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
    __hashtable_iter *out_iter = (__hashtable_iter*)start->__address;
    out_iter->cur = *(struct __bucket_node**)THIS(&p_private->buckets).at(p_private->start);
    out_iter->val = out_iter->val;
    out_iter->ht = this;
    return start;
}

static IterType end(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    struct __inner_iterator *finish = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
    init_iter(finish, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
    __hashtable_iter *out_iter = (__hashtable_iter*)finish->__address;
    out_iter->cur = *(struct __bucket_node**)THIS(&p_private->buckets).at(p_private->finish);
    out_iter->val = out_iter->val;
    out_iter->ht = this;
    return finish;
}

static bool empty(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    return !p_private->nmemb;
}

static size_t size(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    return p_private->nmemb;
}

static size_t bucket_count(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    return THIS(&p_private->buckets).size();
}

static IterType find(void *x)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    size_t n = bkt_num(x, THIS(&p_private->buckets).size(), p_private->hash, p_private->get_key);
    struct __bucket_node **first = THIS(&p_private->buckets).at(n);
    for (struct __bucket_node *cur = *first; cur; cur = cur->next) {
        if (p_private->equal(p_private->get_key(cur->data), p_private->get_key(x))) {
            struct __inner_iterator *iter = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
            init_iter(iter, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
            __hashtable_iter *out_iter = (__hashtable_iter*)iter->__address;
            out_iter->cur = *first;
            out_iter->val = out_iter->val;
            out_iter->ht = this;
            return iter;
        }
    }
    return NULL;
}

static size_t count(void *x)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    size_t n = bkt_num(x, THIS(&p_private->buckets).size(), p_private->hash, p_private->get_key);
    struct __bucket_node **first = THIS(&p_private->buckets).at(n);
    size_t res = 0;
    for (struct __bucket_node *cur = *first; cur; cur = cur->next) {
        if (p_private->equal(p_private->get_key(cur->data), p_private->get_key(x)))
            res++;
    }
    return res;
}

static void resize(size_t new_size)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable *) this->__obj_private;
    const size_t old_size = THIS(&p_private->buckets).size();
    if (new_size > old_size) {
        size_t min_bkt = new_size, max_bkt = 0;
        size_t n = __next_prime(new_size);
        vector tmp = creat_vector(sizeof(struct __bucket_node*));
        THIS(&tmp).resize(n);
        struct __bucket_node **node = (struct __bucket_node**)THIS(&p_private->buckets).at(p_private->start);
        struct __bucket_node **end = (struct __bucket_node**)THIS(&p_private->buckets).at(p_private->finish);
        while (node != end)
        {
            if (*node) {
                struct __bucket_node *first = *node;
                while (first)
                {
                    size_t new_bucket = bkt_num(first->data, n, p_private->hash, p_private->get_key);
                    max_bkt = max_bkt > new_bucket ? max_bkt : new_bucket;
                    min_bkt = min_bkt < new_bucket ? min_bkt : new_bucket;
                    *node = first->next;
                    first->next = *(struct __bucket_node **) THIS(&tmp).at(new_bucket);
                    *(struct __bucket_node **) THIS(&tmp).at(new_bucket) = first;
                    first = *node;
                }
            }
            node++;
        }
        THIS(&p_private->buckets).swap(&tmp);
        destory_vector(&tmp);
        p_private->start = min_bkt;
        p_private->finish = max_bkt + 1;
    }
}

static IterType insert_unique(void *x)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    THIS(this).resize(p_private->nmemb + 1);
    const size_t n = bkt_num(x, THIS(&p_private->buckets).size(), p_private->hash, p_private->get_key);
    struct __bucket_node **first = THIS(&p_private->buckets).at(n);
    for (struct __bucket_node *cur = *first; cur; cur = cur->next) {
        if (p_private->equal(p_private->get_key(cur->data), p_private->get_key(x)) == 0)
            return NULL;
    }
    __insert(first, x, p_private->memb_size);
    if (n < p_private->start || !p_private->nmemb)
        p_private->start = n;
    if (n + 1 > p_private->finish || !p_private->nmemb)
        p_private->finish = n + 1;
    p_private->nmemb++;
    struct __inner_iterator *iter = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
    init_iter(iter, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
    __hashtable_iter *out_iter = (__hashtable_iter*)iter->__address;
    out_iter->cur = *first;
    out_iter->val = out_iter->cur->data;
    out_iter->ht = this;
    return iter;
}

static IterType insert_equal(void *x)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    THIS(this).resize(p_private->nmemb + 1);
    const size_t n = bkt_num(x, THIS(&p_private->buckets).size(), p_private->hash, p_private->get_key);
    struct __bucket_node **first = THIS(&p_private->buckets).at(n);
    __insert(first, x, p_private->memb_size);
    if (n < p_private->start || !p_private->nmemb)
        p_private->start = n;
    if (n + 1 > p_private->finish || !p_private->nmemb)
        p_private->finish = n + 1;
    p_private->nmemb++;
    struct __inner_iterator *iter = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
    init_iter(iter, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
    __hashtable_iter *out_iter = (__hashtable_iter*)iter->__address;
    out_iter->cur = *first;
    out_iter->val = out_iter->cur->data;
    out_iter->ht = this;
    return iter;
}

static void erase(IterType it)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    __hashtable_iter *__iter = (__hashtable_iter*)((struct __inner_iterator*)it)->__address;
    if (__iter->ht != this)
        return;
    struct __bucket_node *cur = __iter->cur;
    if (cur->next) {
        memcpy(cur->data, cur->next->data, p_private->memb_size);
        struct __bucket_node *tmp = cur->next;
        cur->next = cur->next->next;
        cur = tmp;
    }
    __delete_node(cur, p_private->memb_size);
    p_private->nmemb--;
    //判断是否需要更新start和finish
    if (!p_private->nmemb) {
        p_private->start = p_private->finish = 0;
        return;
    }
    if (*(struct __bucket_node**)THIS(&p_private->buckets).at(p_private->start) == NULL) {
        int i;
        for (i = p_private->start + 1; *(struct __bucket_node**)THIS(&p_private->buckets).at(i) == NULL; i++);
        p_private->start = i;
    } else if (*(struct __bucket_node**)THIS(&p_private->buckets).at(p_private->finish - 1) == NULL) {
        int i;
        for (i = p_private->finish - 2; *(struct __bucket_node**)THIS(&p_private->buckets).at(i) == NULL; i--);
        p_private->finish = i + 1;
    }
}

static void clear(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    for (size_t i = p_private->start; i != p_private->finish; i++) {
        struct __bucket_node **node = THIS(&p_private->buckets).at(i);
        struct __bucket_node *cur = *node;
        while (cur)
        {
            *node = cur->next;
            __delete_node(cur, p_private->memb_size);
            cur = *node;
        }
    }
    p_private->nmemb = 0;
    p_private->start = p_private->finish = 0;
}

static void copy_from(const hashtable *ht)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    __private_hashtable *p_ht_private = (__private_hashtable*)ht->__obj_private;
    THIS(this).clear();
    THIS(&p_private->buckets).resize(THIS(&p_ht_private->buckets).size());//增加vector接口再后修改
    memcpy(p_private, p_ht_private, offsetof(__private_hashtable, buckets));
    for (size_t i = p_ht_private->start; i != p_ht_private->finish; i++) {
        struct __bucket_node *cur = *(struct __bucket_node**)THIS(&p_private->buckets).at(i);
        if (cur) {
            struct __bucket_node *copy = __new_node(cur->data, p_ht_private->memb_size);
            *(struct __bucket_node**)THIS(&p_private->buckets).at(i) = copy;
            for (struct __bucket_node *next = cur->next; next; next = next->next) {
                copy->next = __new_node(next->data, p_private->memb_size);
                copy = copy->next;
            }
        }
    }
}

static const hashtable _def_hashtable = {
        begin,
        end,
        empty,
        size,
        insert_unique,
        insert_equal,
        erase,
        clear,
        find,
        count,
        bucket_count,
        resize,
        copy_from
};
void init_hashtable(hashtable *p_ht, size_t memb_size, Compare equal, HashFunc hash, ExtractKey get_key)
{
    *p_ht = _def_hashtable;
    __private_hashtable *p_private = (__private_hashtable*)p_ht->__obj_private;
    init_vector(&p_private->buckets, sizeof(struct __bucket_node*));
    p_private->nmemb = 0;
    *(size_t*)&p_private->memb_size = memb_size;
    p_private->equal = equal;
    p_private->hash = hash;
    p_private->get_key = get_key;
    p_private->start = p_private->finish = 0;
}

void destory_hashtable(hashtable *p_ht)
{
    __private_hashtable *p_private = (__private_hashtable*)p_ht->__obj_private;
    THIS(p_ht).clear();
    destory_vector(&p_private->buckets);
}

hashtable creat_hashtable(size_t memb_size, Compare equal, HashFunc hash, ExtractKey get_key)
{
    hashtable ht;
    init_hashtable(&ht, memb_size, equal, hash, get_key);
    return ht;
}
