#include "bsp_key.h"

BSP_Key_State_t BSP_Key_state = BSP_KEY_RELEASED; // 按键状态

BSP_Key_State_t BSP_Key_state_tmp = BSP_KEY_RELEASED; // 临时按键状态，用于处理按键状态变化

/**
 * @brief 读取板载按键状态
 * @details 读取按键状态并更新全局变量 BSP_Key_value
 * 本函数原本预期实现长按和连按识别，但误触问题很严重故未实现。
 * 测试时发现板载按键很松，即使没有完全按下也会被识别为按下，甚至只是按键朝上也会因为重力误触。
 * 按键误触的持续时间和正常按下没有发现明显区别，暂时无法实现防抖
 */
void BSP_Key_read(void)
{
  static GPIO_PinState key_last_read = GPIO_PIN_SET; // 上次读取的按键状态
  static uint32_t key_press_start_time = 0; // 按键按下开始时间
  static uint32_t key_toggle_time = 0;
  GPIO_PinState key_read = HAL_GPIO_ReadPin(BSP_Key_GPIO_Port, BSP_Key_Pin);
  if (HAL_GetTick() - key_press_start_time > BSP_Key_scan_timedout) // 如果按键按下开始时间超过扫描超时时长
  {
    key_press_start_time = 0; // 重置按键按下开始时间
    key_toggle_time = 0; // 重置按键切换时间
    key_last_read = GPIO_PIN_SET; // 重置上次读取状态为松开
    BSP_Key_state = BSP_Key_state_tmp; // 更新全局按键状态
    BSP_Key_state_tmp = BSP_KEY_RELEASED; // 临时状态重置为松开
  }
  if (key_read == GPIO_PIN_RESET) // 按键被按下
  {
    if (key_last_read == GPIO_PIN_SET) // 上次读取为松开状态
    {
      key_last_read = GPIO_PIN_RESET; // 更新上次读取状态
      key_toggle_time = HAL_GetTick(); // 记录按键按下时间
      if (!key_press_start_time) // 如果按键按下开始时间为0
      {
        key_press_start_time = HAL_GetTick(); // 记录按键按下开始时间
        BSP_Key_state_tmp = BSP_KEY_LONG_PRESSED;
      }
    }
  }
  else // 按键松开
  {
    if (key_last_read == GPIO_PIN_RESET) // 上次读取为按下状态
    {
      key_last_read = GPIO_PIN_SET; // 更新上次读取状态
      if (key_toggle_time) {
        if (HAL_GetTick() - key_toggle_time < BSP_Key_long_press_duration) // 如果按键切换时间没超过长按分界线
        {
          if (BSP_Key_state_tmp == BSP_KEY_LONG_PRESSED) // 如果临时状态是长按
          {
            BSP_Key_state_tmp = BSP_KEY_PRESSED; // 临时状态设置为短按
          }
          else if (BSP_Key_state_tmp == BSP_KEY_PRESSED) // 如果临时状态是短按
          {
            BSP_Key_state_tmp = BSP_KEY_DOUBLE_PRESSED; // 临时状态也设置为双击
          }
        }
        else
        {
          BSP_Key_state_tmp = BSP_KEY_LONG_PRESSED;
        }
      }
    }
  }
}

__weak void BSP_Key_deal(void)
{
  BSP_Key_read(); // 读取按键状态
  BSP_Key_state = BSP_KEY_RELEASED;
}
