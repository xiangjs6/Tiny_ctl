//
// Created by xjs on 2020/8/31.
//

#include "allocator.h"
#include "tctl_allocator.h"

void *allocate(size_t size)
{
    return __allocate(size);
}

void *allocate_n(size_t nmemb, size_t size)
{
    return __allocate(nmemb * size);
}

void *reallocate(void *p, size_t old_size, size_t new_size)
{
    return __reallocate(p, old_size, new_size);
}

void deallocate(void *p, size_t size)
{
    __deallocate(p, size);
}

void deallocate_n(void *p, size_t nmemb, size_t size)
{
    __deallocate(p, nmemb * size);
}

void (*set_malloc_handler(void(*f)(void)))(void)
{
    return __set_malloc_handler(f);
}