
/* Includes ------------------------------------------------------------------*/
#include "usb_prop.h"
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint32_t ProtocolValue;

/* -------------------------------------------------------------------------- */
/*  Structures initializations */
/* -------------------------------------------------------------------------- */

DEVICE Device_Table =
  {
    EP_NUM,
    1
  };

DEVICE_PROP Device_Property =
  {
    TouchPad_init,
    TouchPad_Reset,
    TouchPad_Status_In,
    TouchPad_Status_Out,
    TouchPad_Data_Setup,
    TouchPad_NoData_Setup,
    TouchPad_Get_Interface_Setting,
    TouchPad_GetDeviceDescriptor,
    TouchPad_GetConfigDescriptor,
    TouchPad_GetStringDescriptor,
    0,
    0x40 /*MAX PACKET SIZE*/
  };
USER_STANDARD_REQUESTS User_Standard_Requests =
  {
    TouchPad_GetConfiguration,
    TouchPad_SetConfiguration,
    TouchPad_GetInterface,
    TouchPad_SetInterface,
    TouchPad_GetStatus,
    TouchPad_ClearFeature,
    TouchPad_SetEndPointFeature,
    TouchPad_SetDeviceFeature,
    TouchPad_SetDeviceAddress
  };

ONE_DESCRIPTOR Device_Descriptor =
  {
    (uint8_t*)TouchPad_DeviceDescriptor,
    TOUCHPAD_SIZ_DEVICE_DESC
  };

ONE_DESCRIPTOR Config_Descriptor =
  {
    (uint8_t*)TouchPad_ConfigDescriptor,
    TOUCHPAD_SIZ_CONFIG_DESC
  };

ONE_DESCRIPTOR TouchPad_Report_Descriptor =
  {
    (uint8_t *)TouchPad_ReportDescriptor,
    TOUCHPAD_SIZ_REPORT_DESC
  };

ONE_DESCRIPTOR Mouse_Hid_Descriptor =
  {
    (uint8_t*)TouchPad_ConfigDescriptor + TOUCHPAD_OFF_HID_DESC,
    TOUCHPAD_SIZ_HID_DESC
  };

ONE_DESCRIPTOR String_Descriptor[4] =
  {
    {(uint8_t*)TouchPad_StringLangID, TOUCHPAD_SIZ_STRING_LANGID},
    {(uint8_t*)TouchPad_StringVendor, TOUCHPAD_SIZ_STRING_VENDOR},
    {(uint8_t*)TouchPad_StringProduct, TOUCHPAD_SIZ_STRING_PRODUCT},
    {(uint8_t*)TouchPad_StringSerial, TOUCHPAD_SIZ_STRING_SERIAL}
  };

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * Function Name  : Joystick_init.
  * Description    : Joystick Mouse init routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_init(void)
{
  /* Update the serial number string descriptor with the data from the unique
  ID*/
  Get_SerialNum();

  pInformation->Current_Configuration = 0;
  /* Connect the device */
  PowerOn();

  /* Perform basic device initialization operations */
  USB_SIL_Init();

  bDeviceState = UNCONNECTED;
}

/**
  * Function Name  : TouchPad_Reset.
  * Description    : TouchPad Mouse reset routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_Reset(void)
{
  /* Set TouchPad_DEVICE as not configured */
  pInformation->Current_Configuration = 0;
  pInformation->Current_Interface = 0;/*the default Interface*/

  /* Current Feature initialization */
  pInformation->Current_Feature = TouchPad_ConfigDescriptor[7];
  SetBTABLE(BTABLE_ADDRESS);
  /* Initialize Endpoint 0 */
  SetEPType(ENDP0, EP_CONTROL);
  SetEPTxStatus(ENDP0, EP_TX_STALL);
  SetEPRxAddr(ENDP0, ENDP0_RXADDR);
  SetEPTxAddr(ENDP0, ENDP0_TXADDR);
  Clear_Status_Out(ENDP0);
  SetEPRxCount(ENDP0, Device_Property.MaxPacketSize);
  SetEPRxValid(ENDP0);

  /* Initialize Endpoint 1 */
  SetEPType(ENDP1, EP_INTERRUPT);
  SetEPTxAddr(ENDP1, ENDP1_TXADDR);
  SetEPTxCount(ENDP1, 4);
  SetEPRxStatus(ENDP1, EP_RX_DIS);
  SetEPTxStatus(ENDP1, EP_TX_NAK);

  /* Set this device to response on default address */
  SetDeviceAddress(0);
  bDeviceState = ATTACHED;
}

/**
  * Function Name  : TouchPad_SetConfiguration.
  * Description    : Update the device state to configured.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_SetConfiguration(void)
{
  DEVICE_INFO *pInfo = &Device_Info;

  if (pInfo->Current_Configuration != 0)
  {
    /* Device configured */
    bDeviceState = CONFIGURED;
  }
}

/**
  * Function Name  : TouchPad_SetConfiguration.
  * Description    : Update the device state to addressed.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_SetDeviceAddress (void)
{
  bDeviceState = ADDRESSED;
}
/**
  * Function Name  : TouchPad_Status_In.
  * Description    : TouchPad status IN routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_Status_In(void)
{}

/**
  * Function Name  : TouchPad_Status_Out
  * Description    : TouchPad status OUT routine.
  * Input          : None.
  * Output         : None.
  * Return         : None.
  */
