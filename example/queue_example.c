#include <stdio.h>
#include "../include/tctl_queue.h"
#include "../include/auto_release_pool.h"
#include "../include/tctl_int.h"
#include "../include/tctl_arg.h"

#define Import QUEUE, INT

int main(void)
{
    ARP_CreatePool();
    Queue que = new(T(Queue), T(Int), VAEND);
    for (int i = 0; i < 10; i++)
        THIS(que).push(VA(i));
    printf("front:%lld\n", ((Int)THIS(que).front())->val);
    printf("back:%lld\n", ((Int)THIS(que).back())->val);
    printf("size:%u\n", THIS(que).size());
    printf("empty:%c\n", "YN"[!THIS(que).empty()]);
    for (int i = 0; i < 10; i++) {
        Int temp = THIS(que).front();
        printf("%lld ", temp->val);
        THIS(que).pop();
    }
    putchar('\n');
    printf("empty:%c\n", "YN"[!THIS(que).empty()]);
    delete(que);
    ARP_FreePool();
}
