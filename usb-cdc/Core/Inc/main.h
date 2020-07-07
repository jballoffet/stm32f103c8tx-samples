/*!
 * @file   main.h
 * @brief  Application main file
 * @author Javier Balloffet <javier.balloffet@gmail.com>
 * @date   Jul 5, 2020
 */
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

#include "stm32f1xx_hal.h"

/*!
 * @brief This function is executed in case of error occurrence.
 * @return None.
 */
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif // __MAIN_H
