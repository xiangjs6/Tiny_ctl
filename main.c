#include <stdio.h>
#include "tctl_allocator.h"
#include "tctl_object.h"
#include "tctl_common.h"
#include "tctl_iterator.h"
#include "tctl_vector.h"
#include "tctl_list.h"
#include "tctl_deque.h"
#include "tctl_queue.h"
#include <stdlib.h>

bool cmp(int *a, int *b)
{
    return *a > *b;
}

//queue测试
int main(void)
{
    queue que = creat_queue(sizeof(int));
    for (int i = 0; i < 10; i++)
        THIS(&que).push(&i);
    printf("front:%d\n", *(int*)THIS(&que).front());
    printf("back:%d\n", *(int*)THIS(&que).back());
    printf("size:%d\n", THIS(&que).size());
    printf("empty:%c\n", *("YN" + THIS(&que).empty()));
    for (int i = 0; i < 10; i++)
        THIS(&que).pop();
    printf("empty:%c\n", *("YN" + THIS(&que).empty()));
    destory_queue(&que);
}

//deque测试
/*int main(void)
{
    deque deq = creat_deque(sizeof(int), 9);
    __private_deque *watch = deq.__obj_private;
    //init_deque(&deq, sizeof(int), 9);
    for (int i = 0; i < 10; i++)
        THIS(&deq).push_back(&i);
    for (int i = 10; i < 20; i++)
        THIS(&deq).push_front(&i);
    for (int i = 0; i < 5; i++)
        THIS(&deq).pop_front();
    for (int i = 0; i < 5; i++)
        THIS(&deq).pop_back();
    for (int i = 30; i < 40; i++)
        THIS(&deq).push_back(&i);
    for (int i = 40; i < 50; i++)
        THIS(&deq).push_front(&i);
    deque_iter er_it = *(deque_iter *)THIS(&deq).begin();
    er_it.cur += 4;
    //THIS(&deq).erase(&er_it);
    er_it.map_node++;
    er_it.first = *er_it.map_node;
    er_it.last = *er_it.map_node + 9 * sizeof(int);
    er_it.cur = er_it.first + 3 * sizeof(int);
    THIS(&deq).erase(&er_it);
    er_it.map_node += 1;
    er_it.first = *er_it.map_node;
    er_it.last = *er_it.map_node + 9 * sizeof(int);
    er_it.cur = er_it.first + 3 * sizeof(int);
    THIS(&deq).erase(&er_it);
    int t = 100;
    THIS(&deq).insert(&er_it, &t);
    er_it.map_node -= 2;
    er_it.first = *er_it.map_node;
    er_it.last = *er_it.map_node + 9 * sizeof(int);
    er_it.cur = er_it.first + 5 * sizeof(int);
    t = 101;
    THIS(&deq).insert(&er_it, &t);
    t = 102;
    THIS(&deq).insert(THIS(&deq).begin(), &t);
    //THIS(&deq).clear();
    for (int i = 0; i < THIS(&deq).size(); i++) {
        int *temp = (int *) THIS(&deq).at(i);
        printf("%d ", *temp);
    }
    putchar('\n');
    printf("front:%d\n", *(int*)THIS(&deq).front());
    printf("back:%d\n", *(int*)THIS(&deq).back());
    for (int i = 0; i < THIS(&deq).size(); i++) {
        int *temp = (int *) THIS(&deq).at(i);
        printf("%d ", *temp);
    }
    putchar('\n');
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&deq).begin()); *it != *THIS(&deq).end(); ITER(it).increment()) {
        printf("%d ", **it);
        long long r = ITER(it).diff(THIS(&deq).end());
        r = ITER(it).diff(THIS(&deq).begin());
        r = 0;
    }
    putchar('\n');
    destory_deque(&deq);
}*/

