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
        ARP_ResId_t  id  = ARP_AssignResId(i);
        int *p_int = ARP_MallocARel(sizeof(int));
        *p_int = i;
        ARP_SetResId(p_int, id, NULL);
    }
    ARP_ResId_t id = ARP_AssignResId(0);
    char *pp = ARP_MallocARel(64);
    strcpy(pp, "awsl");
    ARP_SetResId(pp, id, NULL);
    printf("%s %p\n", pp, pp);
    ARP_CreateRelPool();
    char *ppp = ARP_AllocWithResId(id);
    printf("%s %p\n", ppp, ppp);
    ARP_FreePool();
    ARP_Release(p);
    ppp = ARP_AllocWithResId(id);
    printf("%s %p\n", ppp, ppp);
    ARP_FreeResId(id);
    char *last = ARP_MallocARel(32);
    strcpy(last, "hhhhh");
    id = ARP_AssignResId(10);
    ARP_SetResId(last, id, NULL);
    ppp = ARP_AllocWithResId(id);
    printf("%s %p\n", ppp, ppp);
    ARP_FreePool();
    return 0;
}
