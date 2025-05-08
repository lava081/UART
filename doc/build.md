[返回](../README.md)

# 构建指南

请根据你的开发环境选择 [Keil](#使用keil调试) 或 [Arm GNU Toolchain](#从makefile构建)

## 从Makefile文件构建

### 开发工具: [STM32CubeMX](https://www.st.com.cn/zh/development-tools/stm32cubemx.html), [VSCode](https://code.visualstudio.com/download), [MinGW](https://github.com/skeeto/w64devkit/releases)或[Cygwin](https://cygwin.com/install.html), [Arm GNU Toolchain](https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads), [stlink](https://github.com/stlink-org/stlink/releases)

<span style="color:gray">采用本方案单纯因为开发者比较喜欢用VSCode，你可以换成你喜欢的代码编辑器，你甚至可以用记事本和终端。实际上，如果你不用VSCode，直接跳过本指引用到VSCode的部分，依然能正常编译和下载程序。</span>

1. 下载`Arm GNU Toolchain`和其他工具

下载`Arm GNU Toolchain`时请选择`Windows (mingw-w64-x86_64) hosted cross toolchains`->`AArch32 bare-metal target (arm-none-eabi)`

2. 安装开发工具

`STM32CubeMX`需要登录ST账号以下载`MCU Packages`

`stlink`依赖[libusb](https://github.com/libusb/libusb/releases)，请将`libusb`压缩包内`MinGW64/dll/libusb-1.0.dll`拷贝至`stlink/bin/`

`stlink`还需要将`stlink/Program Files (x86)`拷贝至`C:\Program Files (x86)`

3. 添加系统环境变量

将`MinGW`, `Arm GNU Toolchain`, `stlink`的`bin/`路径添加到系统环境变量`Path`

4. 下载本仓库

5. 用`STM32CubeMX`打开`bsp.ioc`

6. 单击`GENERATE CODE`

7. 用`VSCode`打开本项目

在本项目根目录右键二级菜单中选择`通过Code打开`

8. 安装[推荐插件](../.vscode/extensions.json)

推荐`C/C++`, `Cortex-Debug`，其余选装

9. 编译本项目

<span style="color:gray">在VSCode中</span>打开终端，选择新建的`powershell`终端

<span style="color:gray">我想强调的其实是选择powershell，因为在后续调试步骤之后默认终端会变成gdb-server</span>

输入编译指令，回车开始编译

```sh
make
```

编译选项通过修改[Makefile](../Makefile)调整，用法可参考[官方文档](https://www.gnu.org/software/make/manual/make.html)和[编译选项说明](./make.md)或历史提交[fix: 修复Makefile构建](https://github.com/lava081/UART/commit/efb570a388065090de32b70e61156546029b9292)

如果编译成功，你将能找到`build/bsp.elf`

10. <span style="color:gray">(可选)</span>下载芯片svd文件

<span style="color:gray">svd文件可以帮助在调试时查看寄存器的值，如果你不关心寄存器，可以直接删除[.vscode/launch.json](../.vscode/launch.json)的`svdFile`行</span>

st官网的文档支持->[`cad-resources`板块](https://www.st.com/en/microcontrollers-microprocessors/stm32-32-bit-arm-cortex-mcus.html#cad-resources)->筛选`SVD`->下载svd文件

将对应芯片的`.svd`文件拷贝到项目根目录<span style="color:gray">或者你喜欢的位置</span>

将`.svd`文件的路径填写到[.vscode/launch.json](../.vscode/launch.json)的`svdFile`字段中<span style="color:gray">。如果路径中存在反斜杠`\`，请用`/`或`\\`替代</span>

11. 用`ST-LINK`连接你拼好的的开发板

12. 用`Cortex-Debug`下载和调试

按`F5`开始调试，调试器选择`ST-LINK调试`

如果你只想下载，不想调试:
```sh
st-flash --reset write build/bsp.bin 0x8000000
```

更多预配置操作的使用与配置项含义参见[VSCode使用指引](./vscode.md)

## 使用Keil构建

### 开发工具: [STM32CubeMX](https://www.st.com.cn/zh/development-tools/stm32cubemx.html), [Keil UV5](https://www.keil.com/demo/eval/arm.htm)

<span style="color:red">由于开发者使用Makefile构建，更改工具链可能丢失部分项目信息。未来有可能引入高版本C语言特性，不保证与Keil兼容。</span>

1. 配置开发工具，下载所需库文件

2. 下载本仓库

3. 用`STM32CubeMX`打开`bsp.ioc`

4. `Project Manager`选项卡->`Project`选项卡->`Project Settings`栏->`Toolchain / IDE`下拉框->选择`MDK-ARM`

4. 单击`GENERATE CODE`

5. 用`Keil`打开`MDK-ARM/bsp.uvprojx`

6. `Project`菜单栏->`Options for Target`窗口->`C/C++`选项卡->`Include Paths`栏->`...`->添加`../User/Inc`

7. `Project`菜单栏->`Manage`子菜单->`Project Items`窗口->在`Groups`组新建`Application/User/User`->`Add Files`->添加`../User/Src/`下的所有文件

6. 按`F7`编译

7. 用`ST-LINK`连接你拼好的的开发板

8. 按`F8`下板

9. 如果上述步骤中缺什么，相信你能搞定的
