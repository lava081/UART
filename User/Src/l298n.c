#include "l298n.h"

void l298n_set(uint8_t cmd);

// 假设四个轮子按↗↘↖↙排列
void l298n_set(uint8_t cmd)
{
  // 第一块L298N模块的INA~D
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_7, (cmd & (0x01 << 7)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_8, (cmd & (0x01 << 6)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9, (cmd & (0x01 << 5)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_10, (cmd & (0x01 << 4)) ? GPIO_PIN_SET : GPIO_PIN_RESET);

  // 第二块L298N模块的INA~D
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_11, (cmd & (0x01 << 3)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_12, (cmd & (0x01 << 2)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_13, (cmd & (0x01 << 1)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_14, (cmd & (0x01 << 0)) ? GPIO_PIN_SET : GPIO_PIN_RESET);
}

void l298n_stop(void)
{
  l298n_set(0xFF); // 000000000
}

void l298n_forward(void)
{
  l298n_set(0xAA); // 10101010
}

void l298n_backward(void)
{
  l298n_set(0x55); // 01010101
}

void l298n_right(void)
{
  l298n_set(0x5A); // 01011010
}

void l298n_left(void)
{
  l298n_set(0xA5); // 10100101
}