//vector测试
/*int main(void)
{
    vector v = creat_vector(0, sizeof(int), NULL);
    //init_vector(&v, 0, sizeof(int), NULL);
    for (int i = 0; i < 10; i++) {
        int temp = i;
        THIS(&v).push_back(&temp);
    }
    for (int i = 0; i < 5; i++)
        THIS(&v).pop_back();
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d ", **it);
    }
    putchar('\n');
    printf("insert\n");
    ITER_TYPE(int) in_it = NEW_ITER(THIS(&v).begin());
    int temp = 1000;
    THIS(&v).insert(in_it, &temp);
    ITER(in_it).increment();
    ITER(in_it).increment();
    temp = 101;
    THIS(&v).insert(in_it, &temp);
    ITER(in_it).add(3);
    temp = 102;
    THIS(&v).insert(in_it, &temp);
    ITER(in_it).sub(2);
    temp = 103;
    THIS(&v).insert(in_it, &temp);
    ITER(in_it).decrement();
    ITER(in_it).decrement();
    temp = 104;
    THIS(&v).insert(in_it, &temp);
    ITER(in_it).increment();
    THIS(&v).erase(in_it);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&v).begin()); *it != *THIS(&v).end(); ITER(it).increment()) {
        printf("%d ", **it);
        int t = ITER(it).diff(THIS(&v).begin());
        t = 0;
    }
    putchar('\n');
    printf("front:%d\n", *(int*)THIS(&v).front());
    printf("back:%d\n", *(int*)THIS(&v).back());
    printf("empty:%c\n", *("YN" + THIS(&v).empty()));
    THIS(&v).clear();
    printf("empty:%c\n", *("YN" + THIS(&v).empty()));
    printf("capacoty:%d\n", THIS(&v).capacity());
    THIS(&v).resize(9);
    for (int i = 0; i < THIS(&v).size(); i++)
        printf("%d ", *(int*)THIS(&v).at(i));
    putchar('\n');
    destory_vector(&v);
}*/

// list测试
/*
int main(void)
{
    list l = creat_list(sizeof(int));
    //init_list(&l, sizeof(int));
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
    list_iter iter = *(list_iter*)THIS(&l).begin();
    THIS(&l).insert(&iter, &temp);
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
    ITER_TYPE(int) r_it = NEW_ITER(THIS(&l).end());
    ITER(r_it).decrement();
    printf("%d\n", *(int*)ITER(r_it).decrement());
    for (; *r_it != *THIS(&l).end(); ITER(r_it).decrement()) {
        printf("%d\n", **r_it);
    }
    list l2 = creat_list(sizeof(int));
    //init_list(&l2, sizeof(int));
    for (int i = 10; i < 20; i++)
        THIS(&l2).push_back(&i);
    printf("splice\n");
    THIS(&l).splice(THIS(&l).begin(), &l2, THIS(&l2).begin(), THIS(&l2).end());
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
    printf("merge\n");
    list l1 = creat_list(sizeof(int));
    list l3 = creat_list(sizeof(int));
    //init_list(&l1, sizeof(int));
    //init_list(&l3, sizeof(int));
    for (int i = 0; i < 10; i++) {
        THIS(&l1).push_back(&i);
        THIS(&l3).push_back(&i);
    }
    THIS(&l1).merge(&l3, cmp);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l1).begin()); *it != *THIS(&l1).end(); ITER(it).increment())
        printf("%d\n", **it);
    printf("sort\n");
    THIS(&l).clear();
    for (int i = 0; i < 10000; i++) {
        temp = (int)random();
        THIS(&l).push_back(&temp);
    }
    THIS(&l).sort(cmp);
    for (ITER_TYPE(int) it = NEW_ITER(THIS(&l).begin()); *it != *THIS(&l).end(); ITER(it).increment()) {
        printf("%d\n", **it);
    }
    for (int i = 1; i < 10000; i++) {
        if (*(int*)THIS(&l).at(i) < *(int*)THIS(&l).at(i - 1)) {
            printf("bad\n");
            break;
        }
    }
    destory_list(&l);
    return 0;
}
*/
