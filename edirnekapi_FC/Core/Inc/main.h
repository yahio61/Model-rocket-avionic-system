/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "bme280.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct Power
{
	float voltaj;
	float akim;
	float mWatt;
	float mWatt_s;
}power;



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
void deploy_p_1();
void deploy_p_2();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define VOLTAGE_SENS_Pin GPIO_PIN_1
#define VOLTAGE_SENS_GPIO_Port GPIOC
#define CURRENT_SENS_Pin GPIO_PIN_2
#define CURRENT_SENS_GPIO_Port GPIOC
#define P_1_MOS_Pin GPIO_PIN_3
#define P_1_MOS_GPIO_Port GPIOC
#define GPIO_0_Pin GPIO_PIN_0
#define GPIO_0_GPIO_Port GPIOB
#define GPIO_1_Pin GPIO_PIN_1
#define GPIO_1_GPIO_Port GPIOB
#define BUZZER_Pin GPIO_PIN_12
#define BUZZER_GPIO_Port GPIOB
#define LED_Pin GPIO_PIN_13
#define LED_GPIO_Port GPIOB
#define P_2_MOS_Pin GPIO_PIN_7
#define P_2_MOS_GPIO_Port GPIOC
#define LORA_M0_Pin GPIO_PIN_10
#define LORA_M0_GPIO_Port GPIOC
#define LORA_M1_Pin GPIO_PIN_11
#define LORA_M1_GPIO_Port GPIOC
#define INT_ACC_Pin GPIO_PIN_3
#define INT_ACC_GPIO_Port GPIOB
#define INT_ACC_EXTI_IRQn EXTI3_IRQn
#define INT_GYRO_Pin GPIO_PIN_4
#define INT_GYRO_GPIO_Port GPIOB
#define INT_GYRO_EXTI_IRQn EXTI4_IRQn
#define LORA_AUX_Pin GPIO_PIN_9
#define LORA_AUX_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
