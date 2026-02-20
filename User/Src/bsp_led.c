#include "bsp_led.h"

void BSP_LED_ON(void)
{
  HAL_GPIO_WritePin(BSP_LED_GPIO_Port, BSP_LED_Pin, GPIO_PIN_RESET);
}

void BSP_LED_OFF(void)
{
  HAL_GPIO_WritePin(BSP_LED_GPIO_Port, BSP_LED_Pin, GPIO_PIN_SET);
}

void BSP_LED_Toggle(void)
{
  HAL_GPIO_TogglePin(BSP_LED_GPIO_Port, BSP_LED_Pin);
}

void (*BSP_LED_Init)(void) = BSP_LED_OFF;
