#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_int.h"
#include "../include/tctl_pointer_iterator.h"

#define Import VECTOR, INT, ITERATOR

bool even(FormWO_t _x)
{
    Int x = _x.mem;
    return x->val % 2 ? false : true;
}

void display(FormWO_t v)
{
    Int i = v.mem;
    printf("%lld ", i->val);
}

int greater(FormWO_t _a, FormWO_t _b)
{
    Int a = _a.mem;
    Int b = _b.mem;
    return b->val - a->val;
}

void lookIterator(Iterator it)
{
    printf("%lld\n", ((Int)THIS(it).derefer())->val);
}
int main(void)
{
    ARP_CreatePool();
    int ia[] = {12, 17, 20, 22, 23, 30, 33, 40};
    Vector iv = new(T(Vector), VA(T(Int), oriPointIter(ia), oriPointIter(ia, sizeof(ia) / sizeof(int))));
    Iterator out_iter;

    out_iter = lower_bound(THIS(iv).begin(), THIS(iv).end(), VA(21), VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val);
    out_iter = upper_bound(THIS(iv).begin(), THIS(iv).end(), VA(21), VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val);
    out_iter = lower_bound(THIS(iv).begin(), THIS(iv).end(), VA(22), VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val);
    out_iter = upper_bound(THIS(iv).begin(), THIS(iv).end(), VA(22), VAEND);
    printf("%lld\n", ((Int)THIS(out_iter).derefer())->val);

    printf("%s\n", binary_search(THIS(iv).begin(), THIS(iv).end(), VA(33),  VAEND) ? "true" : "false");
    printf("%s\n", binary_search(THIS(iv).begin(), THIS(iv).end(), VA(34),  VAEND) ? "true" : "false");

    next_permutation(THIS(iv).begin(), THIS(iv).end(), VAEND, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    prev_permutation(THIS(iv).begin(), THIS(iv).end(), VAEND, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    random_shuffle(THIS(iv).begin(), THIS(iv).end(), NULL, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    sort(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    sort(THIS(iv).begin(), THIS(iv).end(), VA_FUNC(greater));
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    THIS(iv).push_back(VA(22));
    THIS(iv).push_back(VA(30));
    THIS(iv).push_back(VA(17));

    sort(THIS(iv).begin(), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    Pair pairIte;
    pairIte = equal_range(THIS(iv).begin(), THIS(iv).end(), VA(22), VAEND);
    lookIterator(pairIte->first);
    lookIterator(pairIte->second);

    pairIte = equal_range(THIS(iv).begin(), THIS(iv).end(), VA(25), VAEND);
    lookIterator(pairIte->first);
    lookIterator(pairIte->second);

    random_shuffle(THIS(iv).begin(), THIS(iv).end(), NULL, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    nth_element(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    nth_element(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), VA_FUNC(greater));
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    partition(THIS(iv).begin(), THIS(iv).end(), even, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    ARP_FreePool();
    return 0;
}
