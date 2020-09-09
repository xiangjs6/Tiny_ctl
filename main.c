#include <stdio.h>
#include "tctl_allocator.h"
#include "tctl_object.h"
#include "tctl_common.h"
#include "tctl_iterator.h"
#include "tctl_vector.h"
#include "tctl_list.h"
#include <stdlib.h>


int main(void)
{
    list l;
    init_list(&l, sizeof(int));
    int temp = 1;
    for (int i = 0; i < 10; i++) {
        THIS(&l).push_back(&i);
    }
    for (int i = 0; i < 5; i++) {
        THIS(&l).pop_back();
    }
    int temp1 = 20;
    THIS(&l).pop_front();
    THIS(&l).push_front(&temp1);
    iter_ptr iter = *THIS(&l).begin();
    THIS(&l).insert(iter, &temp);
    printf("first\n");
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l).begin()); *it != *THIS(&l).end(); ITER(it).increment()) {
        printf("%d\n", **it);
        //THIS(&l).insert(*it, &temp);
    }
    THIS(&l).remove(&temp1);
    temp1 = 4;
    THIS(&l).push_back(&temp1);
    printf("second\n");
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l).begin()); *it != *THIS(&l).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    THIS(&l).unique();
    printf("third\n");
    ITER_TYPE(int) it = NEW_ITER(THIS(&l).end());
    ITER(it).decrement();
    for (; *it != *THIS(&l).end(); ITER(it).decrement()) {
        printf("%d\n", **it);
    }
    list l2;
    init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(&l2).push_back(&i);
    printf("splice\n");
    THIS(&l).splice(*THIS(&l).begin(), &l2, *THIS(&l2).begin(), *THIS(&l2).end());
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l).begin()); *it != *THIS(&l).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    printf("reverse\n");
    THIS(&l).reverse();
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l).begin()); *it != *THIS(&l).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    printf("front%d\n", *(int*)THIS(&l).front());
    printf("back%d\n", *(int*)THIS(&l).back());
    printf("size:%d\n", THIS(&l).size());
    destory_list(&l);
    return 0;
}
