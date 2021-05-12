#include "../include/tctl_numeric.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_pointer_iterator.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include <stdio.h>

#define Import VECTOR, ITERATOR, INT
/*void *binary_add(FormWO_t first, FormWO_t second)
{
    int *a = first._.f != POD ? (void*)first.mem : (void*)&first.mem;
    int *b = second._.f != POD ? (void*)second.mem : (void*)&second.mem;
    FormWO_t res = FORM_WITH_OBJ(T(int));
    *(int*)&res.mem = *a + *b;
    return res;
}

FormWO_t binary_sub(FormWO_t first, FormWO_t second)
{
    int *a = first._.f != POD ? (void*)first.mem : (void*)&first.mem;
    int *b = second._.f != POD ? (void*)second.mem : (void*)&second.mem;
    FormWO_t res = FORM_WITH_OBJ(T(int));
    *(int*)&res.mem = *a - *b;
    return res;
}

FormWO_t binary_mul(FormWO_t first, FormWO_t second)
{
    int *a = first._.f != POD ? (void*)first.mem : (void*)&first.mem;
    int *b = second._.f != POD ? (void*)second.mem : (void*)&second.mem;
    FormWO_t res = FORM_WITH_OBJ(T(int));
    *(int*)&res.mem = *a * *b;
    return res;
}

void assign(FormWO_t left, FormWO_t right)
{
    int *a = left._.f != POD ? (void*)left.mem : (void*)&left.mem;
    int *b = right._.f != POD ? (void*)right.mem : (void*)&right.mem;
    *a = *b;
}*/

int main(void)
{
    ARP_CreatePool();
    Vector v = new(T(Vector), T(Int), VAEND);
    for (int i = 0; i < 10; i++)
        THIS(v).push_back(VA(i));
    Int res = new(T(Int), VA(0), NULL);
    //Int r = accumulate(THIS(v).begin(), THIS(v).end(), VA(res), VA_FUNC(binary_add));
    Int r = accumulate(THIS(v).begin(), THIS(v).end(), res, VAEND);
    printf("accumulate:%lld\n", ((Int)r)->val);
    int o_array[10] = {1, 2, 5, 7, 10, 15, 25, 30, 31, 40};
    Int array[10];
    for (int i = 0; i < 10; i++)
        array[i] = new(T(Int), VA(o_array[i]), VAEND);
    //adjacent_difference(oriPointerIter(array), oriPointerIter(array, 10), THIS(v).begin(), VA_FUNC(binary_sub));
    adjacent_difference(oriPointerIter(array), oriPointerIter(array, 10), THIS(v).begin(), VAEND);
    printf("adjacent_difference:");
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    //partial_sum(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), VA_FUNC(binary_add));
    partial_sum(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), VAEND);
    printf("partial_sum:");
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(THIS(v).end()); THIS(it).inc())
        printf("%lld ", ((Int)THIS(it).derefer())->val);
    putchar('\n');
    //r = inner_product(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), VA(res), VA_FUNC(binary_add), VA_FUNC(binary_mul));
    r = inner_product(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), res, VAEND);
    printf("inner_product:%lld\n", r->val);
    delete(v);
    ARP_FreePool();
    return 0;
}
