#include "pwm.h"
#include "tim.h" // 定时器头文件

void pwm_init(void)
{
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1); // 启动定时器1 PWM 输出通道1
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_4);

  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, (uint32_t)(0.6 * (htim1.Init.Period + 1))); // 设置定时器1 PWM 输出通道1 占空比为60%
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_2, (uint32_t)(0.4 * (htim1.Init.Period + 1)));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_3, (uint32_t)(0.2 * (htim1.Init.Period + 1)));
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, (uint32_t)(0.8 * (htim1.Init.Period + 1)));
}

void set_pwm(uint8_t timer, uint8_t channel, uint8_t percent)
{

  TIM_HandleTypeDef *htim; // 定义定时器句柄
  uint8_t TIM_CHANNEL;     // 定义通道号

  switch (timer)
  { // 选择定时器
  case 1:
    htim = &htim1;
    break;
  }

  switch (channel)
  { // 选择通道
  case 1:
    TIM_CHANNEL = TIM_CHANNEL_1;
    break;
  case 2:
    TIM_CHANNEL = TIM_CHANNEL_2;
    break;
  case 3:
    TIM_CHANNEL = TIM_CHANNEL_3;
    break;
  case 4:
    TIM_CHANNEL = TIM_CHANNEL_4;
    break;
  }
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL, (uint32_t)(percent * (htim1.Init.Period + 1) / 100));
}
