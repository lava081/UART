/**
 * @file esp8266.h
 * @brief ESP8266 模块驱动
 * @author lava081
 */
#include "main.h"

/** 自定义配置 */
#define RX_ESP_LEN 2920          // 接收缓冲区大小(bit): ESP8266
#define TX_ESP_RETRY 3           // 最大发送重试次数
#define TX_ESP_TIMEOUT 5000      // 单次发送超时(ms)
#define ESP_PING_URL "gitee.com" // ping的url，不要用IP地址，需确保能解析

extern char rx_esp[RX_ESP_LEN]; // 接收缓冲区: ESP8266

/**
 * @brief ESP 模块的状态枚举类型
 */
enum esp_state_t
{
  ESP_DEALING,       // 处理中
  ESP_OK,            // 操作成功完成
  ESP_ERROR,         // 发生错误或超时
  ESP_WIFI_CONNECTED // wifi已连接
};

extern enum esp_state_t ESP_STATE; // AT指令处理结果标志位

/**
 * @brief 等待ESP开机初始化
 */
void esp_init(void);

/**
 * @brief 在中断中处理MCU从ESP8266收到的数据
 * @param size 接收数据的长度
 */
void rx_esp_deal_IT(uint16_t size);

/**
 * @brief （内部方法）通过串口发送数据到 ESP8266
 * @details 该方法只应由本模块和串口模块调用
 * @param str: 要发送的数据
 * @param len: 数据长度
 */
void tx_esp_send(char *str, uint16_t len);

/**
 * @brief （内部方法）（阻塞）发送数据到ESP8266,出错重传
 * @details 该方法非直接实现消息发送，消息发送请从对应通信类型子模块调用
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_esp_until_success(char *str, uint16_t len);

/**
 * @brief （内部方法）（阻塞）发送数据到ESP8266，超时视为出错，不重传
 * @details 该方法非直接实现消息发送，消息发送请从对应通信类型子模块调用send()函数
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tx_esp_until_result(char *str, uint16_t len);

/**
 * @brief 在回调中处理接收到的TCP数据
 * @param str 去掉头部/r/n的+IPD消息部分,含+IPD
 * @param len 消息部分数据长度
 */
void rx_tcp_deal_IT(char *str, uint16_t len);
