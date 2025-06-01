#include "fc03.h"
#include "tim.h"

void fc03_init(void)
{
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_1);
  HAL_TIM_IC_Start(&htim3, TIM_CHANNEL_2); // 启动定时器3的输入捕获通道1和2
}

uint32_t fc03_get(void)
{
  return HAL_TIM_ReadCapturedValue(&htim3, TIM_CHANNEL_1);
}
