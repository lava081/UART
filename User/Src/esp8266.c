#include "esp8266.h"
#include <string.h> // 字符串库
#include <stdio.h>  // 标准库

char rx_esp[RX_ESP_LEN];
uint8_t ESP_STATE;            // 设置成功标志

void esp_init(void)
{
  ESP_STATE = 2;
  char cmd[25];
  sprintf(cmd, "AT+PING=\"%s\"\r\n", ESP_PING_URL);
  while (ESP_STATE == 2)
  {
    tx_esp_until_result(cmd, strlen(cmd));
    HAL_Delay(500); // 延时等待
  }
  tx_esp_until_success("ATE0\r\n", 6);         // 关闭回显
}

void rx_esp_deal_IT(size_t size)
{
  if (strstr(rx_esp, "OK"))
  { // 设置成功
    ESP_STATE = 1;
  }
  else if (strstr(rx_esp, "ERROR"))
  { // 设置失败
    ESP_STATE = 2;
  }
  else if (strstr(rx_esp, "+IPD"))
  {
    rx_tcp_deal_IT(strstr(rx_esp, "+IPD"), size + rx_esp - strstr(rx_esp, "+IPD"));
  }
  else if (strstr(rx_esp, "WIFI GOT IP"))
  { // 设置成功
    ESP_STATE = 3;
  }
}

void tx_esp_until_success(char *str, size_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  uint8_t retry = 0;                      // 重试次数
  tx_esp_until_result(str, len);         // 发送数据
  while (ESP_STATE == 2)
  {
    if (HAL_GetTick() - start_systick >= TX_ESP_TIMEOUT) // 给个机会，所以实际最多是2倍的超时时间
    {
      if (retry >= TX_ESP_RETRY)
      { // 超过最大重试次数
        return;
      }
      if (ESP_STATE == 2) // 防止延时时间内触发的成功
      {
        tx_esp_until_result(str, len); // 重试发送数据
      }
      retry++;
      start_systick = HAL_GetTick();
    }
  }
  if (ESP_STATE == 1) // 防止延时时间内触发的成功
  {                   // 设置成功
    ESP_STATE = 0;    // 清除标志位
  }
}

void tx_esp_until_result(char *str, size_t len)
{
  uint32_t start_systick = HAL_GetTick(); // 执行操作的系统滴答时间
  ESP_STATE = 0;                          // 清除标志位
  tx_esp_send(str, len);                  // 发送数据
  while (ESP_STATE == 0)
  {
    if (HAL_GetTick() - start_systick >= TX_ESP_TIMEOUT) // 超时
    {
      ESP_STATE = 2; // 设置失败
    }
  }
  if (ESP_STATE == 1)
  {
    ESP_STATE = 0;
  }
}

__weak void rx_tcp_deal_IT(char *str, size_t len)
{
  UNUSED(str);
  UNUSED(len);
}
