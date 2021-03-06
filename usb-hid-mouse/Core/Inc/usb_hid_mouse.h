/*!
 * @file   usb_hid_mouse.h
 * @brief  Module to use the microcontroller as an USB Mouse
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 9, 2020
 *
 * Note: Based on Arduino's Mouse library (see https://github.com/arduino-libraries/Mouse).
 */
#ifndef INC_USB_HID_MOUSE_H_
#define INC_USB_HID_MOUSE_H_

#include <stdint.h>

/*
 * Mouse buttons.
 */
#define BUTTON_LEFT    0x01
#define BUTTON_RIGHT   0x02
#define BUTTON_MIDDLE  0x04

/*!
 * @brief Press and release the given buttons.
 *
 * @param[in] buttons Buttons to click (buttons bitwise OR combination).
 * @return    True (1) in case of success, otherwise false (0).
 */
int USB_HID_Mouse_Click(uint8_t buttons);

/*!
 * @brief Press the given buttons.
 *
 * @param[in] buttons Buttons to press (buttons bitwise OR combination).
 * @return    True (1) in case of success, otherwise false (0).
 */
int USB_HID_Mouse_Press(uint8_t buttons);

/*!
 * @brief Release the given buttons.
 *
 * @param[in] buttons Buttons to release (buttons bitwise OR combination).
 * @return    True (1) in case of success, otherwise false (0).
 */
int USB_HID_Mouse_Release(uint8_t buttons);

/*!
 * @brief Release all buttons.
 * @return True (1) in case of success, otherwise false (0).
 */
int USB_HID_Mouse_ReleaseAll(void);

/*!
 * @brief Move mouse pointer and wheel (relative movement from current position).
 *
 * @param[in] x Indicates the pointer movement along the x axis (Positive values move the pointer to the right).
 * @param[in] y Indicates the pointer movement along the y axis (Positive values move the pointer downwards).
 * @param[in] wheel Indicates the wheel rotation (Positive values rotate the wheel away from the user).
 * @return    True (1) in case of success, otherwise false (0).
 */
int USB_HID_Mouse_Move(int8_t x, int8_t y, int8_t wheel);

#endif // INC_USB_HID_MOUSE_H_
