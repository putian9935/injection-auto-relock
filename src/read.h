/**
 * @file read.h
 * @brief Read from Red Pitaya fast channel IN1 
 * @version 0.1
 * @date 2023-03-21
 * 
 * 
 */
#ifndef READ_H
#define READ_H

#include "rp.h"

void read_prepare();

float read_ch1();

double read_ch1_avg();
#endif