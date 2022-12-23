# ESP32驱动ssd1306

本例程将使用ESP32驱动ssd1306，将为你介绍SSD1306，接线方式以及如何使用Adafruit_GFX、u8g2两种模式进行开发SSD1306。

> 本人踩坑了半天，发现Adafruit_SSD1306并不能驱动器LVGL，其并没有实现关键的setAddrWindow和pushColors函数用于display flushing，因此，对于SSD1306，需要采用的u8g2才能驱动起lvgl开发。

## SSD1306
SSD1306是一款128*64像素可以使用IIC驱动的OLED屏幕，当然其也有提供SPI的版本，在这里仅展示IIC的版本，两者的差别仅仅是接线方式不同。

SSD1306 Oled显示模块共有4个引脚，标记为GND, VCC, SCL和SDA。这种Oled显示模块可以使用3.3V到5V轻松上电。这是一个低功耗显示模块，可以很容易地与Arduino板使用5伏，也可以使用3.3V控制板，如ESP8266和ESP32等。

就像Arduino Uno Arduino Nano, Nodemcu ESP8266;ESP32还提供I2C总线。GPIO21是SDA, GPIO22是SCL。在[接线方式](#接线方式)中引脚都清楚地标记，如下图所示。

使用ESP32的GPIO21和GPIO22引脚可以连接多个支持I2C的设备，没有任何问题，因为每个支持I2C的设备都有一个唯一的地址，可以通过使用下面给出的I2C扫描仪代码找到。

## u8g2，Adafruit_GFX都是什么？
首先，u8g2和Adafruit_GFX你可以理解为都是抽象层的图形库，不对接的任何屏幕的驱动。但事实上，u8g2内部是提供了驱动的，并且提供了很多屏幕驱动的解决方案，如`U8G2_SSD1306_128X64_xxxx`类、`U8G2_UC1638_192X96_xxxx`类，他们都继承了`u8g2`这个类，可以理解为，如果你想要开发屏幕驱动，你只要继承这个类实现对应的功能就好了。

而Adafruit_GFX也是同理，Adafruit_SSD1306是其对应ssd1306驱动实现版本。

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223204252.png"/>

## 开发准备
### 硬件准备

- ESP32
- SSD1306
- 4条母对母的杜邦线

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223080431.png"/>


<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223075205.png"/>

### 接线方式

ESP32的引脚分布可以查看[ESP32-DevKitC V4 入门指南](https://docs.espressif.com/projects/esp-idf/zh_CN/latest/esp32/hw-reference/esp32/get-started-devkitc.html)，具体的接线如下所示，对应的引脚连接上即可。

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223080152.png"/>

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223081230.png"/>

### 开发环境

- Platformio
- lvgl v7.11.0 (v8和v7不兼容，需要注意一下，这里我已经导好包了)

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223075321.png"/>


## Adafruit_GFX开发
Adafruit_GFX开发方式需要用到Adafruit_GFX、Adafruit_SSD1306两个库，Adafruit_GFX是一个GUI的图形图，而Adafruit_SSD1306继承了Adafruit_GFX的属性，做了SSD1306的驱动。

### Adafruit_GFX库安装
事实上,在platformio中提供了三种第三方库安装方式，第一种是命令行模式，第二种是在PIO Home中下载，第三种是直接拿下载好的第三方库放进lib目录下，本人倾向于使用命令行，但是这里第一种、第二种都演示一下。

1. 方法1：命令行

在vscode打开Command Paplette(Ctrl + Shift + P),输入`platformio: new terminal`，进入终端后，输入以下命令。
```
pio lib install Adafruit_SSD1306
```
> 是不是感觉和pip install很像？

success之后就可以直接开始编写代码了。

2. 方法2：在PIO Home中下载

我们直接在PIO Home中搜索Adafruit_SSD1306就好了，然后下载下来，添加到对应项目下。

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223085831.png"/>

### 运行
在安装完第三方库之后，直接copy PIO Home给出的示例，编译运行一下你的SSD1306是否可以跑，如果SSD1306显示出内容了，那么恭喜你，你已经成功完成第一步！

<img src="https://zeeland-bucket.oss-cn-beijing.aliyuncs.com/typora_img/20221223090151.png"/>

## u8g2开发

首先先安装对应的u8g2库，直接去PIO Home中搜导入就ok。

下面直接附上代码，详细的开发过程中的一些驱动函数直接看u8g2的官方文档或者[参考链接](#参考)足矣。

> PIO Home中的u8g2虽然也有给出示例代码，但是需要改动一些地方，不能直接使用，直接copy以下代码即可。

```cpp
#include <Arduino.h>
#include <U8g2lib.h>

#include <Wire.h>

# IIC version
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0,SCL,SDA,U8X8_PIN_NONE);
 
void setup(void) {
  u8g2.begin();
}
 
void loop(void) {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_ncenB14_tr);
    u8g2.drawStr(0,24,"Hello World!");
  } while ( u8g2.nextPage() );
}
```


## 参考
- [https://www.electroniclinic.com/ -recommend -ssd1306 -esp32 -Adafruit_SSD1306](https://www.electroniclinic.com/esp32-oled-ssd1306-circuit-diagram-and-programming/)
- [http://www.taichi-maker.com/arduino-oled-application/ -Adafruit_SSD1306](http://www.taichi-maker.com/homepage/reference-index/display-reference-index/arduino-oled-application/)
- [https://github.com/adafruit/Adafruit_SSD1306 -Adafruit_SSD1306](https://github.com/adafruit/Adafruit_SSD1306)
- [esp笔记（7）单色OLED（SSD1306） 显示与 u8g2 库的使用 -u8g2 -ssd1306 -csdn](https://blog.csdn.net/qq_17351161/article/details/105011779)
- [玩转u8g2 OLED库，一篇就够 -u8g2 -ssd1306 -csdn](https://blog.csdn.net/weixin_30510153/article/details/96227475?spm=1001.2101.3001.6650.2&utm_medium=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-96227475-blog-88898446.pc_relevant_3mothn_strategy_and_data_recovery&depth_1-utm_source=distribute.pc_relevant.none-task-blog-2%7Edefault%7ECTRLIST%7ERate-2-96227475-blog-88898446.pc_relevant_3mothn_strategy_and_data_recovery&utm_relevant_index=5)
- [https://github.com/AhmadRoshanfar/port-oled-ssd1306-lvgl-esp32/ -idf](https://github.com/AhmadRoshanfar/port-oled-ssd1306-lvgl-esp32/)