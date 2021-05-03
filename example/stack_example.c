#include <stdio.h>
#include "../include/tctl_stack.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"

#define Import STACK, INT

int main(void)
{
    ARP_CreatePool();
    Stack st = new(T(Stack), T(Int), VAEND);
    for (int i = 0; i < 10; i++)
        THIS(st).push(VA(i));
    printf("top:%lld\n", ((Int)THIS(st).top())->val);
    printf("size:%u\n", THIS(st).size());
    printf("empty:%c\n", "YN"[!THIS(st).empty()]);
    for (int i = 0; i < 10; i++) {
        Int temp = THIS(st).top();
        THIS(st).pop();
        printf("%lld ", temp->val);
    }
    putchar('\n');
    printf("empty:%c\n", "YN"[!THIS(st).empty()]);
    ARP_FreePool();
    return 0;
}
