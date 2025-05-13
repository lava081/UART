/**
 * @file pwm.c
 * @brief PWM输出控制
 * @details 请提前在CubeMX中将定时器通道配置成PWM生成模式，计算并填写好定时器的Period参数
 * @author lava081
 */
#include "pwm.h"
#include "tim.h" // 系统定时器

/**
 * @brief 启动 PWM 输出
 * @details 设置占空比之前要先启动一下pwm输出，当然你想先设置再启动也行
 */
void pwm_init(void)
{
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // 启动定时器2 PWM 输出通道1，有start当然有stop
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_4);
}

/**
 * @brief 设置 PWM 输出通道占空比
 * @param timer 定时器编号
 * @param channel 通道编号
 * @param percent 占空比，范围0-100.0
 * @details 本函数不需要随定时器Period参数变化而修改
 */
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
  __HAL_TIM_SET_COMPARE(htim, TIM_CHANNEL, (uint32_t)(percent * (htim->Init.Period + 1) / 100)); // 根据Period计算Pulse的值，所以即使频率变了也不用改这里
}
