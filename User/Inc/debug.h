#include "main.h"

void debug_init(void);
void rx_debug_deal(void);

#define RX_DEBUG_LEN 100 // 定义接收缓冲区长度

extern char rx_debug[RX_DEBUG_LEN]; // 字符串类型接收数据缓冲区
