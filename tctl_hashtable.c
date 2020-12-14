//
// Created by xjs on 2020/12/14.
//

#include "tctl_hashtable.h"
#include "auto_release_pool/auto_release_pool.h"
#include "tctl_point_iterator.h"
//private
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
    const __iterator *first = POINT_ITER(__prime_list, __prime_list);
    const __iterator *last = POINT_ITER(__prime_list + __num_primes, __prime_list);
    //const __iterator *pos = lower_bound(first, last, n);
    //return ITER(pos).equal(last) ? __prime_list[__num_primes - 1] : *(unsigned long*)pos->val;
}
//public

static __iterator_obj_func  __def_hashtable_iter = {
        NULL,
        //iter_increment,
        NULL,
        NULL,
        NULL,
        NULL,
        //iter_equal
};

static const iterator_func __def_hashtable_iter_func = INIT_ITER_FUNC(&__def_hashtable_iter);

static IterType begin(void)
{
    hashtable *this = pop_this();
    __private_hashtable *p_private = (__private_hashtable*)this->__obj_private;
    struct __inner_iterator *start = ARP_MallocARel(sizeof(struct __inner_iterator) + sizeof(__hashtable_iter));
    init_iter(start, sizeof(__hashtable_iter), this, p_private->memb_size, &__def_hashtable_iter_func);
    __hashtable_iter *out_iter = (__hashtable_iter*)start->__address;
    out_iter->cur = *p_private->start;
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
    out_iter->cur = *p_private->finish;
    out_iter->val = out_iter->val;
    out_iter->ht = this;
    return finish;
}

void init_hashtable(hashtable *p_ht, size_t memb_size, Compare equal, HashFunc hash, ExtractKey get_key)
{
    __private_hashtable *p_private = (__private_hashtable*)p_ht->__obj_private;
    init_vector(&p_private->buckets, sizeof(struct __bucket_node*));
    p_private->nmemb = 0;
    p_private->memb_size = memb_size;
    p_private->equal = equal;
    p_private->hash = hash;
    p_private->get_key = get_key;
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
