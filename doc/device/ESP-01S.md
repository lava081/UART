# 安信可ESP-01S

## 参考资料

[ESP-AT用户指南](https://docs.espressif.com/projects/esp-at/zh-cn/release-v2.2.0.0_esp8266/AT_Command_Set/Basic_AT_Commands.html)

[ESP-01S规格书](https://docs.ai-thinker.com/_media/esp-01s_%E8%A7%84%E6%A0%BC%E4%B9%A6.pdf)

[ESP8266系列模组专题](https://docs.ai-thinker.com/esp8266)

## 接线

3V3 - 3V3

GND - GND

TX - PA3

RX - PA2

## 关键参数

> 未尽事宜请参考规格书

### WiFi

参考[CWJAP指令](#cwjap)自行连接

* 仅2.4GHz频段(802.11n)

### 串口

应用的速率 2000000 bps (2 Mbps)

> 选这个速率只是因为这是我的串口调试软件的最高波特率，再高就要自定义了，在这偷个懒

* 默认速率 15200 bps

* 模块调试信息速率 74880 bps

* 范围 110 ~ 4608000 bps

### Ping目标地址

> 参与控制开机流程，用于检测是否已联网

`"gitee.com"`

> 在`esp8266.h`中更改

### 连接到的TCP服务器

### 调试服务器

`192.168.43.49`:`2345`(ipv4)

> 在`tcp.h`中更改

## 配置指令参考

> 所有AT指令必须由`\r\n`结尾。由于控制字符不可见，在以下命令中省略。在处理返回消息时也需要注意排除消息中的`\r\n`

> AT指令所有字符串类型参数必须用半角双引号`\"`[string]`\"`包裹

> 把`=`换成`?`就是查询指令

> 有的指令第一次可能不成功，不同版本固件行为可能不同

### 存储到flash的配置项

#### RESTORE

```string
AT+RESTORE
```

重置模块设置

#### CWMODE

```string
AT+CWMODE=1
```

Wi-Fi 模式，`1`为作为设备使用，默认值为`2`

> 因为不想处理设备+软路由模式下的被接入消息，也懒得设置密码

#### CWLAP

```string
AT+CWLAP
```

扫描周围可用wifi，每行括号内的第2和4个参数分别是wifi名称和wifi的MAC地址

#### CWJAP

```string
AT+CWJAP="My WIFI","password"
```

连接到wife，依赖[CWMODE](#cwmode)

1. ssid: 字符串，不涉及特殊字符时是wifi名称
2. pwd: 字符串，wifi密码
3. bssid: 字符串，wifi的MAC地址，存在同名wifi时必填

#### CWQAP

```string
AT+CWQAP
```

从wifi断开

#### UART

```string
AT+UART_DEF=2000000,8,1,0,0
```

设置串口参数

1. baudrate: 波特率
2. databits: 数据位
3. stopbits: 停止位
4. parity: 校验位。 0：None，1：Odd，2：Even
5. flow control: 硬件流量控制

#### SAVETRANSLINK

```string
AT+SAVETRANSLINK=1,"192.168.43.49",2345,"TCP",0
```

设置开机连接到TCP服务器`192.168.43.49`:`2345`并进入透传模式

1. mode: 开关上电透传功能
2. remote host: 字符串，服务器的域名或ip地址
3. remote port: 服务器端口号
4. type: 字符串，可选`"TCP"`，`"TCPv6"`，`"SSL"`，`"SSLv6"`
5. keep alive: 由协议栈实现的 keep-alive 的间隔，默认值为`0`，即关闭

#### +++

```string
+++
```

退出透传(临时)

### 操作指令和重启丢失的配置项

> 记录一下MCU执行的指令参数

#### CIPDOMAIN

```string
AT+CIPDOMAIN="gitee.com"
```

解析域名，解析失败报错

#### PING

```string
AT+PING="gitee.com"
```

连通性测试，填域名的话会被解析到ipv4(优先)地址，超时报错

#### ATE

```string
ATE0
```

关闭命令回显，减少处理接收消息的干扰

> 虽然都用查找子串了，不关应该也行

#### SLEEP

```string
AT+SLEEP=0
```

关闭睡眠模式

#### CIPMUX

```string
AT+CIPMUX=1
```

启用多连接，允许连接到多个TCP服务器

> 已经连接到TCP服务器时此操作必须先断开连接，否则报错

#### CIPSTART

```string
AT+CIPSTART=0,"TCP","192.168.43.49",2345,0
```

连接到TCP服务器`192.168.43.49`:`2345`并取ID为`0`

1. link ID: 自定义连接ID(0 ~ 4)，收发消息用
2. type: 字符串，可选`"TCP"`，`"TCPv6"`
3. remote host: 字符串，服务器的域名或ip地址
4. remote port: 服务器端口号
5. keep alive: 由协议栈实现的 keep-alive 的间隔，默认值为`0`，即关闭

#### CIPSEND

```string
AT+CIPSEND=0,1
```

发送1位(char[`1`])定长数据到ID为`0`的服务器，模块准备好后会发消息确认，之后收满1位数据就会发送到对端，所以在收到确认消息后直接开始数据传输就好，请不要在模块返回确认消息前就发送数据。

1. link ID
2. length: 数据长度，最大值：2048 字节

### 收到的消息

#### WIFI GOT IP

#### +IPD

```string
\r\n+IPD,0,10:abcdefghij
```

收到对端`0`的`10`字节消息`abcdefghij`

1. +IPD
2. link ID
3. length: 消息长度
4. 消息体
