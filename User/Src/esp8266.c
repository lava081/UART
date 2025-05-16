/**
 * @file esp8266.c
 * @brief ESP8266 模块驱动
 * @details 依赖user_uart.c。请提前配置好模块的wifi连接和波特率以及串口对应关系。建议关闭软路由功能或为软路由设置密码以减少无关消息干扰。
 * @author lava081
 */
#include "esp8266.h"
#include <string.h> // str系列和mem系列函数
#include <stdio.h>  // printf系列函数

/** 接收缓冲区: ESP8266 */
char rx_esp[RX_ESP_LEN];

/** AT指令处理结果标志位 */
enum esp_state_t ESP_STATE;

/**
 * @brief 等待ESP开机初始化
 * @details 会一直等到连到互联网，没有超时机制
 * 因为作者笨笨的，老是忘记开热点，而且不等到连上wifi的话后续初始化操作可能会因为失败而被跳过
 */
void esp_init(void)
{
  char cmd[50];
  sprintf(cmd, "AT+CIPDOMAIN=\"%s\"\r\n", ESP_PING_URL); // 尝试解析域名，如果刚开机，会因超时而ESP_ERROR
  tx_esp_until_result(cmd, strlen(cmd));
  while (ESP_STATE == ESP_ERROR) // 在此没有次数限制，所以没网或者解析不上域名的话会卡死在这
  {
    tx_esp_until_result(cmd, strlen(cmd)); // 所以，请先确保你的网络环境能正常解析你提供的域名。如果没有能力解析域名，请把上面的指令换成AT+PING
    HAL_Delay(500);                        // 延时等待
  }
  tx_esp_until_success("ATE0\r\n", 6);        // 关闭回显
  tx_esp_until_success("AT+SLEEP=0\r\n", 13); // 取消休眠
}

/**
 * @brief 在中断中处理MCU从ESP8266收到的数据
 * @param size 接收数据的长度
 */
void rx_esp_deal_IT(uint16_t size)
{
  rx_esp[size] = '\0'; // 让strstr轻松点
  if (strstr(rx_esp, "OK"))
  {
    ESP_STATE = ESP_OK;
  }
  else if (strstr(rx_esp, "ERROR"))
  {
    ESP_STATE = ESP_ERROR;
  }
  else if (strstr(rx_esp, "+IPD")) // 收到对端消息
  {
    rx_tcp_deal_IT(strstr(rx_esp, "+IPD"), size + rx_esp - strstr(rx_esp, "+IPD")); // 浅浅去个头，相信编译器会优化这里的
  }
  else if (strstr(rx_esp, "WIFI GOT IP"))
  {
    ESP_STATE = ESP_WIFI_CONNECTED;
  }
  // memset(rx_esp, 0, size); // 清除接收缓冲区
}

/**
 * @brief 发送数据到ESP8266，出错重传
 * @details 依赖tx_esp_until_result()函数
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_esp_until_success(char *str, uint16_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  uint8_t retry = 0;                      // 重试次数
  tx_esp_until_result(str, len);          // 发送数据
  while (ESP_STATE == ESP_ERROR)
  {
    if (HAL_GetTick() - start_systick >= TX_ESP_TIMEOUT) // 给个机会，所以实际最多是2倍的超时时间
    {
      if (retry >= TX_ESP_RETRY) // 超过最大重试次数
      {
        return;
      }
      if (ESP_STATE == ESP_ERROR) // 防止延时时间内触发的成功
      {
        tx_esp_until_result(str, len); // 重试发送数据
      }
      retry++;
      start_systick = HAL_GetTick(); // 下一轮重试前重新计时
    }
  }
}

/**
 * @brief 发送数据到ESP8266，超时视为出错，不重传
 * @details 依赖tx_esp_send()函数
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_esp_until_result(char *str, uint16_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  ESP_STATE = ESP_DEALING;                // 清除标志位
  tx_esp_send(str, len);                  // 发送数据
  while (ESP_STATE == ESP_DEALING)        // 等待结果
  {
    if (HAL_GetTick() - start_systick >= TX_ESP_TIMEOUT) // 超时
    {
      ESP_STATE = ESP_ERROR;
    }
  }
}

/**
 * @brief 在回调中处理接收到的TCP数据
 * @details 这里是个弱定义的空函数，实际实现会被tcp.c覆盖。这么设计是为了解耦，即使哪天用不上TCP了也不用大改，删掉tcp.c就行
 * @param str 去掉头部/r/n的+IPD消息部分,含+IPD
 * @param len 消息部分数据长度
 */
__weak void rx_tcp_deal_IT(char *str, uint16_t len)
{
  UNUSED(str); // 避免编译器报错，跟HAL库文件学的
  UNUSED(len);
}
