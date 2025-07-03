/**
 * @file user.c
 * @brief 初始化和调用其他用户功能
 * @author lava081
 */
#include "user.h"
#include <string.h> // str系列和mem系列函数

/**
 * @brief 初始化用户部分
 */
void user_init(void)
{
  /** 依次初始化各个外设 */
  user_uart_init();
  pwm_init();
  fc03_init();
  esp_init();
  tcp_init();
  syn6288_init();
  // inmp441_init();
  shome_init();
  debug_init();
}

/**
 * @brief 主循环用户部分
 * @details 用于处理回调中不便处理的耗时逻辑和主逻辑
 */
void user_deal(void)
{
  /** debug.c 有待处理消息  */
  if (rx_debug_deal_param)
  {
    memcpy(rx_debug, rx_debug_ptr, rx_debug_deal_param); // 拷贝数据
    rx_debug_deal(rx_debug_deal_param);
    rx_debug_deal_param = 0; // 清除标志位
  }
  /** shome.c 有待处理消息 */
  if (rx_shome_deal_param)
  {
    memcpy(rx_shome, rx_shome_ptr, rx_shome_deal_param); // 拷贝数据
    rx_shome_deal(rx_shome_deal_param);
    rx_shome_deal_param = 0; // 清除标志位
  }
  if (syn6288_send_param_2)
  {
    _syn6288_send(syn6288_send_param_1, syn6288_send_param_2);
  }
  if (inmp441_buffer_index & 0x10)
  {
    tx_inmp441_send(inmp441_buffer_index);
  }
}
