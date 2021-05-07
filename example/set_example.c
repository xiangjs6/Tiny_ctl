#include <stdio.h>
#include <stdlib.h>
#include "../include/tctl_int.h"
#include "../include/tctl_set.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_any.h"
#include "../include/tctl_arg.h"

#define Import SET, VECTOR, ITERATOR, INT, ANY

int cmp(void *_a, void *_b)
{
    Int a = THIS((MetaObject)_a).cast(T(Int));
    Int b = THIS((MetaObject)_b).cast(T(Int));
    return (int)(a->val - b->val);
}

int main(void)
{
    ARP_CreatePool();
    int i;
    int ia[5] = {0, 1, 2, 3, 4};
    Set s = new(T(Set), T(Int), VA_ANY(TEMP_VAR(void*, &cmp), NULL), VAEND);
    for (i = 0; i < 5; i++)
        THIS(s).insert(VA(ia[i]));
    printf("size:%u\n", THIS(s).size());
    printf("3 count=%u\n", THIS(s).count(VA(ia[4])));
    i = 3;
    THIS(s).insert(VA(i));
    printf("size:%u\n", THIS(s).size());
    printf("3 count=%u\n", THIS(s).count(VA(ia[4])));
    i = 5;
    THIS(s).insert(VA(i));
    printf("size:%u\n", THIS(s).size());
    printf("3 count=%u\n", THIS(s).count(VA(ia[4])));
    i = 1;
    THIS(s).erase(THIS(s).find(VA(i)));
    THIS(s).erase(THIS(s).end());
    THIS(s).erase(THIS(s).begin());
    for (Iterator it = THIS(s).begin(); !THIS(it).equal(THIS(s).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    printf("size:%u\n", THIS(s).size());
    printf("3 count=%u\n", THIS(s).count(VA(ia[4])));
    printf("1 count=%u\n", THIS(s).count(VA(ia[1])));
    Iterator ite1 = THIS(s).begin();
    Iterator ite2 = THIS(s).end();
    for (; !THIS(ite1).equal(ite2); THIS(ite1).inc())
        printf("%lld ", ((Int)THIS(ite1).derefer())->val);
    putchar('\n');
    Any o = VA(ia[3]);
    THIS(ite1).assign(VA(THIS(s).find(o)));
    if (!THIS(ite1).equal(ite2))
        printf("3 found\n");
    o = (void*)VA(ia[1]);
    THIS(ite1).assign(THIS(s).find(o));
    if (THIS(ite1).equal(ite2))
        printf("1 not found\n");
    delete(s);

    Vector v = new(T(Vector), T(Int), VAEND);
    for (int i = 0; i < 10; i++) {
        int temp = (int)rand() % 10;
        printf("%d ", temp);
        THIS(v).push_back(VA(temp));
    }
    putchar('\n');
    Set s1 = new(T(Set), T(Int), VA_ANY(TEMP_VAR(void*, &cmp), NULL), THIS(v).begin(), THIS(v).end(), VAEND);
    for (Iterator it = THIS(s1).begin(); !THIS(it).equal(THIS(s1).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    delete(s1);
    delete(v);
    ARP_FreePool();
    return 0;
}
