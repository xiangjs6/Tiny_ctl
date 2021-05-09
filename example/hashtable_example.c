#include <stdio.h>
#include "../include/tctl_int.h"
#include "../include/tctl_hashtable.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_common.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"
#define Import HASHTABLE, INT
long long cmp(Any _a, Any _b)
{
    Int a = THIS(_a).cast(T(Int));
    Int b = THIS(_b).cast(T(Int));
    return a->val - b->val;
}

void *get_key(void *x)
{
    return x;
}

int main(void)
{
    ARP_CreatePool();
    Hashtable ht = new(T(Hashtable), T(Int), VA_ANY(TEMP_VAR(void*, cmp), NULL), VA_ANY(TEMP_VAR(void*, hash_numeric), NULL),
                       VA_ANY(TEMP_VAR(void*, get_key), NULL), VAEND);
    long long x = 100;
    THIS(ht).insert_unique(VA(x));
    x = 47;
    THIS(ht).insert_unique(VA(x));
    x = 100;
    THIS(ht).insert_unique(VA(x));
    THIS(ht).insert_equal(VA(x));
    x = 1;
    THIS(ht).insert_equal(VA(x));
    THIS(ht).insert_equal(VA(x));
    x = 101;
    THIS(ht).insert_equal(VA(x));
    x = 3;
    THIS(ht).insert_equal(VA(x));
    x = 101;
    Iterator f_it = THIS(ht).find(VA(x));
    THIS(ht).erase(f_it);
    x = 52;
    THIS(ht).insert_equal(VA(x));
    Hashtable ht2 = new(T(Hashtable), T(Int), VA_ANY(TEMP_VAR(void*, cmp), NULL), VA_ANY(TEMP_VAR(void*, hash_numeric), NULL),
                       VA_ANY(TEMP_VAR(void*, get_key), NULL), VAEND);
    for (Iterator it = THIS(ht).begin(); !THIS(it).equal(THIS(ht).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    for (long long i = 0; i < 10000; i++) {
        THIS(ht2).insert_equal(VA(i));
    }
    THIS(ht).swap(ht2);
    THIS(ht).copy_from(ht2);
    for (Iterator it = THIS(ht).begin(); !THIS(it).equal(THIS(ht).end()); THIS(it).inc()) {
        printf("%lld ", ((Int) THIS(it).derefer())->val);
    }
    putchar('\n');
    x = 100;
    printf("%u %u %u\n", THIS(ht).count(VA(x)), THIS(ht).bucket_count(), THIS(ht).max_bucket_count());
    THIS(ht).clear();
    Hashtable ht3 = new(T(Hashtable), T(Int), ht2, VAEND);
    delete(ht);
    delete(ht2);
    for (Iterator it = THIS(ht3).begin(); !THIS(it).equal(THIS(ht3).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    delete(ht3);
    ARP_FreePool();
}
