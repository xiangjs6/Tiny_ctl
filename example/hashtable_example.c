#include <stdio.h>
#include "tctl_hashtable.h"
#include "auto_release_pool/auto_release_pool.h"
int cmp(const int *a, const int *b)
{
    if (*a > *b)
        return 1;
    else if (*a < *b)
        return -1;
    return 0;
}
size_t hash_func(const int *x)
{
    return *x;
}

void *get_key(const int *x)
{
    return x;
}
int main(void)
{
    ARP_CreatePool();
    hashtable ht = creat_hashtable(sizeof(int), cmp, hash_func, get_key);
    int x = 100;
    THIS(&ht).insert_unique(&x);
    x = 47;
    THIS(&ht).insert_unique(&x);
    x = 100;
    THIS(&ht).insert_unique(&x);
    THIS(&ht).insert_equal(&x);
    x = 1;
    THIS(&ht).insert_equal(&x);
    x = 101;
    THIS(&ht).insert_equal(&x);
    x = 3;
    THIS(&ht).insert_equal(&x);
    x = 101;
    iterator(int) f_it = THIS(&ht).find(&x);
    THIS(&ht).erase(f_it);
    hashtable ht2 = creat_hashtable(sizeof(int), cmp, hash_func, get_key);
    for (iterator(int) it = THIS(&ht2).begin(); !ITER(it).equal(THIS(&ht2).end()); ITER(it).increment())
    printf("%d ", *it->val);
    for (int i = 0; i < 1000; i++) {
        THIS(&ht2).insert_equal(&i);
    }
    THIS(&ht).swap(&ht2);
    THIS(&ht).copy_from(&ht2);
    for (iterator(int) it = THIS(&ht).begin(); !ITER(it).equal(THIS(&ht).end()); ITER(it).increment())
    printf("%d ", *it->val);
    putchar('\n');
    x = 100;
    printf("%d %d %llu\n", THIS(&ht).count(&x), THIS(&ht).bucket_count(), THIS(&ht).max_bucket_count());
    THIS(&ht).clear();
    destory_hashtable(&ht);
    destory_hashtable(&ht2);
    ARP_FreePool();
}
