#include "pwm.h"
#include "tim.h" // 定时器头文件

void pwm_init(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // 启动定时器1 PWM 输出通道1
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);

  set_pwm(2, 1, 1); // 设置定时器2 PWM 输出通道1 占空比为1%
  set_pwm(2, 2, 1); // 设置定时器2 PWM 输出通道2 占空比为1%
  set_pwm(2, 3, 1); // 设置定时器2 PWM 输出通道3 占空比为1%
  set_pwm(2, 4, 1); // 设置定时器2 PWM 输出通道4 占空比为1%
}

void set_pwm(uint8_t timer, uint8_t channel, float percent)
{

  TIM_HandleTypeDef *htim; // 定义定时器句柄
  uint8_t TIM_CHANNEL;     // 定义通道号

  if (percent > 100 || percent < 0) // 占空比范围检查
  {
    return;
  }

  switch (timer)
  { // 选择定时器
  case 2:
    htim = &htim2;
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
  default:
    return; // 无效通道
  }
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL, (uint32_t)(percent * (htim->Init.Period + 1) / 100));
}
