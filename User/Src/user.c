#include "user.h"

void user_init(void) { // 调用其他用户初始化函数
  user_uart_init();
  pwm_init();
  debug_init();
}
