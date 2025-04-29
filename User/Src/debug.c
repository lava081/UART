#include "debug.h"
#include "usart.h" 
#include "string.h" // 字符串处理库

char debug_rx[RX_LEN_DEBUG]; // 字符串类型接收数据缓冲区

void debug_init(void)
{
	HAL_UART_Transmit(&huart1, (uint8_t *)"\rUSART1 connected!", 18, HAL_MAX_DELAY); // 开机打个招呼
  HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t *)debug_rx,RX_LEN_DEBUG); // 启动接收空闲中断
}

/**
 * @brief 处理 USART1 接收完成中断回调函数
 * @param huart: 指向 UART_HandleTypeDef 结构体的指针
 * @param Size: 接收到的数据长度
 */
void HAL_UARTEx_RxEventCallback(UART_HandleTypeDef *huart, uint16_t Size)
{
    if (huart->Instance == USART1)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)debug_rx, Size, HAL_MAX_DELAY);	//将接受到的数据再发回上位机
        memset(debug_rx, 0, Size);
        HAL_UARTEx_ReceiveToIdle_IT(&huart1,(uint8_t *)debug_rx,RX_LEN_DEBUG); // 启动接收中断
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
