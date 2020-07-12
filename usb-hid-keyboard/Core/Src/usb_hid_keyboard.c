/*!
 * @file   usb_hid_keyboard.c
 * @brief  Module to use the microcontroller as an USB Keyboard
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 5, 2020
 * 
 * Note: Based on Arduino's Keyboard library (see https://github.com/arduino-libraries/Keyboard).
 */
#include "usb_hid_keyboard.h"
#include "usbd_hid.h"

/*
 * Keys ranges.
 */
#define PRINTING_KEYS_START      0x00
#define MODIFIER_KEYS_START      0x80
#define NON_PRINTING_KEYS_START  0x88

/**
 * Modifier masks.
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

/**
 * ASCII to HID conversion map.
 */
#define SHIFT_FLAG 0x80

const uint8_t ascii_to_hid_map[128] = {
  0x00,             // NUL
  0x00,             // SOH
  0x00,             // STX
  0x00,             // ETX
  0x00,             // EOT
  0x00,             // ENQ
  0x00,             // ACK
  0x00,             // BEL
  0x2A,             // BS Backspace
  0x2B,             // TAB Tab
  0x28,             // LF Enter
  0x00,             // VT
  0x00,             // FF
  0x00,             // CR
  0x00,             // SO
  0x00,             // SI
  0x00,             // DEL
  0x00,             // DC1
  0x00,             // DC2
  0x00,             // DC3
  0x00,             // DC4
  0x00,             // NAK
  0x00,             // SYN
  0x00,             // ETB
  0x00,             // CAN
  0x00,             // EM 
  0x00,             // SUB
  0x00,             // ESC
  0x00,             // FS 
  0x00,             // GS 
  0x00,             // RS 
  0x00,             // US 
  0x2C,             //  ' '
  0x1E|SHIFT_FLAG,  // !
  0x34|SHIFT_FLAG,  // "
  0x20|SHIFT_FLAG,  // #
  0x21|SHIFT_FLAG,  // $
  0x22|SHIFT_FLAG,  // %
  0x24|SHIFT_FLAG,  // &
  0x34,             // '
  0x26|SHIFT_FLAG,  // (
  0x27|SHIFT_FLAG,  // )
  0x25|SHIFT_FLAG,  // *
  0x2E|SHIFT_FLAG,  // +
  0x36,             // ,
  0x2D,             // -
  0x37,             // .
  0x38,             // /
  0x27,             // 0
  0x1E,             // 1
  0x1F,             // 2
  0x20,             // 3
  0x21,             // 4
  0x22,             // 5
  0x23,             // 6
  0x24,             // 7
  0x25,             // 8
  0x26,             // 9
  0x33|SHIFT_FLAG,  // :
  0x33,             // ;
  0x36|SHIFT_FLAG,  // <
  0x2E,             // =
  0x37|SHIFT_FLAG,  // >
  0x38|SHIFT_FLAG,  // ?
  0x1F|SHIFT_FLAG,  // @
  0x04|SHIFT_FLAG,  // A
  0x05|SHIFT_FLAG,  // B
  0x06|SHIFT_FLAG,  // C
  0x07|SHIFT_FLAG,  // D
  0x08|SHIFT_FLAG,  // E
  0x09|SHIFT_FLAG,  // F
  0x0A|SHIFT_FLAG,  // G
  0x0B|SHIFT_FLAG,  // H
  0x0C|SHIFT_FLAG,  // I
  0x0D|SHIFT_FLAG,  // J
  0x0E|SHIFT_FLAG,  // K
  0x0F|SHIFT_FLAG,  // L
  0x10|SHIFT_FLAG,  // M
  0x11|SHIFT_FLAG,  // N
  0x12|SHIFT_FLAG,  // O
  0x13|SHIFT_FLAG,  // P
  0x14|SHIFT_FLAG,  // Q
  0x15|SHIFT_FLAG,  // R
  0x16|SHIFT_FLAG,  // S
  0x17|SHIFT_FLAG,  // T
  0x18|SHIFT_FLAG,  // U
  0x19|SHIFT_FLAG,  // V
  0x1A|SHIFT_FLAG,  // W
  0x1B|SHIFT_FLAG,  // X
  0x1C|SHIFT_FLAG,  // Y
  0x1D|SHIFT_FLAG,  // Z
  0x2F,             // [
  0x31,             // bslash
  0x30,             // ]
  0x23|SHIFT_FLAG,  // ^
  0x2D|SHIFT_FLAG,  // _
  0x35,             // `
  0x04,             // a
  0x05,             // b
  0x06,             // c
  0x07,             // d
  0x08,             // e
  0x09,             // f
  0x0A,             // g
  0x0B,             // h
  0x0C,             // i
  0x0D,             // j
  0x0E,             // k
  0x0F,             // l
  0x10,             // m
  0x11,             // n
  0x12,             // o
  0x13,             // p
  0x14,             // q
  0x15,             // r
  0x16,             // s
  0x17,             // t
  0x18,             // u
  0x19,             // v
  0x1A,             // w
  0x1B,             // x
  0x1C,             // y
  0x1D,             // z
  0x2F|SHIFT_FLAG,  // {
  0x31|SHIFT_FLAG,  // |
  0x30|SHIFT_FLAG,  // }
  0x35|SHIFT_FLAG,  // ~
  0x00              // DEL
};

