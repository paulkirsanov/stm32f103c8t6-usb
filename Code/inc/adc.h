#ifndef __adc_h
#define __adc_h

#include "stm32f10x.h"

void adc1_init(void);
void touchpad_init(void);
void adc_read(ADC_TypeDef *ADC, uint16_t *ADC_data);

#endif
