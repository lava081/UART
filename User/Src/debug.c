#include "debug.h"
#include "usart.h"  // 串口头文件
#include <stdio.h>  // 标准输入输出库
#include <string.h> // 字符串处理库
#include <stdlib.h> // 标准库

char rx_debug[RX_DEBUG_LEN]; // 字符串类型接收数据缓冲区

void debug_init(void)
{
  tx_debug_send("\rUSART1 connected!", 19); // 初始化调试函数
}

void rx_debug_deal(void)
{
  if (strncasecmp(rx_debug, "TIM", 3) == 0) // 操作定时器参数
  {
    uint8_t timer;
    timer = rx_debug[3] - '0';                   // 你问我想用TIM12怎么办？那我问你，你不会改成固定两位编码然后加减乘除吗？
    if (strncasecmp(rx_debug + 4, "CH", 2) == 0) // 设置定时器输出占空比
    {
      char tx_debug[9], tip[3];
      uint8_t channel, percent;
      channel = rx_debug[6] - '0';
      percent = atoi(rx_debug + 8); // 将字符串转换为整数

      if (percent == 0)
      {
        tx_debug_send("\n0 not allowed!", 16);
        return; // 如果占空比为0，则不执行后续操作，小车急停不需要通过设置占空比实现
      }

      if (channel == 0)
      {
        set_pwm(timer, 1, percent);
        set_pwm(timer, 2, percent);
        set_pwm(timer, 3, percent);
        set_pwm(timer, 4, percent);
        strcpy(tip, "ALL");
      }
      else if (channel == 5)
      {
        set_pwm(timer, 1, percent);
        set_pwm(timer, 3, percent);
        strcpy(tip, "1&3");
      }
      else if (channel == 6)
      {
        set_pwm(timer, 2, percent);
        set_pwm(timer, 4, percent);
        strcpy(tip, "2&4");
      }
      else
      {
        set_pwm(timer, channel, percent);
        sprintf(tip, "CH%d", channel); // 将整数转换为字符串
      }
      sprintf(tx_debug, "\n%s %d%%", tip, percent); // 将整数转换为字符串
      tx_debug_send(tx_debug, strlen(tx_debug));
    }
  }
}
