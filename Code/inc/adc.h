#ifndef __adc_h
#define __adc_h

#include "stm32f10x.h"
#include <stdio.h>
#include "usart.h"

#define ADC1_DR_Address    ((u32)0x40012400+0x4c)

void adc1_init(void);
void adc1_init_dma(void);
void touchpad_init(void);
uint16_t adc_read(void);
uint16_t print_value_ADC(void);
//void touchpad_test(void);

#endif
