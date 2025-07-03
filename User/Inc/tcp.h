/**
 * @file tcp.h
 * @brief TCP客户端模块
 * @details 请替换上你自己的TCP服务器信息
 * @author lava081
 */
#include "main.h"

/**
 * @brief 连接到TCP服务器
 */
void tcp_init(void);

/**
 * @brief （阻塞）发消息到对端
 * @param id link ID
 * @param str 发送的字符串
 * @param len 发送数据的长度
 */
void tcp_send(uint8_t id, char *str, uint16_t len);

/** 以下添加要连接的服务器信息，多连接请按照tcp.c的注释添加 */

/** 调试服务器信息，当串口用，相信你的串口调试工具是可以起TCP服务器的，如https://apps.microsoft.com/detail/9NBLGGsH43HDM */
#define DEBUG_SERVER_ID 0                 // debug服务器 link ID
#define DEBUG_SERVER_HOST "192.168.43.49" // 主机号，不允许域名
#define DEBUG_SERVER_PORT 2345            // 端口号
#define DEBUG_SERVER_KEEP_ALIVE 0         // keep alive间隔，单位秒，次数3

/** udp SHOME服务器 */
#define SHOME_SERVER_ID 1                 // shome服务器 link ID
#define SHOME_SERVER_HOST "192.168.43.49" // 主机号，不允许域名
#define SHOME_SERVER_PORT 9191            // 端口号
#define SHOME_LOCAL_PORT 8080             // 本地端口
