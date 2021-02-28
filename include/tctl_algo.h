//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_ALGO_H
#define TINY_CTL_TCTL_ALGO_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
Iterator set_union(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);
Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator adjacent_find(Iterator _first, Iterator _last, .../*Compare*/);
size_t count(Iterator _first, Iterator _last, FormWO_t val, .../*Compare*/);
size_t count_if(Iterator _first, Iterator _last, Predicate pred);
Iterator find(Iterator _first, Iterator _last, FormWO_t val, .../*Compare*/);
Iterator find_if(Iterator _first, Iterator _last, Predicate pred);
Iterator find_end(Iterator _first1, Iterator _last1,
                  Iterator _first2, Iterator _last2, .../*Compare*/);
Iterator find_first_of(Iterator _first1, Iterator _last1,
                       Iterator _first2, Iterator _last2, .../*Equal*/);
UnaryFunc for_each(Iterator _first, Iterator _last, UnaryFunc f);
void generate(Iterator _first, Iterator _last, Generator gen, .../*Assign*/);
Iterator generate_n(Iterator _first, size_t n, Generator gen, .../*Assign*/);
bool includes(Iterator _first1, Iterator _last1,
              Iterator _first2, Iterator _last2, .../*Compare*/);
Iterator max_element(Iterator _first, Iterator _last, .../*Compare*/);
Iterator merge(Iterator _first1, Iterator _last1,
               Iterator _first2, Iterator _last2,
               Iterator _result, .../*Assign, Compare*/);
Iterator min_element(Iterator _first, Iterator _last, .../*Compare*/);
Iterator partition(Iterator _first, Iterator _last, Predicate pred);
Iterator remove_element(Iterator _first, Iterator _last, FormWO_t val, .../*Assign, Compare*/);
Iterator remove_copy(Iterator _first, Iterator _last,
                     Iterator _result, FormWO_t val, .../*Assign, Compare*/);
Iterator remove_if(Iterator _first, Iterator _last, Predicate pred, .../*Assign*/);
Iterator remove_copy_if(Iterator _first, Iterator _last,
                        Iterator _result, Predicate pred, .../*Assign*/);
Iterator search(Iterator _first1, Iterator _last1,
                Iterator _first2, Iterator _last2, .../*Compare*/);
#endif //TINY_CTL_TCTL_ALGO_H
