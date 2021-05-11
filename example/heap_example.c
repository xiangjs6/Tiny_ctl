//
// Created by 95142 on 2021/5/12.
//
#include "../include/tctl_vector.h"
#include "../include/tctl_deque.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include "../include/tctl_heap.h"
#include <stdio.h>
#include <stdlib.h>

#define Import INT, VECTOR, DEQUE

int cmp(const void *a, const void *b)
{
    Int x = (void*)a;
    Int y = (void*)b;
    return (int)(x->val - y->val);
}

int main(void)
{
    ARP_CreatePool();
    int temp;
    Vector v = new(T(Vector), T(Int), VAEND);
    for (int i = 0; i < 15; i++) {
        temp = rand() % 10;
        printf("%d ", temp);
        THIS(v).push_back(VA(temp));
    }
    putchar('\n');
    make_heap(THIS(v).begin(), THIS(v).end(), cmp);
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lld ", ((Int)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    temp = 100;
    THIS(v).push_back(VA(temp));
    push_heap(THIS(v).begin(), THIS(v).end(), cmp);
    printf("%lld\n", ((Int)THIS(v).front())->val);
    temp = 0;
    THIS(v).push_back(VA(temp));
    push_heap(THIS(v).begin(), THIS(v).end(), cmp);
    printf("%lld\n", ((Int)THIS(v).back())->val);
    pop_heap(THIS(v).begin(), THIS(v).end(), cmp);
    THIS(v).pop_back();
    sort_heap(THIS(v).begin(), THIS(v).end(), cmp);
    for (int i = 0; i < THIS(v).size(); i++)
        printf("%lld ", ((Int)THIS(v).brackets(VA(i)))->val);
    putchar('\n');
    Deque deq = new(T(Deque), T(Int), VAEND);
    for (int i = 0; i < 100000; i++)
        THIS(deq).push_back(VA(rand()));
    sort_heap(THIS(deq).begin(), THIS(deq).end(), cmp);
    for (int i = 1; i < 100000; i++) {
        if (((Int)THIS(deq).brackets(VA(i)))->val < ((Int)THIS(deq).brackets(VA(i - 1)))->val) {
            printf("bad\n");
            break;
        }
    }
    putchar('\n');
    delete(v);
    ARP_FreePool();
    return 0;
}