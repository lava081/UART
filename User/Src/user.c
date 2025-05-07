#include "user.h"

void user_init(void)
{ // 调用其他用户初始化函数
  user_uart_init();
  pwm_init();
  tcp_init();
  debug_init();
}

void user_deal(void)
{
  if (rx_tcp_msg_deal_param) // 接收数据处理状态
  {
    rx_tcp_msg_deal(rx_tcp_msg_deal_param); // 处理接收数据
    rx_tcp_msg_deal_param = 0;              // 清除接收数据处理状态
  }
  else if (rx_debug_deal_param) // 接收数据处理状态
  {
    rx_debug_deal(rx_debug_deal_param); // 处理接收数据
    rx_debug_deal_param = 0;            // 清除接收数据处理状态
  }
}
