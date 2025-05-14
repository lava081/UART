#include "main.h"

#define TX_SYN6288_LEN 200 // 待发送文本缓冲区大小(bit): syn6288

void syn6288_send(const char *utf8, const uint16_t utf8_len);

void tx_syn6288_send(char *str, uint16_t len);

extern uint8_t rx_syn6288_state;
