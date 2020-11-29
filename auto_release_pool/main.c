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
    ARP_ResId_t id = ARP_AssignResId(0);
    char *pp = ARP_MallocARel(64);
    strcpy(pp, "awsl\n");
    ARP_SetResId(pp, id, NULL);
    printf("%s", pp);
    ARP_CreateRelPool();
    char *ppp = ARP_AllocWithResId(id);
    printf("%s", ppp);
    ARP_FreePool();
    ARP_Release(p);
    ARP_FreePool();
    return 0;
}
