//
// Created by xjs on 2020/12/12.
//

#include "../include/tctl_debug.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
void exit_log(char *format, ...)
{
    va_list v;
    va_start(v, format);
    vfprintf(stderr, format, v);
    va_end(v);
    exit(-1);
}