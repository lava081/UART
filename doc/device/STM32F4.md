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

## GPIO

### 输出速度(Maximum output speed)

> 见数据手册`7.4.3 GPIO 端口输出速度寄存器`

通常而言，速度越高，输出能力越低

* Low: 2 MHz
* Medium: 25 MHz
* High: 50 MHz
* Very High: 80/100 MHz

## TIM

定时器

> 部分输出通道有互补通道，例如，当`CH1`应输出高电平时，`CH1N`输出低电平

### PWM输出

* 设置对应`Channel`为`PWM Generation`

> PWM(脉宽调试)的典型应用场景是通过调节占空比(高低电平占比)使得高频翻转的固定电压电平输出可以近似看作0至高电平电压的直流输出

#### 时钟源(Clock Source)

内部时钟(Internal Clock)

#### 频率

由`Counter Period (ARR)`决定，计算公式为

$$ f = \frac{APB Timer Clock}{ARR+1} $$

例如，APB1总线的Timer时钟频率为`72 MHz`，Period设为`1999`，则输出PWM波的频率为

$$ f_{PWM} = \frac{72 MHz}{1999+1} = 36 kHz $$

#### 占空比

由`Pulse`决定，计算公式为

$$ V_{out} = V_{in} \cdot \frac{Pulse}{Period+1} \times 100 \% $$

例如，当`Period (ARR)`为`1999`，`Pulse`为`2000`时，占空比为

$$ \frac{2000}{1999+1} = 100 \% $$

在`PWM mode 1`下输出恒为高电平

## UART

异步串口，与USART的区别是少了同步串口相关功能，同步串口需要时钟信号。

* 记得为有接收需求的串口配置接收中断

> USART中，`S`意为`Sync`同步，`A`意为`Async`异步，`R`是`RX`接收器，`T`是`TX`发送器。我认为理论上它能兼容所有使用高低电平作为信号的协议，只要你有足够多的USART，并且有能力自行解码，部分协议可能还需要电平转换。

## 杂项

### 外围总线挂载点

见数据手册`2.3 存储器映射`。需注意`APB2`速度高于`APB1`，且定时器速度为外设的`2`倍
