#include <stdio.h>
#include "tctl_point_iterator.h"
#include "tctl_iterator.h"
#include "auto_release_pool/auto_release_pool.h"
int main(void)
{
    ARP_CreatePool();
    int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    iterator(int) start = POINT_ITER(array, array);
    for (; !ITER(start).equal(POINT_ITER(array + sizeof(array) / sizeof(int), array)); ITER(start).increment())
        printf("%d \n", *start->val);
    ARP_FreePool();
    return 0;
}
