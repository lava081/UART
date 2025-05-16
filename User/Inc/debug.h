/**
 * @file debug.h
 * @brief 调试逻辑
 * @author lava081
 */
#include "main.h"

void debug_init(void);

/**
 * @brief 在主循环处理调试信息
 * @param size 接收数据的长度
 */
void rx_debug_deal(uint16_t size);

/**
 * @brief 发送数据到调试串口
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_debug_send(char *str, uint16_t len);

#define RX_DEBUG_LEN 2920 // 定义接收缓冲区大小(bit): debug

extern char rx_debug[RX_DEBUG_LEN]; // 接收缓冲区: debug

extern volatile uint16_t rx_debug_deal_param; // debug接收函数入参