/*
 * USB handler.
 */
extern USBD_HandleTypeDef hUsbDeviceFS;

/*
 * Keyboard report structure.
 */
typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keys[6];
} KeyboardReport;

/*
 * Keyboard report.
 */
static KeyboardReport keyboard_report;

/*!
 * @brief Add a key to keyboard report.
 *
 * @param[in] key Key to add.
 * @return    True (1) in case of success, otherwise false (0).
 */
static int add_key_to_report(uint8_t key);

/*!
 * @brief Remove a key from keyboard report.
 *
 * @param[in] key Key to remove.
 * @return    True (1) in case of success, otherwise false (0).
 */
static int remove_key_from_report(uint8_t key);

/*!
 * @brief Send keyboard report.
 * @return True (1) in case of success, otherwise false (0).
 */
static int send_report(void);

int USB_HID_Keyboard_Tap(uint8_t key) {
  if (!USB_HID_Keyboard_Press(key)) {
    return 0;
  }
  return USB_HID_Keyboard_Release(key);
}

int USB_HID_Keyboard_Press(uint8_t key) {
  if (key >= NON_PRINTING_KEYS_START) {
    // Key is a non-printing key.
    key -= NON_PRINTING_KEYS_START;
  } else if (key >= MODIFIER_KEYS_START) {
    // Key is a modifier key. Set proper flag in modifiers byte.
    keyboard_report.modifiers |= (1 << (key - MODIFIER_KEYS_START));
    key = 0;
  } else {
    // Key is a printing key. Convert from ASCII to HID code.
    key = ascii_to_hid_map[key];
    if (!key) {
      // Invalid key.
      return 0;
    }
    if (key & SHIFT_FLAG) {
      // Set shift flag in modifiers byte and clear bit used as flag in key value.
      keyboard_report.modifiers |= KEY_MOD_LSHIFT;
      key &= ~SHIFT_FLAG;
    }
  }

  // Add key to keyboard report.
  if (!add_key_to_report(key)) {
    return 0;
  }

  return send_report();
}

int USB_HID_Keyboard_Release(uint8_t key) {
  if (key >= NON_PRINTING_KEYS_START) {
    // Key is a non-printing key.
    key -= NON_PRINTING_KEYS_START;
  } else if (key >= MODIFIER_KEYS_START) {
    // Key is a modifier key. Clear proper flag in modifiers byte.
    keyboard_report.modifiers &= ~(1 << (key - MODIFIER_KEYS_START));
    key = 0;
  } else {
    // Key is a printing key. Convert from ASCII to HID code.
    key = ascii_to_hid_map[key];
    if (!key) {
      // Invalid key.
      return 0;
    }
    if (key & SHIFT_FLAG) {
      // Clear shift flag in modifiers byte and clear bit used as flag in key value.
      keyboard_report.modifiers &= ~KEY_MOD_LSHIFT;
      key &= ~SHIFT_FLAG;
    }
  }

  // Remove key from keyboard report.
  if (!remove_key_from_report(key)) {
    return 0;
  }

  return send_report();
}

int USB_HID_Keyboard_ReleaseAll(void) {
  int i = 0;
  for (i = 0; i < 6; i++) {
    keyboard_report.keys[i] = 0;
  }
  keyboard_report.modifiers = 0;
  return send_report();
}

int USB_HID_Keyboard_Write(uint8_t* keys, int size) {
  int i;
  for (i = 0; i < size; i++) {
    if (!USB_HID_Keyboard_Tap(keys[i])) {
      break;
    }
  }
  return i;
}

static int add_key_to_report(uint8_t key) {
  int i;

  // Check that the key isn't already in the report.
  for (i = 0; i < 6; i++) {
    if (keyboard_report.keys[i] == key) {
      // Key already in report. No action required.
      return 1;
    }
  }

  // Add key into the first empty slot available.
  for (i = 0; i < 6; i++) {
    if (keyboard_report.keys[i] == 0) {
      keyboard_report.keys[i] = key;
      return 1;
    }
  }

  // No empty slot available.
  return 0;
}

static int remove_key_from_report(uint8_t key) {
  int i;

  if (key == 0) {
    return 1;
  }

  for (i = 0; i < 6; i++) {
    if (keyboard_report.keys[i] == key) {
      keyboard_report.keys[i] = 0;
      return 1;
    }
  }

  // Key not found.
  return 0;
}

static int send_report(void) {
  if (USBD_HID_SendReport(&hUsbDeviceFS, (uint8_t*) &keyboard_report, sizeof(KeyboardReport)) != HAL_OK) {
    return 0;
  }

  // Small delay to avoid communication issues.
  HAL_Delay(50);
  return 1;
}
