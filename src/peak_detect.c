
#include "peak_detect.h"
#include "write.h"
#include "read.h"
#include "config.h"

extern double current_setpoint;
#include <unistd.h> 

float peak_detect()
{
    double min_height = 0., ret;
    for (double i = 0.1; i < 0.9; i += 1. / 2000)
    {
        write_ch1(i);
        usleep(500);
        double cur_height = read_ch1_avg();

        if (cur_height < min_height)
        {
            min_height = cur_height;
            ret = i;
        }
    }
    current_setpoint = SETP_TO_MIN_RATIO * min_height; // setpoint slight above 
    return ret;
}
