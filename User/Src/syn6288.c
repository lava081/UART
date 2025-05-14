#include "syn6288.h"
#include "user_string.h"
#include "user_uart.h"
#include "debug.h"

char tx_syn6288[TX_SYN6288_LEN + 6] = {0xFD, 0x00, 0x00, 0x01, 0x03}; // 发送缓冲区: syn6288

uint8_t rx_syn6288_state;

void syn6288_send(const char *utf8, const uint16_t utf8_len)
{
  while (rx_syn6288_state == 0x41 || rx_syn6288_state == 0x4E) // 等待上次发送完成
  {
    HAL_Delay(500);
  }
  uint16_t utf16BE_len = utf8_utf16BE(tx_syn6288 + 5, utf8); // 调用转换函数
  if (utf16BE_len > TX_SYN6288_LEN)
  {
    utf16BE_len = TX_SYN6288_LEN; // 如果转换后的长度超过缓冲区大小，则截断
  }
  tx_syn6288[1] = (uint8_t)((utf16BE_len + 3) >> 8);   // 高字节
  tx_syn6288[2] = (uint8_t)((utf16BE_len + 3) & 0xFF); // 低字节
  uint8_t check_sum = 0;                               // 校验和
  for (uint8_t i = 0; i < utf16BE_len + 5; i++)
  {
    check_sum ^= tx_syn6288[i]; // 异或校验
  }
  tx_syn6288[utf16BE_len + 5] = check_sum;
  tx_syn6288_send(tx_syn6288, utf16BE_len + 6); // 发送数据
}
