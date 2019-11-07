#include "main.h"

void rcc_init(void);

__IO uint8_t PrevXferComplete = 1;

int main(void)
{
	rcc_init();
	
  Set_System();
  USB_Interrupts_Config();
  Set_USBClock();
  USB_Init();
	
	delay_tim1_init();
	led_init();
	usart1_init();
	adc1_init_dma();
	touchpad_init();
	timer2_init();
  
  while (1)
  {
    if(bDeviceState == CONFIGURED)
    {
      /*if((TouchPadState() != 0) && (PrevXferComplete))
      {
        TouchPad_Send(TouchPadState());
      }*/
			TouchPadState();
    }
  }
}

void rcc_init(void)
{
	RCC->CR |= ((uint32_t)RCC_CR_HSEON);												//Enable HSE
	while(!(RCC->CR & RCC_CR_HSERDY));													//Ready start HSE
	
	FLASH->ACR = FLASH_ACR_PRFTBE | FLASH_ACR_LATENCY;					//Clock flash memory
	
	RCC->CFGR |= RCC_CFGR_HPRE_DIV1;														//AHB = SYSCLK/1
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV4;														//APB1 = HCLK/4
	RCC->CFGR |= RCC_CFGR_PPRE2_DIV4;														//APB2 = HCLK/4
	
	RCC->CFGR &= ~RCC_CFGR_PLLMULL;															//clear PLLMULL bits
	RCC->CFGR &= ~RCC_CFGR_PLLSRC;															//clear PLLSRC bits
	RCC->CFGR &= ~RCC_CFGR_PLLXTPRE;														//clear PLLXPRE bits
	
	RCC->CFGR |= RCC_CFGR_PLLSRC;																//source HSE
	RCC->CFGR |= RCC_CFGR_PLLXTPRE_HSE;													//sorce HSE/1 = 8MHz
	RCC->CFGR |= RCC_CFGR_PLLMULL9;															//PLL x9: clock = 8MHz * 9 = 72 MHz
	
	RCC->CR |= RCC_CR_PLLON;																		//enable PLL
	while(!(RCC->CR & RCC_CR_PLLRDY));													//wait till PLL is ready
	
	RCC->CFGR &= ~RCC_CFGR_SW;																	//clear SW bits
	RCC->CFGR |= RCC_CFGR_SW_PLL;																//select surce SYSCLK = PLL
	while((RCC->CFGR & RCC_CFGR_SWS) != RCC_CFGR_SWS_1){}				//wait till PLL is used
}

void TIM2_IRQHandler(void)
{
	if(TIM2->SR & TIM_SR_UIF)
	{
		TIM2->SR &= ~TIM_SR_UIF;
		
		GPIOC->ODR ^= GPIO_ODR_ODR13;
	}
}
