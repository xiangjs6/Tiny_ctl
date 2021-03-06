#include "../include/tctl_numeric.h"
#include "../include/tctl_vector.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_pointer_iterator.h"
#include <stdio.h>

#define Import VECTOR, ITERATOR
FormWO_t binary_add(FormWO_t first, FormWO_t second)
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
}

int main(void)
{
    ARP_CreatePool();
    Vector v = new(T(Vector), VA(T(int)));
    for (int i = 0; i < 10; i++)
        THIS(v).push_back(VA(i));
    int res = 0;
    FormWO_t r = accumulate(THIS(v).begin(), THIS(v).end(), VA(res), VA_FUNC(binary_add));
    printf("accumulate:%d\n", *(int*)&r.mem);
    int array[10] = {1, 2, 5, 7, 10, 15, 25, 30, 31, 40};
    adjacent_difference(oriPointIter(array), oriPointIter(array, 10), THIS(v).begin(), VA_FUNC(binary_sub));
    printf("adjacent_difference:");
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(VA(THIS(v).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    partial_sum(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), VA_FUNC(binary_add));
    printf("partial_sum:");
    for (Iterator it = THIS(v).begin(); !THIS(it).equal(VA(THIS(v).end())); THIS(it).inc())
        printf("%d ", *(int*)THIS(it).derefer());
    putchar('\n');
    res = 0;
    r = inner_product(THIS(v).begin(), THIS(v).end(), THIS(v).begin(), VA(res), VA_FUNC(binary_add), VA_FUNC(binary_mul));
    printf("inner_product:%d\n", *(int*)&r.mem);
    delete(v);
    ARP_FreePool();
    return 0;
}
