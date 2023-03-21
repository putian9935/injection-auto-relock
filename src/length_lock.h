/**
 * @file length_lock.h
 * @brief Lock the length of a scanning FP cavity. 
 * @version 0.1
 * @date 2023-03-21
 * 
 * 
 */
#ifndef LENGTH_LOCK_H
#define LENGTH_LOCK_H

#include "peak_detect.h"

extern double cur_out; 
double get_error();
double get_trans();
void init_length_lock(double, double, double, double);
void update_length_lock();
#endif