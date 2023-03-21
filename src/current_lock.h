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

void init_current_lock(double, double, double, double);
void update_current_lock();

#endif 