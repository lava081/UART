#include "debug.h"
#include "tcp.h" // TCP头文件
#include "pwm.h"
#include "user_uart.h" // 串口头文件
#include <stdio.h>     // 标准输入输出库
#include <string.h>    // 字符串处理库
#include <stdlib.h>    // 标准库

char rx_debug[RX_DEBUG_LEN];    // 字符串类型接收数据缓冲区
size_t rx_debug_deal_param = 0; // 接收数据处理状态

void debug_init(void)
{
  tx_debug_send("\r调试串口已连接!", 23); // 初始化调试函数
}

void rx_debug_deal(size_t size)
{
  tx_debug_send(rx_debug, size);            // 发送接收数据
  if (strncmp(rx_debug, "TIM", 3) == 0) // 操作定时器参数
  {
    rx_debug[size] = '\0'; // 添加字符串结束符
    uint8_t timer;
    timer = rx_debug[3] - '0';                   // 你问我想用TIM12怎么办？那我问你，你不会改成固定两位编码然后加减乘除吗？
    if (strncmp(rx_debug + 4, "CH", 2) == 0) // 设置定时器输出占空比
    {
      char tip[5];
      uint8_t channel;
      float percent;
      channel = rx_debug[6] - '0'; // 取出通道号
      percent = (float)atof(rx_debug + 8); // 将字符串转换为浮点数

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
      char tx_debug[10]; // 发送缓冲区
      sprintf(tx_debug, "\n%s %d%%", tip, (int)percent); // 不知道为什么打印不了%f，投降
      tx_debug_send(tx_debug, strlen(tx_debug));
    }
  }
}
