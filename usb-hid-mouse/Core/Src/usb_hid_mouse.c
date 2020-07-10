/*!
 * @file   usb_hid_mouse.h
 * @brief  Module to use the microcontroller as an USB Mouse
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 9, 2020
 */
#include "usb_hid_mouse.h"
#include "usbd_hid.h"

#define NO_BUTTON 0

extern USBD_HandleTypeDef hUsbDeviceFS;

/*!
 * @brief Send button report (no movements whatsoever).
 * @return None.
 */
static void USB_HID_Mouse_SendButtonReport();

/* 
 * USB HID Mouse input report (supported version) - 4 bytes:
 *
 *  Byte |  D7   |  D6   |  D5   |  D4   |  D3   |   D2   |  D1   |  D0   |
 * ------+-----------------------------------------------------------------
 *   0   |   0   |   0   |   0   |   0   |   0   | Middle | Right | Left  | (buttons)
 *   1   |                             X Axis                             |     
 *   2   |                             Y Axis                             |
 *   3   |                         Vertical Wheel                         |
 */
// TODO: Replace this with a structure and/or union to make it more intuitive to use.
static uint8_t report[4];

void USB_HID_Mouse_Move(uint8_t x, uint8_t y, uint8_t wheel) {
  // TODO: Support larger movements (loop until zero).
  report[1] = x;
  report[2] = y;
  report[3] = wheel;
  USBD_HID_SendReport(&hUsbDeviceFS, report, sizeof(report));
}

void USB_HID_Mouse_Click(uint8_t buttons) {
  USB_HID_Mouse_Press(buttons);
  USB_HID_Mouse_Release(buttons);
}

void USB_HID_Mouse_Press(uint8_t buttons) {
  // Set required buttons' bits.
  report[0] |= buttons;
  USB_HID_Mouse_SendButtonReport();
}

void USB_HID_Mouse_Release(uint8_t buttons) {
  // Clear required buttons' bits.
  report[0] &= ~buttons;
  USB_HID_Mouse_SendButtonReport();
}

void USB_HID_Mouse_ReleaseAll() {
  USB_HID_Mouse_SendButtonReport(NO_BUTTON);
}

static void USB_HID_Mouse_SendButtonReport() {
  report[1] = 0;
  report[2] = 0;
  report[3] = 0;
  USBD_HID_SendReport(&hUsbDeviceFS, report, sizeof(report));
}
