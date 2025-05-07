#include "tcp.h"
#include "user_uart.h"
#include <string.h> // 字符串库
#include <stdio.h>  // 标准库
#include <stdlib.h> // 标准库

#include "debug.h"

char rx_tcp[RX_TCP_LEN];
char rx_tcp_msg[RX_TCP_LEN];
size_t rx_tcp_msg_deal_param = 0; // 接收数据处理状态
uint8_t TCP_STATE = 0;            // 设置成功标志

void _tx_tcp_until_success(char *str, size_t len);
void _tx_tcp_until_result(char *str, size_t len);

void tcp_init(void)
{
  _tx_tcp_until_result("AT+CIPSEND=0,1\r\n", 16);
  if (TCP_STATE == 0)
  { // wifi无需初始化
    _tx_tcp_until_result("\n", 1);
    return;
  }
  while (TCP_STATE == 2) // 等待设置成功
  {
    HAL_Delay(500); // 延时等待
  } // 清除标志位
  _tx_tcp_until_success("ATE0\r\n", 6);         // 关闭回显
  _tx_tcp_until_success("AT+CIPMUX=1\r\n", 13); // 设置多连接模式
  char command[100];
  sprintf(command, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d,%d\r\n", DEBUG_SERVER_ID, DEBUG_SERVER_HOST, DEBUG_SERVER_PORT, DEBUG_SERVER_KEEP_ALIVE); // 连接debug端口
  _tx_tcp_until_success(command, strlen(command));
}

void tcp_send(uint8_t id, char *str, size_t len)
{
  char command[20];
  sprintf(command, "AT+CIPSEND=%d,%d\r\n", id, len);
  _tx_tcp_until_success(command, strlen(command));
  _tx_tcp_until_success(str, len); // 发送数据
}

void rx_tcp_stat_deal_IT(size_t size)
{
  if (strstr(rx_tcp, "OK"))
  { // 设置成功
    TCP_STATE = 1;
  }
  else if (strstr(rx_tcp, "ERROR"))
  { // 设置失败
    TCP_STATE = 2;
  }
  else if (strstr(rx_tcp, "+IPD"))
  {
    memcpy(rx_tcp_msg, rx_tcp, size); // 拷贝数据
    rx_tcp_msg_deal_param = size;     // 设置接收数据处理状态
    memset(rx_tcp, 0, size);          // 清空接收缓冲区
  }
  else if (strstr(rx_tcp, "WIFI GOT IP"))
  { // 设置成功
    TCP_STATE = 3;
  }
}

void rx_tcp_msg_deal(size_t size)
{
  char *p = NULL;
  p = strstr(rx_tcp_msg, "+IPD"); // 查找接收数据标志 // "+IPD,0,6:shshsh"
  char length[5];
  char *p1 = NULL;
  p1 = strchr(p, ':');   // 查找数据起始标志
  p += 5;                // 跳过"+IPD,"
  uint8_t id = *p - '0'; // 获取连接id
  p += 2;                // 跳过连接id和","
  if (!p1 || p1 - p > 4)
  {                // 数据长度超过4位
    TCP_STATE = 2; // 设置失败
    return;
  }
  memcpy(length, p, p1 - p);   // 拷贝数据长度
  uint16_t len = atoi(length); // 转换为整数
  p1++;                        // 跳过":"
  switch (id)
  {
  case DEBUG_SERVER_ID: // 处理debug端口数据
    while (rx_debug_deal_param)
    {
    } // 等待接收数据处理完成
    memcpy(rx_debug, p1, len);   // 拷贝数据
    rx_debug_deal_param = len;   // 通知接收数据处理函数开始处理数据
    memset(rx_tcp_msg, 0, size); // 清空接收缓冲区
    break;
  }
}

void _tx_tcp_until_result(char *str, size_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  TCP_STATE = 0;                          // 清除标志位
  tx_tcp_send(str, len);                  // 发送数据
  while (TCP_STATE == 0)
  {
    if (HAL_GetTick() - start_systick >= TX_TCP_TIMEOUT) // 超时
    {
      TCP_STATE = 2; // 设置失败
    }
  }
  if (TCP_STATE == 1)
  {
    TCP_STATE = 0;
  }
}

void _tx_tcp_until_success(char *str, size_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  uint8_t retry = 0;                      // 重试次数
  _tx_tcp_until_result(str, len);         // 发送数据
  while (TCP_STATE == 2)
  {
    if (HAL_GetTick() - start_systick >= TX_TCP_TIMEOUT) // 给个机会，所以实际最多是2倍的超时时间
    {
      if (retry >= TX_TCP_RETRY)
      { // 超过最大重试次数
        return;
      }
      if (TCP_STATE == 2) // 防止延时时间内触发的成功
      {
        _tx_tcp_until_result(str, len); // 重试发送数据
      }
      retry++;
      start_systick = HAL_GetTick();
    }
  }
  if (TCP_STATE == 1) // 防止延时时间内触发的成功
  {                   // 设置成功
    TCP_STATE = 0;    // 清除标志位
  }
}

void tx_debug_send(char *str, size_t len)
{
  tcp_send(DEBUG_SERVER_ID, str, len); // 发送数据
}
