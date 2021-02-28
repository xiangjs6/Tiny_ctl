//
// Created by xjs on 2020/12/20.
//

#ifndef TINY_CTL_TCTL_ALGO_H
#define TINY_CTL_TCTL_ALGO_H
#include "tctl_def.h"
#include "tctl_iterator.h"
#include "tctl_metaclass.h"
//Set Operation
Iterator set_union(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator set_intersection(Iterator _first1, Iterator _last1,
                   Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator set_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);

Iterator set_symmetric_difference(Iterator _first1, Iterator _last1,
                        Iterator _first2, Iterator _last2, Iterator _result, ...);

//Data Processing 
Iterator adjacent_find(Iterator _first, Iterator _last, .../*Equal*/);

size_t count(Iterator _first, Iterator _last, FormWO_t val, .../*Compare*/);

size_t count_if(Iterator _first, Iterator _last, Predicate pred);

Iterator find(Iterator _first, Iterator _last, FormWO_t val, .../*Equal*/);

Iterator find_if(Iterator _first, Iterator _last, Predicate pred);

Iterator find_end(Iterator _first1, Iterator _last1,
                  Iterator _first2, Iterator _last2, .../*Equal*/);

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

Iterator partition(Iterator _first, Iterator _last, Predicate pred, .../*Swapfunc*/);

Iterator remove_element(Iterator _first, Iterator _last, FormWO_t val, .../*Assign, Compare*/);

Iterator remove_copy(Iterator _first, Iterator _last,
                     Iterator _result, FormWO_t val, .../*Assign, Compare*/);
Iterator remove_if(Iterator _first, Iterator _last, Predicate pred, .../*Assign*/);

Iterator remove_copy_if(Iterator _first, Iterator _last,
                        Iterator _result, Predicate pred, .../*Assign*/);

void replace(Iterator _first, Iterator _last, FormWO_t old_val, FormWO_t new_val, .../*Assign, Compare*/);

Iterator replace_copy(Iterator _first, Iterator _last,
                      Iterator _result, FormWO_t old_val,
                      FormWO_t new_val, .../*Assign, Compare*/);

void replace_if(Iterator _first, Iterator _last, Predicate pred, FormWO_t new_val, .../*Assign*/);

Iterator replace_copy_if(Iterator _first, Iterator _last, Iterator _result, Predicate pred, FormWO_t new_val, .../*Assign*/);

void reserve(Iterator _first, Iterator _last, .../*Swapfunc*/);

Iterator reserve_copy(Iterator _first, Iterator _last, Iterator _result, .../*Assign*/);

void rotate(Iterator _first, Iterator _middle, Iterator _last, .../*Assign*/);

Iterator rotate_copy(Iterator _first, Iterator _middle, Iterator _last, Iterator _result);

Iterator search(Iterator _first1, Iterator _last1,
                Iterator _first2, Iterator _last2, .../*Equal*/);

Iterator search_n(Iterator _first, Iterator _last, long long count, FormWO_t val, .../*Eqial*/);

Iterator unique(Iterator _first, Iterator _last, .../*Assign, Equal*/);

Iterator unique_copy(Iterator _first, Iterator _last, Iterator _result, .../*Assign, Equal*/);
#endif //TINY_CTL_TCTL_ALGO_H
