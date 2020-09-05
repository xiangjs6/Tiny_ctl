//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_ALLOCATOR_H
#define TINY_CTL_ALLOCATOR_H

#include <stddef.h>

void *allocate(size_t size);
void *reallocate(void *p, size_t old_size, size_t new_size);
void deallocate(void *p, size_t size);
void (*set_malloc_handler(void(*f)(void)))(void);
#endif //TINY_CTL_ALLOCATOR_H
