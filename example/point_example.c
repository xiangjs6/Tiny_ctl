#include <stdio.h>
#include "../include/tctl_pointer_iterator.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"

#define Import ITERATOR, INT
int main(void)
{

    ARP_CreatePool();
    Int array[10];
    for (int i = 0; i < 10; i++)
        array[i] = new(T(Int), VA(i), VAEND);
    Iterator start = oriPointerIter(array);
    for (; !THIS(start).equal(oriPointerIter(array, sizeof(array) / sizeof(int))); THIS(start).inc())
        printf("%lld ", ((Int)THIS(start).derefer())->val);
    putchar('\n');

    Iterator r_it = THIS(start).reverse_iterator();
    do
    {
        THIS(r_it).inc();
        printf("%lld ", ((Int)THIS(r_it).derefer())->val);
    } while(!THIS(r_it).equal(oriPointerIter(array)));
    putchar('\n');
    for (int i = 0; i < 10; i++) {
        delete(array[i]);
    }
    ARP_FreePool();
    return 0;
}
