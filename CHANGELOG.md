# 2025-socchina分支 版本历史记录

由于开发前期，变更频繁，仅记录主要特性

## 0.0.1

* New features

1. 串口中断发送和空闲接收
2. 调试串口

## 0.0.2

* New features

1. pwm输出

## 0.0.3

* New features

1. 连接tcp服务器
2. 提高主频到节能模式的上限`144`MHz
3. 通过`User/`目录解耦用户逻辑，方便迁移
4. 通过`doc/`目录提供markdown类型的说明文档

* Breaking changes

1. 移除`Drivers/`文件夹转而由用户自行生成
2. 从Keil转向直接使用Makefile和GNU工具链
