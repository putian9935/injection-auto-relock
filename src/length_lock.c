#include "length_lock.h"

#include <unistd.h>  // for usleep

#include "config.h"
#include "read.h"
#include "write.h"

#define DELAY_UPDATE 100

double offset, cur_out;
static double const dx = 1. / 64;
double get_error() {
    write_ch1(cur_out - dx);
    usleep(DELAY_UPDATE);
    double left = read_ch1_avg();

    write_ch1(cur_out + dx);
    usleep(DELAY_UPDATE);
    double right = read_ch1_avg();
    return (right - left) / dx;
}

double get_trans() { return read_ch1(); }

static double kp, ki, kd;

void init_length_lock(double kp_, double ki_, double kd_, double offset_) {
    kp = kp_;
    ki = ki_;
    kd = kd_;
    write_ch1(offset_);
    offset = cur_out = offset_;
}

void update_length_lock() {
    static double last_error = 0., integral = 0.;

    double error = get_error();
    float out = (kp * error + ki * integral + kd * (error - last_error)) + offset;
    if (out > LENGTH_LOCK_UPPER_BOUND || out < LENGTH_LOCK_LOWER_BOUND) {
        offset = cur_out = peak_detect();
        write_ch1(offset);
        integral = last_error = 0.;
    } else {
        write_ch1(out);
        integral += error;
        last_error = error;
        cur_out = out;
    }
}
