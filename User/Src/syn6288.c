#include "syn6288.h"
#include "user_string.h"
#include "user_uart.h"
#include "debug.h"

char tx_syn6288[TX_SYN6288_LEN + 6] = {0xFD, 0x00, 0x00, 0x01, 0x03}; // 发送缓冲区: syn6288

uint8_t rx_syn6288_state;

char *syn6288_send_param_1;             // 参数1: _syn6288_send()，指向要发送的UTF-8字符串
volatile uint16_t syn6288_send_param_2; // 参数2: _syn6288_send()，要发送的UTF-8字符串长度
uint32_t syn6288_start_systick;         // 发送开始的系统滴答时间: syn6288

void syn6288_init(void)
{
  syn6288_start_systick = HAL_GetTick(); // 初始化发送开始时间
  syn6288_send("[o1][v5][t5]", 12);      // word-by-word，音量1(max16)，语速5(max5)
}

void _syn6288_send(char *utf8, uint16_t utf8_len)
{
  if (rx_syn6288_state == SYN6288_ACCEPT || rx_syn6288_state == SYN6288_BUSY)
  {
    if (syn6288_start_systick + TX_SYN6288_TIMEOUT > HAL_GetTick()) // 等待上次发送完成
    {
      return; // 如果没有数据要发且未超时，则直接返回
    }
  }
  if (utf8_len > 1.5 * TX_SYN6288_LEN) // 检查长度是否超过限制
  {
    utf8_len = (uint16_t)(1.5 * TX_SYN6288_LEN); // 如果超过，则截断
    syn6288_send_param_1 += utf8_len;
  }
  syn6288_send_param_2 -= utf8_len;                                    // 更新发送参数指针和长度
  uint16_t utf16BE_len = utf8_utf16BE(tx_syn6288 + 5, utf8, utf8_len); // 调用转换函数
  if (utf16BE_len > TX_SYN6288_LEN)
  {
    utf16BE_len = TX_SYN6288_LEN; // 如果转换后的长度超过缓冲区大小，则截断
  }
  tx_syn6288[1] = (uint8_t)((utf16BE_len + 3) >> 8);                    // 高字节
  tx_syn6288[2] = (uint8_t)((utf16BE_len + 3) & 0xFF);                  // 低字节
  tx_syn6288[utf16BE_len + 5] = xor_check(tx_syn6288, utf16BE_len + 5); // 计算校验和
  HAL_Delay(1);                                                         // 芯片手册要求多段语音时等待1ms，实测不等的话最后一段会因为太短，发的快，模块收不到，感谢小小陈的指点
  tx_syn6288_send(tx_syn6288, utf16BE_len + 6);                         // 发送数据
  syn6288_start_systick = HAL_GetTick();                                // 更新发送开始时间
  rx_syn6288_state = SYN6288_BUSY;                                      // 设置状态为发送中
}

void syn6288_send(char *utf8, uint16_t utf8_len)
{
  syn6288_send_param_1 = utf8;     // 保存发送参数
  syn6288_send_param_2 = utf8_len; // 保存发送长度
}
