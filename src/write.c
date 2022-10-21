#include "write.h"

void write_prepare()
{
    rp_GenReset();
    rp_GenWaveform(RP_CH_1, RP_WAVEFORM_DC_NEG);
    rp_GenWaveform(RP_CH_2, RP_WAVEFORM_DC_NEG); 

    rp_GenAmp(RP_CH_1, 1.);
    rp_GenAmp(RP_CH_2, 1.);

    rp_GenOutEnable(RP_CH_1);
    rp_GenOutEnable(RP_CH_2);
}

void write_ch1(float v)
{
    rp_GenAmp(RP_CH_1, 1. - v);
}
void write_ch2(float v)
{
    rp_GenAmp(RP_CH_2, 1. - v);
}

#include<unistd.h>
void write_ch1_delay(float v)
{
    write_ch1(v);
    usleep(100);
}
