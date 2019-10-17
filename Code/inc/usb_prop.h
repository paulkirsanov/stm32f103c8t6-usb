
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void TouchPad_init(void);
void TouchPad_Reset(void);
void TouchPad_SetConfiguration(void);
void TouchPad_SetDeviceAddress (void);
void TouchPad_Status_In (void);
void TouchPad_Status_Out (void);
RESULT TouchPad_Data_Setup(uint8_t);
RESULT TouchPad_NoData_Setup(uint8_t);
RESULT TouchPad_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *TouchPad_GetDeviceDescriptor(uint16_t );
uint8_t *TouchPad_GetConfigDescriptor(uint16_t);
uint8_t *TouchPad_GetStringDescriptor(uint16_t);
RESULT TouchPad_SetProtocol(void);
uint8_t *TouchPad_GetProtocolValue(uint16_t Length);
RESULT TouchPad_SetProtocol(void);
uint8_t *TouchPad_GetReportDescriptor(uint16_t Length);
uint8_t *TouchPad_GetHIDDescriptor(uint16_t Length);

/* Exported define -----------------------------------------------------------*/
#define TouchPad_GetConfiguration          NOP_Process
//#define TouchPad_SetConfiguration          NOP_Process
#define TouchPad_GetInterface              NOP_Process
#define TouchPad_SetInterface              NOP_Process
#define TouchPad_GetStatus                 NOP_Process
#define TouchPad_ClearFeature              NOP_Process
#define TouchPad_SetEndPointFeature        NOP_Process
#define TouchPad_SetDeviceFeature          NOP_Process
//#define TouchPad_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22

#endif /* __USB_PROP_H */
