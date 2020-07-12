/*!
 * @file   main.c
 * @brief  Application main file
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 5, 2020
 */
#include "main.h"
#include "usb_device.h"
#include "usb_hid_keyboard.h"

/*!
 * @brief System clock configuration.
 * @return None.
 */
void SystemClock_Config(void);

/*!
 * @brief GPIO initialization.
 * @return None.
 */
static void MX_GPIO_Init(void);

/*!
 * @brief Simple function to test USB HID keyboard behaviour.
 * @return None.
 */
static void test_keyboard(void);

/*!
 * @brief Application entry point.
 * @return Execution final status.
 */
int main(void) {
  // MCU Configuration.
  // Reset of all peripherals, initializes the Flash interface and the Systick.
  HAL_Init();

  // Configure the system clock.
  SystemClock_Config();

  // Initialize all configured peripherals.
  MX_GPIO_Init();
  MX_USB_DEVICE_Init();

  // Infinite loop.
  while (1) {
    test_keyboard();
    HAL_Delay(1000);
  }
}

void SystemClock_Config(void) {
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  // Initializes the CPU, AHB and APB busses clocks.
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL6;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
    Error_Handler();
  }
  // Initializes the CPU, AHB and APB busses clocks.
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK) {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
    Error_Handler();
  }
}

static void MX_GPIO_Init(void) {
  // GPIO Ports Clock Enable.
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
}

void Error_Handler(void) {
  // TODO: Implement error handler.
}

static void test_keyboard(void) {
  USB_HID_Keyboard_Write((uint8_t*) "Hello, world!\n", 14);
  USB_HID_Keyboard_Write((uint8_t*) "Sent from ", 10);
  USB_HID_Keyboard_Press(KEY_LEFT_SHIFT);
  USB_HID_Keyboard_Tap('s');
  USB_HID_Keyboard_Tap('t');
  USB_HID_Keyboard_Tap('m');
  USB_HID_Keyboard_Release(KEY_LEFT_SHIFT);
  USB_HID_Keyboard_Tap('3');
  USB_HID_Keyboard_Tap('2');
  USB_HID_Keyboard_Tap('.');
  USB_HID_Keyboard_Tap('\n');
  USB_HID_Keyboard_ReleaseAll();
}
