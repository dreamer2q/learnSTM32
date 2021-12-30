## Engine

游戏引擎源自于[eleeye](https://github.com/xqbase/eleeye)，使用`xmake`进行构建，在`m1`电脑上进行测试。

### 目录结构

```bash
├── adaptor # 串口适配器，用于实现游戏引擎和嵌入式设备进行串口通信
├── base # 引擎公用代码(这部分可以进一步删减)
├── book # 开局库
├── build # 构建目录
├── eleeye # eleeye 引擎代码
└── include # 头文件
```

### 使用

1. 安装`xmake`等构建环境，完成`eleeye`引擎构建。
2. 修改`adaptor/usbserial.c`串口设备位置。

```c
// 修改成自己的设备驱动
// 可通过`ls`查看`/dev`目录
const char* SERIAL_PORT = "/dev/tty.usbserial-CBAOb10CD20";

// 修改引擎位置
const char* ENGINE_PATH = "../build/eleeye";
```

3. 编译运行`usbserial.c`，用于和嵌入式设备进行通信。

