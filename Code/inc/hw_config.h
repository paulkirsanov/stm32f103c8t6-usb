
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

#include "platform_config.h"
#include "usb_type.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_pwr.h"
#include "usb_istr.h"

#define CURSOR_STEP     5
#define THRESHOLD				10

#define DOWN            2
#define LEFT            3
#define RIGHT           4
#define UP              5

void Set_System(void);
void Set_USBClock(void);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void TouchPad_Send(uint8_t Keys);
void TouchPadState(void);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/
