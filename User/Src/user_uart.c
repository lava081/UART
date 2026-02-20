/**
 * @file user_uart.c
 * @brief 处理串口和外设的对应关系
 * @details 统一分配串口和外设的对应关系，统一处理串口接收中断
 * 外设需在头文件中提供发送函数的声明，接收缓冲区和接收处理函数，并实现接收函数
 * 请提前在CubeMX中配置好串口的通信参数和中断，记得勾选初始化和IRQhandler以及调用HALhandler
 * @author lava081
 */
#include "user_uart.h"
#include "usart.h" // 系统串口

/** 所有需要收发串口的模块在这导入 */
#include "esp8266.h"
#include "syn6288.h"

/**
 * @brief 依次启用各串口接收
 */
void user_uart_init(void)
{
  HAL_UARTEx_ReceiveToIdle_DMA(&huart6, (uint8_t *)rx_esp, RX_ESP_LEN); // 串口接收空闲中断
  HAL_UART_Receive_IT(&huart3, &rx_syn6288_state, 1);                  // 单字节串口接收中断
}

/**
 * @brief 接收完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 * @param Size: 接收到的数据长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t size)
{
  if (huart->Instance == USART6)
  {
    rx_esp_deal_IT(size);                                                // 调用外设的接收处理函数，调用完记得清缓冲区
    HAL_UARTEx_ReceiveToIdle_DMA(&huart6, (uint8_t *)rx_esp, RX_ESP_LEN); // 重新开启接收中断
  }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART3)
  {
    HAL_UART_Receive_IT(&huart3, &rx_syn6288_state, 1); // 重新开启接收中断
  }
}

/** 以下实现各外设的串口发送函数 */

/**
 * @brief 从串口发送数据到 ESP8266
 * @param str: 要发送的数据
 * @param len: 数据长度
 */
void tx_esp_send(char *str, uint16_t len)
{
  HAL_UART_Transmit_DMA(&huart6, (uint8_t *)str, len); // 发送中断
}

/**
 * @brief 从串口发送数据到语音合成模块
 * @param str: 要发送的数据
 * @param len: 数据长度
 */
void tx_syn6288_send(char *str, uint16_t len)
{
  HAL_UART_Transmit_IT(&huart3, (uint8_t *)str, len); // 发送中断
}

/**
 * @brief 串口错误回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 * @details
 * 该函数在串口发生错误时被调用，重新开启接收中断。
 * 可以通过查看 huart->ErrorCode 来获取错误类型。
 */
void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  huart->ErrorCode = HAL_UART_ERROR_NONE;
  if (huart == &huart6)
  {
    HAL_UARTEx_ReceiveToIdle_DMA(&huart6, (uint8_t *)rx_esp, RX_ESP_LEN); // 重新开启接收中断
  }
  else if (huart == &huart3)
  {
    HAL_UART_Receive_IT(&huart3, &rx_syn6288_state, 1); // 重新开启接收中断
  }
}
