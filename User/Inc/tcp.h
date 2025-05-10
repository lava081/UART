#include "main.h"

void tcp_init(void);
void tcp_send(uint8_t id, char *str, size_t len);

#define DEBUG_SERVER_ID 0                 // 本地服务器 link ID
#define DEBUG_SERVER_HOST "192.168.43.49" // 主机号，不允许域名
#define DEBUG_SERVER_PORT 2345            // 端口号
#define DEBUG_SERVER_KEEP_ALIVE 0         // keep alive间隔，单位秒，次数3

extern size_t rx_tcp_msg_deal_param; // 接收数据处理状态
