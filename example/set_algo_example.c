#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_multiset.h"
#include "../include/tctl_int.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"

#define Import INT, MULTISET, VECTOR, ITERATOR

int cmp(void *_a, void *_b)
{
    Int a = _a;
    Int b = _b;
    return (int)(a->val - b->val);
}

int main(void)
{
    ARP_CreatePool();
    long long ia1[6] = {1, 3, 5, 7, 9, 11};
    long long ia2[7] = {1, 1, 2, 3, 5, 8, 13};
    Int Ia1[6];
    Int Ia2[7];
    for (int i = 0; i < 6; i++)
        Ia1[i] = new(T(Int), VA(ia1[i]), VAEND);
    for (int i = 0; i < 7; i++)
        Ia2[i] = new(T(Int), VA(ia2[i]), VAEND);

    MultiSet S1 = new(T(MultiSet), T(Int), VA_ANY(TEMP_VAR(void*,cmp), NULL), oriPointerIter(Ia1), oriPointerIter(Ia1, 6), VAEND);
    MultiSet S2 = new(T(MultiSet), T(Int), VA_ANY(TEMP_VAR(void*,cmp), NULL), oriPointerIter(Ia2), oriPointerIter(Ia2, 7), VAEND);
    Vector v = new(T(Vector), T(Int), VA(13), VAEND);

    for (Iterator it = THIS(S1).begin(); !THIS(it).equal(THIS(S1).end()); THIS(it).inc()) //1 3 5 7 9 11
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    for (Iterator it = THIS(S2).begin(); !THIS(it).equal(THIS(S2).end()); THIS(it).inc()) //1 1 2 3 5 8 13
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    Iterator first1 = THIS(S1).begin();
    Iterator last1 = THIS(S1).end();
    Iterator first2 = THIS(S2).begin();
    Iterator last2 = THIS(S2).end();

    printf("Union of S1 and S2: ");
    set_union(first1, last1, first2, last2, THIS(v).begin(), VAEND);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc()) //Union of S1 and S2: 1 1 2 3 5 7 8 9 11 13 0 0 0
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    delete(v);
    v = new(T(Vector), T(Int), VA(13), VAEND);
    printf("Intersection of S1 and S2: ");
    set_intersection(first1, last1, first2, last2, THIS(v).begin(), VAEND);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc()) //Intersection of S1 and S2: 1 3 5 0 0 0 0 0 0 0 0 0 0
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    delete(v);
    v = new(T(Vector), T(Int), VA(13), VAEND);
    printf("Difference of S1 and S2 (S1-S2): ");
    set_difference(first1, last1, first2, last2, THIS(v).begin(), VAEND);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc()) //Difference of S1 and S2 (S1-S2): 7 9 11 0 0 0 0 0 0 0 0 0 0
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    delete(v);
    v = new(T(Vector), T(Int), VA(13), VAEND);
    printf("Symmetric difference of S1 and S2: ");
    set_symmetric_difference(first1, last1, first2, last2, THIS(v).begin(), VAEND);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc()) //Symmetric difference of S1 and S2: 1 2 7 8 9 11 13 0 0 0 0 0 0
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    delete(v);
    v = new(T(Vector), T(Int), VA(13), VAEND);
    printf("Difference of S1 and S2 (S2-S1): ");
    set_difference(first2, last2, first1, last1, THIS(v).begin(), VAEND);
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc()) //Difference of S1 and S2 (S2-S1): 1 2 8 13 0 0 0 0 0 0 0 0 0
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');

    for (int i = 0; i < 6; i++)
        delete(Ia1[i]);
    for (int i = 0; i < 7; i++)
        delete(Ia2[i]);
    ARP_FreePool();
    return 0;
}
