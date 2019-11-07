
#ifndef __USB_DESC_H
#define __USB_DESC_H

#include "usb_lib.h"

#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define TOUCHPAD_SIZ_HID_DESC                   0x09
#define TOUCHPAD_OFF_HID_DESC                   0x12

#define TOUCHPAD_SIZ_DEVICE_DESC                18
#define TOUCHPAD_SIZ_CONFIG_DESC                34
#define TOUCHPAD_SIZ_REPORT_DESC                74
#define TOUCHPAD_SIZ_STRING_LANGID              4
#define TOUCHPAD_SIZ_STRING_VENDOR              38
#define TOUCHPAD_SIZ_STRING_PRODUCT             30
#define TOUCHPAD_SIZ_STRING_SERIAL              26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

extern const uint8_t TouchPad_DeviceDescriptor[TOUCHPAD_SIZ_DEVICE_DESC];
extern const uint8_t TouchPad_ConfigDescriptor[TOUCHPAD_SIZ_CONFIG_DESC];
extern const uint8_t TouchPad_ReportDescriptor[TOUCHPAD_SIZ_REPORT_DESC];
extern const uint8_t TouchPad_StringLangID[TOUCHPAD_SIZ_STRING_LANGID];
extern const uint8_t TouchPad_StringVendor[TOUCHPAD_SIZ_STRING_VENDOR];
extern const uint8_t TouchPad_StringProduct[TOUCHPAD_SIZ_STRING_PRODUCT];
extern uint8_t TouchPad_StringSerial[TOUCHPAD_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */
