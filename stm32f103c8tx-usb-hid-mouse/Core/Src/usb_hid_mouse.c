/*!
 * @file   usb_hid_mouse.c
 * @brief  Module to use the microcontroller as an USB Mouse
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 9, 2020
 *
 * Note: Based on Arduino's Mouse library (see https://github.com/arduino-libraries/Mouse).
 */
#include "usb_hid_mouse.h"
#include "usbd_hid.h"

/*
 * USB handler.
 */
extern USBD_HandleTypeDef hUsbDeviceFS;

/*
 * Mouse report structure.
 */
typedef struct {
  uint8_t buttons;
  int8_t x;
  int8_t y;
  int8_t wheel;
} MouseReport;

/*
 * Mouse report.
 */
static MouseReport mouse_report;

/*!
 * @brief Send mouse report.
 * @return True (1) in case of success, otherwise false (0).
 */
static int send_report(void);

int USB_HID_Mouse_Click(uint8_t buttons) {
  if (!USB_HID_Mouse_Press(buttons)) {
    return 0;
  }
  return USB_HID_Mouse_Release(buttons);
}

int USB_HID_Mouse_Press(uint8_t buttons) {
  // Set required buttons' bits.
  mouse_report.buttons |= buttons;
  return USB_HID_Mouse_Move(0, 0, 0);
}

int USB_HID_Mouse_Release(uint8_t buttons) {
  // Clear required buttons' bits.
  mouse_report.buttons &= ~buttons;
  return USB_HID_Mouse_Move(0, 0, 0);
}

int USB_HID_Mouse_ReleaseAll() {
  mouse_report.buttons = 0;
  return USB_HID_Mouse_Move(0, 0, 0);
}

int USB_HID_Mouse_Move(int8_t x, int8_t y, int8_t wheel) {
  mouse_report.x = x;
  mouse_report.y = y;
  mouse_report.wheel = wheel;
  return send_report();
}

static int send_report(void) {
  if (USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &mouse_report, sizeof(MouseReport)) != HAL_OK) {
    return 0;
  }

  // Small delay to avoid communication issues.
  HAL_Delay(50);
  return 1;
}
