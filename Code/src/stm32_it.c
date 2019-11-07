
#include "stm32_it.h"

extern __IO uint32_t DevRemoteWakeup;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  while (1)
  {
  }
}

void BusFault_Handler(void)
{
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

void USB_LP_CAN1_RX0_IRQHandler(void)
{
  USB_Istr();
}

//void EXTI9_5_IRQHandler(void)
//{
  //if (EXTI_GetITStatus(KEY_BUTTON_EXTI_LINE) != RESET)
  //{
    /* Check if the remote wakeup feature is enabled (it could be disabled 
        by the host through ClearFeature request) */
   // if ((pInformation->Current_Feature & 0x20) && (DevRemoteWakeup == 0))
    //{      
     // pInformation->Current_Feature &= ~0x20; 
      /* Set DevRemoteWakeup when doing the remote wakeup */
     // DevRemoteWakeup = 1;

      /* Exit low power mode and re-configure clocks */
    //  Resume(RESUME_INTERNAL);
    //}
  
    /* Clear the EXTI line pending bit */
    //EXTI_ClearITPendingBit(KEY_BUTTON_EXTI_LINE);
 // }
//}

void USBWakeUp_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR18;
}
