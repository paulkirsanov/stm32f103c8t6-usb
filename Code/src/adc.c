#include "adc.h"
uint16_t DataADC_DMA = 0;

void adc1_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL &= ~GPIO_CRL_MODE1;
	
	ADC1->CR1 = 0;
	ADC1->SQR1 = 0;
	
	ADC1->CR2 |= ADC_CR2_CAL;
	while(!(ADC1->CR2 & ADC_CR2_CAL)){}
	
	ADC1->CR2 = ADC_CR2_EXTSEL;
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	ADC1->CR2 |= ADC_CR2_CONT;
	
	ADC1->SQR3 = 1;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
}

void adc1_init_dma(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	RCC->APB2ENR |= RCC_APB2ENR_ADC1EN;
	RCC->AHBENR |= RCC_AHBENR_DMA1EN;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF1;
	GPIOA->CRL &= ~GPIO_CRL_MODE1;
	
	ADC1->CR1 = 0;
	ADC1->SQR1 = 0;
	
	ADC1->CR2 |= ADC_CR2_CAL;
	while(!(ADC1->CR2 & ADC_CR2_CAL)){}
	
	ADC1->CR2 = ADC_CR2_EXTSEL;
	ADC1->CR2 |= ADC_CR2_EXTTRIG;
	ADC1->CR2 |= ADC_CR2_CONT;
		
	ADC1->CR2 |= ADC_CR2_DMA;
	
	ADC1->SQR3 = 1;
	ADC1->CR2 |= ADC_CR2_ADON;
	ADC1->CR2 |= ADC_CR2_SWSTART;
	
	DMA1_Channel1->CPAR |= ADC1_DR_Address; //Указываем адрес периферии
	DMA1_Channel1->CMAR |= (uint32_t)&DataADC_DMA; //Задаем адрес памяти
	DMA1_Channel1->CNDTR = 1; //Количество пересылаемых значений
	DMA1_Channel1->CCR = 0;
	DMA1_Channel1->CCR &= ~DMA_CCR1_DIR; //Указываем направление передачи данных, из периферии в память
	DMA1_Channel1->CCR &= ~DMA_CCR1_PINC; //Адрес периферии не инкрементируем после каждой пересылки
	DMA1_Channel1->CCR |= DMA_CCR1_MINC; //Адрес памяти инкрементируем после каждой пересылки.
	DMA1_Channel1->CCR |= DMA_CCR1_PSIZE_0; //Размерность данных периферии - 16 бит
	DMA1_Channel1->CCR |= DMA_CCR1_MSIZE_0; //Размерность данных памяти - 16 бит
	DMA1_Channel1->CCR |= DMA_CCR1_PL; //Приоритет - очень высокий
	DMA1_Channel1->CCR |= DMA_CCR1_CIRC; //Разрешаем работу DMA в циклическом режиме
	DMA1_Channel1->CCR |= DMA_CCR1_EN; //Разрешаем работу 1-го канала DMA
}

void touchpad_init(void)
{
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;

	GPIOA->CRL &= ~GPIO_CRL_CNF2;
	GPIOA->CRL |= GPIO_CRL_CNF2_0;
	GPIOA->CRL |= GPIO_CRL_MODE2_1;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF3;
	GPIOA->CRL |= GPIO_CRL_CNF3_0;
	GPIOA->CRL |= GPIO_CRL_MODE3_1;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF4;
	GPIOA->CRL |= GPIO_CRL_CNF4_0;
	GPIOA->CRL |= GPIO_CRL_MODE4_1;
	
	GPIOA->CRL &= ~GPIO_CRL_CNF5;
	GPIOA->CRL |= GPIO_CRL_CNF5_0;
	GPIOA->CRL |= GPIO_CRL_MODE5_1;
}

uint16_t adc_read(void)
{
	return ADC1->DR;
}

uint16_t print_value_ADC(void)
{
	return DataADC_DMA;
}
