/*!
 * @file   usb_hid_keyboard.h
 * @brief  Module to use the microcontroller as an USB Keyboard
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 5, 2020
 */
#ifndef INC_USB_HID_KEYBOARD_H_
#define INC_USB_HID_KEYBOARD_H_

/*!
 * @brief Send string as key presses.
 *
 * @param[in] string String to send.
 * @return	  None.
 */
void USB_HID_Keyboard_SendString(char* string);

/*!
 * @brief Send char as a single key press.
 *
 * @param[in] ch Character to send.
 * @return	  None.
 */
void USB_HID_Keyboard_SendChar(char ch);

#endif // INC_USB_HID_KEYBOARD_H_
