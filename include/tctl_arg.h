//
// Created by xjs on 2021/4/23.
//

#ifndef TINY_CTL_TCTL_ARG_H
#define TINY_CTL_TCTL_ARG_H
#include "map_macro.h"

void *_valueAux(int t, ...);

//可变参数的结尾标识
extern void *VAEND;
//为变量生成正确的obj
#define _VA_AUX(_t) _Generic(_t,                                                     \
                float              : _valueAux('f', _t),                             \
                double             : _valueAux('F', _t),                             \
                char               : _valueAux('c', _t),                             \
                short              : _valueAux('s', _t),                             \
                int                : _valueAux('i', _t),                             \
                long               : _valueAux('l', _t),                             \
                long long          : _valueAux('L', _t),                             \
                unsigned char      : _valueAux('c', _t),                             \
                unsigned short     : _valueAux('s', _t),                             \
                unsigned int       : _valueAux('i', _t),                             \
                unsigned long      : _valueAux('l', _t),                             \
                unsigned long long : _valueAux('L', _t),                             \
                default : _valueAux("oO"[!_Generic(_t, Import, default : NULL)], _t))
//对每个参数返回正确的obj对象
#define VA(...) MAP_LIST(_VA_AUX, ##__VA_ARGS__)
#define VA_ST(val) _valueAux('S', sizeof(val), val)
#endif //TINY_CTL_TCTL_ARG_H
