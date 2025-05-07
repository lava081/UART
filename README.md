# 基本信息

开发环境: [STM32CubeMX](https://www.st.com.cn/zh/development-tools/stm32cubemx.html), [Keil UV5](https://www.keil.com/demo/eval/arm.htm)

Firmware Package: STM32Cube FW_F4 V1.28.1

芯片: STM32F407ZGT6

材料:

1. 鹿小班 LXB407ZG-P1 V2.0 核心板
2. ESP-01S

# 食用指南

1. 配置开发工具，下载所需库文件

2. 下载本仓库

3. 用`STM32CubeMX`打开`bsp.ioc`

4. 单击`GENERATE CODE`

5. 用`Keil`打开`MDK-ARM/bsp.uvprojx`

6. 按`F7`

7. 用`ST-LINK`连接你拼好的的开发板

8. 按`F8`

9. 如果上述步骤中缺什么，相信你能下载到的
