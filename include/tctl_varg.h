#ifndef INCLUDE/TCTL_VARG_H
#define INCLUDE/TCTL_VARG_H
#include "tctl_metaclass.h"

enum va_type {POD, OBJ};
typedef struct {
    unsigned long cur;
    unsigned long len;
    void *head;
} varg_t;
//VA的结尾描述变量
#define VAEND NULL
//为变量生成正确的va参数
#define _VA_AUX(_t) _Generic(_t,                                     \
                            float              : _valueAux('f', _t), \
                            double             : _valueAux('F', _t), \
                            char               : _valueAux('c', _t), \
                            short              : _valueAux('s', _t), \
                            int                : _valueAux('i', _t), \
                            long               : _valueAux('l', _t), \
                            long long          : _valueAux('L', _t), \
                            unsigned char      : _valueAux('c', _t), \
                            unsigned short     : _valueAux('s', _t), \
                            unsigned int       : _valueAux('i', _t), \
                            unsigned long      : _valueAux('l', _t), \
                            unsigned long long : _valueAux('L', _t), \
                            default            : _valueAux('p', _t)
//对多个参数生成varg_t
#define VA(...) MAP_LIST(_VA_AUX, ##__VA_ARGS__)


#endif //INCLUDE/TCTL_VARG_H
