/**
 * @file tcp.c
 * @brief TCP客户端模块
 * @details 请在tcp.h中填写你自己的TCP服务器信息
 * @author lava081
 */
#include "tcp.h"
#include "esp8266.h"
#include <string.h> // str系列和mem系列函数
#include <stdio.h>  // printf系列函数
#include <stdlib.h> // atoi函数

/** 请在此导入实际消费tcp消息的模块 */
#include "debug.h"

/**
 * @brief 连接到TCP服务器
 * @details 支持连接到多个TCP服务器，上限5个。安全起见请确保调试服务器是开着的，否则可能导致开机变慢。
 */
void tcp_init(void)
{
  char command[100];
  sprintf(command, "AT+CIPSEND=%d,1\r\n", DEBUG_SERVER_ID); // 起手先尝试给调试服务器发消息
  tx_esp_until_result(command, strlen(command));
  if (ESP_STATE != ESP_ERROR) // 发出去了说明：1.ESP8266已经是多连接模式了 2.连着调试服务器
  {
    tx_esp_until_success("\n", 1); // 如果连着服务器，要把字符发出去以避免影响下一条指令
    goto server_end;               // 在这因为只连接一个服务器所以直接跳到结尾。多个服务器的情况请改成下一个服务器的连接检测处
  }
  tx_esp_until_success("AT+CIPMUX=1\r\n", 13); // 设置多连接模式

  /** 以下连接到服务器 */
debug_server: // debug服务器 本行系示范，未被实际使用，会导致标签未使用警告
  /** 连接debug服务器 */
  sprintf(command, "AT+CIPSTART=%d,\"TCP\",\"%s\",%d,%d\r\n", DEBUG_SERVER_ID, DEBUG_SERVER_HOST, DEBUG_SERVER_PORT, DEBUG_SERVER_KEEP_ALIVE);
  tx_esp_until_success(command, strlen(command));

server_end: // 连接完毕
}

/**
 * @brief 发消息到对端
 * @details 带重传机制
 * @param id link ID
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tcp_send(uint8_t id, char *str, uint16_t len)
{
  char command[20];
  sprintf(command, "AT+CIPSEND=%d,%d\r\n", id, len); // 通知ESP8266发送定长数据，等待就绪。该模式不会因收到字符串'\''0'而结束（谁家好人结束符用字符串的\0啊）
  tx_esp_until_success(command, strlen(command));
  tx_esp_until_success(str, len); // 发送数据
}

/**
 * @brief 在回调中处理接收到的TCP数据
 * @details 从入参中取出TCP消息内容分发给对应消费者
 * @param str 去掉头部/r/n的+IPD消息部分,含+IPD
 * 例如：+IPD,0,13:abcdefghijklm
 * @param len 消息部分数据长度
 */
void rx_tcp_deal_IT(char *str, uint16_t str_len)
{
  str += 5;                // 跳过"+IPD,"
  uint8_t id = *str - '0'; // link ID
  str += 2;                // 跳过连接id和","
  char *p1 = NULL;         // 消息起始标志':'
  p1 = strchr(str, ':');   // 查找消息起始标志
  if (!p1 || p1 - str > 4) // 数据长度超过4位,按文档不应该超过 2920
  {
    return;
  }
  char length[5];                // 消息长度
  memcpy(length, str, p1 - str); // 拷贝数据长度
  uint16_t len = atoi(length);   // 转换为整数
  p1++;                          // 跳过':'

  /** 将提取出的消息分发给消费者 */
  switch (id)
  {
    /** 以下添加消息消费者 */
  case DEBUG_SERVER_ID:
    rx_debug_ptr = p1; // 设置接收缓冲区指针，指向消息内容
    rx_debug_deal_param = len; // 通知接收函数开始处理数据
    break;
  }
}

/**
 * @brief 发送数据到调试串口
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_debug_send(char *str, uint16_t len)
{
  tcp_send(DEBUG_SERVER_ID, str, len); // 发送数据到debug服务器
}
