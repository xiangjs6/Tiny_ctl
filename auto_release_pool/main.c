#include <stdio.h>
#include "auto_release_pool.h"
#include <string.h>

int main()
{
    ARP_CreateRelPool();
    char *p = ARP_MallocARel(32);
    strcpy(p, "hello world\n");
    printf("%s", p);
    printf("%d %d\n", ARP_GetPoolsCount(), ARP_GetPoolNodesCount());
    ARP_FreePool();
    return 0;
}
