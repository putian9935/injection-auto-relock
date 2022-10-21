#include "current_lock.h"
#include "read.h"
#include "write.h"

#include <time.h>
#include <unistd.h>
#include <stdio.h>

double current_setpoint;

static double kp, ki, kd;

static double get_error()
{
    return -(read_ch1_avg() - current_setpoint);
}

void init_current_lock(double kp_, double ki_, double kd_, double offset_)
{
    kp = kp_;
    ki = ki_;
    kd = kd_;
    write_ch2(offset_);
    current_offset = offset_;
}

#include <stdlib.h>
void update_current_lock()
{
    static double last_error = 0., integral = 0.;

    double error = get_error();
    float out = (kp * error + ki * integral + kd * (error - last_error)) + current_offset;
    if (out > .8 || out < 0.2) // unlocks
    {
        integral = 0.;
        last_error = 0;
        while (read_ch1_avg() > .9 * current_setpoint)
        {
            printf("relocking\n");
            write_ch2(1);
            usleep(160000);
            write_ch2(0.5);
            usleep(40000);
        }
    }
    else
    {
        write_ch2(out);
        integral += error;
        last_error = error;
    }
}

