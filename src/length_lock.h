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

// output of PZT current 
extern double cur_out; 

/**
 * @brief Get the error signal 
 * 
 * Form the error signal by step the PZT up and down. 
 * 
 * @return double 
 * The error signal
 */
double get_error();

/**
 * @brief Get the cavity transmission 
 * 
 * @return double 
 * The transmission signal 
 */
double get_trans();

/**
 * @brief Initialize the length PID lock 
 * 
 * @param kp P gain
 * @param ki I gain
 * @param kd D gain
 * @param ofst offset (DC) output
 */
void init_length_lock(double kp, double ki, double kd, double ofst);

/**
 * @brief Update the length PID lock for one step  
 */
void update_length_lock();
#endif