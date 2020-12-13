//
// Created by xjs on 2020/11/24.
//

#ifndef AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
#define AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
#include <stddef.h>
//! Destructor function
typedef void (*dtorfunc_t)(void*);
//! create a new pool
void ARP_CreatePool(void);
//! release the pool and all its block reference
void ARP_FreePool(void);
//! get number of memory blocks referenced by this pool
int ARP_GetPoolNodesCount(void);
//! get number of pools
int ARP_GetPoolsCount(void);

//! decrement reference count by 1
int ARP_Release(void *pMemLoc);
//! increment reference count by 1
void *ARP_Retain(void *pMemLoc);
//! join and exit memory in release pool
int ARP_JoinARel(void *pMemLoc);
int ARP_ExitARel(void *pMemLoc);
//! allocate memory just like 'calloc' WITHOUT adding to current pool
void *ARP_Calloc(size_t num, size_t size);
void *ARP_Malloc(size_t len);
//! just like above with a destructor function
void * ARP_CallocDtor(size_t num, size_t size, dtorfunc_t dtorFunc);
void *ARP_MallocDtor(size_t len, dtorfunc_t dtorFunc);
//! allocate memory and add itself to the current pool
void *ARP_CallocARel(size_t num, size_t size);
void *ARP_MallocARel(size_t len);
//! just like above with a destructor function
void *ARP_CallocARelDtor(size_t num, size_t size, dtorfunc_t dtorFunc);
void *ARP_MallocARelDtor(size_t len, dtorfunc_t dtorFunc);
//! just reallocate memory
void *ARP_Realloc(void *pMemLoc, size_t size);

#endif //AUTO_RELEASE_POOL_AUTO_RELEASE_POOL_H
