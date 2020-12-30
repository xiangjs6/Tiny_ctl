﻿//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_NUMERIC_H
#define TINY_CTL_TCTL_NUMERIC_H

#include "tctl_iterator.h"
#include "tctl_def.h"
typedef struct {
    UnaryOperation inc;
    UnaryOperation dec;
    BinaryOperation add;
    BinaryOperation sub;
    BinaryOperation mul;
    BinaryOperation div;
    BinaryOperation mod;
    void (*assign)(void *first, const void *second);
    int (*cmp)(const void *first, const void *second);
} NumOperations;
void *accumulate(const IterType first, const IterType last, void *init, BinaryOperation binary_op);
IterType adjacent_difference(const IterType first, const IterType last, const IterType result, BinaryOperation binary_op);
void *inner_product(const IterType first1, const IterType last1, const IterType first2, void *init, BinaryOperation binary_op1, BinaryOperation binary_op2);
IterType partial_sum(const IterType first, const IterType last, const IterType result, BinaryOperation binary_op);
void iota(const IterType first, const IterType last, void *x, const NumOperations *ops);
#endif //TINY_CTL_TCTL_NUMERIC_H