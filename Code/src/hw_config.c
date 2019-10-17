
#include "hw_config.h"

ErrorStatus HSEStartUpStatus;

static uint8_t state_right = 0;

/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t PrevXferComplete;

/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
//static void Delay(__IO uint32_t nTime);

void Set_System(void)
{
	RCC->APB1ENR |= RCC_APB1ENR_PWREN;
	RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
	
	GPIOA->CRH &= ~GPIO_CRH_CNF11_1 | GPIO_CRH_CNF12_1;
	GPIOA->CRH |= GPIO_CRH_MODE11 | GPIO_CRH_MODE12;

	RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;
	
	GPIOC->CRH &= ~GPIO_CRH_CNF13_0;
	GPIOC->CRH |= GPIO_CRH_MODE13;
} 

void Set_USBClock(void)
{
	RCC->CFGR &= ~RCC_CFGR_USBPRE;
	RCC->APB1ENR |= RCC_APB1ENR_USBEN;
}

/**
  * Function Name  : Leave_LowPowerMode.
  * Description    : Restores system clocks and power while exiting suspend mode.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Leave_LowPowerMode(void)
{
  DEVICE_INFO *pInfo = &Device_Info;
  
  /* Set the device state to the correct state */
  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
  else
  {
    bDeviceState = ATTACHED;
  }
}

/**
  * Function Name  : USB_Interrupts_Config.
  * Description    : Configures the USB interrupts.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void USB_Interrupts_Config(void)
{
  /* 2 bit for pre-emption priority, 2 bits for subpriority */
//  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the USB interrupt */

//  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;
//  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	
	NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
	NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0);
}

void USB_Cable_Config (FunctionalState NewState)
{
  if (NewState != DISABLE)
		GPIOC->BSRR = GPIO_BSRR_BR13;
  else
		GPIOC->BSRR = GPIO_BSRR_BS13;
}

uint8_t TouchPadState(void)
{
	if(state_right != 5)
	{
		state_right++;
		return RIGHT;
	}
	else state_right = 0;

	return 0;
}

void TouchPad_Send(uint8_t Keys)
{
  uint8_t Mouse_Buffer[4] = {0, 0, 0, 0};
  int8_t X = 0, Y = 0;
  
	switch (Keys)
  {
		case LEFT:
			X -= CURSOR_STEP;
			break;

		case RIGHT:
			X += CURSOR_STEP;
			break;

		case UP:
			Y -= CURSOR_STEP;
			break;

		case DOWN:
			Y += CURSOR_STEP;
			break;

		default:
			return;
  }
  /* prepare buffer to send */
	Mouse_Buffer[1] = X;
	Mouse_Buffer[2] = Y;
  
  /* Reset the control token to inform upper layer that a transfer is ongoing */
  PrevXferComplete = 0;
  
  /* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
  USB_SIL_Write(EP1_IN, Mouse_Buffer, 4);
  
  /* Enable endpoint for transmission */
  SetEPTxValid(ENDP1);
}

/**
  * @brief  Inserts a delay time.
  * @param  nTime: specifies the delay time length, in milliseconds.
  * @retval None
  */
/*static void Delay(__IO uint32_t nTime)
{
  for(; nTime != 0; nTime--);
}*/

/**
  * Function Name  : Get_SerialNum.
  * Description    : Create the serial number string descriptor.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void Get_SerialNum(void)
{
  uint32_t Device_Serial0, Device_Serial1, Device_Serial2;

  Device_Serial0 = *(uint32_t*)ID1;
  Device_Serial1 = *(uint32_t*)ID2;
  Device_Serial2 = *(uint32_t*)ID3;
  
  Device_Serial0 += Device_Serial2;

  if (Device_Serial0 != 0)
  {
    IntToUnicode (Device_Serial0, &TouchPad_StringSerial[2] , 8);
    IntToUnicode (Device_Serial1, &TouchPad_StringSerial[18], 4);
  }
}

/**
  * Function Name  : HexToChar.
  * Description    : Convert Hex 32Bits value into char.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len)
{
  uint8_t idx = 0;
  
  for( idx = 0 ; idx < len ; idx++)
  {
    if( ((value >> 28)) < 0xA )
    {
      pbuf[ 2* idx] = (value >> 28) + '0';
    }
    else
    {
      pbuf[2* idx] = (value >> 28) + 'A' - 10; 
    }
    
    value = value << 4;
    
    pbuf[ 2* idx + 1] = 0;
  }
}
