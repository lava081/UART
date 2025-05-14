/**
 * @file pwm.h
 * @brief PWM输出控制
 * @author lava081
 */
#include "main.h"

/**
 * @brief 启动 PWM 输出
 */
void pwm_init(void);

/**
 * @brief 设置 PWM 输出通道占空比
 * @param timer 定时器编号
 * @param channel 通道编号
 * @param percent 占空比，范围0-100.0
 */
void set_pwm(uint8_t timer, uint8_t channel, float percent);
