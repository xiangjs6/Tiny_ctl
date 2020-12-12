#include <stdio.h>
#include "auto_release_pool.h"
#include <string.h>

int main()
{
    ARP_CreateRelPool();
    char *p = ARP_Retain(ARP_MallocARel(32));
    strcpy(p, "hello world\n");
    printf("%s", p);
    printf("%d %d\n", ARP_GetPoolsCount(), ARP_GetPoolNodesCount());

    char *pp = ARP_MallocARel(64);
    strcpy(pp, "awsl");
    printf("%s %p\n", pp, pp);
    ARP_CreateRelPool();
    ARP_FreePool();
    char *last = ARP_MallocARel(32);
    strcpy(last, "hhhhh");
    ARP_ExitARel(p);
    ARP_FreePool();
    ARP_Realloc(p, 1024);
    ARP_Release(p);
    ARP_JoinARel(p);
    return 0;
}
