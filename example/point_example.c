#include <stdio.h>
#include "../include/tctl_pointer_iterator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"

#define Import ITERATOR, INT
int main(void)
{

    ARP_CreatePool();
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    Iterator start = oriPointIter(array);
    for (; !THIS(start).equal(VA(oriPointIter(array, sizeof(array) / sizeof(int)))); THIS(start).inc())
        printf("%d ", *(int*)THIS(start).derefer());
    putchar('\n');
    Int obj_array[10];
    for (int i = 0; i < 10; i++)
        obj_array[i] = new(T(Int), VA(i));
    start = oriPointIter(obj_array);
    for (; !THIS(start).equal(VA(oriPointIter(obj_array, 10))); THIS(start).inc())
        printf("%lld ", ((Int)THIS(start).derefer())->val);
    putchar('\n');

    Iterator r_it = THIS(start).reverse_iterator();
    do
    {
        THIS(r_it).inc();
        printf("%lld ", ((Int)THIS(r_it).derefer())->val);
    } while(!THIS(r_it).equal(VA(oriPointIter(obj_array))));
    putchar('\n');
    ARP_FreePool();
    return 0;
}
