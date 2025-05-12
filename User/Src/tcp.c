#include "tcp.h"
#include "esp8266.h"
#include <string.h> // 字符串库
#include <stdio.h>  // 标准库
#include <stdlib.h> // 标准库

#include "debug.h"

void tcp_init(void)
{
  char command[100];
  sprintf(command, "AT+CIPSEND=%d,1\r\n", DEBUG_SERVER_ID); // 设置keep alive时间
  tx_esp_until_result(command, strlen(command));
  if (ESP_STATE != 2)
  { // tcp无需初始化
    tx_esp_until_success("\n", 1);
    return;
  }
  tx_esp_until_success("AT+CIPMUX=1\r\n", 13); // 设置多连接模式
  sprintf(command, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d,%d\r\n", DEBUG_SERVER_ID, DEBUG_SERVER_HOST, DEBUG_SERVER_PORT, DEBUG_SERVER_KEEP_ALIVE); // 连接debug端口
  tx_esp_until_success(command, strlen(command));
}

void tcp_send(uint8_t id, char *str, size_t len)
{
  char command[20];
  sprintf(command, "AT+CIPSEND=%d,%d\r\n", id, len);
  tx_esp_until_success(command, strlen(command));
  tx_esp_until_success(str, len); // 发送数据
}

void rx_tcp_deal_IT(char *str, size_t str_len)
{
  str += 5;                // 跳过"+IPD,"
  uint8_t id = *str - '0'; // 获取连接id
  str += 2;                // 跳过连接id和","
  char *p1 = NULL;
  p1 = strchr(str, ':');   // 查找数据起始标志
  if (!p1 || p1 - str > 4)
  {                // 数据长度超过4位
    return;
  }
  char length[5];
  memcpy(length, str, p1 - str);   // 拷贝数据长度
  uint16_t len = atoi(length); // 转换为整数
  p1++;                        // 跳过":"
  switch (id)
  {
  case DEBUG_SERVER_ID: // 处理debug端口数据
    memcpy(rx_debug, p1, len);   // 拷贝数据
    rx_debug_deal_param = len;   // 通知接收数据处理函数开始处理数据
    break;
  }
}

void tx_debug_send(char *str, size_t len)
{
  tcp_send(DEBUG_SERVER_ID, str, len); // 发送数据
}