void TouchPad_Status_Out (void)
{}

/**
  * Function Name  : TouchPad_Data_Setup
  * Description    : Handle the data class specific requests.
  * Input          : Request Nb.
  * Output         : None.
  * Return         : USB_UNSUPPORT or USB_SUCCESS.
  */
RESULT TouchPad_Data_Setup(uint8_t RequestNo)
{
  uint8_t *(*CopyRoutine)(uint16_t);

  CopyRoutine = NULL;
  if ((RequestNo == GET_DESCRIPTOR)
      && (Type_Recipient == (STANDARD_REQUEST | INTERFACE_RECIPIENT))
      && (pInformation->USBwIndex0 == 0))
  {
    if (pInformation->USBwValue1 == REPORT_DESCRIPTOR)
    {
      CopyRoutine = TouchPad_GetReportDescriptor;
    }
    else if (pInformation->USBwValue1 == HID_DESCRIPTOR_TYPE)
    {
      CopyRoutine = TouchPad_GetHIDDescriptor;
    }

  } /* End of GET_DESCRIPTOR */

  /*** GET_PROTOCOL ***/
  else if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
           && RequestNo == GET_PROTOCOL)
  {
    CopyRoutine = TouchPad_GetProtocolValue;
  }
  if (CopyRoutine == NULL)
  {
    return USB_UNSUPPORT;
  }
  pInformation->Ctrl_Info.CopyData = CopyRoutine;
  pInformation->Ctrl_Info.Usb_wOffset = 0;
  (*CopyRoutine)(0);
  return USB_SUCCESS;
}

/**
  * Function Name  : TouchPad_NoData_Setup
  * Description    : handle the no data class specific requests
  * Input          : Request Nb.
  * Output         : None.
  * Return         : USB_UNSUPPORT or USB_SUCCESS.
  */
RESULT TouchPad_NoData_Setup(uint8_t RequestNo)
{
  if ((Type_Recipient == (CLASS_REQUEST | INTERFACE_RECIPIENT))
      && (RequestNo == SET_PROTOCOL))
  {
    return TouchPad_SetProtocol();
  }

  else
  {
    return USB_UNSUPPORT;
  }
}

/**
  * Function Name  : TouchPad_GetDeviceDescriptor.
  * Description    : Gets the device descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the device descriptor.
  */
uint8_t *TouchPad_GetDeviceDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Device_Descriptor);
}

/**
  * Function Name  : TouchPad_GetConfigDescriptor.
  * Description    : Gets the configuration descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *TouchPad_GetConfigDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Config_Descriptor);
}

/**
  * Function Name  : TouchPad_GetStringDescriptor
  * Description    : Gets the string descriptors according to the needed index
  * Input          : Length
  * Output         : None.
  * Return         : The address of the string descriptors.
  */
uint8_t *TouchPad_GetStringDescriptor(uint16_t Length)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  if (wValue0 >= 4)
  {
    return NULL;
  }
  else
  {
    return Standard_GetDescriptorData(Length, &String_Descriptor[wValue0]);
  }
}

/**
  * Function Name  : TouchPad_GetReportDescriptor.
  * Description    : Gets the HID report descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *TouchPad_GetReportDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &TouchPad_Report_Descriptor);
}

/**
  * Function Name  : TouchPad_GetHIDDescriptor.
  * Description    : Gets the HID descriptor.
  * Input          : Length
  * Output         : None.
  * Return         : The address of the configuration descriptor.
  */
uint8_t *TouchPad_GetHIDDescriptor(uint16_t Length)
{
  return Standard_GetDescriptorData(Length, &Mouse_Hid_Descriptor);
}

/**
  * Function Name  : TouchPad_Get_Interface_Setting.
  * Description    : tests the interface and the alternate setting according to the
  *                  supported one.
  * Input          : - Interface : interface number.
  *                  - AlternateSetting : Alternate Setting number.
  * Output         : None.
  * Return         : USB_SUCCESS or USB_UNSUPPORT.
  */
RESULT TouchPad_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting)
{
  if (AlternateSetting > 0)
  {
    return USB_UNSUPPORT;
  }
  else if (Interface > 0)
  {
    return USB_UNSUPPORT;
  }
  return USB_SUCCESS;
}

/**
  * Function Name  : TouchPad_SetProtocol
  * Description    : TouchPad Set Protocol request routine.
  * Input          : None.
  * Output         : None.
  * Return         : USB SUCCESS.
  */
RESULT TouchPad_SetProtocol(void)
{
  uint8_t wValue0 = pInformation->USBwValue0;
  ProtocolValue = wValue0;
  return USB_SUCCESS;
}

/**
  * Function Name  : TouchPad_GetProtocolValue
  * Description    : get the protocol value
  * Input          : Length.
  * Output         : None.
  * Return         : address of the protocol value.
  */
uint8_t *TouchPad_GetProtocolValue(uint16_t Length)
{
  if (Length == 0)
  {
    pInformation->Ctrl_Info.Usb_wLength = 1;
    return NULL;
  }
  else
  {
    return (uint8_t *)(&ProtocolValue);
  }
}
