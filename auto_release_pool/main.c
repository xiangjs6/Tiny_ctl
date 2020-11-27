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
    for (int i = 0; i < 128; i++) {
        ARP_ResId_t id = ARP_AssignResId(0);
    }
    ARP_FreePool();
    ARP_Release(p);
    return 0;
}
