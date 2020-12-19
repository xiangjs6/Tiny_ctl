//
// Created by xjs on 2020/12/19.
//

#ifndef TINY_CTL_TCTL_HASH_FUN_H
#define TINY_CTL_TCTL_HASH_FUN_H
#include <stddef.h>
size_t hash_char(const void*);
size_t hash_short(const void*);
size_t hash_int(const void*);
size_t hash_long(const void*);
size_t hash_llong(const void*);
size_t hash_uchar(const void*);
size_t hash_ushort(const void*);
size_t hash_uint(const void*);
size_t hash_ulong(const void*);
size_t hash_ullong(const void*);
size_t hash_str(const void*);
#endif //TINY_CTL_TCTL_HASH_FUN_H
