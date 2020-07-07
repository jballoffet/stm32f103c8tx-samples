/*!
 * @file   usb_hid_keyboard.c
 * @brief  Module to use the microcontroller as an USB Keyboard
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 5, 2020
 */
#include "usb_hid_keyboard.h"
#include "usbd_hid.h"

extern USBD_HandleTypeDef hUsbDeviceFS;

/**
 * Modifier masks - used for the first byte in the HID report.
 * Note: The second byte in the report is reserved, 0x00.
 */
#define KEY_MOD_NONE   0x00
#define KEY_MOD_LCTRL  0x01
#define KEY_MOD_LSHIFT 0x02
#define KEY_MOD_LALT   0x04
#define KEY_MOD_LMETA  0x08
#define KEY_MOD_RCTRL  0x10
#define KEY_MOD_RSHIFT 0x20
#define KEY_MOD_RALT   0x40
#define KEY_MOD_RMETA  0x80

// Scan codes.
#define KEY_NONE 0x00   // No key pressed

#define KEY_A 0x04      // Keyboard a and A

#define KEY_1 0x1e      // Keyboard 1 and !

#define KEY_ENTER 0x28  // Keyboard Return (ENTER)
#define KEY_SPACE 0x2c  // Keyboard Spacebar
#define KEY_DOT 0x37    // Keyboard . and >
#define KEY_SLASH 0x38  // Keyboard / and ?

void USB_HID_Keyboard_SendString(char* string) {
  uint8_t i = 0;

  while (string[i])	{
    USB_HID_Keyboard_SendChar(string[i]);
    i++;
  }
}

void USB_HID_Keyboard_SendChar(char ch) {
  uint8_t HID_buffer[8] = {0};

  // Check character case.
  if (ch >= 'a' && ch <= 'z') {
    HID_buffer[0] = KEY_MOD_NONE;
    // Convert character to HID letter.
    HID_buffer[2] = (uint8_t)((ch - 'a') + KEY_A);
  } else if (ch >= 'A' && ch <= 'Z') {
    // Add left shift key press.
    HID_buffer[0] = KEY_MOD_LSHIFT;
    // Convert character to HID letter.
    HID_buffer[2] = (uint8_t)((ch - 'A') + KEY_A);
  } else {
    // TODO: This is horrible! Create a list with the symbols and iterate over it to get the proper values.
    switch(ch) {
      case ' ':
        HID_buffer[2] = KEY_SPACE;
        break;
      case '.':
        HID_buffer[2] = KEY_DOT;
        break;
      case '\n':
        HID_buffer[2] = KEY_ENTER;
        break;
      case '!':
        HID_buffer[0] = KEY_MOD_LSHIFT;
        HID_buffer[2] = KEY_1;
        break;
      case '?':
        HID_buffer[0] = KEY_MOD_LSHIFT;
        HID_buffer[2] = KEY_SLASH;
        break;
      default:
        HID_buffer[2] = KEY_NONE;
    }
  }

  // Press keys and wait to get them recognized.
  if(USBD_HID_SendReport(&hUsbDeviceFS, HID_buffer, 8) != HAL_OK) {
    // TODO: Return error code.
    return;
  }
  HAL_Delay(15);

  // Release keys and wait to get them recognized.
  HID_buffer[0] = KEY_MOD_NONE;
  HID_buffer[2] = KEY_NONE;
  if(USBD_HID_SendReport(&hUsbDeviceFS, HID_buffer, 8) != HAL_OK) {
    // TODO: Return error code.
    return;
  }
  HAL_Delay(15);
}
