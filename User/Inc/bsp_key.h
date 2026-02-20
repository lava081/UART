#include "main.h"
typedef enum // 按键状态
{
  BSP_KEY_RELEASED = 0x00, // 按键松开
  BSP_KEY_PRESSED = 0x01,  // 按键短按
  BSP_KEY_DOUBLE_PRESSED = 0x02, // 按键双击
  BSP_KEY_LONG_PRESSED = 0x10,  // 按键长按
} BSP_Key_State_t;
extern BSP_Key_State_t BSP_Key_state;
#define BSP_Key_scan_timedout 5000 // 按键扫描超时时长，单位ms
#define BSP_Key_long_press_duration 1000 // 按键长按分界线，单位ms
void BSP_Key_read(void);
void BSP_Key_deal(void); // 处理按键状态变化
