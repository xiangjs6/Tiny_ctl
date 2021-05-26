//
// Created by xjs on 2021/4/23.
//

#ifndef TINY_CTL_TCTL_ARG_H
#define TINY_CTL_TCTL_ARG_H
#include "macro_tool.h"

void *_valueAux(int t, ...);

//可变参数的结尾标识
extern void *VAEND;
//为变量生成正确的obj
#define _VA_VAL(_t, ...) _Generic(_t,                                                \
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
                default : _valueAux("Oo"[!_Generic(_t, Import, default : NULL)], _t))
//对每个参数返回正确的obj对象
#define _VA_ONE(...)
#define _VA_TWOORMORE(...) FIRST(__VA_ARGS__),
#define _VA_ANYONE(val, ...) _valueAux('A', &val, sizeof(val), MERGE(_VA_, NUM(__VA_ARGS__))(__VA_ARGS__) NULL)
#define _VA_FUNC(val, ...) _valueAux('M', &val)
#define _VA_AUX(val, MACRO_FUNC, ...) _VA_##MACRO_FUNC(val, __VA_ARGS__)
#define VA(...) EXPAND(_VA_AUX, FIRST(__VA_ARGS__) REST(__VA_ARGS__), VAL)
#endif //TINY_CTL_TCTL_ARG_H
