# 鹿小班 LXB407ZG-P1 V2.0 核心板

## 参考资料

1. [STM32F407ZG - ST](https://www.st.com.cn/zh/microcontrollers-microprocessors/stm32f407zg.html)
2. [RM0090参考手册中文翻译](https://gitcode.com/Open-source-documentation-tutorial/46f32)

## 板载资源

外部高速晶振(提供HSE): 8 MHz

外部低速晶振(提供LSE): 32.768 kHz

UART: 连接到`USART1`

## 接线

STM32 - STLink

5V - 5V

GND - GND

DIO - SWDIO

CLK - SWCLK

## 时钟配置(RCC)

高速时钟源: 8 MHz 外部晶振(HSE)

系统时钟(SYSCLK): 144 MHz

> 如需节能，可先降频至 144 MHz，然后在`RCC`的`Parameter Settings`中选择`Power Regulator Voltage Scale 2`

> 如需高性能，可先在`RCC`的`Parameter Settings`中选择`Power Regulator Voltage Scale 1`，然后升频至 168 MHz

主频(HCLK): 144 MHz

`AHB`: 144 MHz

`APB1`外设(PCLK1): 36 MHz

`APB1`定时器: 72 MHz

`APB2`外设(PCLK2): 72 MHz

`APB2`定时器: 144 MHz

## 中断配置(NVIC)

### 串口中断

* 勾选`Generate Enable in Init`

* 勾选`Generate IRQ handler`

* 勾选`Call HAL handler`

## 杂项

### 外围总线挂载点

见数据手册 `2.3 存储器映射`。需注意`APB2`速度高于`APB1`，且定时器速度为外设的`2`倍
