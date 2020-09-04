//
// Created by xjs on 2020/8/31.
//

#ifndef TINY_CTL_TCTL_OBJECT_H
#define TINY_CTL_TCTL_OBJECT_H

#include <stddef.h>
#include "tctl_def.h"

/*
 * tctl的对象规则
 * 1、使用对象前调用THIS或者pthis传递对象地址
 * 2、建议将私有变量重新封装到单独结构体中，并将其大小填充到对象结构体
 * 3、建议私有成员函数使用同一结构体指针，指向一个不变常量上
 * 4、遵守各个泛类对象指定的规则，比如迭代器
 * */
void **pthis(void);
#define THIS(p) ((*(typeof(p))((*pthis()) = p)))

#endif //TINY_CTL_TCTL_OBJECT_H
