#include <stdio.h>
#include "../include/tctl_stack.h"
#include "../include/auto_release_pool.h"

#define Import STACK

int main(void)
{
    ARP_CreatePool();
    Stack st = new(T(Stack), VA(T(int)));
    for (int i = 0; i < 10; i++)
        THIS(st).push(VA(i));
    printf("top:%d\n", *(int*)THIS(st).top());
    printf("size:%ld\n", THIS(st).size());
    printf("empty:%c\n", "YN"[!THIS(st).empty()]);
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(st).top();
        THIS(st).pop();
        printf("%d ", temp);
    }
    putchar('\n');
    printf("empty:%c\n", "YN"[!THIS(st).empty()]);
    ARP_FreePool();
    return 0;
}
