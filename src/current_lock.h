/**
 * @file current_lock.h
 * @brief Lock the current of the slave laser. 
 * @version 0.1
 * @date 2023-03-21
 * 
 * 
 */
#ifndef CURRENT_LOCK_H
#define CURRENT_LOCK_H

double current_offset, current_setpoint; 

/**
 * @brief Initialize the current PID lock 
 * 
 * @param kp P gain
 * @param ki I gain
 * @param kd D gain
 * @param ofst offset (DC) output
 */
void init_current_lock(double kp, double ki, double kd, double ofst);

/**
 * @brief Update the length PID lock for one step  
 */
void update_current_lock();

#endif 