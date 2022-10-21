#ifndef CURRENT_LOCK_H
#define CURRENT_LOCK_H

double current_offset, current_setpoint; 

void init_current_lock(double, double, double, double);
void update_current_lock();

#endif 