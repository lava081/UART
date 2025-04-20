/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h> // 引入标准输入输出库
#include <string.h> // 引入字符串处理库
#include <stdlib.h> // 引入标准库
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  MX_UART4_Init();
  MX_UART5_Init();
  MX_USART2_UART_Init();
  MX_USART3_UART_Init();
  MX_USART6_UART_Init();
  MX_TIM1_Init();
  /* USER CODE BEGIN 2 */

	HAL_UART_Transmit(&huart1, (uint8_t *)"\r\nUSART1 connected!", 20, HAL_MAX_DELAY); // 开机打个招呼
  
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动定时器1 PWM 输出通道1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2); // 启动定时器1 PWM 输出通道2

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(0.6 * (htim1.Init.Period + 1))); // 设置定时器1 PWM 输出通道1 占空比为60%
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(1.0 * (htim1.Init.Period + 1))); // 设置定时器1 PWM 输出通道2 占空比为100%

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/**
 * @brief 设置 TIM1 的 PWM 输出
 * @param receivedDataUSART1: 接收到的数据
 */
void set_pwm(char *receivedDataUSART1) {
  if (strncasecmp(receivedDataUSART1, "T1", 2) == 0) // Timer1
  {
    int percent = atoi(receivedDataUSART1 + 6); // 将字符串转换为整数
    char sendDataUART1[16]; // 定义一个字符数组用于存储转换后的数据
    if (strncmp(receivedDataUSART1 + 2, "CH1", 3) == 0) // Timer1 Channel1
    {
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(percent * (htim1.Init.Period + 1) / 100)); // 设置定时器1 PWM 输出通道1 占空比
      sprintf(sendDataUART1, "\nCH1 percent:%d", percent); // 将整数转换为字符串
    }
    else if (strncmp(receivedDataUSART1 + 2, "CH2", 3) == 0) // Timer1 Channel2
    {
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(percent * (htim1.Init.Period + 1) / 100)); // 设置定时器1 PWM 输出通道2 占空比
      sprintf(sendDataUART1, "\nCH2 percent:%d", percent); // 将整数转换为字符串
    }
    else if (strncmp(receivedDataUSART1 + 2, "ALL", 3) == 0) // Timer1 All
    {
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(percent * (htim1.Init.Period + 1) / 100)); // 设置定时器1 PWM 输出通道1 占空比
      __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(percent * (htim1.Init.Period + 1) / 100)); // 设置定时器1 PWM 输出通道2 占空比
      sprintf(sendDataUART1, "\n1&2 percent:%d", percent); // 将整数转换为字符串
    }
    HAL_UART_Transmit(&huart1, (uint8_t *)sendDataUART1, strlen(sendDataUART1), HAL_MAX_DELAY); // 发送设置成功的提示信息
  }
}

/**
 * @brief 处理 USART1 接收完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) // 判断是否是 USART1 的中断请求
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)"\nuart1:", 7, HAL_MAX_DELAY); // 输出接收成功提示,注意删除\0
    HAL_UART_Transmit(&huart1, (uint8_t *)&receivedDataUSART1, sizeof(receivedDataUSART1), HAL_MAX_DELAY); // 回显接收到的数据
    if (strncasecmp(receivedDataUSART1, "T1", 2) == 0) // Timer1
    {
      set_pwm(receivedDataUSART1); // 设置 PWM 输出
    }
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&receivedDataUSART1, sizeof(receivedDataUSART1) - 1); // 重新启动接收中断
  }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
