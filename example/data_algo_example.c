#include <stdbool.h>
#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_multiset.h"
#include "../include/tctl_int.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_any.h"

#define Import INT, MULTISET, VECTOR, ITERATOR

bool less7(const void *v)
{
    Int i = (void*)v;
    return i->val < 7;
}

bool less6(const void *v)
{
    Int i = (void*)v;
    return i->val < 6;
}

bool less5(const void *v)
{
    Int i = (void*)v;
    return i->val < 5;
}

bool equal8(const void *v)
{
    Int i = (void*)v;
    return i->val == 8;
}

bool greater2(const void *v)
{
    Int i = (void*)v;
    return i->val > 2;
}

void *minus2(void *v)
{
    Int i = v;
    Int ret = THIS(i).cast(T(Int));
    ret->val -= 2;
    return ret;
}

void display(const void *v)
{
    Int i = (void*)v;
    printf("%lld ", i->val);
}

bool even(const void *v)
{
    Int i = (void*)v;
    return i->val % 2 ? false : true;
}

void *even_by_two(void)
{
    static long long _x = 0;
    _x += 2;
    return to_Int(&_x, T(Int));
}

int main(void)
{
    ARP_CreatePool();
    int ia[] = {0, 1, 2, 3, 4, 5, 6, 6, 6, 7, 8};
    Int Ia[11];
    for (int i = 0; i < 11; i++)
        Ia[i] = new(T(Int), VA(ia[i]));
    Vector iv = new(T(Vector), T(Int), oriPointerIter(Ia), oriPointerIter(Ia, sizeof(Ia) / sizeof(Int)), VAEND);

    Int val = new(T(Int), VA(6), VAEND);
    Iterator out_it;
    //adjacent_find
    out_it = adjacent_find(THIS(iv).begin(), THIS(iv).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 6

    //count
    printf("%u\n", count(THIS(iv).begin(), THIS(iv).end(), VA(val), VAEND)); // 3
    printf("%u\n", count_if(THIS(iv).begin(), THIS(iv).end(), &less7)); // 9

    //find
    val->val = 4;
    out_it = find(THIS(iv).begin(), THIS(iv).end(), VA(val), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 4
    out_it = find_if(THIS(iv).begin(), THIS(iv).end(), &greater2);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val);// 3

    Vector iv2 = new(T(Vector), T(Int), oriPointerIter(Ia, 6), oriPointerIter(Ia, 8), VAEND); //{6, 6}
    out_it = find_end(THIS(iv).begin(), THIS(iv).end(), THIS(iv2).begin(), THIS(iv2).end(), VAEND);
    THIS(out_it).self_add(VA(3));
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 8
    
    out_it = find_first_of(THIS(iv).begin(), THIS(iv).end(), THIS(iv2).begin(), THIS(iv2).end(), VAEND);
    THIS(out_it).self_add(VA(3));
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 7

    //for_each
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 0 1 2 3 4 5 6 6 6 7 8
    putchar('\n');

    //generate
    generate(THIS(iv2).begin(), THIS(iv2).end(), even_by_two);
    for_each(THIS(iv2).begin(), THIS(iv2).end(), &display); // 2 4
    putchar('\n');

    generate_n(THIS(iv).begin(), 3, even_by_two);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 6 8 10 3 4 5 6 6 6 7 8
    putchar('\n');

    //remove
    remove_element(THIS(iv).begin(), THIS(iv).end(), THIS((Any)VA(6)).cast(T(Int)), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 8 10 3 4 5 7 8 6 6 7 8
    putchar('\n');

    Vector iv3 = new(T(Vector), T(Int), VA(12), VAEND);
    remove_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), VA(6), VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); // 8 10 3 4 5 7 8 7 8 0 0 0
    putchar('\n');

    remove_if(THIS(iv).begin(), THIS(iv).end(), &less6);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); // 8 10 7 8 6 6 7 8 6 7 8
    putchar('\n');

    remove_copy_if(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), less7);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); // 8 10 7 8 7 8 7 8 8 0 0 0
    putchar('\n');

    //replace
    replace(THIS(iv).begin(), THIS(iv).end(), VA(6), VA(3), VAEND);
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); //8 10 7 8 3 3 7 8 3 7 8
    putchar('\n');

    replace_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), VA(3), VA(5), VAEND);
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); //8 10 7 8 5 5 7 8 5 7 8 0
    putchar('\n');

    replace_if(THIS(iv).begin(), THIS(iv).end(), &less5, VA(2));
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); //8 10 7 8 2 2 7 8 2 7 8
    putchar('\n');

    replace_copy_if(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin(), &equal8, VA(9));
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); //9 10 7 9 2 2 7 9 2 7 9 0
    putchar('\n');

    //reverse
    reverse(THIS(iv).begin(), THIS(iv).end());
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); //8 7 2 8 7 2 2 8 7 10 8
    putchar('\n');

    reverse_copy(THIS(iv).begin(), THIS(iv).end(), THIS(iv3).begin());
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); //8 10 7 8 2 2 7 8 2 7 8 0
    putchar('\n');

    //rotate
    rotate(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(4)), THIS(iv).end());
    for_each(THIS(iv).begin(), THIS(iv).end(), &display); //7 2 2 8 7 10 8 8 7 2 8
    putchar('\n');

    rotate_copy(THIS(iv).begin(), THIS(THIS(iv).begin()).add(VA(5)), THIS(iv).end(), THIS(iv3).begin());
    for_each(THIS(iv3).begin(), THIS(iv3).end(), &display); //10 8 8 7 2 8 7 2 2 8 7 0
    putchar('\n');

    //search
    int ia2[3] = {2, 8};
    Int Ia2[3];
    for (int i = 0; i < 3; i++)
        Ia2[i] = new(T(Int), VA(ia2[i]), VAEND);
    Vector iv4 = new(T(Vector), T(Int), oriPointerIter(Ia2), oriPointerIter(Ia2, 2), VAEND);
    out_it = search(THIS(iv).begin(), THIS(iv).end(), THIS(iv4).begin(), THIS(iv4).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 2

    out_it = search_n(THIS(iv).begin(), THIS(iv).end(), 2, THIS((Any)VA(8)).cast(T(Int)), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 8

    //swap_ranges
    swap_ranges(THIS(iv4).begin(), THIS(iv4).end(), THIS(iv).begin());
    for_each(THIS(iv).begin(), THIS(iv).end(), display); // 2 8 2 8 7 10 8 8 7 2 8
    putchar('\n');
    for_each(THIS(iv4).begin(), THIS(iv4).end(), display); // 7 2
    putchar('\n');

    //transform
    transform(THIS(iv).begin(), THIS(iv).end(), THIS(iv).begin(), minus2);
    for_each(THIS(iv).begin(), THIS(iv).end(), display); // 0 6 0 6 5 8 6 6 5 0 6
    putchar('\n');

    /***********************/
    Vector iv5 = new(T(Vector), T(Int), oriPointerIter(Ia), oriPointerIter(Ia, sizeof(Ia) / sizeof(Int)), VAEND);
    Vector iv6 = new(T(Vector), T(Int), oriPointerIter(Ia, 4), oriPointerIter(Ia, 8), VAEND);
    Vector iv7 = new(T(Vector), T(Int), VA(15), VAEND);
    //max min
    out_it = max_element(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 8
    out_it = min_element(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    printf("%lld\n", ((Int)THIS(out_it).derefer())->val); // 0

    //includes
    bool r_b = includes(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv6).begin(), THIS(iv6).end(), VAEND);
    printf("%s\n", r_b ? "true" : "false"); //true

    //merge
    merge(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv6).begin(), THIS(iv6).end(), THIS(iv7).begin(), VAEND, VAEND);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display); // 0 1 2 3 4 4 5 5 6 6 6 6 6 7 8
    putchar('\n');

    //partition
    partition(THIS(iv7).begin(), THIS(iv7).end(), even);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display); //0 8 2 6 4 4 6 6 6 6 5 5 3 7 1
    putchar('\n');

    //unique
    unique(THIS(iv5).begin(), THIS(iv5).end(), VAEND);
    for_each(THIS(iv5).begin(), THIS(iv5).end(), display); //0 1 2 3 4 5 6 7 8 7 8
    putchar('\n');

    unique_copy(THIS(iv5).begin(), THIS(iv5).end(), THIS(iv7).begin(), VAEND);
    for_each(THIS(iv7).begin(), THIS(iv7).end(), display); //0 1 2 3 4 5 6 7 8 7 8 5 3 7 1
    putchar('\n');

    for (int i = 0; i < 11; i++)
        delete(Ia[i]);
    for (int i = 0; i < 3; i++)
        delete(Ia2[i]);

    delete(iv);
    delete(iv2);
    delete(iv3);
    delete(iv4);
    delete(iv5);
    delete(iv6);
    delete(iv7);

    ARP_FreePool();
    return 0;
}
