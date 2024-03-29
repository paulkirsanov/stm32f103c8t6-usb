
#include "usb_lib.h"

/**
  * Function Name  : USB_SIL_Init
  * Description    : Initialize the USB Device IP and the Endpoint 0.
  * Input          : None.
  * Output         : None.
  * Return         : Status.
  **/
uint32_t USB_SIL_Init(void)
{
  /* USB interrupts initialization */
  /* clear pending interrupts */
  _SetISTR(0);
  wInterrupt_Mask = IMR_MSK;
  /* set interrupts mask */
  _SetCNTR(wInterrupt_Mask);
  return 0;
}

/**
  * Function Name  : USB_SIL_Write
  * Description    : Write a buffer of data to a selected endpoint.
  * Input          : - bEpAddr: The address of the non control endpoint.
  *                  - pBufferPointer: The pointer to the buffer of data to be written
  *                    to the endpoint.
  *                  - wBufferSize: Number of data to be written (in bytes).
  * Output         : None.
  * Return         : Status.
  **/
uint32_t USB_SIL_Write(uint8_t bEpAddr, uint8_t* pBufferPointer, uint32_t wBufferSize)
{
  /* Use the memory interface function to write to the selected endpoint */
  UserToPMABufferCopy(pBufferPointer, GetEPTxAddr(bEpAddr & 0x7F), wBufferSize);

  /* Update the data length in the control register */
  SetEPTxCount((bEpAddr & 0x7F), wBufferSize);
  
  return 0;
}

/**
  * Function Name  : USB_SIL_Read
  * Description    : Write a buffer of data to a selected endpoint.
  * Input          : - bEpAddr: The address of the non control endpoint.
  *                  - pBufferPointer: The pointer to which will be saved the 
  *                     received data buffer.
  * Output         : None.
  * Return         : Number of received data (in Bytes).
  **/
uint32_t USB_SIL_Read(uint8_t bEpAddr, uint8_t* pBufferPointer)
{
  uint32_t DataLength = 0;

  /* Get the number of received data on the selected Endpoint */
  DataLength = GetEPRxCount(bEpAddr & 0x7F);
  
  /* Use the memory interface function to write to the selected endpoint */
  PMAToUserBufferCopy(pBufferPointer, GetEPRxAddr(bEpAddr & 0x7F), DataLength);

  /* Return the number of received data */
  return DataLength;
}
