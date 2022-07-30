/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "cmsis_os.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

//Screen handles, and identifiers - List your screens here
typedef enum {
	DEVICE_TEST,
	HOME,
	MAIN_MENU,
	SETTINGS
} Screens;


//Screen dimensions
#define WIDTH 480
#define HEIGHT 320

//Direction of some elements
typedef enum {
	HORIZONTAL,
	VERTICAL
} Orientation;


//Handles for threads which need to receive signals from external sources.
extern osThreadId touchTaskHandle;
extern osThreadId changeScreenTaskHandle;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
void home_onPress(int id);
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD13_Pin GPIO_PIN_13
#define LCD13_GPIO_Port GPIOC
#define LCD14_Pin GPIO_PIN_14
#define LCD14_GPIO_Port GPIOC
#define LCD15_Pin GPIO_PIN_15
#define LCD15_GPIO_Port GPIOC
#define SW_UP_Pin GPIO_PIN_0
#define SW_UP_GPIO_Port GPIOC
#define SW_DOWN_Pin GPIO_PIN_1
#define SW_DOWN_GPIO_Port GPIOC
#define SW_LEFT_Pin GPIO_PIN_2
#define SW_LEFT_GPIO_Port GPIOC
#define SW_MIDDLE_Pin GPIO_PIN_3
#define SW_MIDDLE_GPIO_Port GPIOC
#define DIO3_Pin GPIO_PIN_0
#define DIO3_GPIO_Port GPIOA
#define DIO4_Pin GPIO_PIN_1
#define DIO4_GPIO_Port GPIOA
#define DIO5_Pin GPIO_PIN_2
#define DIO5_GPIO_Port GPIOA
#define DIO6_Pin GPIO_PIN_3
#define DIO6_GPIO_Port GPIOA
#define DIO7_Pin GPIO_PIN_4
#define DIO7_GPIO_Port GPIOA
#define SW_RIGHT_Pin GPIO_PIN_4
#define SW_RIGHT_GPIO_Port GPIOC
#define W25Q128_CS_Pin GPIO_PIN_5
#define W25Q128_CS_GPIO_Port GPIOC
#define DIO0_Pin GPIO_PIN_0
#define DIO0_GPIO_Port GPIOB
#define DIO1_Pin GPIO_PIN_1
#define DIO1_GPIO_Port GPIOB
#define DIO2_Pin GPIO_PIN_2
#define DIO2_GPIO_Port GPIOB
#define PWR_LED_Pin GPIO_PIN_10
#define PWR_LED_GPIO_Port GPIOB
#define LCD_RD_Pin GPIO_PIN_11
#define LCD_RD_GPIO_Port GPIOB
#define LCD_WR_Pin GPIO_PIN_12
#define LCD_WR_GPIO_Port GPIOB
#define LCD_RST_Pin GPIO_PIN_13
#define LCD_RST_GPIO_Port GPIOB
#define LCD_D_C_Pin GPIO_PIN_14
#define LCD_D_C_GPIO_Port GPIOB
#define LCD_CS_Pin GPIO_PIN_15
#define LCD_CS_GPIO_Port GPIOB
#define CTP_RST_Pin GPIO_PIN_6
#define CTP_RST_GPIO_Port GPIOC
#define CTP_INT_Pin GPIO_PIN_7
#define CTP_INT_GPIO_Port GPIOC
#define CTP_INT_EXTI_IRQn EXTI9_5_IRQn
#define LCD8_Pin GPIO_PIN_8
#define LCD8_GPIO_Port GPIOC
#define LCD9_Pin GPIO_PIN_9
#define LCD9_GPIO_Port GPIOC
#define LCD0_Pin GPIO_PIN_8
#define LCD0_GPIO_Port GPIOA
#define LCD1_Pin GPIO_PIN_9
#define LCD1_GPIO_Port GPIOA
#define LCD2_Pin GPIO_PIN_10
#define LCD2_GPIO_Port GPIOA
#define LCD3_Pin GPIO_PIN_11
#define LCD3_GPIO_Port GPIOA
#define LCD4_Pin GPIO_PIN_12
#define LCD4_GPIO_Port GPIOA
#define LCD5_Pin GPIO_PIN_13
#define LCD5_GPIO_Port GPIOA
#define LCD6_Pin GPIO_PIN_14
#define LCD6_GPIO_Port GPIOA
#define LCD7_Pin GPIO_PIN_15
#define LCD7_GPIO_Port GPIOA
#define LCD10_Pin GPIO_PIN_10
#define LCD10_GPIO_Port GPIOC
#define LCD11_Pin GPIO_PIN_11
#define LCD11_GPIO_Port GPIOC
#define LCD12_Pin GPIO_PIN_12
#define LCD12_GPIO_Port GPIOC
#define DIO8_Pin GPIO_PIN_3
#define DIO8_GPIO_Port GPIOB
#define DIO9_Pin GPIO_PIN_4
#define DIO9_GPIO_Port GPIOB
#define DIO10_Pin GPIO_PIN_5
#define DIO10_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
