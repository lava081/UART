#include "main.h"

#define TX_SYN6288_LEN 200 // 待发送文本缓冲区大小(bit): syn6288

#define TX_SYN6288_TIMEOUT (uint32_t)80000 // 发送超时时间(ms)

/**
 * @brief SYN6288 模块的状态枚举类型
 */
typedef enum syn6288_state_t
{
  SYN6288_INIT = (uint8_t)0x4A,   // 芯片初始化成功
  SYN6288_ACCEPT = (uint8_t)0x41, // 收到正确的命令帧
  SYN6288_REJECT = (uint8_t)0x45, // 收到不能识别命令帧
  SYN6288_BUSY = (uint8_t)0x4E,   // 芯片处在正在播音状态
  SYN6288_IDLE = (uint8_t)0x4F,   // 芯片处于空闲状态
} syn6288_state_t;

void syn6288_init(void);

void syn6288_send(char *utf8, uint16_t utf8_len);

void _syn6288_send(char *utf8, uint16_t utf8_len);

void tx_syn6288_send(char *str, uint16_t len);

extern uint8_t rx_syn6288_state;

extern char *syn6288_send_param_1;
extern volatile uint16_t syn6288_send_param_2;
