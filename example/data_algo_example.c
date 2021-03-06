#include <stdbool.h>
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

bool less5(FormWO_t v)
{
    Int i = v.mem;
    return i->val < 5;
}

bool equal8(FormWO_t v)
{
    Int i = v.mem;
    return i->val == 8;
}

bool greater2(FormWO_t v)
{
    Int i = v.mem;
    return i->val > 2;
}

FormWO_t minus2(FormWO_t v)
{
    Int i = v.mem;
    return VA(i->val - 2);
}

void display(FormWO_t v)
{
    Int i = v.mem;
    printf("%lld ", i->val);
}

bool even(FormWO_t v)
{
    Int i = v.mem;
    return i->val % 2 ? false : true;
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
    generate(THIS(iv2).begin(), THIS(iv2).end(), even_by_two);
    for_each(THIS(iv2).begin(), THIS(iv2).end(), display);
    putchar('\n');

    generate_n(THIS(iv).begin(), 3, even_by_two);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    //remove
    remove_element(THIS(iv).begin(), THIS(iv).end(), VA(6), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    Vector iv3 = new(T(Vector), VA(T(Int), 12));
    remove_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), VA(6), VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    remove_if(THIS(iv).begin(), THIS(iv).end(), less6);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    remove_copy_if(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), less7);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    //replace
    replace(THIS(iv).begin(), THIS(iv).end(), VA(6), VA(3), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    replace_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), VA(3, 5), VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    replace_if(THIS(iv).begin(), THIS(iv).end(), less5, VA(2));
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    replace_copy_if(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), equal8, VA(9));
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    //reverse
    reverse(THIS(iv).begin(), THIS(iv).end());
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    reverse_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin());
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    //rotate
    rotate(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(4)), THIS(iv).end());
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    rotate_copy(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), THIS(iv3).begin());
    for_each(THIS(iv3).begin(), THIS(iv3).end(), display);
    putchar('\n');

    //search
    int ia2[3] = {2, 8};
    Vector iv4 = new(T(Vector), VA(T(Int), oriPointIter(ia2), oriPointIter(ia2, 2)));
    out_it = search(THIS(iv).begin(), THIS(iv).end(), THIS(iv4).begin(), THIS(iv4).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    out_it = search_n(THIS(iv).begin(), THIS(iv).end(), 2, VA(8), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    //swap_ranges
    swap_ranges(THIS(iv4).begin(), THIS(iv4).end(), THIS(iv).begin());
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');
    for_each(THIS(iv4).begin(), THIS(iv4).end(), display);
    putchar('\n');

    //transform
    transform(THIS(iv).begin(), THIS(iv).end(), THIS(iv).begin(), minus2);
    for_each(THIS(iv).begin(), THIS(iv).end(), display);
    putchar('\n');

    /***********************/
    Vector iv5 = new(T(Vector), VA(T(Int), oriPointIter(ia), oriPointIter(ia, sizeof(ia) / sizeof(int))));
    Vector iv6 = new(T(Vector), VA(T(Int), oriPointIter(ia, 4), oriPointIter(ia, 8)));
    Vector iv7 = new(T(Vector), VA(T(Int), 15));
    //max min
    out_it = max_element(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);
    out_it = min_element(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);

    //includes
    bool r_b = includes(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv6).begin(), THIS(iv6).end(), VAEND);
    printf("%s\n", r_b ? "true" : "false");

    //merge
    merge(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv6).begin(), THIS(iv6).end(), THIS(iv7).begin(), VAEND, VAEND);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display);
    putchar('\n');

    //partition
    partition(THIS(iv7).begin(), THIS(iv7).end(), even);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display);
    putchar('\n');

    //unique
    unique(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    for_each(THIS(iv5).begin(), THIS(iv5).end(), display);
    putchar('\n');

    unique_copy(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv7).begin(), VAEND);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display);
    putchar('\n');

    ARP_FreePool();
}
