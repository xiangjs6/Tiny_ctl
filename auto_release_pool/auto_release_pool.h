//
// Created by xjs on 2020/11/24.
//

#ifndef AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
#define AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
#include <stdlib.h>
typedef struct {
    unsigned long long major;
    unsigned long long minor;
} ARP_ResId_t;
//! resident_memory Constructor
typedef void (*Res_ctorfunc_t)(void *pMemLoc, void *pre_pMemLoc);

//! Destructor function
typedef void (*dtorfunc_t)(void*);
//! create a new pool
int ARP_CreateRelPool(void);
//! release the pool and all its block reference
void ARP_FreePool(void);
//! get number of memory blocks referenced by this pool
int ARP_GetPoolNodesCount(void);
//! get number of pools
int ARP_GetPoolsCount(void);

//! increment reference count by 1
int ARP_Release(void *pMemLoc);
//! decrement reference count by 1
int ARP_Retain(void *pMemLoc);

//! allocate memory just like 'calloc' WITHOUT adding to current pool
void *ARP_Calloc(size_t num, size_t size, .../*dtorfunc_t*/);
void *ARP_Malloc(size_t len, .../*dtorfunc_t*/);
//! just like above with a destructor function
//void * ARP_CallocDtor(size_t num, size_t size, dtorfunc_t dtorFunc);
//! allocate memory and add itself to the current pool
void *ARP_CallocARel(size_t num, size_t size, .../*dtorfunc_t*/);
void *ARP_MallocARel(size_t len, .../*dtorfunc_t*/);
//! just like above with a destructor function
//void * ARP_CallocARelDtor(size_t num, size_t size, dtorfunc_t dtorFunc);
//! allocate memory whith a resident_id in release pool
void *ARP_AssignResId(void *pMemLoc);

//! just reallocate memory
void *ARP_Realloc(void * pMemLoc, size_t size);
#endif //AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
