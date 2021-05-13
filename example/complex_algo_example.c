#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_int.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"

#define Import VECTOR, INT, ITERATOR

bool even(const void *_x)
{
    Int x = (void*)_x;
    return x->val % 2 ? false : true;
}

void display(const void *v)
{
    Int i = (void*)v;
    printf("%lld ", i->val);
}

int greater(const void *_a, const void *_b)
{
    Int a = (void*)_a;
    Int b = (void*)_b;
    return (int)(b->val - a->val);
}

int lesser(const void *_a, const void *_b)
{
    Int a = (void*)_a;
    Int b = (void*)_b;
    return (int)(a->val - b->val);
}

void lookIterator(Iterator it)
{
    printf("%lld\n", ((Int)THIS(it).derefer())->val);
}
int main(void)
{
    ARP_CreatePool();
    int ia[] = {12, 17, 20, 22, 23, 30, 33, 40};
    Int Ia[8];
    for (int i = 0; i < 8; i++)
        Ia[i] = new(T(Int), VA(ia[i]), VAEND);
    Vector iv = new(T(Vector), T(Int), oriPointerIter(Ia), oriPointerIter(Ia, 8), VAEND);
    Iterator out_iter;

    Int v = new(T(Int), VA(21), VAEND);
    out_iter = lower_bound(THIS(iv).begin(), THIS(iv).end(), v, VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val); // 22
    out_iter = upper_bound(THIS(iv).begin(), THIS(iv).end(), v, VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val); // 22
    v->val = 22;
    out_iter = lower_bound(THIS(iv).begin(), THIS(iv).end(), v, VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val); // 22
    out_iter = upper_bound(THIS(iv).begin(), THIS(iv).end(), v, VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val); // 23

    v->val = 33;
    printf("%s\n", binary_search(THIS(iv).begin(), THIS(iv).end(), v,  VAEND) ? "true" : "false"); // true
    v->val = 34;
    printf("%s\n", binary_search(THIS(iv).begin(), THIS(iv).end(), v,  VAEND) ? "true" : "false"); // false

    next_permutation(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 12 17 20 22 23 30 40 33
    putchar('\n');

    prev_permutation(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 12 17 20 22 23 30 33 40
    putchar('\n');

    random_shuffle(THIS(iv).begin(), THIS(iv).end(), NULL);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 23 17 33 20 12 30 40 22
    putchar('\n');

    sort(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 12 17 20 22 23 30 33 40
    putchar('\n');

    sort(THIS(iv).begin(), THIS(iv).end(), &greater); // 40 33 30 23 22 20 17 12
    for_each(THIS(iv).begin(), THIS(iv).end(), &display);
    putchar('\n');

    THIS(iv).push_back(VA(22));
    THIS(iv).push_back(VA(30));
    THIS(iv).push_back(VA(17));

    sort(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 12 17 17 20 22 22 23 30 30 33 40
    putchar('\n');

    Pair pairIte;
    pairIte = equal_range(THIS(iv).begin(), THIS(iv).end(), THIS((Any)VA(22)).cast(T(Int)), VAEND);
    lookIterator(pairIte->first); // 22
    lookIterator(pairIte->second); // 23

    pairIte = equal_range(THIS(iv).begin(), THIS(iv).end(), THIS((Any)VA(25)).cast(T(Int)), VAEND);
    lookIterator(pairIte->first); // 30
    lookIterator(pairIte->second); // 30

    random_shuffle(THIS(iv).begin(), THIS(iv).end(), NULL);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 22 33 12 40 20 22 23 30 17 17 30
    putchar('\n');

    nth_element(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 17 17 12 22 20 22 23 30 33 30 40
    putchar('\n');

    nth_element(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), &greater);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 40 30 33 30 23 22 20 22 17 17 12
    putchar('\n');

    partition(THIS(iv).begin(), THIS(iv).end(), &even);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 40 30 12 30 22 22 20 23 17 17 33
    putchar('\n');

    int ia2[] = {69, 23, 80, 42, 17, 15, 26, 51, 19, 12, 45, 72};
    Int Ia2[12];
    for (int i = 0; i < 12; i++)
        Ia2[i] = new(T(Int), VA(ia2[i]), VAEND);
    Vector iv2 = new(T(Vector), T(Int), oriPointerIter(Ia2), oriPointerIter(Ia2, 12), VAEND);
    partial_sort(THIS(iv2).begin(), THIS(THIS(iv2).begin()).add(VA(7)), THIS(iv2).end(), &greater);
    for_each(THIS(iv2).begin(), THIS(iv2).end(), &display); // 80 72 69 51 45 42 26 15 17 12 19 23
    putchar('\n');

    int ia3[] = {1, 3, 5, 7, 2, 4, 6, 8, 10};
    Int Ia3[9];
    for (int i = 0; i < 9; i++)
        Ia3[i] = new(T(Int), VA(ia3[i]), VAEND);
    Vector iv3 = new(T(Vector), T(Int), oriPointerIter(Ia3), oriPointerIter(Ia3, 9), VAEND);
    inplace_merge(THIS(iv3).begin(), THIS(THIS(iv3).begin()).add(VA(4)), THIS(iv3).end(), VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); // 1 2 3 4 5 6 7 8 10
    putchar('\n');

    mergesort(THIS(iv2).begin(), THIS(iv2).end(), VAEND);
    for_each(THIS(iv2).begin(), THIS(iv2).end(), &display); // 12 15 17 19 23 26 42 45 51 69 72 80
    putchar('\n');

    delete(iv);
    delete(iv2);
    delete(iv3);
    ARP_FreePool();
    return 0;
}
