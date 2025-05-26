#include "i2s.h"
#include "inmp441.h"
#include "debug.h"
#include <stdio.h> // sprintf函数
#include <string.h> // memcpy函数

uint16_t inmp441_buffer[2][INMP441_BUFFER_SIZE / 2];    // INMP441数据缓冲区
char tx_inmp441_buffer[INMP441_BUFFER_SIZE * 2 / 8]; // INMP441发送缓冲区

volatile uint8_t inmp441_buffer_index; // INMP441缓冲区索引

void inmp441_switch_buffer(void);

void inmp441_init(void)
{
  inmp441_buffer_index = 0; // 初始化缓冲区索引
  inmp441_switch_buffer(); // 启动DMA接收
}

void inmp441_switch_buffer(void)
{
  HAL_I2S_Receive_DMA(&hi2s3, inmp441_buffer[inmp441_buffer_index & 0x01], INMP441_BUFFER_SIZE / 2); // 启动DMA接收
  inmp441_buffer_index ^= 0x01; // 切换缓冲区索引
}

void HAL_I2S_RxCpltCallback(I2S_HandleTypeDef *hi2s)
{
  
  if (hi2s == &hi2s3)
  {
    inmp441_switch_buffer(); // 切换到下一个缓冲区
    inmp441_buffer_index |= 0x10; // 设置高位，表示有数据待处理
  }
}

void tx_inmp441_send(uint8_t buffer_index)
{
  inmp441_buffer_index &= ~0x10; // 清除高位，表示数据已处理
  volatile uint8_t offset = 2;
  // 每4个16位采样取第1个采样存入inmp441发送缓冲区
  for (uint16_t i = 0; i < 10; i ++)
  {
    if (inmp441_buffer[buffer_index & 0x01][i] == 0 && inmp441_buffer[buffer_index & 0x01][i + 1] == 0)
    {
      offset = i + 2; // 找到第一个非0采样的偏移
      break;
    }
  }
  for (uint16_t i = offset; i < INMP441_BUFFER_SIZE / 2; i += 4)
  {
    uint32_t word;
    word = inmp441_buffer[buffer_index & 0x01][i + 1] + (inmp441_buffer[buffer_index & 0x01][i] << 16); // 将两个16位采样合并为一个32位采样
    word >>= 7;
    tx_inmp441_buffer[(i - offset) * 2 / 4 + 0] = (uint8_t)((word >> 4) & 0xFF);
    tx_inmp441_buffer[(i - offset) * 2 / 4 + 1] = (uint8_t)((word >> 12) & 0x7F);
    tx_inmp441_buffer[(i - offset) * 2 / 4 + 1] |= (uint8_t)((word >> 16) & 0x80); // 设置符号位
    // tx_inmp441_buffer[(i - offset) * 3 / 4 + 2] = (uint8_t)((word >> 16) & 0xFF);
  }
  tx_debug_send(tx_inmp441_buffer, INMP441_BUFFER_SIZE * 2 / 8); // 发送INMP441数据
  // char info[20];
  // sprintf(info, "\n%lu ms", HAL_GetTick()); // 获取系统运行时间
  // tx_debug_send(info, strlen(info)); // 发送系统运行时间
}
