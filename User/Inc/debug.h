#include "main.h"

void debug_init(void);
void rx_debug_deal(size_t size);

#define RX_DEBUG_LEN 100 // 定义接收缓冲区长度

extern char rx_debug[RX_DEBUG_LEN]; // 字符串类型接收数据缓冲区

extern size_t rx_debug_deal_param; // 接收数据处理状态
