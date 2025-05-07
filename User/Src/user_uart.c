/**
 * @file user_uart.c
 * @brief 串口和外设的对应关系
 * @details 统一分配串口和外设的对应关系，统一处理串口接收中断，外设需在头文件中提供接收缓冲区和接收处理函数
 * @author lava081
 */
#include "user_uart.h" // 用户头文件
#include "usart.h"     // 串口头文件
#include "string.h"    // 字符串处理头文件

/** 所有需要收串口的头文件定义一下接受缓冲区然后在这导入 */
#include "tcp.h"

void user_uart_init(void)
{
  HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)rx_tcp, RX_TCP_LEN);
}

/**
 * @brief 处理接收完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 * @param Size: 接收到的数据长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
  if (huart->Instance == USART1)
  {
    rx_tcp_stat_deal_IT(Size); // 处理接收数据
    HAL_UARTEx_ReceiveToIdle_IT(&huart1, (uint8_t *)rx_tcp, RX_TCP_LEN);
  }
}

void tx_tcp_send(char *str, size_t len)
{
  HAL_UART_Transmit_IT(&huart1, (uint8_t *)str, len); // 发送数据
}
