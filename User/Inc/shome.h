/**
 * @file shome.h
 * @brief 智能家居控制
 * @author lava081
 */
#include "main.h"

/** 风扇 */
void fan_on(void);
void fan_off(void);

#define fan_port_1  GPIOE
#define fan_pin_1 GPIO_PIN_7


/** 消息处理 */

void shome_init(void);

/**
 * @brief 在主循环处理调试信息
 * @param size 接收数据的长度
 */
void rx_shome_deal(uint16_t size);

/**
 * @brief 发送数据到调试串口
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_shome_send(char *str, uint16_t len);

#define RX_SHOME_LEN 2048 // 定义接收缓冲区大小(bit): shome

extern char rx_shome[RX_SHOME_LEN]; // 接收缓冲区: shome

extern char *rx_shome_ptr; // 接收缓冲区指针，该指针的内容会在主循环中被拷贝到接收缓冲区

extern volatile uint16_t rx_shome_deal_param; // shome接收函数入参

