/**
 * @file write.h
 * @brief Write to Red Pitaya fast channel OUT1 and OUT2. 
 * @version 0.1
 * @date 2023-03-21
 * 
 */
#ifndef WRITE_H
#define WRITE_H

#include "rp.h"

void write_prepare(); 

void write_ch1(float);
void write_ch2(float);
#endif 