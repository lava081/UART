#include "main.h"

void rx_tcp_stat_deal_IT(size_t size);
void rx_tcp_msg_deal(size_t size);
void tcp_init(void);
void tx_debug_send(char *str, size_t len);
void tcp_send(uint8_t id, char *str, size_t len);

#define RX_TCP_LEN 500 // 定义接收缓冲区长度
#define TX_TCP_RETRY 10 // 定义最大发送重试次数
#define TX_TCP_TIMEOUT 500 // 定义单次发送超时时间，单位ms

#define DEBUG_SERVER_ID 0 // 本地服务器 link ID
#define DEBUG_SERVER_HOST "192.168.43.49" // 主机号，不允许域名
#define DEBUG_SERVER_PORT 2345 // 端口号
#define DEBUG_SERVER_KEEP_ALIVE 0 // keep alive间隔，单位秒，次数3

extern char rx_tcp[RX_TCP_LEN]; // 字符串类型接收数据缓冲区
extern char rx_tcp_msg[RX_TCP_LEN]; // 字符串类型接收数据缓冲区
extern size_t rx_tcp_msg_deal_param; // 接收数据处理状态
