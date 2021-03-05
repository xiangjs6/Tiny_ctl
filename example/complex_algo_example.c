#include <stdio.h>
#include "../include/tctl_algo.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_vector.h"
#include "../include/tctl_int.h"
#include "../include/tctl_pointer_iterator.h"

#define Import VECTOR, INT, ITERATOR

bool even(int x)
{
    return x % 2 ? false : true;
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
    ARP_FreePool();
    return 0;
}
