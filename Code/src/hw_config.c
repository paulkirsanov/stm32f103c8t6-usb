
#include "hw_config.h"
#include "adc.h"

signed int  value_x = 0;
signed int  value_y = 0;

//static signed int previousX = 0;
//static signed int previousY = 0;

char str1[13] = {0};

bool update = FALSE;

ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
extern __IO uint8_t PrevXferComplete;

/* Private function prototypes -----------------------------------------------*/
static void IntToUnicode (uint32_t value , uint8_t *pbuf , uint8_t len);
float convert(float value, float From1, float From2, float To1, float To2);
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

void TouchPadState(void)
{
	update = FALSE;
	int8_t TouchPad_Buffer[4] = {0, 0, 0, 0};
	
	GPIOA->BSRR = GPIO_BSRR_BS2;
	GPIOA->BSRR = GPIO_BSRR_BS3;
	
	GPIOA->BRR = GPIO_BRR_BR4;
	GPIOA->BRR = GPIO_BRR_BR5;
	
	value_x = convert(((float)print_value_ADC())*3/4096, 0.55, 0.8, -127.0, 127.0);
	
	if(value_x < 200)
	{
		GPIOA->BSRR = GPIO_BSRR_BS2;
		GPIOA->BSRR = GPIO_BSRR_BS4;
	
		GPIOA->BRR = GPIO_BRR_BR3;
		GPIOA->BRR = GPIO_BRR_BR5;
		
		value_y = convert(((float)print_value_ADC())*3/4096, 0.55, 0.8, -127.0, 127.0);
		
		if(value_x < -THRESHOLD){
			TouchPad_Buffer[2] = -CURSOR_STEP;
		}else if(value_x > THRESHOLD){
			TouchPad_Buffer[2] = CURSOR_STEP;
		}else{
			TouchPad_Buffer[2] = 0;
		}
		
		if(value_y < -THRESHOLD){
			TouchPad_Buffer[1] = -CURSOR_STEP;
		}else if(value_y > THRESHOLD){
			TouchPad_Buffer[1] = CURSOR_STEP;
		}else{
			TouchPad_Buffer[1] = 0;
		}
		
		/*if(TouchPad_Buffer[1] == 0 && TouchPad_Buffer[2] == 0)
		{
			TouchPad_Buffer[0] = 1;
		}*/
		
		PrevXferComplete = 0;
		USB_SIL_Write(EP1_IN, (uint8_t *) TouchPad_Buffer, 4);
		SetEPTxValid(ENDP1);
	}
}

void TouchPad_Send(uint8_t Keys)
{
  /*uint8_t TouchPad_Buffer[4] = {0, 0, 0, 0};
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
  }*/
	
	/* prepare buffer to send */
//	TouchPad_Buffer[1] = Y;
//	TouchPad_Buffer[2] = X;
	
	/* Reset the control token to inform upper layer that a transfer is ongoing */
//	PrevXferComplete = 0;
	
	/* Copy mouse position info in ENDP1 Tx Packet Memory Area*/
//	USB_SIL_Write(EP1_IN, TouchPad_Buffer, 4);
	
	/* Enable endpoint for transmission */
//	SetEPTxValid(ENDP1);
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

float convert(float value, float From1, float From2, float To1, float To2)
{
	return (value - From1) / (From2 - From1) * (To2 - To1) + To1;
}
