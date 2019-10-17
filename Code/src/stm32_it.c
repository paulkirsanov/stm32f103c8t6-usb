
/* Includes ------------------------------------------------------------------*/
#include "stm32_it.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t DevRemoteWakeup;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * Function Name  : NMI_Handler
  * Description    : This function handles NMI exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void NMI_Handler(void)
{
}

/**
  * Function Name  : HardFault_Handler
  * Description    : This function handles Hard Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : MemManage_Handler
  * Description    : This function handles Memory Manage exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : BusFault_Handler
  * Description    : This function handles Bus Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : UsageFault_Handler
  * Description    : This function handles Usage Fault exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * Function Name  : SVC_Handler
  * Description    : This function handles SVCall exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void SVC_Handler(void)
{
}

/**
  * Function Name  : DebugMon_Handler
  * Description    : This function handles Debug Monitor exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void DebugMon_Handler(void)
{
}

/**
  * Function Name  : PendSV_Handler
  * Description    : This function handles PendSVC exception.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void PendSV_Handler(void)
{
}

/**
  * Function Name  : SysTick_Handler
  * Description    : This function handles SysTick Handler.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void SysTick_Handler(void)
{
}

/******************************************************************************/
/*                 STM32 Peripherals Interrupt Handlers                       */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32xxx.s).                                                */
/******************************************************************************/

/**
  * Function Name  : USB_IRQHandler
  * Description    : This function handles USB Low Priority interrupts
  *                  requests.
  * Input          : None
  * Output         : None
  * Return         : None
  */
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

/**
  * Function Name  : USBWakeUp_IRQHandler
  * Description    : This function handles USB WakeUp interrupt request.
  * Input          : None
  * Output         : None
  * Return         : None
  */
void USBWakeUp_IRQHandler(void)
{
	EXTI->PR = EXTI_PR_PR18;
}
