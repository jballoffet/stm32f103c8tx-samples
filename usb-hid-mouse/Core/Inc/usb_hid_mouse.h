/*!
 * @file   usb_hid_mouse.h
 * @brief  Module to use the microcontroller as an USB Mouse
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 9, 2020
 */
#ifndef INC_USB_HID_MOUSE_H_
#define INC_USB_HID_MOUSE_H_

#include <stdint.h>

/*
 * Supported mouse buttons.
 */
#define LEFT_BUTTON 1
#define RIGHT_BUTTON 2
#define MIDDLE_BUTTON 4

/*!
 * @brief Move mouse pointer and wheel (relative movement from current position).
 *
 * @param[in] x Indicates the pointer movement along the x axis (Positive values move the pointer to the right).
 * @param[in] y Indicates the pointer movement along the y axis (Positive values move the pointer downwards).
 * @param[in] wheel Indicates the wheel rotation (Positive values rotate the wheel away from the user).
 * @return    None.
 */
void USB_HID_Mouse_Move(uint8_t x, uint8_t y, uint8_t wheel);

/*!
 * @brief Press and release the given buttons.
 *
 * @param[in] buttons Buttons to click (buttons bitwise OR combination).
 * @return    None.
 */
void USB_HID_Mouse_Click(uint8_t buttons);

/*!
 * @brief Press the given buttons.
 *
 * @param[in] buttons Buttons to press (buttons bitwise OR combination).
 * @return    None.
 */
void USB_HID_Mouse_Press(uint8_t buttons);

/*!
 * @brief Release the given buttons.
 *
 * @param[in] buttons Buttons to release (buttons bitwise OR combination).
 * @return    None.
 */
void USB_HID_Mouse_Release(uint8_t buttons);

/*!
 * @brief Release all buttons.
 * @return None.
 */
void USB_HID_Mouse_ReleaseAll();

#endif // INC_USB_HID_MOUSE_H_
