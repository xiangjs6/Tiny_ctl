#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_multiset.h"
#include "../include/tctl_int.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"

#define Import INT, MULTISET, VECTOR, ITERATOR

bool less7(FormWO_t v)
{
    Int i = v.mem;
    return i->val < 7;
}

bool less6(FormWO_t v)
{
    Int i = v.mem;
    return i->val < 6;
}

bool greater2(FormWO_t v)
{
    Int i = v.mem;
    return i->val > 2;
}

void display(FormWO_t v)
{
    Int i = v.mem;
    printf("%lld ", i->val);
}

FormWO_t even_by_two(void)
{
    static long long _x = 0;
    return VA(_x += 2);
}

int main(void)
{
    ARP_CreatePool();
    int ia[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
    Vector iv = new(T(Vector), VA(T(Int), oriPointIter(ia), oriPointIter(ia, sizeof(ia) / sizeof(int))));

    Int val = new(T(Int), VA(6));
    FormWO_t res;
    Iterator out_it;
    //adjacent_find
    out_it = adjacent_find(THIS(iv).begin(), THIS(iv).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    //count
    printf("%ld\n", count(THIS(iv).begin(), THIS(iv).end(), VA(val), VAEND));
    printf("%ld\n", count_if(THIS(iv).begin(), THIS(iv).end(), less7));

    //find
    val->val = 4;
    out_it = find(THIS(iv).begin(), THIS(iv).end(), VA(val), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);
    out_it = find_if(THIS(iv).begin(), THIS(iv).end(), greater2);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    Vector iv2 = new(T(Vector), VA(T(Int), oriPointIter(ia, 6), oriPointIter(ia, 8)));
    out_it = find_end(THIS(iv).begin(), THIS(iv).end(), THIS(iv2).begin(), THIS(iv2).end(), VAEND);
    THIS(out_it).self_add(VA(3));
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);
    
    out_it = find_first_of(THIS(iv).begin(), THIS(iv).end(), THIS(iv2).begin(), THIS(iv2).end(), VAEND);
    THIS(out_it).self_add(VA(3));
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    //for_each
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    //generate
    generate(THIS(iv2).begin(), THIS(iv2).end(), even_by_two, VAEND);
    for_each(THIS(iv2).begin(), THIS(iv2).end(), display);
    putchar('\n');

    generate_n(THIS(iv).begin(), 3, even_by_two, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    //remove
    remove_element(THIS(iv).begin(), THIS(iv).end(), VA(6), VAEND, VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    Vector iv3 = new(T(Vector), VA(T(Int), 12));
    remove_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), VA(6), VAEND, VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    remove_if(THIS(iv).begin(), THIS(iv).end(), less6);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    remove_copy_if(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), less7);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    ARP_FreePool();
}
