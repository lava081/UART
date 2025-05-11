#include "main.h"

#define RX_ESP_LEN 500     // 定义接收缓冲区长度
#define TX_ESP_RETRY 3    // 定义最大发送重试次数
#define TX_ESP_TIMEOUT 5000 // 定义单次发送超时时间，单位ms
#define ESP_PING_URL "gitee.com" // 定义ping的url

extern char rx_esp[RX_ESP_LEN];      // 字符串类型接收数据缓冲区
extern uint8_t ESP_STATE;

void esp_init(void);
void rx_esp_deal_IT(size_t size);
void tx_esp_send(char *str, size_t len);

void tx_esp_until_success(char *str, size_t len);
void tx_esp_until_result(char *str, size_t len);

void rx_tcp_deal_IT(char *str, size_t len);
