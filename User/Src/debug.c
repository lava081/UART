#include "debug.h"
#include "usart.h" 

char debug_rx[RX_LEN_DEBUG]; // 字符串类型接收数据缓冲区

void debug_init(void)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"\rUSART1 connected!", 17, HAL_MAX_DELAY); // 开机打个招呼
  HAL_UART_Receive_IT(&huart1, (uint8_t *)&debug_rx, sizeof(debug_rx)); // 启动接收中断
}

/**
 * @brief 处理 USART1 接收完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
  if (huart->Instance == USART1) // 判断是否是 USART1 的中断请求
  {
    HAL_UART_Transmit(&huart1, (uint8_t *)&debug_rx, sizeof(debug_rx), HAL_MAX_DELAY); // 回显接收到的数据
    HAL_UART_Receive_IT(&huart1, (uint8_t *)&debug_rx, sizeof(debug_rx)); // 重新启动接收中断
  }
}

/**
 * @brief 处理 USART1 发送完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 */
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
	if (huart->Instance == USART1) // 判断是否是 USART1 的中断请求
	{
    // 啥都不干
	}
}
