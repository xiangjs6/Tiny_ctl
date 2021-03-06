#include <stdio.h>
#include "../include/tctl_hashtable.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_common.h"
#define Import HASHTABLE
int cmp(FormWO_t _a, FormWO_t _b)
{
    int *a = _a.mem;
    int *b = _b.mem;
    return *a - *b;
}

FormWO_t get_key(FormWO_t x)
{
    return x;
}

int main(void)
{
    ARP_CreatePool();
    Hashtable ht = new(T(Hashtable), VA(T(int), VA_FUNC(cmp), VA_FUNC(hash_numeric), VA_FUNC(get_key)));
    int x = 100;
    THIS(ht).insert_unique(VA(x));
    x = 47;
    THIS(ht).insert_unique(VA(x));
    x = 100;
    THIS(ht).insert_unique(VA(x));
    THIS(ht).insert_equal(VA(x));
    x = 1;
    THIS(ht).insert_equal(VA(x));
    x = 101;
    THIS(ht).insert_equal(VA(x));
    x = 3;
    THIS(ht).insert_equal(VA(x));
    x = 101;
    Iterator f_it = THIS(ht).find(VA(x));
    THIS(ht).erase(f_it);
    Hashtable ht2 = new(T(Hashtable), VA(T(int), VA_FUNC(cmp), VA_FUNC(hash_numeric), VA_FUNC(get_key)));
    for (Iterator it = THIS(ht).begin(); !THIS(it).equal(VA(THIS(ht).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    for (int i = 0; i < 100000; i++) {
        THIS(ht2).insert_equal(VA(i));
    }
    THIS(ht).swap(ht2);
    THIS(ht).copy_from(ht2);
    for (Iterator it = THIS(ht).begin(); !THIS(it).equal(VA(THIS(ht).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    x = 100;
    printf("%lu %lu %lu\n", THIS(ht).count(VA(x)), THIS(ht).bucket_count(), THIS(ht).max_bucket_count());
    THIS(ht).clear();
    Hashtable ht3 = new(T(Hashtable), VA(T(int), ht2));
    delete(ht);
    delete(ht2);
    for (Iterator it = THIS(ht3).begin(); !THIS(it).equal(VA(THIS(ht3).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    delete(ht3);
    ARP_FreePool();
}
