//
// Created by 95142 on 2021/5/12.
//
#include "../include/tctl_priority_queue.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"
#include <stdio.h>
#include <stdlib.h>

#define Import INT, PRIORITY_QUEUE

int cmp(void *a, void *b)
{
    Int x = a;
    Int y = b;
    return (int)(x->val - y->val);
}

int main(void)
{
    ARP_CreatePool();
    Priority_Queue pri_que = new(T(Priority_Queue), T(Int), VAEND, VA_ANY(TEMP_VAR(void*, &cmp), NULL));
    for (int i = 0; i < 300000; i++) {
        int temp = rand();
        THIS(pri_que).push(VA(temp));
    }
    long long pre = ((Int)THIS(pri_que).top())->val;
    THIS(pri_que).pop();
    printf("%lld ", pre);
    while (!THIS(pri_que).empty()) {
        long long temp = ((Int)THIS(pri_que).top())->val;
        printf("%lld ", temp);
        if (temp > pre) {
            puts("bad!!!!");
            break;
        }
        THIS(pri_que).pop();
    }
    putchar('\n');
    delete(pri_que);
    ARP_FreePool();
    return 0;
}