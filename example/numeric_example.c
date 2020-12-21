#include "tctl_numeric.h"
#include "tctl_vector.h"
#include "auto_release_pool.h"
#include "tctl_point_iterator.h"
#include <stdio.h>
void *binary_add(const void *first, const void *second, void *res)
{
    if (!res) {
        res = ARP_MallocARel(sizeof(int));
        *(int*)res = 0;
    }
    if (!first || !second) {
        if (first)
            *(int*)res = *(int*)first;
        else if (second)
            *(int*)res = *(int*)second;
        else
            *(int*)res = 0;
    } else
        *(int*)res = *(int*)first + *(int*)second;
    return res;
}
void *binary_sub(const void *first, const void *second, void *res)
{
    if (!res) {
        res = ARP_MallocARel(sizeof(int));
        *(int*)res = 0;
    }
    if (!first || !second) {
        if (first)
            *(int*)res = *(int*)first;
        else if (second)
            *(int*)res = -*(int*)second;
        else
            *(int*)res = 0;
    } else
        *(int*)res = *(int*)first - *(int*)second;
    return res;
}
void *binary_mul(const void *first, const void *second, void *res)
{
    if (!res) {
        res = ARP_MallocARel(sizeof(int));
        *(int*)res = 0;
    }
    if (!first || !second)
        *(int*)res = 0;
    else
        *(int*)res = *(int*)first * *(int*)second;
    return res;
}
int main(void)
{
    ARP_CreatePool();
    vector v = creat_vector(sizeof(int));
    for (int i = 0; i < 10; i++)
        THIS(&v).push_back(&i);
    int res = 0;
    accumulate(THIS(&v).begin(), THIS(&v).end(), &res, binary_add);
    printf("accumulate:%d\n", res);
    int array[10] = {1, 2, 5, 7, 10, 15, 25, 30, 31, 40};
    adjacent_difference(POINT_ITER(array, array), POINT_ITER(array + 10, array), THIS(&v).begin(), binary_sub);
    printf("adjacent_difference:");
    for (iterator(int) it = THIS(&v).begin(); !ITER(it).equal(THIS(&v).end()); ITER(it).inc())
        printf("%d ", *it->val);
    putchar('\n');
    partial_sum(THIS(&v).begin(), THIS(&v).end(), THIS(&v).begin(), binary_add);
    printf("partial_sum:");
    for (iterator(int) it = THIS(&v).begin(); !ITER(it).equal(THIS(&v).end()); ITER(it).inc())
        printf("%d ", *it->val);
    putchar('\n');
    res = 0;
    inner_product(THIS(&v).begin(), THIS(&v).end(), THIS(&v).begin(), &res, binary_add, binary_mul);
    printf("inner_product:%d\n", res);
    destory_vector(&v);
    ARP_FreePool();
}
