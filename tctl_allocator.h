//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_ALLOCATOR_H
#define TINY_CTL_TCTL_ALLOCATOR_H
#include <stddef.h>

void *__allocate(size_t n);
void *__reallocate(void *p, size_t old_size, size_t new_size);
void __deallocate(void *p, size_t n);

/*void *__malloc_alloc(size_t n);
void *__malloc_realloc(void *p, size_t n);
void __malloc_dealloc(void *p, size_t n);
void *__fragment_alloc(size_t n);
void __fragment_dealloc(void *p, size_t n);*/
void (*__set_malloc_handler(void(*f)(void)))(void);
#endif //TINY_CTL_TCTL_ALLOCATOR_H
