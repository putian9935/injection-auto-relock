#include "read.h"

void read_prepare()
{
    rp_AcqReset();
    rp_AcqSetTriggerSrc(RP_TRIG_SRC_DISABLED);
}

float read_ch1()
{
    static float reading = 0.;
    static uint32_t sz = 1;

    rp_AcqStart();
    rp_AcqGetOldestDataV(RP_CH_1, &sz, &reading);
    rp_AcqStop();

    return reading;
}

#define READ_AVG 50
double read_ch1_avg()
{
    double ret = 0;
    for (int c = 0; c < READ_AVG; ++c)
        ret += read_ch1();
    return ret / READ_AVG;
}
