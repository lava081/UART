/**
 * @file user.h
 * @brief 初始化和调用其他用户功能
 * @author lava081
 */

/** 在此导入所有用户逻辑的头文件 */
#ifndef __MAIN_H
#include "debug.h"
#include "pwm.h"
#include "user_uart.h"
#include "esp8266.h"
#include "tcp.h"
#include "syn6288.h"
#include "inmp441.h"
#include "fc03.h"
#endif

/**
 * @brief 初始化用户部分
 */
void user_init(void);

/**
 * @brief 主循环用户部分
 */
void user_deal(void); // 处理接收数据函数
