/**
 * @file debug.c
 * @brief 调试逻辑
 * @author lava081
 */
#include "debug.h"
#include "tcp.h"
#include "pwm.h"
#include "esp8266.h"
#include <string.h>  // str系列和mem系列函数
#include <stdio.h>   // printf系列函数
#include <stdlib.h>  // atof函数
#include <stdbool.h> // bool类型

char rx_debug[RX_DEBUG_LEN];      // 接收缓冲区: debug
uint16_t rx_debug_deal_param = 0; // debug接收函数入参

uint16_t neofetch(char *info_buffer);
#define neorows 13
char info_buffer[neorows * 70];

void debug_init(void)
{
  uint16_t len = neofetch(info_buffer);
  tx_debug_send(info_buffer, len); // 发送系统信息

  tx_debug_send("\r调试串口已连接!", 23); // 初始化调试函数
}

/**
 * @brief 在主循环处理调试信息
 * @details
 * TIM2CH0:50.0 定时器2所有通道50%占空比
 * AT+RST 发送AT指令的重启指令到ESP模块
 * @param size 接收数据的长度
 */
void rx_debug_deal(uint16_t size)
{
  tx_debug_send(rx_debug, size);        // 回显收到的数据
  if (strncmp(rx_debug, "TIM", 3) == 0) // 操作定时器
  {
    rx_debug[size] = '\0'; // 添加字符串结束符
    uint8_t timer;
    timer = rx_debug[3] - '0';               // 你问我想用TIM12怎么办？那我问你，你不会改成固定两位编码然后加减乘除吗？
    if (strncmp(rx_debug + 4, "CH", 2) == 0) // 设置定时器输出占空比
    {
      char tip[5];
      uint8_t channel;
      float percent;
      channel = rx_debug[6] - '0';         // 取出通道号
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
      char tx_debug[10];                                 // 发送缓冲区
      sprintf(tx_debug, "\n%s %d%%", tip, (int)percent); // 不知道为什么打印不了%f，投降
      tx_debug_send(tx_debug, strlen(tx_debug));
    }
  }
  else if (strncmp(rx_debug, "AT", 2) == 0)
  {
    strcat(rx_debug, "\r\n");
    tx_esp_until_result(rx_debug, size + 2); // 发送AT指令
    // 不知道为什么,不能不清理接收缓冲区,所以拿不到返回了,去断点里看吧
    //  tx_debug_send(rx_esp, strlen(rx_esp)); // 发送AT指令返回的数据
  }
  else if (strncmp(rx_debug, "neofetch", 8) == 0)
  {
    uint16_t len = neofetch(info_buffer);
    tx_debug_send(info_buffer, len); // 发送系统信息
  }
  else
  {
    tx_debug_send("\ncommand not found", 18); // 未知命令
  }
  memset(rx_debug, 0, size); // 清理接收缓冲区
}

static uint8_t logo_st[neorows][6] = {
    {14, 28},
    {7, 35},
    {6, 5},
    {5, 5},
    {5, 4, 4, 16, 4, 7},
    {4, 5, 5, 14, 5, 6},
    {3, 8, 6, 11, 4, 6},
    {2, 11, 6, 8, 4, 6},
    {1, 14, 5, 6, 4, 6},
    {1, 14, 5, 5, 4, 7},
    {19, 5, 4, 7},
    {18, 5, 5, 4}};

static char sys_info[neorows][30] = {
    "lava081@STM32F4xx",
    "------------------",
    "OS: None thumb",
    "Device: STM32F407ZGT6",
    "CPU: Cortex-M4 (1) @ 144MHz",
    "FPU: FPv4-SP-D16",
    "Memory: 192k bit",
    "Flash: 1024k bit",
    "Storage: disabled",
    "Shell: debug.c",
    "Terminal: TCP@ESP8266",
    "Network: ESP8266@USART2",
    "Uptime: "};

uint16_t neofetch(char *info_buffer)
{
  char *info_ptr = info_buffer;
  uint8_t i, j, k;
  bool black = 0;
  for (i = 0; i < neorows; i++)
  {
    char *y_ptr = info_ptr;
    *info_ptr++ = '\n';
    for (j = 0; j < 6; j++)
    {
      for (k = 0; k < logo_st[i][j]; k++)
      {
        if (black)
        {
          *info_ptr++ = '*';
        }
        else
        {
          *info_ptr++ = ' ';
        }
      }
      black = !black;
    }
    while (info_ptr - y_ptr < 45)
    {
      *info_ptr++ = ' ';
    }
    for (j = 0; j < strlen(sys_info[i]); j++)
    {
      *info_ptr++ = sys_info[i][j];
    }
  }
  sprintf(info_ptr, "%d secs", (int)(HAL_GetTick() / 1000)); // 获取系统运行时间
  return strlen(info_buffer);
}
