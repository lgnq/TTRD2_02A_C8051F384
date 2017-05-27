#ifndef __MCU_H__
#define __MCU_H__

#include "main.h"

// SYSCLK frequency in Hz
#define SYSCLK (48000000)

SBIT(LED_R, SFR_P0, 0);                  
SBIT(LED_G, SFR_P0, 1);                

void sysclk_init(void);
void port_init(void);

void timer_init(uint32_t tick);
void timer_start(void);
void timer_int_enable(void);

void int_enable(void);
void int_disable(void);

#endif