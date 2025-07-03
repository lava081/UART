#include "shome.h"
#include "syn6288.h"
#include <string.h> // str系列和mem系列函数
#include <stdio.h>  // printf系列函数

/** 风扇 */
/**
 * @brief 开风扇
 */
void fan_on()
{
  HAL_GPIO_WritePin(fan_port_1, fan_pin_1, GPIO_PIN_RESET);
}

/**
 * @brief 关风扇
 */
void fan_off()
{
  HAL_GPIO_WritePin(fan_port_1, fan_pin_1, GPIO_PIN_SET);
}

/** 消息处理 */
char rx_shome[RX_SHOME_LEN];               // 接收缓冲区: shome
char *rx_shome_ptr; // 接收缓冲区指针
volatile uint16_t rx_shome_deal_param = 0; // shome接收函数入参

/**
 * @brief 初始化智能家居控制
 */
void shome_init(void)
{
  fan_off();
  tx_shome_send("Ready.", 8); // 发送初始化完成消息
  static char startup_ok[30];
  sprintf(startup_ok, "已开机，耗时%3d秒", (int)(HAL_GetTick() / 1000)); // 获取开机时间
  // sprintf(startup_ok, "已开机"); // 获取开机时间
  syn6288_send(startup_ok, 24); // 发送语音合成指令
}

/**
 * @brief 处理接收到的控制消息
 * @param size 接收数据的长度
 */
void rx_shome_deal(uint16_t size)
{
  rx_shome[size] = '\0';          // 添加字符串结束符
  if (strncmp(rx_shome, "FAN_",4) == 0)
  {
    if (strncmp(rx_shome + 4, "ON", 2) == 0) // 开风扇
    {
      fan_on();
    }
    else if (strncmp(rx_shome + 4, "OF", 2) == 0) // 关风扇
    {
      fan_off();
    }
  }
  else if (strncmp(rx_shome, "say", 3) == 0)
  {
    syn6288_send(rx_shome + 4, size - 4); // 发送语音合成指令
  }
}
