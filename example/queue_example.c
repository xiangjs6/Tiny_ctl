#include <stdio.h>
#include "../include/tctl_queue.h"
#include "../include/auto_release_pool.h"

#define Import QUEUE

int main(void)
{
    ARP_CreatePool();
    Queue que = new(T(Queue), VA(T(int)));
    for (int i = 0; i < 10; i++)
        THIS(que).push(VA(i));
    printf("front:%d\n", *(int*)THIS(que).front());
    printf("back:%d\n", *(int*)THIS(que).back());
    printf("size:%ld\n", THIS(que).size());
    printf("empty:%c\n", "YN"[!THIS(que).empty()]);
    for (int i = 0; i < 10; i++) {
        int temp = *(int*)THIS(que).front();
        printf("%d ", temp);
        THIS(que).pop();
    }
    putchar('\n');
    printf("empty:%c\n", "YN"[!THIS(que).empty()]);
    delete(que);
    ARP_FreePool();
}
