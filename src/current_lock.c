#include "current_lock.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#include "config.h"
#include "read.h"
#include "write.h"

double current_setpoint;

static double kp, ki, kd;

static double get_error() { return -(read_ch1_avg() - current_setpoint); }

void init_current_lock(double kp_, double ki_, double kd_, double offset_) {
    kp = kp_;
    ki = ki_;
    kd = kd_;
    write_ch2(offset_);
    current_offset = offset_;
}

void update_current_lock() {
    static double last_error = 0., integral = 0.;

    double error = get_error();
    // new PID output
    float out = (kp * error + ki * integral + kd * (error - last_error)) +
                current_offset;
    // printf("current_out %lf sp %lf ofset %lf \n", out, current_setpoint,
    // current_offset);
    // unlocks, also takes care of  anti-windup
    if (out > CURRENT_LOCK_UPPER_BOUND || out < CURRENT_LOCK_UPPER_BOUND) {
        printf("unlock detected at out %lf\n", out);
        integral = 0.;
        last_error = 0;
#ifndef RELOCK_WITH_STEP
        while (read_ch1_avg() > RELOCK_TO_SETPOINT_RATIO * current_setpoint) {
            printf("relocking\n");
            write_ch2(RELOCK_HIGH_CURRENT);
            usleep(RELOCK_HIGH_HOLDTIME);
            float current = RELOCK_HIGH_CURRENT;
            while (current > CURRENT_LOCK_LOWER_BOUND) {
                current = current - 0.001;
                write_ch2(current);
                if (read_ch1_avg() >
                    RELOCK_TO_SETPOINT_RATIO * current_setpoint)
                    return;
                usleep(1000);
            }

            printf("relock at %lf\n", current);
        }
#else
        while (read_ch1_avg() > RELOCK_TO_SETPOINT_RATIO * current_setpoint) {
            printf("relocking\n");
            write_ch2(RELOCK_HIGH_CURRENT);
            usleep(RELOCK_HIGH_HOLDTIME);
            write_ch2(RELOCK_LOW_CURRENT);
            usleep(RELOCK_LOW_HOLDTIME);
        }
#endif
    } else {
        write_ch2(out);
        integral += error;
        last_error = error;
    }
}
