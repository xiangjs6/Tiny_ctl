//
// Created by xjs on 2021/2/8.
//

#ifndef TINY_CTL__TCTL_UNORDERED_MAP_H
#define TINY_CTL__TCTL_UNORDERED_MAP_H
#include "../../include/tctl_unordered_map.h"
#include "_tctl_class.h"
struct Unordered_MapSelector {
    char _[sizeof(struct ClassSelector)];
    Iterator (*begin)(void);
    Iterator (*end)(void);
    size_t (*size)(void);
    bool (*empty)(void);
    void (*erase)(Iterator iter);
    Iterator (*insert)(Pair x);
    size_t (*count)(FormWO_t x);
    Iterator (*find)(FormWO_t x);
    size_t (*bucket_count)(void);
    size_t (*max_bucket_count)(void);
    void (*reserve)(size_t);
    void (*clear)(void);
    void (*swap)(Unordered_Map);
};          

Form_t _Unordered_MapClass(void);
extern const struct Unordered_MapSelector *_Unordered_MapS;
#endif //TINY_CTL__TCTL_UNORDERED_MAP_H
